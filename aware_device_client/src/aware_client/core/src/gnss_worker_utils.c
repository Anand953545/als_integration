/*****************************************************************************
 Copyright (c) 2020 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "app_utils_misc.h"
#include "qapi_loc_wrapper.h"
#include "qapi_timer.h"
#include "aware_log.h"

// hacky fix for redefinition error
#undef true
#undef false
#include "qapi_device_info.h"

// Internal QAPI handles
static qapi_Device_Info_Hndl_t gDeviceInfoHandle = NULL;
static uint8 gIsDefaultAppPriorityGnss = 0;
static uint8 gIsWwanSleepCbSet = 0;

static char _GnssWorkerStateArr[12][40] =
{
    "UNKNOWN",
    "IDLE",
    "WAITING_FOR_GNSS_LOAD",
    "WAITING_FOR_AD_DL_COMPLETE",
    "AD_DL_COMPLETE",
    "GNSS_LOADED",
    "BACKOFF_BAD_WWAN_CONNECTION",
    "BACKOFF_WEAK_GNSS_SIGNALS",
    "BACKOFF_WWAN_HIGH_PRIORITY",
    "GNSS_FIX_GENERATED",
    "GNSS_RETRY",
    "WAITING_FOR_NON_GNSS_POSITION",
};

const char* app_utils_gnss_worker_state_str(gnss_worker_state state)
{
    return _GnssWorkerStateArr[state];
}

const char* POWER_HIGH_STR = "POWER_HIGH";
const char* POWER_MED_STR = "POWER_MED";
const char* POWER_LOW_STR = "POWER_LOW";
const char* POWER_INVALID_STR = "POWER_INVALID";
const char* app_utils_power_level_str(qapi_Location_Power_Level_t power)
{
    if (QAPI_LOCATION_POWER_HIGH == power)
    {
        return POWER_HIGH_STR;
    }
    if (QAPI_LOCATION_POWER_MED == power)
    {
        return POWER_MED_STR;
    }
    if (QAPI_LOCATION_POWER_LOW == power)
    {
        return POWER_LOW_STR;
    }
    return POWER_INVALID_STR;
}

const char* ACCURACY_HIGH_STR = "ACCURACY_HIGH";
const char* ACCURACY_MED_STR = "ACCURACY_MED";
const char* ACCURACY_LOW_STR = "ACCURACY_LOW";
const char* ACCURACY_UNKNOWN_STR = "ACCURACY_UNKNOWN";
const char* ACCURACY_INVALID_STR = "ACCURACY_INVALID";
const char* app_utils_accuracy_level_str(qapi_Location_Accuracy_Level_t accuracy)
{
    if (QAPI_LOCATION_ACCURACY_HIGH == accuracy)
    {
        return ACCURACY_HIGH_STR;
    }
    if (QAPI_LOCATION_ACCURACY_MED == accuracy)
    {
        return ACCURACY_MED_STR;
    }
    if (QAPI_LOCATION_ACCURACY_LOW == accuracy)
    {
        return ACCURACY_LOW_STR;
    }
    if (QAPI_LOCATION_ACCURACY_UNKNOWN == accuracy)
    {
        return ACCURACY_UNKNOWN_STR;
    }
    return ACCURACY_INVALID_STR;
}

void app_utils_log_engine_status_recvd(
        gnss_worker_state state,
        qapi_Location_Meta_Data_Engine_Status_t status)
{
    switch(status)
    {
    case QAPI_LOCATION_ENGINE_STATUS_UNKNOWN:
        LOG_DEBUG("Recvd Engine Status [UNKNOWN] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [DATA_CALL_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_FAILED:
        LOG_DEBUG("Recvd Engine Status [DATA_CALL_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_START:
        LOG_DEBUG("Recvd Engine Status [XTRA_DL_START] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [XTRA_DL_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_FAILED:
        LOG_DEBUG("Recvd Engine Status [XTRA_DL_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_THROTTLED:
        LOG_DEBUG("Recvd Engine Status [XTRA_DL_THROTTLED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_START:
        LOG_DEBUG("Recvd Engine Status [NTP_DL_START] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [NTP_DL_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_FAILED:
        LOG_DEBUG("Recvd Engine Status [NTP_DL_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_THROTTLED:
        LOG_DEBUG("Recvd Engine Status [NTP_DL_THROTTLED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_START:
        LOG_DEBUG("Recvd Engine Status [GTPWWAN_POS_START] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [GTPWWAN_POS_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_FAILED:
        LOG_DEBUG("Recvd Engine Status [GTPWWAN_POS_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_START:
        LOG_DEBUG("Recvd Engine Status [WIFI_POS_START] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [WIFI_POS_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_FAILED:
        LOG_DEBUG("Recvd Engine Status [WIFI_POS_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_LOAD_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [LOAD_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_LOAD_FAILED:
        LOG_DEBUG("Recvd Engine Status [LOAD_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_UNLOADED:
        LOG_DEBUG("Recvd Engine Status [UNLOADED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_FIX_FAILED:
        LOG_DEBUG("Recvd Engine Status [FIX_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_POS_ACC_CHECK_FAILED:
        LOG_DEBUG("Recvd Engine Status [POS_ACC_CHECK_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [FIX_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_START:
        LOG_DEBUG("Recvd Engine Status [GTPWWAN_MAINT_START] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_SUCCESS:
        LOG_DEBUG("Recvd Engine Status [GTPWWAN_MAINT_SUCCESS] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_FAILED:
        LOG_DEBUG("Recvd Engine Status [GTPWWAN_MAINT_FAILED] (%d) in state (%d)", status, state);
        break;
    case QAPI_LOCATION_ENGINE_STATUS_WEAK_GNSS_SIGNALS:
        LOG_DEBUG("Recvd Engine Status [WEAK_GNSS_SIGNALS] (%d) in state (%d)", status, state);
        break;
    default:
        LOG_DEBUG("Recvd Engine Status [INVALID] (%d) in state (%d)", status, state);
        LOG_ERROR("Invalid engine status received");
        break;
    }
}

static void _gnss_worker_utils_init_device_info()
{
    if (NULL == gDeviceInfoHandle)
    {
        qapi_Status_t res = qapi_Device_Info_Init_v2(&gDeviceInfoHandle);
        if (QAPI_OK != res)
        {
            LOG_ERROR("qapi_Device_Info_Init_v2 failed");
            return;
        }
        else
        {
            LOG_INFO("qapi_Device_Info_Init_v2 success.");
        }

        res = qapi_Device_Info_Pass_Pool_Ptr(gDeviceInfoHandle, app_utils_get_byte_pool());
        if(QAPI_OK != res)
        {
            LOG_ERROR("qapi_Device_Info_Pass_Pool_Ptr returned error %d", res);
            return;
        }
        else
        {
            LOG_INFO("qapi_Device_Info_Pass_Pool_Ptr success.");
        }
    }
}

void gnss_worker_utils_set_app_priority(uint8 setGnssHighPriority)
{
    _gnss_worker_utils_init_device_info();

    int priorityValue[1] = {};
    // 0 = GNSS , 1 = WWAN
    if (setGnssHighPriority)
    {
        LOG_INFO("Setting GNSS High priority");
        priorityValue[0] = 0;  // GNSS
    }
    else
    {
        LOG_INFO("Setting WWAN High priority");
        priorityValue[0] = 1;  // WWAN
    }
    qapi_Status_t res = qapi_Device_Info_Set(
            gDeviceInfoHandle, QAPI_DEVICE_INFO_SET_APP_PRIORITY_E, 1, priorityValue);

    if (QAPI_OK == res)
    {
        LOG_INFO("Priority change successful.");
    }
    else
    {
        LOG_ERROR("Priority change failed.");
    }
}

void gnss_worker_utils_set_gnss_high_priority()
{
    gnss_worker_utils_set_app_priority(1);
}
void gnss_worker_utils_set_wwan_high_priority()
{
    gnss_worker_utils_set_app_priority(0);
}

uint8 gnss_worker_utils_is_gnss_high_priority()
{
    _gnss_worker_utils_init_device_info();

    // 0 = GNSS , 1 = WWAN
    qapi_Device_Info_t deviceInfo = {};
    qapi_Status_t res = qapi_Device_Info_Get_v2(
            gDeviceInfoHandle, QAPI_DEVICE_INFO_GET_APP_PRIORITY_E, &deviceInfo);
    if (QAPI_OK == res)
    {
        if (deviceInfo.u.valueint == QAPI_DEVICE_INFO_GNSS_PRIORITY)
        {
            return 1;
        }
        else if (deviceInfo.u.valueint == QAPI_DEVICE_INFO_WWAN_PRIORITY)
        {
            return 0;
        }
        else
        {
            LOG_ERROR("Invalid App priority fetched from device info QAPI");
        }
    }
    else
    {
        LOG_ERROR("Failed to get current app priority");
    }

    return 0;
}

void gnss_worker_utils_restore_default_app_priority()
{
    uint8 isCurrentPriorityGnss = gnss_worker_utils_is_gnss_high_priority();

    if (isCurrentPriorityGnss && !gIsDefaultAppPriorityGnss)
    {
        LOG_INFO("Restoring app priority to default value [WWAN]");
        gnss_worker_utils_set_wwan_high_priority();
    }

    if (!isCurrentPriorityGnss && gIsDefaultAppPriorityGnss)
    {
        LOG_INFO("Restoring app priority to default value [GNSS]");
        gnss_worker_utils_set_gnss_high_priority();
    }
}

void gnss_worker_utils_log_state_event(
        gnss_worker_state state, gnss_worker_event event,
        qapi_Location_Meta_Data_Engine_Status_t status)
{
    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        LOG_DEBUG("Entering State [%s] (%d)",
                app_utils_gnss_worker_state_str(state), state);
    }

    if (event == GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE)
    {
        app_utils_log_engine_status_recvd(state, status);
    }

    if (event == GNSS_WORKER_EVENT_SINGLESHOT_TIMEOUT)
    {
        LOG_INFO("Recvd [Singleshot Timeout] in state (%d)", state);
    }

    if (event == GNSS_WORKER_EVENT_ABSOLUTE_MOTION)
    {
        LOG_INFO("Recvd [Motion] in state (%d)", state);
    }
}

void gnss_worker_utils_init()
{
    if (gnss_worker_utils_is_gnss_high_priority())
    {
        LOG_INFO("Default APP Priority is GNSS");
        gIsDefaultAppPriorityGnss = 1;
    }
    else
    {
        LOG_INFO("Default APP Priority is WWAN");
        gIsDefaultAppPriorityGnss = 0;
    }
}

static void _gnss_worker_utils_wwan_sleep_callback(
        qapi_Device_Info_Hndl_t deviceInfoHandle, const qapi_Device_Info_t* info)
{
    LOG_DEBUG("WWAN Sleep callback :: hndl %p info %p", deviceInfoHandle, info);

    // Validate callback parameters
    if (deviceInfoHandle != gDeviceInfoHandle)
    {
        LOG_ERROR("Invalid device info handle in callback.");
        return;
    }
    if (info->id != QAPI_DEVICE_INFO_WWAN_SLEEP_INFO_E)
    {
        LOG_ERROR("Unsolicited info id %d", info->id);
        return;
    }
    if (info->info_type != QAPI_DEVICE_INFO_TYPE_INTEGER_E)
    {
        LOG_ERROR("Invalid device info type %d", info->info_type);
        return;
    }

    uint32 sleepDuration = ((uint32)info->u.valueint)/1000;
    LOG_DEBUG("WWAN Sleep Duration = %d seconds", sleepDuration);

    // send message to gnss worker thread
    gnss_worker_send_msg_cb_wwan_sleep(sleepDuration);
}

void gnss_worker_utils_set_wwan_sleep_callback()
{
    _gnss_worker_utils_init_device_info();

    if (gIsWwanSleepCbSet == 0)
    {
        qapi_Status_t res = qapi_Device_Info_Set_Callback_v2(
                gDeviceInfoHandle, QAPI_DEVICE_INFO_WWAN_SLEEP_INFO_E,
                _gnss_worker_utils_wwan_sleep_callback);
        if (QAPI_OK == res)
        {
            LOG_INFO("Set wwan sleep callback success.");
        }
        else
        {
            LOG_ERROR("Failed to set wwan sleep callback.");
        }

        gIsWwanSleepCbSet = 1;
    }
}