#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gnss_worker_thread.h"
#include "gnss_worker_sm.h"
#include "app_thread.h"
#include "app_utils_misc.h"
#include "qapi_loc_wrapper.h"
#include "cfg_mgr.h"
#include "app_utils_misc.h"
#include "aware_main_thread.h"
#include "aware_log.h"

/* Thread Constants */
static char* _GnssWorkerTaskName = "locationTask";
static app_thread_id _GnssWorkerTaskId = APP_THREAD_ID_GNSS_WORKER;

static TX_MUTEX* _GnssWorkerMutex;
static TX_EVENT_FLAGS_GROUP* _GnssWorkerSingleshotSignal;

// Internal struct to allow passing message data to this thread
// message handlers
typedef struct gnss_worker_msg
{
    qapi_Location_t* locationOutPtr;

    // gnss_worker_send_msg_request_delayed_location
    uint32 reportDelaySec;
    qapi_Location_Power_Level_t maxPower;
    qapi_Location_Accuracy_Level_t minAccuracy;
    uint32 allowPriorityInversion;

    // gnss_worker_send_msg_cb_singleshot
    qapi_Location_t location;
    qapi_Location_Error_t err;

    gtp_data_t gtpdata;

    // gnss_worker_send_msg_cb_meta_data
    qapi_Location_Meta_Data_t metaData;

    // gnss_worker_send_msg_cb_motion
    qapi_Location_Motion_Info_t motionInfo;

    // gnss_worker_send_msg_cb_nmea
    qapi_Gnss_Nmea_t nmea;

    // gnss_worker_send_msg_cb_geofence_breach
    qapi_Location_t geofenceBreachLocation;
    qapi_Geofence_Breach_t geofenceBreachType;
    uint64_t geofenceBreachTimestamp;

    // Callback to process in thread context
    gnss_worker_thread_cb_type callback;
    uint32 cbData;

    // Tracking start request
    uint32 trackingTbf;
    uint32 trackingDbf;
    qapi_Location_Accuracy_Level_t trackingAccuracy;

    // Geofence add request
    // Geofence parameters
    double geofenceLatitude;
    double geofenceLongitude;
    double geofenceRadius;
    uint32 geofenceMask;
    uint32 geofenceResponsivenessMillis;
    uint32 geofenceDwellMillis;

    // WWAN sleep callback
    uint32 wwanSleepDuration;

} gnss_worker_msg;

int gnss_worker_get_task_id()
{
    return _GnssWorkerTaskId;
}
char* gnss_worker_get_task_name()
{
    return _GnssWorkerTaskName;
}

static qapi_Location_t* _BestAvailLocationOutPtr;

// INTERNAL FUNCTIONS
static void gnss_worker_send_msg_request_best_avail_location();


void gnss_worker_report_singleshot_location(qapi_Location_t location)
{
    LOG_INFO("gnss_worker_report_singleshot_location");

    // Send the location to controller thread
    aware_main_thread_send_msg_report_location(location);
}

void gnss_worker_report_singleshot_gtpdata(gtp_data_t gtpdata)
{
    LOG_INFO("gnss_worker_report_singleshot_gtp_data");

    // Send the location to controller thread
    aware_main_thread_send_msg_report_gtpdata(gtpdata);
}

/*-------------------------------------------------------------------------*/
/**   Synchronous wrappers on top of message processing in this thread
  @brief	Synchronous wrappers :: Get best Available Location
  @param 	locationOutPtr 	 Location details
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_sync_get_best_avail_location(qapi_Location_t* locationOutPtr)
{
    // Set output memory
    _BestAvailLocationOutPtr = locationOutPtr;
    if (_BestAvailLocationOutPtr == NULL)
    {
        LOG_ERROR("Null Location pointer to fetch best avail pos");
        return;
    }

    _BestAvailLocationOutPtr->size = 0;

    // Send the message
    gnss_worker_send_msg_request_best_avail_location();

    LOG_DEBUG("Waiting for best avail pos..");

    // Now block on the best available position fetched signal
    // This signal will be sent by gnss_worker thread when best available position
    // is fetched and populated in the _BestAvailLocationOutPtr
    // Then we allow this function to return.
    // For the caller, this is a blocking function which fetches the best available position.
    //app_utils_timedwait_on_signal(
    //        _GnssWorkerSingleshotSignal, _GnssWorkerMutex, MILLIS_TO_TICKS(1000));

    if (_BestAvailLocationOutPtr->size != 0)
    {
        LOG_DEBUG("Got best avail pos..");
    }
    else
    {
        LOG_ERROR("Failed to get best avail pos.");
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Report best Available Location
  @param 	locationOutPtr 	 Location details
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_report_best_avail_location(qapi_Location_t location)
{
    LOG_DEBUG("gnss_worker_report_best_avail_location");
    //memcpy(_BestAvailLocationOutPtr, &location, sizeof(qapi_Location_t));

    // Set the singleshot callback signal
    //app_utils_set_signal(_GnssWorkerSingleshotSignal, _GnssWorkerMutex);

    // Also send the location to controller thread
    aware_main_thread_send_msg_report_location(location);
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Utility methods for external threads to post messages to this thread
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_send_msg_request_best_avail_location()
{
    app_thread_send_msg(
            _GnssWorkerTaskId,
            GNSS_WORKER_MSG_ID_REQUEST_BEST_AVAIL_LOCATION,
            NULL, 0);
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Send msg request for delayed Location
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_send_msg_request_delayed_location(
        uint32 reportDelaySec, qapi_Location_Power_Level_t maxPower,
        qapi_Location_Accuracy_Level_t minAccuracy, uint32 allowPriorityInversion)
{
    LOG_DEBUG("gnss_worker_send_msg_request_delayed_location");
    gnss_worker_msg* workerMsg = NULL;
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    int32 res = app_utils_byte_allocate((void**)&workerMsg, workerMsgSize);
    if (0 == res)
    {
        memset(workerMsg, 0, workerMsgSize);

        workerMsg->reportDelaySec = reportDelaySec;
        workerMsg->maxPower = maxPower;
        workerMsg->minAccuracy = minAccuracy;
        workerMsg->allowPriorityInversion = allowPriorityInversion;

        app_thread_send_msg(
                _GnssWorkerTaskId,
                GNSS_WORKER_MSG_ID_REQUEST_DELAYED_LOCATION,
                workerMsg, workerMsgSize);

        app_utils_byte_release((void*)workerMsg);
    }
    else
    {
        LOG_ERROR("Failed to allocate %d bytes", workerMsgSize);
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Send msg callback for single shot location
  @param 	locationOutPtr 	 Location details
  @param    err     Error type
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_send_msg_cb_singleshot(
        qapi_Location_t location, qapi_Location_Error_t err)
{
    LOG_DEBUG("gnss_worker_send_msg_cb_singleshot");
    gnss_worker_msg* workerMsg = NULL;
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    int32 res = app_utils_byte_allocate((void**)&workerMsg, workerMsgSize);
    if (0 == res)
    {
        memset(workerMsg, 0, workerMsgSize);

        memcpy(&workerMsg->location, &location, sizeof(qapi_Location_t));
        workerMsg->err = err;

        app_thread_send_msg(
                _GnssWorkerTaskId,
                GNSS_WORKER_MSG_ID_CB_SINGLESHOT,
                workerMsg, workerMsgSize);

        app_utils_byte_release((void*)workerMsg);
    }
    else
    {
        LOG_ERROR("Failed to allocate %d bytes", workerMsgSize);
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Send msg callback for meta dada
  @param 	metaData 	 metaData details
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_send_msg_cb_meta_data(qapi_Location_Meta_Data_t metaData)
{
    gnss_worker_msg* workerMsg = NULL;
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    int32 res = app_utils_byte_allocate((void**)&workerMsg, workerMsgSize);
    if (0 == res)
    {
        memset(workerMsg, 0, workerMsgSize);

        memcpy(&workerMsg->metaData, &metaData, sizeof(qapi_Location_Meta_Data_t));

        app_thread_send_msg(
                _GnssWorkerTaskId,
                GNSS_WORKER_MSG_ID_CB_META_DATA,
                workerMsg, workerMsgSize);

        app_utils_byte_release((void*)workerMsg);
    }
    else
    {
        LOG_ERROR("Failed to allocate %d bytes", workerMsgSize);
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Send msg callback for motion
  @param 	motionInfo 	 motion details
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_send_msg_cb_motion(qapi_Location_Motion_Info_t motionInfo)
{
    gnss_worker_msg* workerMsg = NULL;
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    int32 res = app_utils_byte_allocate((void**)&workerMsg, workerMsgSize);
    if (0 == res)
    {
        memset(workerMsg, 0, workerMsgSize);

        memcpy(&workerMsg->motionInfo, &motionInfo, sizeof(qapi_Location_Motion_Info_t));

        app_thread_send_msg(
                _GnssWorkerTaskId,
                GNSS_WORKER_MSG_ID_CB_MOTION_TRACKING,
                workerMsg, workerMsgSize);

        app_utils_byte_release((void*)workerMsg);
    }
    else
    {
        LOG_ERROR("Failed to allocate %d bytes", workerMsgSize);
    }
}

void gnss_worker_send_msg_cb_nmea(qapi_Gnss_Nmea_t nmea)
{
    gnss_worker_msg workerMsg = {0};
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    memcpy(&workerMsg.nmea, &nmea, sizeof(qapi_Gnss_Nmea_t));

    app_thread_send_msg(
            _GnssWorkerTaskId,
            GNSS_WORKER_MSG_ID_CB_NMEA,
            &workerMsg, workerMsgSize);
}

void gnss_worker_send_msg_cb_wwan_sleep(uint32 sleepDuration)
{
    gnss_worker_msg workerMsg = {0};
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    workerMsg.wwanSleepDuration = sleepDuration;

    app_thread_send_msg(
            _GnssWorkerTaskId,
            GNSS_WORKER_MSG_ID_CB_WWAN_SLEEP,
            &workerMsg, workerMsgSize);
}

void gnss_worker_send_msg_process_cb(gnss_worker_thread_cb_type callback, uint32 cbData)
{
    gnss_worker_msg workerMsg = {0};
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    workerMsg.callback = callback;
    workerMsg.cbData = cbData;

    app_thread_send_msg(
            _GnssWorkerTaskId,
            GNSS_WORKER_MSG_ID_PROCESS_CB,
            &workerMsg, workerMsgSize);
}

void gnss_worker_send_msg_cb_tracking(qapi_Location_t location)
{
    gnss_worker_msg workerMsg = {0};
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    memcpy(&workerMsg.location, &location, sizeof(qapi_Location_t));

    app_thread_send_msg(
            _GnssWorkerTaskId,
            GNSS_WORKER_MSG_ID_CB_TRACKING,
            &workerMsg, workerMsgSize);
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Send msg callback for GTP Blob
  @param 	locationOutPtr 	 Location details
  @param    err     Error type
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_send_msg_cb_singleshot_gtp(
        qapi_Client_Request_Info_t clientRequestInfo)
{
    LOG_DEBUG("gnss_worker_send_msg_cb_singleshot_gtp");
    gnss_worker_msg* workerMsg = NULL;
    size_t workerMsgSize = sizeof(gnss_worker_msg);

    if(clientRequestInfo.requestType == QAPI_CLIENT_REQUEST_TYPE_GTPWWAN_POS)
    {
        int32 res = app_utils_byte_allocate((void**)&workerMsg, workerMsgSize);
        if (0 == res)
        {
            memset(workerMsg, 0, workerMsgSize);

            size_t gtpbuffersize = clientRequestInfo.requestDataBufferSize;
            
            // LOG_INFO("var gtpbuffersize = %d", gtpbuffersize);
            if(gtpbuffersize != 0)
            {
                // Note: This allocated memory need to be cleared upon message receive.
                res = app_utils_byte_allocate((void**)&(workerMsg->gtpdata.dataBufferPtr), gtpbuffersize);
                if (0 == res)
                {
                    // LOG_INFO("dataBufferPtr = %d", (uint32)workerMsg->gtpdata.dataBufferPtr);
                    memscpy(workerMsg->gtpdata.dataBufferPtr, gtpbuffersize, clientRequestInfo.requestDataBuffer, gtpbuffersize);
                    workerMsg->gtpdata.dataBufferSize = gtpbuffersize;

                    // int i;
                    // for(i=0;i<gtpbuffersize;i++)
                    // {
                    //     LOG_INFO("gtp data: 0x%02X", ((uint8 *)workerMsg->gtpdata.dataBufferPtr)[i]);
                    // }
                    
                    app_thread_send_msg(
                            _GnssWorkerTaskId,
                            GNSS_WORKER_MSG_ID_CB_SINGLESHOT_GTP,
                            workerMsg, workerMsgSize);

                    LOG_INFO("GNSS_WORKER_MSG_ID_CB_SINGLESHOT_GTP msg sent");
                }
            }
            app_utils_byte_release((void*)workerMsg);
        }
        else
        {
            LOG_ERROR("Failed to allocate %d bytes", workerMsgSize);
        }
    }
    else
    {
        LOG_INFO("Non GTP Request");
    }
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Message Handlers for this thread/ request for best available location
  @param 	msg 	message string
  @param    size    size of msg
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _gnss_worker_handle_req_best_avail_location(void* msg, size_t size)
{
    LOG_DEBUG("_gnss_worker_handle_req_best_avail_location");
    // Message parameters
    qapi_Location_t* locationOutPtr = NULL;

    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg != NULL || size != 0)
    {
        LOG_ERROR("Invalid msg, no payload expected for best avail request");
        return;
    }

    // Process
    gnss_worker_sm_request_best_avail_location();
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Message Handlers for this thread/ request for delayed location
  @param 	msg 	message string
  @param    size    size of msg
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _gnss_worker_handle_req_delayed_location(void* msg, size_t size)
{
    LOG_DEBUG("_gnss_worker_handle_req_delayed_location");
    // Message parameters
    uint32 reportDelaySec = 0;
    qapi_Location_Power_Level_t maxPower = 0;
    qapi_Location_Accuracy_Level_t minAccuracy = 0;
    uint32 allowPriorityInversion = 0;

    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // Extract parameters
    reportDelaySec = workerMsg->reportDelaySec;
    maxPower = workerMsg->maxPower;
    minAccuracy = workerMsg->minAccuracy;
    allowPriorityInversion = workerMsg->allowPriorityInversion;

    // Process
    gnss_worker_sm_request_delayed_location(
            reportDelaySec, maxPower, minAccuracy, allowPriorityInversion);
}

/* QAPI Callbacks */
/* QAPI Callbacks */
static void _gnss_worker_handle_cb_singleshot(void* msg, size_t size)
{
    // Message parameters
    qapi_Location_t location = {};
    qapi_Location_Error_t err = QAPI_LOCATION_ERROR_GENERAL_FAILURE;

    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // Extract parameters
    location = workerMsg->location;
    err = workerMsg->err;

    // notify the state machine
    gnss_worker_sm_singleshot_cb(location, err);
}

static void _gnss_worker_handle_cb_singleshot_gtp(void* msg, size_t size)
{
    // Message parameters
    gtp_data_t gtpdata = {};

    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // LOG_INFO("gtpsize after receiving msg in gnss thread b/f copy= %d", workerMsg->gtpdata.dataBufferSize);
    // LOG_INFO("gtpbufferptr after receiving msg in gnss thread b/f copy= %d", workerMsg->gtpdata.dataBufferPtr);

    // Extract parameters
    gtpdata = workerMsg->gtpdata;

    // LOG_INFO("gtpsize after receiving msg in gnss thread = %d", gtpdata.dataBufferSize);
    // LOG_INFO("gtpbufferptr after receiving msg in gnss thread = %d", gtpdata.dataBufferPtr);

    // notify the state machine
    gnss_worker_sm_singleshot_gtp_cb(gtpdata);
}

static void _gnss_worker_handle_cb_metadata(void* msg, size_t size)
{
    // Message parameters
    qapi_Location_Meta_Data_t metaData = {};

    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // Extract parameters
    metaData = workerMsg->metaData;
    gnss_worker_sm_meta_data_cb(metaData);
}

static void _gnss_worker_handle_cb_motion(void* msg, size_t size)
{
    // Message parameters
    qapi_Location_Motion_Info_t motionInfo = {};

    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // Extract parameters
    motionInfo = workerMsg->motionInfo;
    gnss_worker_sm_motion_info_cb(motionInfo);
}

static void _gnss_worker_handle_cb_nmea(void* msg, size_t size)
{
    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }
}

static void _gnss_worker_handle_cb_wwan_sleep(void* msg, size_t size)
{
    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }
    //gnss_worker_sm_wwan_sleep_cb(workerMsg->wwanSleepDuration);
}

static void _gnss_worker_handle_process_cb(void* msg, size_t size)
{
    // validate
    gnss_worker_msg* workerMsg = (gnss_worker_msg*)msg;
    if (workerMsg == NULL || size != sizeof(gnss_worker_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // Message parameters
    gnss_worker_thread_cb_type callback = workerMsg->callback;
    uint32 cbData = workerMsg->cbData;

    // Invoke the callback
    if (callback != NULL)
    {
        callback(cbData);
    }
}

// Message router
/*-------------------------------------------------------------------------*/
/**
  @brief	Process gnss message
  @param    msgId   message Id
  @param 	msg 	message string
  @param    msgSize    size of msg
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _gnss_worker_process_msg(int msgId, void *msg, size_t msgSize)
{
    LOG_DEBUG("Processing msgId %d", msgId);

    switch (msgId)
    {
    case GNSS_WORKER_MSG_ID_REQUEST_BEST_AVAIL_LOCATION:
        LOG_DEBUG("Recvd REQUEST_BEST_AVAIL_LOCATION msgSize %d", msgSize);
        _gnss_worker_handle_req_best_avail_location(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_REQUEST_DELAYED_LOCATION:
        LOG_DEBUG("Recvd REQUEST_DELAYED_LOCATION msgSize %d", msgSize);
        _gnss_worker_handle_req_delayed_location(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_REQUEST_TRACKING_START:
        LOG_DEBUG("Recvd REQUEST_TRACKING_START msgSize %d", msgSize);
        //_gnss_worker_handle_req_tracking_start(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_REQUEST_TRACKING_STOP:
        LOG_DEBUG("Recvd REQUEST_TRACKING_STOP msgSize %d", msgSize);
        //_gnss_worker_handle_req_tracking_stop(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_REQUEST_GEOFENCE_ADD:
        LOG_DEBUG("Recvd REQUEST_GEOFENCE_ADD msgSize %d", msgSize);
        //_gnss_worker_handle_req_geofence_add(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_REQUEST_GEOFENCE_REMOVE:
        LOG_DEBUG("Recvd REQUEST_GEOFENCE_REMOVE msgSize %d", msgSize);
        //_gnss_worker_handle_req_geofence_remove(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_TRACKING:
        LOG_DEBUG("Recvd CB_TRACKING msgSize %d", msgSize);
        //_gnss_worker_handle_cb_tracking(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_SINGLESHOT:
        LOG_DEBUG("Recvd CB_SINGLESHOT msgSize %d", msgSize);
        _gnss_worker_handle_cb_singleshot(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_SINGLESHOT_GTP:
        LOG_DEBUG("Recvd CB_SINGLESHOT_GTP msgSize %d", msgSize);
        _gnss_worker_handle_cb_singleshot_gtp(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_META_DATA:
        LOG_DEBUG("Recvd CB_META_DATA msgSize %d", msgSize);
        _gnss_worker_handle_cb_metadata(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_MOTION_TRACKING:
        LOG_DEBUG("Recvd CB_MOTION_TRACKING msgSize %d", msgSize);
        _gnss_worker_handle_cb_motion(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_NMEA:
        LOG_DEBUG("Recvd CB_NMEA msgSize %d", msgSize);
        _gnss_worker_handle_cb_nmea(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_GEOFENCE_BREACH:
        LOG_DEBUG("Recvd CB_GEOFENCE_BREACH msgSize %d", msgSize);
        //_gnss_worker_handle_cb_geofence_breach(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_CB_WWAN_SLEEP:
        LOG_DEBUG("Recvd CB_WWAN_SLEEP msgSize %d", msgSize);
        _gnss_worker_handle_cb_wwan_sleep(msg, msgSize);
        break;

    case GNSS_WORKER_MSG_ID_PROCESS_CB:
        LOG_DEBUG("Recvd PROCESS_CB msgSize %d", msgSize);
        _gnss_worker_handle_process_cb(msg, msgSize);
        break;

    default:
        LOG_DEBUG("Incorrect msgId %d", msgId);
    }
}

/*-------------------------------------------------------------------------*/
/**
  @brief	This is the entry point for Location task
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void gnss_worker_task_entry(ULONG args) {

    LOG_INFO("GNSS Worker Thread started. TCB %x TID %d", tx_thread_identify(), args);

    // Initializations
    app_utils_mutex_init(&_GnssWorkerMutex, "_GnssWorkerMutex");
    app_utils_init_signal(&_GnssWorkerSingleshotSignal, "_GnssWorkerSingleshotSignal");

    gnss_worker_sm_init();

    AppMsg *payload = NULL;
    while (0 == app_thread_rcv_msg(args, (void**) &payload))
    {
        _gnss_worker_process_msg(payload->msgId, payload->msg, payload->msgSize);

        app_utils_byte_release(payload->msg);
        app_utils_byte_release(payload);
    }
}
