/******************************************************************************
   Copyright (c) 2020 Qualcomm Technologies, Inc.
   All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 *******************************************************************************/
#include "app_types.h"
#include "app_utils_misc.h"

#include "qapi_timer.h"
#include "aware_log.h"

void app_utils_timer_init(
        void** timerHandleOut, app_utils_timer_cb_type cbPtr, uint32 cbData)
{
    if (NULL != *timerHandleOut)
    {
        LOG_ERROR("Timer already initialized");
        return;
    }

    qapi_TIMER_define_attr_t defineAttr = {};

    defineAttr.deferrable = FALSE;
    defineAttr.cb_type = QAPI_TIMER_FUNC1_CB_TYPE;
    defineAttr.sigs_func_ptr = cbPtr;
    defineAttr.sigs_mask_data = cbData;

    qapi_Status_t res = qapi_Timer_Def(timerHandleOut, &defineAttr);
    if(QAPI_OK != res)
    {
        LOG_ERROR("Failed to define timer");
    }
}

void app_utils_timer_start(void* timerHandle, uint32 sec)
{
    if (NULL == timerHandle || sec == 0)
    {
        LOG_ERROR("Invalid args to start timer %p %d", timerHandle, sec);
        return;
    }

    qapi_TIMER_set_attr_t setAttr = {};

    setAttr.unit = QAPI_TIMER_UNIT_SEC;
    setAttr.reload = TRUE;
    setAttr.time = sec;

    qapi_Status_t res = qapi_Timer_Set(timerHandle, &setAttr);
    if(QAPI_OK != res)
    {
        LOG_ERROR("Failed to start timer.");
    }
}

void app_utils_timer_stop(void* timerHandle)
{
    qapi_Status_t res = qapi_Timer_Stop(timerHandle);
    if(QAPI_OK != res)
    {
        LOG_ERROR("Failed to stop timer");
    }
}

void app_utils_timer_delete(void** timerHandle)
{
    qapi_Status_t res = qapi_Timer_Undef(*timerHandle);
    if(QAPI_OK != res)
    {
        LOG_ERROR("Failed to stop timer");
    }
    *timerHandle = NULL;
}
