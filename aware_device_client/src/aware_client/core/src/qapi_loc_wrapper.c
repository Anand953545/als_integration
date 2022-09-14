/*****************************************************************************
 Copyright (c) 2020 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 *****************************************************************************/
#include "qapi_loc_wrapper.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "app_types.h"
#include "app_thread.h"
#include "app_utils_misc.h"
#include "qapi_timer.h"
#include "txm_module.h"
#include "qapi_location.h"
#include "aware_log.h"

// User buffer provided to location QAPI for callbacks
#define USER_BUFFER_SIZE 4096
static uint8_t _UserBuffer[USER_BUFFER_SIZE];

// Callbacks provided to location QAPI
static qapi_Location_Callbacks_t _LocationCallbacks;
static qapi_Location_Client_Options_t _LocClientOptions;

// client id obtained from loc init call
static qapi_loc_client_id _ClientId = (qapi_loc_client_id)-1;
static uint32 _SingleshotSessionId = (uint32)-1;
static uint32 _TrackingSessionId = 0;
static uint32 _BestAvailSessionId = 0;
static uint32 _MotionTrackingSessionId = 0;
static uint32* _GeofenceIdArray = NULL;

// Mutex and signals
TX_MUTEX* _MutexResponse = NULL;
TX_EVENT_FLAGS_GROUP* _SignalResponse = NULL;
TX_EVENT_FLAGS_GROUP* _SignalCollectiveResponse = NULL;

// These 2 APIs are for TIMED wait on response callback
static int _wait_for_response_cb()
{
    uint32 timeout = 1000;

    LOG_INFO("Waiting for response callback");
    app_utils_mutex_get(_MutexResponse);
    uint32 ret = app_utils_timedwait_on_signal(
            _SignalResponse, _MutexResponse, MILLIS_TO_TICKS(timeout));
    app_utils_mutex_put(_MutexResponse);

    if (ret != 0)
    {
        LOG_ERROR("No response cb for %d millis", timeout);
    }
    return ret == 0? 0:-1;
}

// These 2 APIs are for TIMED wait on response callback
static int _wait_for_collective_response_cb()
{
    uint32 timeout = 1000;

    LOG_INFO("Waiting for collective response callback");
    app_utils_mutex_get(_MutexResponse);
    uint32 ret = app_utils_timedwait_on_signal(
            _SignalCollectiveResponse, _MutexResponse, MILLIS_TO_TICKS(timeout));
    app_utils_mutex_put(_MutexResponse);

    if (ret != 0)
    {
        LOG_ERROR("No response cb for %d millis", timeout);
    }
    return ret == 0? 0:-1;
}

/*--------------------------------------------------------------------------
 * Must be invoked before any other API in this file
 *-------------------------------------------------------------------------*/
void qapi_loc_wrapper_setup()
{
    app_utils_init_signal(&_SignalResponse, "_SignalResponse");
    app_utils_init_signal(&_SignalCollectiveResponse, "_SignalCollectiveResponse");
    app_utils_mutex_init(&_MutexResponse, "_MutexResponse");
}

/*--------------------------------------------------------------------------
 * LOC INIT - DEINIT APIs
 *-------------------------------------------------------------------------*/
int qapi_loc_wrapper_init()
{
    memset(&_LocationCallbacks, 0, sizeof(qapi_Location_Callbacks_t));
    _LocationCallbacks.size = sizeof(qapi_Location_Callbacks_t);

    _LocationCallbacks.capabilitiesCb        = qapi_loc_wrapper_capabilities_cb;
    _LocationCallbacks.responseCb            = qapi_loc_wrapper_response_cb;
    _LocationCallbacks.collectiveResponseCb  = qapi_loc_wrapper_collective_response_cb;
    _LocationCallbacks.trackingCb            = qapi_loc_wrapper_tracking_cb;
    _LocationCallbacks.batchingCb            = qapi_loc_wrapper_batching_cb;
    _LocationCallbacks.geofenceBreachCb      = qapi_loc_wrapper_geofence_breach_cb;
    _LocationCallbacks.singleShotCb          = qapi_loc_wrapper_single_shot_cb;
    _LocationCallbacks.gnssDataCb            = qapi_loc_wrapper_gnss_data_cb;
    _LocationCallbacks.metaDataCb            = qapi_loc_wrapper_gnss_meta_data_cb;
    _LocationCallbacks.gnssNmeaCb            = qapi_loc_wrapper_gnss_nmea_cb;
    _LocationCallbacks.motionTrackingCb      = qapi_loc_wrapper_motion_tracking_cb;
     _LocationCallbacks.clientRequestCb      = qapi_loc_wrapper_client_request_cb;
    memset(&_LocClientOptions,0,sizeof(qapi_Location_Client_Options_t));
 
     _LocClientOptions.size = sizeof(qapi_Location_Client_Options_t);
     _LocClientOptions.pCallbacks = &_LocationCallbacks;
     _LocClientOptions.isMasterClient = 1; 
    
    LOG_INFO("qapi_Loc_Init_V2");
    qapi_Location_Error_t ret = qapi_Loc_Init_V2(&_ClientId, &_LocClientOptions);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("LOC INIT returned failure ret %d", ret);
        return -1;
    }

    LOG_INFO("qapi_Loc_Set_User_Buffer");
    ret = qapi_Loc_Set_User_Buffer(_ClientId, (uint8_t*)_UserBuffer, (size_t)USER_BUFFER_SIZE);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("Set user buffer failed ret %d", ret);
        return -1;
    }

    return 0;
}

int qapi_loc_wrapper_deinit(uint32 clientIdx)
{
    LOG_INFO("qapi_Loc_Deinit");
    qapi_Location_Error_t ret = qapi_Loc_Deinit(_ClientId);

    if (ret == QAPI_LOCATION_ERROR_SUCCESS)
    {
        _ClientId = (qapi_loc_client_id)-1;
    }
    else
    {
        LOG_ERROR("LOC DEINIT returned failure ret %d", ret);
        return -1;
    }

    return 0;
}


/*--------------------------------------------------------------------------
 * SINGLESHOT APIs
 *-------------------------------------------------------------------------*/
int qapi_loc_wrapper_get_single_shot(
        uint32 powerLevel, uint32 accuracy, uint32 timeout)
{
    qapi_Singleshot_v3_Options_t    singleshotOptions_v3 = { 0 };

    singleshotOptions_v3.size = sizeof(qapi_Singleshot_v3_Options_t);

    if(powerLevel == QAPI_LOCATION_POWER_HIGH)
    {
        LOG_INFO("get singleshot location tech GNSS selected with accuracy %d", BEST_AVAIL_ACC_THRESHOLD_METERS);
        singleshotOptions_v3.locationTech = QAPI_LOCATION_TECH_GNSS_BIT;
        singleshotOptions_v3.gnssAccuracyMeters = BEST_AVAIL_ACC_THRESHOLD_METERS;
    }
    else
    {
        LOG_INFO("get singleshot location tech GTP1.0 selected with accuracy 10000");
        singleshotOptions_v3.locationTech = QAPI_LOCATION_TECH_GTP_1_0_BIT;
        singleshotOptions_v3.gnssAccuracyMeters = 10000;    //Min Accuracy for GTP
    }
    
    singleshotOptions_v3.timeout = SINGLESHOT_SESSION_TIMEOUT_SEC * 1000;

    LOG_INFO("qapi_Loc_Get_Single_Shot_V3");
    qapi_Location_Error_t ret =
            qapi_Loc_Get_Single_Shot_V3(_ClientId, &singleshotOptions_v3, &_SingleshotSessionId);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("Get singleshot returned failure ret %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();

    return waitRet;
}

int qapi_loc_wrapper_cancel_single_shot()
{
    LOG_INFO("qapi_Loc_Cancel_Single_Shot");
    qapi_Location_Error_t ret =
            qapi_Loc_Cancel_Single_Shot(_ClientId, _SingleshotSessionId);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("QAPI invocation failed, ret %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();

    return waitRet;
}


/*--------------------------------------------------------------------------
 * TRACKING APIs
 *-------------------------------------------------------------------------*/
int qapi_loc_wrapper_tracking_start(
        uint32 tbf, uint32 dbf, uint32 accuracy)
{
    qapi_Location_Options_t trackingOptions = { 0 };
    trackingOptions.size = sizeof(qapi_Location_Options_t);
    trackingOptions.minInterval = tbf;
    trackingOptions.minDistance = dbf;
    trackingOptions.accuracyLevel = accuracy;

    LOG_INFO("qapi_Loc_Start_Tracking");
    qapi_Location_Error_t ret = qapi_Loc_Start_Tracking(
            _ClientId, &trackingOptions, &_TrackingSessionId);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("QAPI invocation failed, ret %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();

    return waitRet;
}

int qapi_loc_wrapper_tracking_stop()
{
    LOG_INFO("qapi_Loc_Stop_Tracking");
    qapi_Location_Error_t ret =
            qapi_Loc_Stop_Tracking(_ClientId, _TrackingSessionId);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("QAPI invocation failed, ret %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();

    return waitRet;
}

/*--------------------------------------------------------------------------
 * META DATA APIs
 *-------------------------------------------------------------------------*/
int qapi_loc_wrapper_get_best_available_position()
{
    LOG_INFO("qapi_Loc_Get_Best_Available_Position");
    qapi_Location_Error_t ret =
           qapi_Loc_Get_Best_Available_Position(_ClientId, &_BestAvailSessionId);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
       LOG_ERROR("QAPI invocation failed, ret %d", ret);
       _BestAvailSessionId = (uint32)-1;
       return -1;
    }

    int waitRet = _wait_for_response_cb();

    return waitRet;
}

/*--------------------------------------------------------------------------
 * MOTION APIs
 *-------------------------------------------------------------------------*/
int qapi_loc_wrapper_start_motion_tracking(
        qapi_Location_Motion_Type_Mask_t mask)
{
    qapi_Location_Motion_Options_t motionOptions={0};

    motionOptions.size=sizeof(qapi_Location_Motion_Options_t);
    motionOptions.motionTypeMask = mask;

    LOG_INFO("qapi_Loc_Start_Motion_Tracking");
    qapi_Location_Error_t ret =
            qapi_Loc_Start_Motion_Tracking(_ClientId, &motionOptions, &_MotionTrackingSessionId);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
       LOG_ERROR("QAPI invocation failed, ret %d", ret);
       return -1;
    }

    int waitRet = _wait_for_response_cb();

    return waitRet;
}

int qapi_loc_wrapper_stop_motion_tracking()
{
    LOG_INFO("qapi_Loc_Stop_Motion_Tracking");
    qapi_Location_Error_t ret =
            qapi_Loc_Stop_Motion_Tracking(_ClientId, _MotionTrackingSessionId);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
       LOG_ERROR("QAPI invocation failed, ret %d", ret);
       return -1;
    }

    int waitRet = _wait_for_response_cb();

    return waitRet;
}

/*--------------------------------------------------------------------------
 * GEOFENCE APIs
 *-------------------------------------------------------------------------*/
int qapi_loc_wrapper_add_geofence(
        double latitude, double longitude, double radius,
        uint16 breachTypeMask, uint32 responsivenessMillis, uint32 dwellTimeMillis)
{
    qapi_Geofence_Option_t options = { 0 };
    qapi_Geofence_Info_t info = {0};
    options.size = sizeof(qapi_Geofence_Option_t);
    info.size = sizeof(qapi_Geofence_Info_t);

    options.breachTypeMask = breachTypeMask;
    options.responsiveness = responsivenessMillis;
    options.dwellTime = dwellTimeMillis;

    info.latitude = latitude;
    info.longitude = longitude;
    info.radius = radius;

    uint32* idArray = NULL;
    LOG_INFO("qapi_Loc_Add_Geofences");
    qapi_Location_Error_t ret = qapi_Loc_Add_Geofences(
            _ClientId, 1, &options, &info, &_GeofenceIdArray);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("QAPI invocation failed, ret %d", ret);
        return -1;
    }

    int waitRet = _wait_for_collective_response_cb();

    return waitRet;
}

int qapi_loc_wrapper_remove_geofence()
{
    LOG_INFO("qapi_Loc_Remove_Geofences");
    qapi_Location_Error_t ret =  qapi_Loc_Remove_Geofences(
            _ClientId, 1, _GeofenceIdArray);

    if (ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("QAPI invocation failed, ret %d", ret);
        return -1;
    }

    int waitRet = _wait_for_collective_response_cb();

    return waitRet;
}


/*--------------------------------------------------------------------------
 * GNSS Constellation & AD DL Related APIs
 *-------------------------------------------------------------------------*/
int qapi_loc_wrapper_set_gnss_constellation_config(
    uint32 ConstellationTypeMask)
{
    qapi_Gnss_Config_t config;

    memset(&config, 0, sizeof(qapi_Gnss_Config_t));
    //Setting xtraconfig option to zero.
    config.xtraConfig.size = 0;

    //Set Config Mask for GNSS Constellation Config Change
    config.size = sizeof(qapi_Gnss_Config_t);
    config.configMask = QAPI_GNSS_CONFIG_TYPE_GNSS_SV_TYPE_CONFIG;
    config.svTypeConfig.size = sizeof(qapi_Gnss_Sv_Type_Config_t);
    config.svTypeConfig.enabledSvTypesMask = ConstellationTypeMask;

    LOG_INFO("Qapi Set GNSS Constellation Config");

    qapi_Location_Error_t ret = qapi_Loc_Set_Gnss_Config(
                    _ClientId,
                    &config,
                    &_SingleshotSessionId);
    if(ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("qapi_Loc_Set_Gnss_Config failed, ret = %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();
    
    return waitRet;  
}


int qapi_loc_wrapper_set_gnss_xtra_config(
    uint32 ConfigMask, uint32 FileDuration, uint32 ConstellationMask, uint8 IntStatus )
{
    qapi_Gnss_Config_t config;

    memset(&config, 0, sizeof(qapi_Gnss_Config_t));
    //Set Gnss Config Option to zero.
    config.svTypeConfig.size = 0;

    //Set config mask for GNSS XTRA Config Change
    config.size = sizeof(qapi_Gnss_Config_t);
    config.configMask = QAPI_GNSS_CONFIG_TYPE_XTRA_CONFIG;
    config.xtraConfig.size = sizeof(qapi_Xtra_Config_t);
    config.xtraConfig.configMask = ConfigMask;
    config.xtraConfig.xtraFileDuration = FileDuration;
    config.xtraConfig.xtraConstellationMask = ConstellationMask;
    config.xtraConfig.xtraIntEnabled = IntStatus;

    LOG_INFO("Qapi Set GNSS XTRA Config");

    qapi_Location_Error_t ret = qapi_Loc_Set_Gnss_Config(
                    _ClientId,
                    &config,
                    &_SingleshotSessionId);
    if(ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("qapi_Loc_Set_Gnss_Config failed, ret = %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();
    
    return waitRet;   
}


int qapi_loc_wrapper_trigger_ad_update(uint32 ADType)
{
    qapi_AD_Update_Options_t options;

    memset(&options, 0, sizeof(qapi_AD_Update_Options_t));

    options.size = sizeof(qapi_AD_Update_Options_t);
    options.adType = ADType;

    qapi_Location_Error_t ret = qapi_Loc_Trigger_AD_Update(
                                _ClientId,
                                &options,
                                &_SingleshotSessionId);
    if(ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("qapi_Loc_Trigger_AD_Update failed, ret = %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();
    
    return waitRet;  
}


int qapi_loc_wrapper_get_ad_status(uint32 ADType)
{
    qapi_Location_Error_t ret = qapi_Loc_Get_AD_Status(
                                _ClientId,
                                ADType,
                                &_SingleshotSessionId);
    if(ret != QAPI_LOCATION_ERROR_SUCCESS)
    {
        LOG_ERROR("qapi_Loc_Get_AD_Status failed, ret = %d", ret);
        return -1;
    }

    int waitRet = _wait_for_response_cb();
    
    return waitRet; 
}

