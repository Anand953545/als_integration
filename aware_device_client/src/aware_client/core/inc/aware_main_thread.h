/*****************************************************************************
  Copyright (c) 2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
*****************************************************************************/
#ifndef __AWARE_MAIN_THREAD_H__
#define __AWARE_MAIN_THREAD_H__

#include <stdint.h>
#include "qapi_location.h"
#include "app_context.h"

typedef enum
{
    AWARE_MAIN_THREAD_MSG_ID_MIN,

    AWARE_MAIN_THREAD_MSG_ID_INIT,
    AWARE_MAIN_THREAD_MSG_ID_REPORT_LOCATION,
    AWARE_MAIN_THREAD_MSG_ID_REPORT_GTP_DATA,
    AWARE_MAIN_THREAD_MSG_ID_REPORT_SENSOR_DATA,
    AWARE_MAIN_THREAD_MSG_ID_CONFIG_UPDATE,

    AWARE_MAIN_THREAD_MSG_ID_MAX
} AwareMainThreadMsgId;

void aware_main_thread_task_entry(unsigned long args);
char* aware_main_thread_get_task_name(void);
int aware_main_thread_get_task_id(void);

// Send messages to this thread, which will be processed after a
// context switch to this thread.
void aware_main_thread_send_msg_init(void);
void aware_main_thread_send_msg_report_location(qapi_Location_t location);
void aware_main_thread_send_msg_report_gtpdata(gtp_data_t gtpdata);
void aware_main_thread_send_msg_report_sensor_data(sensor_data_t sensor_data);
void aware_main_thread_send_msg_config_update(void* msg, size_t msgSize);

#endif /* __AWARE_MAIN_THREAD_H__ */
