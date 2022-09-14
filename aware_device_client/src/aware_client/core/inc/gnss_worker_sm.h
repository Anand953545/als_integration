/*****************************************************************************
  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
*****************************************************************************/
#ifndef __GNSS_WORKER_SM_H__
#define __GNSS_WORKER_SM_H__

#include <stdint.h>
#include "txm_module.h"
#include "qapi_location.h"
#include "gnss_worker_thread.h"
#include "gnss_worker_utils.h"
#include "app_context.h"

#define BEST_AVAIL_ACC_THRESHOLD_METERS      500
#define SINGLESHOT_SESSION_TIMEOUT_SEC       120

// Init
void gnss_worker_sm_init();

// INCOMING REQUESTS
// Request best available position
void gnss_worker_sm_request_best_avail_location();
void gnss_worker_sm_request_delayed_location(
        uint32 reportDelaySec, qapi_Location_Power_Level_t maxPower,
        qapi_Location_Accuracy_Level_t minAccuracy, uint32 allowPriorityInversion);

// QAPI LOCATION CALLBACKS
void gnss_worker_sm_singleshot_cb(
        qapi_Location_t location, qapi_Location_Error_t err);
void gnss_worker_sm_singleshot_gtp_cb(gtp_data_t gtpdata);
void gnss_worker_sm_meta_data_cb(qapi_Location_Meta_Data_t metaData);
void gnss_worker_sm_motion_info_cb(qapi_Location_Motion_Info_t motionInfo);

// Device info callbacks
void gnss_worker_sm_wwan_sleep_cb(uint32 sleepDuration);
#endif /* __GNSS_WORKER_SM_H__ */
