/*****************************************************************************
  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
*****************************************************************************/
#ifndef __GNSS_WORKER_UTILS_H__
#define __GNSS_WORKER_UTILS_H__

#include <stdint.h>
#include "gnss_worker_thread.h"

// STATES
typedef enum
{
    GNSS_WORKER_STATE_UNKNOWN = 0,

    GNSS_WORKER_STATE_IDLE,
    GNSS_WORKER_STATE_WAITING_FOR_GNSS_LOAD,
    GNSS_WORKER_STATE_WAITING_FOR_AD_DL_COMPLETE,
    GNSS_WORKER_STATE_AD_DL_COMPLETE,
    GNSS_WORKER_STATE_GNSS_LOADED,
    GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION,
    GNSS_WORKER_STATE_BACKOFF_WEAK_GNSS_SIGNALS,
    GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY,
    GNSS_WORKER_STATE_GNSS_FIX_GENERATED,
    GNSS_WORKER_STATE_GNSS_RETRY,

    GNSS_WORKER_STATE_WAITING_FOR_NON_GNSS_POSITION,

} gnss_worker_state;

// AD DL States
typedef enum
{
    GNSS_WORKER_AD_DL_STATE_IDLE = 0,

    GNSS_WORKER_AD_DL_STATE_ONGOING,
    GNSS_WORKER_AD_DL_STATE_SUCCESS,
    GNSS_WORKER_AD_DL_STATE_FAILED,

} gnss_worker_ad_dl_state;

// EVENTS
typedef enum
{
    GNSS_WORKER_EVENT_UNKNOWN = 0,

    GNSS_WORKER_EVENT_STATE_ENTRY,

    GNSS_WORKER_EVENT_SINGLESHOT_REQUEST,
    GNSS_WORKER_EVENT_TRACKING_START_REQUEST,
    GNSS_WORKER_EVENT_GEOFENCE_ADD_REQUEST,

    GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE,
    GNSS_WORKER_EVENT_SINGLESHOT_TIMEOUT,
    GNSS_WORKER_EVENT_ABSOLUTE_MOTION,

    GNSS_WORKER_EVENT_NMEA_CB,
    GNSS_WORKER_EVENT_GEOFENCE_BREACH_CB,
    GNSS_WORKER_EVENT_WWAN_SLEEP_CB,

    GNSS_WORKER_EVENT_REPORT_DELAY_TIMER_EXPIRY,
    GNSS_WORKER_EVENT_BACKOFF_TIMER_EXPIRY,
    GNSS_WORKER_EVENT_POSITION_REFRESH_TIMER_EXPIRY,
    GNSS_WORKER_EVENT_AD_DL_COMPLETE_TIMER_EXPIRY,

} gnss_worker_event;


void gnss_worker_utils_set_app_priority(uint8 setGnssHighPriority);
void gnss_worker_utils_set_gnss_high_priority();
void gnss_worker_utils_set_wwan_high_priority();
uint8 gnss_worker_utils_is_gnss_high_priority();
void gnss_worker_utils_restore_default_app_priority();

void gnss_worker_utils_log_state_event(
        gnss_worker_state state, gnss_worker_event event,
        qapi_Location_Meta_Data_Engine_Status_t status);

void gnss_worker_utils_init();

void gnss_worker_utils_set_wwan_sleep_callback();

#endif /* __GNSS_WORKER_UTILS_H__ */
