/*****************************************************************************
 Copyright (c) 2020 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 *****************************************************************************/
#include <stdint.h>

#include "app_types.h"
#include "app_utils_misc.h"
#include "qapi_loc_wrapper.h"
#include "qapi_location.h"
#include "gnss_worker_thread.h"
#include "aware_log.h"

extern TX_MUTEX* _MutexResponse;
extern TX_EVENT_FLAGS_GROUP* _SignalResponse;
extern TX_EVENT_FLAGS_GROUP* _SignalCollectiveResponse;

#define LOCATION_MODE 1

// This one cb for gnss worker to be invoked in loc context
void gnss_worker_sm_singleshot_cb_in_loc_context(
        qapi_Location_t location, qapi_Location_Error_t err);

// This one cb for gnss worker to be invoked in meta data context
void gnss_worker_sm_ntp_status_cb_meta_data_context(
    qapi_Time_AD_Status_t *ptimeAdStatus);

// This one cb for gnss worker to be invoked in meta data context
void gnss_worker_sm_xtra_status_cb_meta_data_context(
    qapi_Xtra_AD_Status_t *pxtraAdStatus);

// This one only used for synchronized waiting for XTRA INT AD Update Status
// This one gets invoked in meta callback context
void gnss_worker_sm_xtra_int_status_cb_meta_data_context(
    qapi_Xtra_Int_AD_Status_t *pxtraIntAdStatus);

// This one only used for synchronized waiting for AD Download
// This one gets invoked in meta callback context
void gnss_worker_sm_engine_status_cb_meta_data_context(
    qapi_Location_Meta_Data_t metaData);

/*---------------------------------------------------------
 * RESPONSE CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_response_cb(qapi_Location_Error_t err, uint32_t id)
{
    LOG_DEBUG("RESPONSE CB (err %d id %d)", err, id);

    app_utils_set_signal(_SignalResponse, _MutexResponse);
}

/*---------------------------------------------------------
 * COLLECTIVE RESPONSE CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_collective_response_cb(
        size_t count, qapi_Location_Error_t* err, uint32_t* ids)
{
    LOG_DEBUG("COLLECTIVE RESPONSE CB (count %d)", count);

    for (uint32 i = 0; i < count; ++i) {
        LOG_DEBUG("ID[%d] = %d, err[%d] = %d", i, ids[i], i, err[i]);
    }

    app_utils_set_signal(_SignalCollectiveResponse, _MutexResponse);
}

/*---------------------------------------------------------
 * CAPABILITIES CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_capabilities_cb(
        qapi_Location_Capabilities_Mask_t capabilitiesMask)
{
    LOG_DEBUG("CAPABILITIES CB (mask 0x%x)", capabilitiesMask);
}

/*---------------------------------------------------------
 * TRACKING CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_tracking_cb(qapi_Location_t location)
{
    LOG_DEBUG("TRACKING CB");

    uint64_t ts_sec = location.timestamp/1000;

    LOG_DEBUG("Time(HH:MM:SS): %02d:%02d:%02d",
            (int)((ts_sec/3600)%24), (int)((ts_sec/60)%60), (int)(ts_sec%60));

    LOG_DEBUG("LAT: %d.%05d LON: %d.%05d ACC: %d.%02d",
        (int)location.latitude, (abs((int)(location.latitude * 100000))) % 100000,
        (int)location.longitude, (abs((int)(location.longitude * 100000))) % 100000,
        (int)location.accuracy, (abs((int)(location.accuracy * 100))) % 100);

    LOG_DEBUG("ALT: %d.%02d ALT_MSL: %d.%02d",
        (int)location.altitude, (abs((int)(location.altitude * 100))) % 100,
        (int)location.altitudeMeanSeaLevel, (abs((int)(location.altitudeMeanSeaLevel * 100))) % 100);

    LOG_DEBUG("SPEED: %d.%02d BEAR: %d.%02d TIME: 0x%x%x FLAGS: %u",
        (int)location.speed, (abs((int)(location.speed * 100))) % 100,
        (int)location.bearing, (abs((int)(location.bearing * 100))) % 100,
        (int)(location.timestamp >> 32), (int)location.timestamp, location.flags);

    gnss_worker_send_msg_cb_tracking(location);
}

/*---------------------------------------------------------
 * BATCHING CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_batching_cb(size_t count, qapi_Location_t* location)
{
    LOG_DEBUG("BATCHING CB (count %d)", count);

    uint64_t ts_sec = 0;
    for (uint32 i = 0; i < count; ++i) {
        ts_sec = location[i].timestamp/1000;
        LOG_DEBUG("Time(HH:MM:SS): %02d:%02d:%02d",
                (int)((ts_sec/3600)%24), (int)((ts_sec/60)%60), (int)(ts_sec%60));

        LOG_DEBUG("location[%d]:", i);

        LOG_DEBUG("LAT: %d.%05d LON: %d.%05d ACC: %d.%02d",
            (int)location[i].latitude, (abs((int)(location[i].latitude * 100000))) % 100000,
            (int)location[i].longitude, (abs((int)(location[i].longitude * 100000))) % 100000,
            (int)location[i].accuracy, (abs((int)(location[i].accuracy * 100))) % 100);

        LOG_DEBUG("ALT: %d.%02d ALT_MSL: %d.%02d",
            (int)location[i].altitude, (abs((int)(location[i].altitude * 100))) % 100,
            (int)location[i].altitudeMeanSeaLevel, (abs((int)(location[i].altitudeMeanSeaLevel * 100))) % 100);

        LOG_DEBUG("SPEED: %d.%02d BEAR: %d.%02d TIME: 0x%x%x FLAGS: %u",
            (int)location[i].speed, (abs((int)(location[i].speed * 100))) % 100,
            (int)location[i].bearing, (abs((int)(location[i].bearing * 100))) % 100,
            (int)(location[i].timestamp >> 32), (int)location[i].timestamp, location[i].flags);
    }
}

/*---------------------------------------------------------
 * GEOFENCE BREACH CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_geofence_breach_cb(
        qapi_Geofence_Breach_Notification_t notification)
{
    LOG_DEBUG("GEOFENCE BREACH CB (count %d)", notification.count);

    // Breach type
    if (notification.type == QAPI_GEOFENCE_BREACH_ENTER) {
        LOG_DEBUG("Breach Type: ENTER");
    } else if (notification.type == QAPI_GEOFENCE_BREACH_EXIT) {
        LOG_DEBUG("Breach Type: EXIT");
    } else if (notification.type == QAPI_GEOFENCE_BREACH_DWELL_IN) {
        LOG_DEBUG("Breach Type: DWELL IN");
    } else if (notification.type == QAPI_GEOFENCE_BREACH_DWELL_OUT) {
        LOG_DEBUG("Breach Type: DWELL OUT");
    } else {
        LOG_DEBUG("Breach type invalid: %d", notification.type);
    }

    // Timestamp and List of IDs received
    uint64_t ts_sec = notification.location.timestamp/1000;
    LOG_DEBUG("Time(HH:MM:SS): %02d:%02d:%02d : "
            "List of IDs recvd (count %d):",
            (int)((ts_sec/3600)%24), (int)((ts_sec/60)%60), (int)(ts_sec%60),
            notification.count);
    for(uint32 i = 0; i < notification.count; i++) {
        LOG_DEBUG("ID: %d", notification.ids[i]);
    }

    LOG_DEBUG("LAT: %d.%05d LON: %d.%05d ACC: %d.%02d",
        (int)notification.location.latitude,
        (abs((int)(notification.location.latitude * 100000))) % 100000,
        (int)notification.location.longitude,
        (abs((int)(notification.location.longitude * 100000))) % 100000,
        (int)notification.location.accuracy,
        (abs((int)(notification.location.accuracy * 100))) % 100);

    LOG_DEBUG("ALT: %d.%02d ALT_MSL: %d.%02d",
        (int)notification.location.altitude,
        (abs((int)(notification.location.altitude * 100))) % 100,
        (int)notification.location.altitudeMeanSeaLevel,
        (abs((int)(notification.location.altitudeMeanSeaLevel * 100))) % 100);

    LOG_DEBUG("SPEED: %d.%02d BEAR: %d.%02d TIME: 0x%x%x FLAGS: %u",
        (int)notification.location.speed,
        (abs((int)(notification.location.speed * 100))) % 100,
        (int)notification.location.bearing,
        (abs((int)(notification.location.bearing * 100))) % 100,
        (int)(notification.location.timestamp >> 32),
        (int)notification.location.timestamp, notification.location.flags);
}

/*---------------------------------------------------------
 * SINGLESHOT CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_single_shot_cb(
        qapi_Location_t location, qapi_Location_Error_t err)
{
    LOG_DEBUG("SINGLESHOT CB");
    LOG_INFO("qapi_loc_wrapper_single_shot_cb, err = %d", err);

    if (QAPI_LOCATION_ERROR_TIMEOUT == err)
    {
        LOG_DEBUG("Timeout occurred, location is invalid");
    }
    else
    {
        uint64_t ts_sec = location.timestamp / 1000;

        LOG_DEBUG("Time(HH:MM:SS): %02d:%02d:%02d",
            (int)((ts_sec / 3600) % 24), (int)((ts_sec / 60) % 60), (int)(ts_sec % 60));

        LOG_DEBUG("LAT: %d.%05d LON: %d.%05d ACC: %d.%02d",
            (int)location.latitude, (abs((int)(location.latitude * 100000))) % 100000,
            (int)location.longitude, (abs((int)(location.longitude * 100000))) % 100000,
            (int)location.accuracy, (abs((int)(location.accuracy * 100))) % 100);

        LOG_DEBUG("ALT: %d.%02d ALT_MSL: %d.%02d",
            (int)location.altitude, (abs((int)(location.altitude * 100))) % 100,
            (int)location.altitudeMeanSeaLevel, (abs((int)(location.altitudeMeanSeaLevel * 100))) % 100);

        LOG_DEBUG("SPEED: %d.%02d BEAR: %d.%02d TIME: 0x%x%x FLAGS: %u",
            (int)location.speed, (abs((int)(location.speed * 100))) % 100,
            (int)location.bearing, (abs((int)(location.bearing * 100))) % 100,
            (int)(location.timestamp >> 32), (int)location.timestamp, location.flags);
    }

    if (LOCATION_MODE == 0)
    {
        gnss_worker_sm_singleshot_cb_in_loc_context(location, err);
    }

    // LOG_INFO("location.flags %d", location.flags);
    if(QAPI_LOCATION_IS_BEST_AVAIL_POS_BIT & location.flags)
    {
        LOG_INFO("Best Available Location received");
    }
    else
    {
        gnss_worker_send_msg_cb_singleshot(location, err);
    } 
}

/*---------------------------------------------------------
 * GNSS DATA CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_gnss_data_cb(qapi_Gnss_Data_t gnssData)
{
    LOG_DEBUG("GNSS DATA CB");

    LOG_DEBUG("JAMMER_GPS: %u JAMMER_GLONASS: %u",
        gnssData.jammerInd[QAPI_GNSS_SV_TYPE_GPS],
        gnssData.jammerInd[QAPI_GNSS_SV_TYPE_GLONASS]);

    LOG_DEBUG("JAMMER_BEIDOU: %u JAMMER_GALILEO: %u",
        gnssData.jammerInd[QAPI_GNSS_SV_TYPE_BEIDOU],
        gnssData.jammerInd[QAPI_GNSS_SV_TYPE_GALILEO]);
}

/*---------------------------------------------------------
 * META DATA CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_gnss_meta_data_cb(qapi_Location_Meta_Data_t metaData)
{
    LOG_DEBUG("GNSS META DATA CB");

    char engineStatusArr[32][60] = {
            "QAPI_LOCATION_ENGINE_STATUS_UNKNOWN",
            "QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_START",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_THROTTLED",
            "QAPI_LOCATION_ENGINE_STATUS_NTP_DL_START",
            "QAPI_LOCATION_ENGINE_STATUS_NTP_DL_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_NTP_DL_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_NTP_DL_THROTTLED",
            "QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_START",
            "QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_START",
            "QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_LOAD_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_LOAD_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_UNLOADED",
            "QAPI_LOCATION_ENGINE_STATUS_FIX_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_POS_ACC_CHECK_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_START",
            "QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_GNSS_WEAK_SIGNALS",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_START",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_SUCCESS",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_FAILED",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_THROTTLED",
            "QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_DISABLED",
    };

    LOG_INFO("FLAGS: %p (backoff time 0x1 / nearest geofence 0x2 / engine status 0x10)", metaData.flags);
    if (QAPI_LOCATION_META_DATA_HAS_BACKOFF_TIME & metaData.flags)
    {
        LOG_DEBUG("BACKOFF TIME: %d millis", metaData.backoffTimeMillis);
    }
    if (QAPI_LOCATION_META_DATA_HAS_DISTANCE_TO_NEAREST_GEOFENCE & metaData.flags)
    {
        LOG_DEBUG("NEAREST GEOFENCE: %d meters", metaData.nearestGeofenceMeters);
    }
    if (QAPI_LOCATION_META_DATA_HAS_ENGINE_STATUS & metaData.flags)
    {
        LOG_INFO("ENGINE STATUS: %d [%s]", metaData.engineStatus, engineStatusArr[metaData.engineStatus]);

        gnss_worker_sm_engine_status_cb_meta_data_context(metaData);
    }

    if (QAPI_LOCATION_META_DATA_HAS_AD_STATUS_INFO & metaData.flags)
    {
        if (QAPI_AD_TYPE_TIME & metaData.adStatusInfo.validAdTypeMask)
        {
            //LOG_INFO("Time uncertainty in milliseconds = %u", metaData.adStatusInfo.timeAdStatus.tuncMs);
            gnss_worker_sm_ntp_status_cb_meta_data_context(&(metaData.adStatusInfo.timeAdStatus));
        }

        if (QAPI_AD_TYPE_XTRA & metaData.adStatusInfo.validAdTypeMask)
        {
            //LOG_INFO("xtraAdStatus, xtraDataStartTime = %llu", metaData.adStatusInfo.xtraAdStatus.xtraDataStartTime);
            //LOG_INFO("xtraAdStatus, xtraFileDurationHrs = %u", metaData.adStatusInfo.xtraAdStatus.xtraFileDurationHrs);
            //LOG_INFO("xtraAdStatus, xtraFileGenerationTime = %llu", metaData.adStatusInfo.xtraAdStatus.xtraFileGenerationTime);
        }   
        if (QAPI_AD_TYPE_XTRA_INTEGRITY & metaData.adStatusInfo.validAdTypeMask)
        {
            // LOG_INFO("lastXtraIntDlSuccess in milliseconds = %llu", metaData.adStatusInfo.xtraIntAdStatus.lastXtraIntDlSuccess);
            gnss_worker_sm_xtra_int_status_cb_meta_data_context(&(metaData.adStatusInfo.xtraIntAdStatus));
        }

    }

    gnss_worker_send_msg_cb_meta_data(metaData);
}

/*---------------------------------------------------------
 * NMEA CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_gnss_nmea_cb(qapi_Gnss_Nmea_t nmea)
{
    LOG_DEBUG("GNSS NMEA CB");

    LOG_DEBUG("Timestamp: 0x%x%x", (int)(nmea.timestamp >> 32), (int)nmea.timestamp);
    LOG_DEBUG("NMEA: %s", nmea.nmea);
    LOG_DEBUG("Source: %d", nmea.source);

    gnss_worker_send_msg_cb_nmea(nmea);
}

/*---------------------------------------------------------
 * MOTION TRACKING CALLBACKs
 *--------------------------------------------------------*/
void qapi_loc_wrapper_motion_tracking_cb(qapi_Location_Motion_Info_t info)
{
    LOG_DEBUG("MOTION TRACKING CB");

    const char* motionType[8][50];
    uint64_t ts_sec = info.timestamp/1000;

    motionType[QAPI_LOCATION_MOTION_TYPE_REST][0]             = "QAPI_LOCATION_MOTION_TYPE_REST";
    motionType[QAPI_LOCATION_MOTION_TYPE_MOVE_ABSOLUTE][0]    = "QAPI_LOCATION_MOTION_TYPE_MOVE_ABSOLUTE";
    motionType[QAPI_LOCATION_MOTION_TYPE_MOVE_SIGNIFICANT][0] = "QAPI_LOCATION_MOTION_TYPE_MOVE_SIGNIFICANT";

    LOG_DEBUG("Time(HH:MM:SS): %02d:%02d:%02d",
            (int)((ts_sec/3600)%24), (int)((ts_sec/60)%60), (int)(ts_sec%60));

    LOG_DEBUG("Motion Type %s", motionType[info.motionType][0]);

    gnss_worker_send_msg_cb_motion(info);
}

void qapi_loc_wrapper_client_request_cb(qapi_Client_Request_Info_t clientRequestInfo)
{
    LOG_INFO("Client Request Callback");
    LOG_INFO("Size: %d",clientRequestInfo.size);
    LOG_INFO("requestType: %d", clientRequestInfo.requestType);
    LOG_INFO("requestadType: %d", clientRequestInfo.adType);
    LOG_INFO("requestDataBuffer: %d",clientRequestInfo.requestDataBufferSize);

    gnss_worker_send_msg_cb_singleshot_gtp(clientRequestInfo);
}
