/*****************************************************************************
  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
*****************************************************************************/
#ifndef __GNSS_WORKER_THREAD_H__
#define __GNSS_WORKER_THREAD_H__

#include <stdint.h>
#include "app_types.h"
#include "qapi_location.h"
#include "app_context.h"

#define DEFAULT_LOCATION_SESSION_MAX_POWER_LEVEL_GNSS       0 // 0 - for GNSS, 1- for GTP
#define DEFAULT_LOCATION_SESSION_MAX_POWER_LEVEL_GTP        1 // 0 - for GNSS, 1- for GTP
#define DEFAULT_LOCATION_SESSION_MIN_ACCURACY_LEVEL    1
#define DEFAULT_ALLOW_SYSTEM_PRIORITY_INVERSION        1
#define REQUEST_GNSS_DATA 0
#define REQUEST_GTP_DATA 1

// Messages that can be posted to this thread
typedef enum
{
    GNSS_WORKER_MSG_ID_MIN,

    // Incoming requests to this thread
    GNSS_WORKER_MSG_ID_REQUEST_BEST_AVAIL_LOCATION,
    GNSS_WORKER_MSG_ID_REQUEST_DELAYED_LOCATION,
    GNSS_WORKER_MSG_ID_REQUEST_TRACKING_START,
    GNSS_WORKER_MSG_ID_REQUEST_TRACKING_STOP,
    GNSS_WORKER_MSG_ID_REQUEST_GEOFENCE_ADD,
    GNSS_WORKER_MSG_ID_REQUEST_GEOFENCE_REMOVE,

    // Callback from location stack
    GNSS_WORKER_MSG_ID_CB_TRACKING,
    GNSS_WORKER_MSG_ID_CB_SINGLESHOT,
    GNSS_WORKER_MSG_ID_CB_SINGLESHOT_GTP,
    GNSS_WORKER_MSG_ID_CB_META_DATA,
    GNSS_WORKER_MSG_ID_CB_MOTION_TRACKING,
    GNSS_WORKER_MSG_ID_CB_NMEA,
    GNSS_WORKER_MSG_ID_CB_GEOFENCE_BREACH,

    // Device info callbacks
    GNSS_WORKER_MSG_ID_CB_WWAN_SLEEP,

    // Generic message to process callback in gnss worker context
    GNSS_WORKER_MSG_ID_PROCESS_CB,

    GNSS_WORKER_MSG_ID_MAX
} gnss_worker_msg_id;

void gnss_worker_task_entry(unsigned long args);
char* gnss_worker_get_task_name();
int gnss_worker_get_task_id();

// Synchronous wrappers on top of message processing in this thread
void gnss_worker_sync_get_best_avail_location(qapi_Location_t* locationOutPtr);

// Send messages to this thread, which will be processed after a
// context switch to this thread.
void gnss_worker_send_msg_request_delayed_location(
        uint32 reportDelaySec, qapi_Location_Power_Level_t maxPower,
        qapi_Location_Accuracy_Level_t minAccuracy, uint32 allowPriorityInversion);

// Meant for context switch from location module callback to GNSS Worker thread
void gnss_worker_send_msg_cb_singleshot_gtp(
        qapi_Client_Request_Info_t clientRequestInfo);
void gnss_worker_send_msg_cb_tracking(qapi_Location_t location);
void gnss_worker_send_msg_cb_singleshot(qapi_Location_t location, qapi_Location_Error_t err);
void gnss_worker_send_msg_cb_meta_data(qapi_Location_Meta_Data_t metaData);
void gnss_worker_send_msg_cb_motion(qapi_Location_Motion_Info_t motionInfo);
void gnss_worker_send_msg_cb_nmea(qapi_Gnss_Nmea_t nmea);
void gnss_worker_send_msg_cb_geofence_breach(
        qapi_Location_t location, qapi_Geofence_Breach_t type, uint64_t timestamp);

void gnss_worker_send_msg_cb_wwan_sleep(uint32 sleepDuration);

void gnss_worker_send_msg_request_tracking_start(
        uint32 tbf, uint32 dbf, qapi_Location_Accuracy_Level_t accuracy,
        uint32 allowPriorityInversion);
void gnss_worker_send_msg_request_tracking_stop();

void gnss_worker_send_msg_request_add_geofence(
        double latitude, double longitude, double radius,
        qapi_Geofence_Breach_Mask_t mask, uint32 responsiveness, uint32 dwellTime,
        uint32 allowPriorityInversion);
void gnss_worker_send_msg_request_remove_geofence();

// Msg to process specified callback in gnss worker thread context
typedef void (*gnss_worker_thread_cb_type)(uint32 cbData);
void gnss_worker_send_msg_process_cb(gnss_worker_thread_cb_type callback, uint32 cbData);

// Incoming calls from GNSS Worker SM Module
void gnss_worker_report_best_avail_location(qapi_Location_t location);
void gnss_worker_report_singleshot_location(qapi_Location_t location);
void gnss_worker_report_singleshot_gtpdata(gtp_data_t gtpdata);

#endif /* __GNSS_WORKER_THREAD_H__ */
