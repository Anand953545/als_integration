/*****************************************************************************
  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
*****************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "app_thread.h"
#include "app_utils_misc.h"
#include "aware_main_thread.h"

#include "gnss_worker_thread.h"
#include "data_mgr_thread.h"
#include "fota_thread.h"

#include "aware_log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
#include "aware_app.h"

#undef true
#undef false
#include "pb_encode.h"
#include "location.pb.h"
#include "sensor_mgr_thread.h"

/* Thread Constants */
static app_thread_id _AwareMainTaskId = APP_THREAD_ID_AWARE_MAIN;
static char* _AwareMainTaskName = "aware_main_thread";

static qapi_TIMER_handle_t periodic_checkin_timer;

static void _aware_main_thread_location_request(uint32 pos_type);
static void periodic_checkin_timer_expiry_cb(uint32 userData);
static void _aware_main_thread_dam_download_request(void);

static int periodic_checkin_counter = 0;
qapi_Location_t location;

/*-------------------------------------------------------------------------*/
/**
  @brief    Get task Id
  @return   Aware mai thread task Id
 */
/*--------------------------------------------------------------------------*/
int aware_main_thread_get_task_id()
{
    return _AwareMainTaskId;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get task name
  @return   Aware main thread task name
 */
/*--------------------------------------------------------------------------*/
char* aware_main_thread_get_task_name()
{
    return _AwareMainTaskName;
}

// Internal struct to allow passing message data to this thread
typedef struct controller_msg
{
    // report location
    qapi_Location_t location;
    //report gtp data
    gtp_data_t gtpdata;
    //sensor data
    sensor_data_t sensor_data;
} controller_msg;

/*-------------------------------------------------------------------------*/
/**
  @brief    Encode field and string
  @param    field    Field header to be encode
  @param    stream   String to be encode
  @param    arg      String length
  @return   1 on success, 0 on failure
 */
/*--------------------------------------------------------------------------*/
bool write_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_string(stream, *arg, strlen(*arg));
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Convenience functions to send messages to this local controller thread
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_main_thread_send_msg_init()
{
    app_thread_send_msg(
            _AwareMainTaskId,
            AWARE_MAIN_THREAD_MSG_ID_INIT, NULL, 0);
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Send location info to thread
  @param    location Structure variable where all location information are available
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_main_thread_send_msg_report_location(qapi_Location_t location)
{
    controller_msg* msg = NULL;
    size_t msgSize = sizeof(controller_msg);

    int32 res = app_utils_byte_allocate((void**)&msg, msgSize);
    if (0 == res)
    {
        memset(msg, 0, msgSize);
        msg->location = location;

        app_thread_send_msg(
                _AwareMainTaskId,
                AWARE_MAIN_THREAD_MSG_ID_REPORT_LOCATION,
                msg, msgSize);

        app_utils_byte_release((void*)msg);
    }
    else
    {
        LOG_ERROR("Failed to allocate %d bytes", msgSize);
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Send gtp data to thread
  @param    gtpdata Structure variable where all gtp data information are available
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_main_thread_send_msg_report_gtpdata(gtp_data_t gtpdata)
{
    controller_msg* msg = NULL;
    size_t msgSize = sizeof(controller_msg);

    int32 res = app_utils_byte_allocate((void**)&msg, msgSize);
    if (0 == res)
    {
        memset(msg, 0, msgSize);

        msg->gtpdata = gtpdata;

        // LOG_INFO("gtpsize before sending msg to main thread = %d", gtpdata.dataBufferSize);
        // LOG_INFO("gtpbufferptr before sending msg to main thread = %d", gtpdata.dataBufferPtr);

        app_thread_send_msg(
                _AwareMainTaskId,
                AWARE_MAIN_THREAD_MSG_ID_REPORT_GTP_DATA,
                msg, msgSize);

        app_utils_byte_release((void*)msg);
    }
    else
    {
        LOG_ERROR("Failed to allocate %d bytes", msgSize);
    }
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Send sensor data to thread
  @param    sensor Structure variable
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_main_thread_send_msg_report_sensor_data(sensor_data_t sensor_data)
{
    controller_msg* msg = NULL;
    size_t msgSize = sizeof(controller_msg);

    int32 res = app_utils_byte_allocate((void**)&msg, msgSize);
    if (0 == res)
    {
        memset(msg, 0, msgSize);
        msg->sensor_data = sensor_data;

        app_thread_send_msg(
                _AwareMainTaskId,
                AWARE_MAIN_THREAD_MSG_ID_REPORT_SENSOR_DATA,
                msg, msgSize);

        app_utils_byte_release((void*)msg);
    }
    else
    {
        LOG_ERROR("Failed to allocate %d bytes", msgSize);
    }
}

void aware_main_thread_send_msg_config_update(void* msg, size_t msgSize)
{
    app_thread_send_msg(
        _AwareMainTaskId,
        AWARE_MAIN_THREAD_MSG_ID_CONFIG_UPDATE,
        msg, msgSize);
}

static void _aware_main_thread_handle_msg_init(void* dataPtr, size_t dataSize)
{
    LOG_DEBUG("Aware Main Init");

    //send activation message to aware cloud
    if(app_ctx.runtime_cfg.last_app_state == APP_STATE_UNPROVISIONED) {
        data_mgr_publish_activation_message();
    }

    //start the periodic checkin timer
    periodic_checkin_timer = start_app_timer(5, &periodic_checkin_timer_expiry_cb);
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Request for download the dam app
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _aware_main_thread_dam_download_request()
{
    fota_thread_send_msg_dam_download();
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Request for location report
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _aware_main_thread_location_request(uint32 pos_type)
{
    
    uint32 power_level = 0;
    if (pos_type == REQUEST_GNSS_DATA)
    {
        power_level = DEFAULT_LOCATION_SESSION_MAX_POWER_LEVEL_GNSS;
    }
    else if (pos_type ==  REQUEST_GTP_DATA)
    {
        power_level = DEFAULT_LOCATION_SESSION_MAX_POWER_LEVEL_GTP;
    }
    else
    {
        // do nothing
    }
    gnss_worker_send_msg_request_delayed_location(
            app_ctx.app_cfg.active_normal_checkin_period * 60,
            power_level,
            DEFAULT_LOCATION_SESSION_MIN_ACCURACY_LEVEL,
            DEFAULT_ALLOW_SYSTEM_PRIORITY_INVERSION);
    
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Update location msg
  @param    data    pointer to msg string
  @param    size    string size
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _aware_main_thread_handle_msg_report_location(void* data, size_t size)
{
    LOG_DEBUG("Aware Main Thread: Report Location");

    // validate
    controller_msg* msg = (controller_msg*)data;
    if (msg == NULL || size != sizeof(controller_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // Extract parameters
    location = msg->location;

    app_utils_log_location(location);
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Update gtp msg
  @param    data    pointer to msg string
  @param    size    string size
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _aware_main_thread_handle_msg_report_gtpdata(void* data, size_t size)
{
    LOG_DEBUG("Aware Main Thread: Report GTP Data");

    // validate
    controller_msg* msg = (controller_msg*)data;
    if (msg == NULL || size != sizeof(controller_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // LOG_INFO("gtpsize after receiving msg in main thread = %d", gtpdata.dataBufferSize);
    // LOG_INFO("gtpbufferptr after receiving msg in main thread = %d", gtpdata.dataBufferPtr);

    app_utils_log_gtpdata(msg->gtpdata);

    // Need to release gtpdatabuffer after logging, if not will lead to memory leak.
    app_utils_byte_release((void*)msg->gtpdata.dataBufferPtr);
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Update sensor data msg
  @param    data    pointer to msg string
  @param    size    string size
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void _aware_main_thread_handle_msg_report_sensor_data(void* data, size_t size)
{
    uint8_t pb_buffer[128];
    size_t message_length;
    bool status;

    LOG_DEBUG("Aware Main Thread: Report Sensor Data");

    controller_msg* msg = (controller_msg*)data;
    if (msg == NULL || size != sizeof(controller_msg))
    {
        LOG_ERROR("Invalid msg");
        return;
    }

    // Extract parameters
    sensor_data_t sensor_data = msg->sensor_data;

    char buffer[16] = {0};

    size_t msg_len = sizeof(buffer);
    uint64_t time_ms = get_current_time_in_milliseconds();

    LocationMessage message = LocationMessage_init_zero;

    message.aware_device_id.funcs.encode = &write_string;
    message.aware_device_id.arg = app_ctx.dhu_cfg.device_id;

    message.has_message_id = true;
    message.message_id = 2;

    message.has_sequence_number = true;
    message.sequence_number = 1;

    message.has_retry_number = true;
    message.retry_number = 0;

    message.has_timestamp = true;
    message.timestamp = time_ms;

    message.has_time_offset_fix = true;
    message.time_offset_fix = 0;

    message.location_type.funcs.encode = &write_string;
    message.location_type.arg = "LatLong";
    
    memset(buffer, 0, msg_len);
    snprintf(buffer, msg_len, "%d.%05d", 
      (int) location.latitude, (abs((int)(location.latitude * 100000))) % 100000);

    message.has_latitude = true;
    message.latitude = app_utils_string_to_float(buffer);

    memset(buffer, 0, msg_len);
    snprintf(buffer, msg_len, "%d.%05d", 
      (int) location.longitude, (abs((int)(location.longitude * 100000))) % 100000);

    message.has_longitude = true;
    message.longitude = app_utils_string_to_float(buffer);

    memset(buffer, 0, msg_len);
    snprintf(buffer, msg_len, "%d.%02d", 
      (int) location.altitude, (abs((int)(location.altitude * 100))) % 100);

    message.has_altitude = true;
    message.altitude = app_utils_string_to_float(buffer);

    message.has_als = true;
    message.als = sensor_data.als;

    message.has_temperature = true;
    message.temperature = sensor_data.temperature;

    message.has_pressure = true;
    message.pressure = sensor_data.pressure;

    message.has_humidity = true;
    message.humidity = sensor_data.humidity;

    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));

    /* Now we are ready to encode the message! */
    status = pb_encode(&stream, LocationMessage_fields, &message);
    message_length = stream.bytes_written;

    LOG_DEBUG("Telemetry Msg Length: %d", message_length);
    data_mgr_thread_send_msg_periodic_checkin(pb_buffer, message_length);
}

static void _aware_main_thread_handle_msg_config_update(void* msg, size_t msgSize)
{
    char* pch = NULL;

    LOG_DEBUG("Aware Main Thread: Config Update");

    cfg_mgr_apply_update((char*)msg, msgSize);

    //reset timer
    if(NULL != (pch = strstr((char*)msg, AWARE_ENDPOINT_URL_CONFIG_KEY))) {
        restart_app_timer(periodic_checkin_timer, 5);
    } else if(NULL != (pch = strstr((char*)msg, "a.c.ci.i.p"))) {
        LOG_DEBUG("Aware Main Thread: Restart timer");
        //restart the timer
        restart_app_timer(periodic_checkin_timer, app_ctx.app_cfg.active_normal_checkin_period * 60);
    }
}

static void _aware_main_thread_process_msg(int msgId, void *msgPtr, size_t msgSize)
{
    LOG_DEBUG("Aware Main Thread: received message %d", msgId);

    // Make sure to free any data pointer within the message
    // As part of handling the message..
    switch (msgId)
    {
        case AWARE_MAIN_THREAD_MSG_ID_INIT:
            _aware_main_thread_handle_msg_init(msgPtr, msgSize);
            break;
        case AWARE_MAIN_THREAD_MSG_ID_REPORT_LOCATION:
            _aware_main_thread_handle_msg_report_location(msgPtr, msgSize);
            
            break;
        case AWARE_MAIN_THREAD_MSG_ID_REPORT_GTP_DATA:
            _aware_main_thread_handle_msg_report_gtpdata(msgPtr, msgSize);
            // LOG_INFO("*********** Requesting GNSS Pos*************")
            _aware_main_thread_location_request(REQUEST_GNSS_DATA);
            break;
        case AWARE_MAIN_THREAD_MSG_ID_REPORT_SENSOR_DATA:
            _aware_main_thread_handle_msg_report_sensor_data(msgPtr, msgSize);
            break;
        case AWARE_MAIN_THREAD_MSG_ID_CONFIG_UPDATE:
            _aware_main_thread_handle_msg_config_update(msgPtr, msgSize);
            break;
        default :
            LOG_ERROR("Invalid msgId %d", msgId);
    }

    LOG_DEBUG("Aware Main Thread: message %d processing complete", msgId);
}

/*-------------------------------------------------------------------------*/
/**
  @brief	This is the entry point for Local Controller task
  @param    args  TID
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_main_thread_task_entry(ULONG args)
{
    AppMsg *payload = NULL;

    LOG_INFO("Aware Main Thread started. TCB %x TID %d", tx_thread_identify(), args);

    while(0 == app_thread_rcv_msg(args, (void **)&payload))
    {
        _aware_main_thread_process_msg(payload->msgId, payload->msg, payload->msgSize);
        if (payload->msg != NULL)
        {
            app_utils_byte_release(payload->msg);
        }
        app_utils_byte_release(payload);
    }
}

/*-------------------------------------------------------------------------*/
/**
  @brief	periodic checkin timer expiry callback
  @param    userData
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void periodic_checkin_timer_expiry_cb(uint32 userData)
{
    //publish scripted data on demo mode
    if(app_ctx.dhg_cfg.demo_mode)
    {
        data_mgr_report_scripted_location();
    } else {
	    if(periodic_checkin_counter == 0) {
	        LOG_INFO("softversion%s:",app_ctx.dhc_cfg.sw_version);  
	        if(strcasecmp(app_ctx.dhc_cfg.sw_version, "1.0.0") == 0) {
	            _aware_main_thread_dam_download_request();
	        } else {
	            _aware_main_thread_location_request(REQUEST_GTP_DATA);
	        }
	    } else {
	        _aware_main_thread_location_request(REQUEST_GTP_DATA);
		}
    }

    periodic_checkin_counter++;

    //restrt the timer
    restart_app_timer(periodic_checkin_timer, app_ctx.app_cfg.active_normal_checkin_period*60);

    return;
}