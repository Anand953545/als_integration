/*****************************************************************************
 Copyright (c) 2020 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stringl.h"
#include "gnss_worker_sm.h"
#include "gnss_worker_utils.h"

#include "app_utils_misc.h"
#include "qapi_loc_wrapper.h"

#include "qapi_timer.h"
#include "aware_log.h"
#include "aware_utils.h"


#define TEST_ENABLE 1
#define MIN_REPORT_DELAY_TIMER_DURATION_SEC 60
#define MAX_BACKOFF_COUNT 2
#define BACKOFF_DELAY_SEC 60
#define AD_DL_COMPLETE_WAIT_ENABLED 0
#define AD_DL_COMPLETE_WAIT_SEC 10
#define WWAN_SLEEP_EVENT_ENABLED 0

// Refresh the position few seconds before report is needed
#define POSITION_REFRESH_INTERVAL_SEC 15

// WWAN sleep retry threshold
#define MIN_WWAN_SLEEP_FOR_GNSS_RETRY_SEC 15

//TODO Later change the value here
// XTRA Integrity Assistance UTC time Threashold
//UTC millis since epoch value for last successful  XTRA Integrity download
#define XTRA_INT_ASSISTANCE_THRESHOLD_IN_MS 86400000 //24Hrs in ms
// Time Uncertainty Threashold
#define MIN_TIME_UNCERTAINTY_THRESHOLD_IN_MS 8000 //8 Sec in ms

// Default GNSS Constellation Mask used for gnss_constellation_config and gnss_xtra_config
// 1. GPS       - 0
// 2. GPS+GLO   - 1
#define DEFAULT_CONSTELLATION_MASK 0

// XTRA CONFIGURATIONS
#define XTRA_CONFIG_MASK (QAPI_XTRA_CONFIG_TYPE_XTRA_INT_ENABLED |        \
                          QAPI_XTRA_CONFIG_TYPE_XTRA_CONSTELLATION_MASK | \
                          QAPI_XTRA_CONFIG_TYPE_XTRA_FILE_DURATION)

#define XTRA_FILE_DURATION QAPI_XTRA_CONFIG_FILE_DURATION_72_HRS
#define XTRA_CONSTELLATION_MASK DEFAULT_CONSTELLATION_MASK
#define XTRA_INT_ENABLE 1

static void _gnss_worker_sm_trigger_xtra_int_update(void);
static uint8 _gnss_worker_sm_is_xtra_int_update_required(void);
static uint8 _gnss_worker_sm_is_xtra_int_idle_state(void);
/***************************************************************************
    INTERNAL STATE DATA
****************************************************************************/
typedef struct gnss_worker_sm_singleshot_request_params
{
    qapi_Location_Power_Level_t maxPower;
    qapi_Location_Accuracy_Level_t minAccuracy;
    uint32 allowPriorityInversion;

} gnss_worker_sm_singleshot_request_params;

typedef struct gnss_worker_sm_state_data
{
    gnss_worker_state state;

    uint8 isLocInitDone;
    uint8 isBestAvailRequestPending;
    uint8 isBestAvailSyncRequestPending;
    uint8 isReportDelayTimerRunning;
    uint8 isBackoffTimerRunning;
    uint8 isPositionRefreshTimerRunning;
    uint8 isAdDlCompleteTimerRunning;

    // Fetch best avail pos in sync
    qapi_Location_t bestAvailLocation;

    //AD DL - Statuses
    qapi_Time_AD_Status_t timeAdStatus;        /**< Time AD status */
    qapi_Xtra_AD_Status_t xtraAdStatus;        /**< XTRA AD status */
    qapi_Xtra_Int_AD_Status_t xtraIntAdStatus; /**< XTRA Integrity AD */

    // Internal Timers
    void *reportDelayTimerHandle;
    void *backoffTimerHandle;
    void *positionRefreshTimerHandle;
    void *adDlCompleteTimerHandle;

    // Mutex for blocking
    TX_MUTEX *gnssWorkerSmMutex;
    TX_EVENT_FLAGS_GROUP *gnssWorkerSmSingleshotSignal;
    TX_EVENT_FLAGS_GROUP *gnssWorkerSmNTPStatusSignal;
    TX_EVENT_FLAGS_GROUP *gnssWorkerSmXTRAStatusSignal;
    TX_EVENT_FLAGS_GROUP *gnssWorkerSmXTRAIntStatusSignal;

    TX_EVENT_FLAGS_GROUP *gnssWorkerSmNTPDLCompleteSignal;
    TX_EVENT_FLAGS_GROUP *gnssWorkerSmXTRADLCompleteSignal;
    TX_EVENT_FLAGS_GROUP *gnssWorkerSmXTRAIntDLCompleteSignal;
    // Delayed request parameters
    gnss_worker_sm_singleshot_request_params delayedRequestParams;

    // Incoming request parameters
    gnss_worker_sm_singleshot_request_params newRequestParams;

    // Ongoing request parameters
    gnss_worker_sm_singleshot_request_params ongoingRequestParams;

    // Queued request parameters
    // - to be handled when app moves to IDLE state
    uint8 isRequestQueuedForIdleState;
    gnss_worker_sm_singleshot_request_params queuedRequestParams;

    uint8 backoffCount;

    gnss_worker_ad_dl_state xtraDlState;
    gnss_worker_ad_dl_state ntpDlState;
    gnss_worker_ad_dl_state xtraIntDlStatus;
    gnss_worker_ad_dl_state gtpDlState;

    uint32 wwanSleepDuration;

} gnss_worker_sm_state_data;

// Internal state
static gnss_worker_sm_state_data _StateData = {};

/***************************************************************************
    Internal Method Declarations
****************************************************************************/
static void _gnss_worker_sm_request_singleshot(
    qapi_Location_Power_Level_t maxPower, qapi_Location_Accuracy_Level_t minAccuracy,
    uint32 allowPriorityInversion);
static void _gnss_worker_sm_cancel_singleshot();

static void gnss_worker_sm_report_delay_timer_cb_in_timer_context(uint32 cbData);
static void gnss_worker_sm_backoff_timer_cb_in_timer_context(uint32 cbData);
static void gnss_worker_sm_position_refresh_timer_cb_in_timer_context(uint32 cbData);
static void gnss_worker_sm_ad_dl_complete_timer_cb_in_timer_context(uint32 cbData);

// State change methods
// Return 0 if event is handled, -1 otherwise
static int _gnss_worker_sm_state_idle(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_waiting_for_gnss_load(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_waiting_for_ad_dl_complete(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_ad_dl_complete(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_gnss_loaded(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_backoff_bad_wwan_connection(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_backoff_weak_gnss_signals(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_backoff_wwan_high_priority(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_gnss_retry(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_gnss_fix_generated(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);
static int _gnss_worker_sm_state_waiting_for_non_gnss_position(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);

// Returns 0 if event is handled by some state, or -1 otherwise
static int _gnss_worker_sm_report_event(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status);

static void _gnss_worker_sm_start_report_delay_timer(
    uint32 delaySec, qapi_Location_Power_Level_t maxPower,
    qapi_Location_Accuracy_Level_t minAccuracy, uint32 allowPriorityInversion);
static void _gnss_worker_sm_cancel_report_delay_timer();

static void _gnss_worker_sm_start_backoff_timer(uint32 durationSec);
static void _gnss_worker_sm_cancel_backoff_timer();

static void _gnss_worker_sm_start_position_refresh_timer(uint32 durationSec);
static void _gnss_worker_sm_cancel_position_refresh_timer();

static void _gnss_worker_sm_start_ad_dl_complete_timer(uint32 durationSec);
static void _gnss_worker_sm_cancel_ad_dl_complete_timer();

static void _gnss_worker_sm_enter_state(gnss_worker_state state);

/***************************************************************************
    INTERNAL METHODS
****************************************************************************/
#if 0
uint64_t gnss_get_current_time_in_milliseconds(void)
{
   uint16_t y;
   uint16_t m;
   uint16_t d;
   uint64_t t;
 
 	qapi_time_get_t time_info;
	qapi_time_get(QAPI_TIME_JULIAN, &time_info);
	

   //Year
   y = time_info.time_julian.year;
   //Month of year
   m = time_info.time_julian.month;
   //Day of month
   d = time_info.time_julian.day;
 
   //January and February are counted as months 13 and 14 of the previous year
   if(m <= 2)
   {
      m += 12;
      y -= 1;
   }
 
   //Convert years to days
   t = (365 * y) + (y / 4) - (y / 100) + (y / 400);
   //Convert months to days
   t += (30 * m) + (3 * (m + 1) / 5) + d;
   //Unix time starts on January 1st, 1970
   t -= 719561;
   //Convert days to seconds
   t *= 86400;
   //Add hours, minutes and seconds
   t += (3600 * time_info.time_julian.hour) + (60 * time_info.time_julian.minute) + time_info.time_julian.second;
 
	//convert seconds to milliseconds
	t *= 1000;

   //Return current milliseconds
   return t;
}
#endif
static void _gnss_worker_sm_report_delay_timer_cb(uint32 cbData)
{
    // stop the recurring timer
    _gnss_worker_sm_cancel_report_delay_timer();

    // Report event to be handled if in idle state
    _gnss_worker_sm_report_event(
        GNSS_WORKER_EVENT_REPORT_DELAY_TIMER_EXPIRY, 0);
}

static void _gnss_worker_sm_backoff_timer_cb(uint32 cbData)
{
    // stop the recurring timer
    _gnss_worker_sm_cancel_backoff_timer();

    // Report backoff expiry to be handled in current backoff state
    _gnss_worker_sm_report_event(
        GNSS_WORKER_EVENT_BACKOFF_TIMER_EXPIRY, 0);
}

static void _gnss_worker_sm_position_refresh_timer_cb(uint32 cbData)
{
    // stop the recurring timer
    _gnss_worker_sm_cancel_position_refresh_timer();

    // Report backoff expiry to be handled in current backoff state
    int res = _gnss_worker_sm_report_event(
        GNSS_WORKER_EVENT_POSITION_REFRESH_TIMER_EXPIRY, 0);
}

static void _gnss_worker_sm_ad_dl_complete_timer_cb(uint32 cbData)
{
    // stop the recurring timer
    _gnss_worker_sm_cancel_ad_dl_complete_timer();

    // Report expiry to be handled in current state
    int res = _gnss_worker_sm_report_event(
        GNSS_WORKER_EVENT_AD_DL_COMPLETE_TIMER_EXPIRY, 0);
}

// Internal method to get best avail position synchronously
static int _gnss_worker_sm_get_best_avail_loc_sync()
{
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

    _StateData.bestAvailLocation.size = 0;
    _StateData.isBestAvailSyncRequestPending = 1;

    // Send request to qapi layer
    qapi_loc_wrapper_get_best_available_position();

    app_utils_timedwait_on_signal(
        _StateData.gnssWorkerSmSingleshotSignal, _StateData.gnssWorkerSmMutex, MILLIS_TO_TICKS(1000));

    _StateData.isBestAvailSyncRequestPending = 0;
    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);

    return (_StateData.bestAvailLocation.size != 0) ? 0 : -1;
}

static void _gnss_worker_sm_start_report_delay_timer(
    uint32 delaySec, qapi_Location_Power_Level_t maxPower,
    qapi_Location_Accuracy_Level_t minAccuracy, uint32 allowPriorityInversion)
{
    // Create timer if it doesn't exist
    if (_StateData.reportDelayTimerHandle == NULL)
    {
        app_utils_timer_init(
            &_StateData.reportDelayTimerHandle,
            gnss_worker_sm_report_delay_timer_cb_in_timer_context,
            (uint32)&_StateData);
    }

    LOG_DEBUG("Delaying singleshot start by %d sec", delaySec);

    _StateData.delayedRequestParams.maxPower = maxPower;
    _StateData.delayedRequestParams.minAccuracy = minAccuracy;
    _StateData.delayedRequestParams.allowPriorityInversion = allowPriorityInversion;

    if (_StateData.isReportDelayTimerRunning)
    {
        _gnss_worker_sm_cancel_report_delay_timer();
    }

    app_utils_timer_start(_StateData.reportDelayTimerHandle, delaySec);
    _StateData.isReportDelayTimerRunning = 1;
}

static void _gnss_worker_sm_cancel_report_delay_timer()
{
    app_utils_timer_stop(_StateData.reportDelayTimerHandle);
    _StateData.isReportDelayTimerRunning = 0;
}

static void _gnss_worker_sm_start_backoff_timer(uint32 durationSec)
{
    // Create timer if it doesn't exist
    if (_StateData.backoffTimerHandle == NULL)
    {
        app_utils_timer_init(
            &_StateData.backoffTimerHandle,
            gnss_worker_sm_backoff_timer_cb_in_timer_context,
            (uint32)&_StateData);
    }

    if (_StateData.isBackoffTimerRunning)
    {
        _gnss_worker_sm_cancel_backoff_timer();
    }

    app_utils_timer_start(_StateData.backoffTimerHandle, durationSec);
    _StateData.isBackoffTimerRunning = 1;
}

static void _gnss_worker_sm_cancel_backoff_timer()
{
    app_utils_timer_stop(_StateData.backoffTimerHandle);
    _StateData.isBackoffTimerRunning = 0;
}

static void _gnss_worker_sm_start_position_refresh_timer(uint32 durationSec)
{
    // Create timer if it doesn't exist
    if (_StateData.positionRefreshTimerHandle == NULL)
    {
        app_utils_timer_init(
            &_StateData.positionRefreshTimerHandle,
            gnss_worker_sm_position_refresh_timer_cb_in_timer_context,
            (uint32)&_StateData);
    }

    if (_StateData.isPositionRefreshTimerRunning)
    {
        _gnss_worker_sm_cancel_position_refresh_timer();
    }

    LOG_INFO("Starting refresh timer..");
    app_utils_timer_start(_StateData.positionRefreshTimerHandle, durationSec);
    _StateData.isPositionRefreshTimerRunning = 1;
}

static void _gnss_worker_sm_cancel_position_refresh_timer()
{
    LOG_INFO("Canceling refresh timer..");
    app_utils_timer_stop(_StateData.positionRefreshTimerHandle);
    _StateData.isPositionRefreshTimerRunning = 0;
}

static void _gnss_worker_sm_start_ad_dl_complete_timer(uint32 durationSec)
{
    if (!AD_DL_COMPLETE_WAIT_ENABLED)
    {
        LOG_ERROR("AD DL Complete Wait logic is disabled, can't start timer.");
        return;
    }

    // Create timer if it doesn't exist
    if (_StateData.adDlCompleteTimerHandle == NULL)
    {
        app_utils_timer_init(
            &_StateData.adDlCompleteTimerHandle,
            gnss_worker_sm_ad_dl_complete_timer_cb_in_timer_context,
            (uint32)&_StateData);
    }

    if (_StateData.isAdDlCompleteTimerRunning)
    {
        _gnss_worker_sm_cancel_ad_dl_complete_timer();
    }

    app_utils_timer_start(_StateData.adDlCompleteTimerHandle, durationSec);
    _StateData.isAdDlCompleteTimerRunning = 1;
}

static void _gnss_worker_sm_cancel_ad_dl_complete_timer()
{
    app_utils_timer_stop(_StateData.adDlCompleteTimerHandle);
    _StateData.isAdDlCompleteTimerRunning = 0;
}

void _gnss_worker_sm_update_dl_ongoing(qapi_Location_Meta_Data_Engine_Status_t status)
{
    switch (status)
    {
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_START:
        _StateData.xtraDlState = GNSS_WORKER_AD_DL_STATE_ONGOING;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_SUCCESS:
        _StateData.xtraDlState = GNSS_WORKER_AD_DL_STATE_SUCCESS;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_FAILED:
        _StateData.xtraDlState = GNSS_WORKER_AD_DL_STATE_FAILED;
        break;

    case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_START:
        _StateData.ntpDlState = GNSS_WORKER_AD_DL_STATE_ONGOING;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_SUCCESS:
        _StateData.ntpDlState = GNSS_WORKER_AD_DL_STATE_SUCCESS;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_FAILED:
        _StateData.ntpDlState = GNSS_WORKER_AD_DL_STATE_FAILED;
        break;

    case QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_START:
        _StateData.xtraIntDlStatus = GNSS_WORKER_AD_DL_STATE_ONGOING;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_SUCCESS:
        _StateData.xtraIntDlStatus = GNSS_WORKER_AD_DL_STATE_SUCCESS;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_FAILED:
        _StateData.xtraIntDlStatus = GNSS_WORKER_AD_DL_STATE_FAILED;
        break;

    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_START:
        _StateData.gtpDlState = GNSS_WORKER_AD_DL_STATE_ONGOING;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_SUCCESS:
        _StateData.gtpDlState = GNSS_WORKER_AD_DL_STATE_SUCCESS;
        break;
    case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_FAILED:
        _StateData.gtpDlState = GNSS_WORKER_AD_DL_STATE_FAILED;
        break;

    default:
        break;
    }
}

uint8 _gnss_worker_sm_is_any_ad_dl_ongoing()
{
    return (_StateData.xtraDlState == GNSS_WORKER_AD_DL_STATE_ONGOING ||
            _StateData.xtraIntDlStatus == GNSS_WORKER_AD_DL_STATE_ONGOING ||
            _StateData.ntpDlState == GNSS_WORKER_AD_DL_STATE_ONGOING ||
            _StateData.gtpDlState == GNSS_WORKER_AD_DL_STATE_ONGOING)
               ? 1
               : 0;
}
uint8 _gnss_worker_sm_is_any_ad_dl_failed()
{
    return (_StateData.xtraDlState == GNSS_WORKER_AD_DL_STATE_FAILED ||
            _StateData.xtraIntDlStatus == GNSS_WORKER_AD_DL_STATE_FAILED ||
            _StateData.ntpDlState == GNSS_WORKER_AD_DL_STATE_FAILED ||
            _StateData.gtpDlState == GNSS_WORKER_AD_DL_STATE_FAILED)
               ? 1
               : 0;
}
uint8 _gnss_worker_sm_is_xtra_or_ntp_dl_ongoing()
{
    return (_StateData.xtraDlState == GNSS_WORKER_AD_DL_STATE_ONGOING ||
             _StateData.xtraIntDlStatus == GNSS_WORKER_AD_DL_STATE_ONGOING ||
            _StateData.ntpDlState == GNSS_WORKER_AD_DL_STATE_ONGOING) ? 1 : 0;
}
uint8 _gnss_worker_sm_is_xtra_or_ntp_dl_failed()
{
    return (_StateData.xtraDlState == GNSS_WORKER_AD_DL_STATE_FAILED ||
             _StateData.xtraIntDlStatus == GNSS_WORKER_AD_DL_STATE_FAILED ||
            _StateData.ntpDlState == GNSS_WORKER_AD_DL_STATE_FAILED) ? 1 : 0;
}

uint8 _gnss_worker_sm_is_ntp_idle_state()
{
    LOG_INFO("Checking NTP DL State, ntpDlState = %d", _StateData.ntpDlState);
    return (_StateData.ntpDlState == GNSS_WORKER_AD_DL_STATE_IDLE) ? 1 : 0;
}

uint8 _gnss_worker_sm_is_ntp_update_required()
{
    int ret = -1;

    LOG_INFO("Checking NTP Status");
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

    memset(&_StateData.timeAdStatus, 0, sizeof(qapi_Time_AD_Status_t));

    uint32 ADType = QAPI_AD_TYPE_TIME;
    ret = qapi_loc_wrapper_get_ad_status(ADType);

    if (ret != 0)
    {
        LOG_ERROR("Get NTP Status fail");
        app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
        return 0;
    }

    app_utils_timedwait_on_signal(
            _StateData.gnssWorkerSmNTPStatusSignal, _StateData.gnssWorkerSmMutex, MILLIS_TO_TICKS(2000));

    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);

    if (_StateData.timeAdStatus.size == 0)
    {
        LOG_INFO("_StateData.timeAdStatus.size = 0");
        return 0;
    }

    LOG_INFO("NTP Status, tuncMs = %u", _StateData.timeAdStatus.tuncMs);
#ifdef TEST_ENABLE
    return 1;
#else
    return (_StateData.timeAdStatus.tuncMs > MIN_TIME_UNCERTAINTY_THRESHOLD_IN_MS)? 1:0 ;
#endif
}

void _gnss_worker_sm_trigger_ntp_update()
{
    int ret = -1;

    LOG_INFO("Triggering NTP Update");
    uint32 ADType = QAPI_AD_TYPE_TIME;
    ret = qapi_loc_wrapper_trigger_ad_update(ADType);

    if (ret != 0)
    {
        _StateData.ntpDlState = GNSS_WORKER_AD_DL_STATE_FAILED;
        LOG_ERROR("Trigger NTP Update fail");
    }
    else
    {
        app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

        app_utils_timedwait_on_signal(
            _StateData.gnssWorkerSmNTPDLCompleteSignal, _StateData.gnssWorkerSmMutex, MILLIS_TO_TICKS(120000));
        
        app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
        LOG_INFO("NTP Update Complete");
    }
}

uint8 _gnss_worker_sm_is_xtra_idle_state()
{
    LOG_INFO("Checking XTRA DL State, xtraDlState = %d", _StateData.xtraDlState);
    return (_StateData.xtraDlState == GNSS_WORKER_AD_DL_STATE_IDLE) ? 1 : 0;
}

uint8 _gnss_worker_sm_is_xtra_dl_required()
{
    int ret = -1;

    LOG_INFO("Checking XTRA Status");
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

    memset(&_StateData.xtraAdStatus, 0, sizeof(qapi_Xtra_AD_Status_t));

    uint32 ADType = QAPI_AD_TYPE_XTRA;
    ret = qapi_loc_wrapper_get_ad_status(ADType);

    if (ret != 0)
    {
        LOG_ERROR("Get XTRA Status fail");
        app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
        return 0;
    }

    app_utils_timedwait_on_signal(
            _StateData.gnssWorkerSmXTRAStatusSignal, _StateData.gnssWorkerSmMutex, MILLIS_TO_TICKS(2000));

    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);

    if (_StateData.xtraAdStatus.size == 0)
    {
        LOG_INFO("_StateData.xtraAdStatus.size = 0");
        return 1;   // for testing only, otherwise return 0;
    }

    LOG_INFO("XTRA Status, xtraDataStartTime = %llu", _StateData.xtraAdStatus.xtraDataStartTime);
    LOG_INFO("XTRA Status, xtraFileDurationHrs = %u", _StateData.xtraAdStatus.xtraFileDurationHrs);
    LOG_INFO("XTRA Status, xtraFileGenerationTime = %llu", _StateData.xtraAdStatus.xtraFileGenerationTime);

#ifdef TEST_ENABLE
    return 1;
#else
    uint64_t time_ms = get_current_time_in_milliseconds();

    if (0 == _StateData.xtraAdStatus.xtraFileDurationHrs)
    {
        return 1;
    }
    else
    {
        if (time_ms >= (_StateData.xtraAdStatus.xtraDataStartTime + (_StateData.xtraAdStatus.xtraFileDurationHrs * 3600000)))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
#endif
}

void _gnss_worker_sm_trigger_xtra_dl()
{
    int ret = -1;

    LOG_INFO("Configuring XTRA settings");
    ret = qapi_loc_wrapper_set_gnss_xtra_config(
        XTRA_CONFIG_MASK,
        XTRA_FILE_DURATION,
        XTRA_CONSTELLATION_MASK,
        XTRA_INT_ENABLE);

    if (ret != 0)
    {
        _StateData.xtraDlState = GNSS_WORKER_AD_DL_STATE_FAILED;
        LOG_INFO("XTRA Config fail, ret = %d", ret);
        return;
    }

    LOG_INFO("Triggering XTRA DL");
    uint32 ADType = QAPI_AD_TYPE_XTRA;
    ret = qapi_loc_wrapper_trigger_ad_update(ADType);

    if (ret != 0)
    {
        _StateData.xtraDlState = GNSS_WORKER_AD_DL_STATE_FAILED;
        LOG_ERROR("Trigger XTRA DL fail");
    }
    else
    {
        app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

        app_utils_timedwait_on_signal(
            _StateData.gnssWorkerSmXTRADLCompleteSignal, _StateData.gnssWorkerSmMutex, MILLIS_TO_TICKS(120000));

        app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
        LOG_INFO("XTRA DL Complete");
    }
}

static uint8 _gnss_worker_sm_is_xtra_int_idle_state()
{
    LOG_INFO("Checking XTRA INT DL State, xtraIntDlStatus = %d", _StateData.xtraIntDlStatus);
    return (_StateData.xtraIntDlStatus == GNSS_WORKER_AD_DL_STATE_IDLE) ? 1:0;
}
static uint8 _gnss_worker_sm_is_xtra_int_update_required(void)
{
    int ret = -1;

    LOG_INFO("Checking XTRA INT Status");
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

    memset(&_StateData.xtraIntAdStatus, 0, sizeof(qapi_Xtra_Int_AD_Status_t));

    uint32 ADType = QAPI_AD_TYPE_XTRA_INTEGRITY;
    ret = qapi_loc_wrapper_get_ad_status(ADType);

    if (ret != 0)
    {
        LOG_ERROR("Get XTRA INT Status fail");
        app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
        return 0;
    }

    app_utils_timedwait_on_signal(
        _StateData.gnssWorkerSmXTRAIntStatusSignal, _StateData.gnssWorkerSmMutex, MILLIS_TO_TICKS(2000));

    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);

    if (_StateData.xtraIntAdStatus.size == 0)
    {
        LOG_INFO("_StateData.xtraIntAdStatus.size = 0");
        return 0;
    }

    uint64_t time_ms = get_current_time_in_milliseconds();
  
#ifdef TEST_ENABLE
    LOG_INFO("time_ms = %llu", time_ms);
    return 1; 
#else
    if (time_ms > (_StateData.xtraIntAdStatus.lastXtraIntDlSuccess + XTRA_INT_ASSISTANCE_THRESHOLD_IN_MS))
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif
}

static void _gnss_worker_sm_trigger_xtra_int_update(void)
{
    int ret = -1;

    LOG_INFO("Triggering XTRA INT DL");
    uint32 ADType = QAPI_AD_TYPE_XTRA_INTEGRITY;
    ret = qapi_loc_wrapper_trigger_ad_update(ADType);

    if( ret != 0)
    {
        _StateData.xtraIntDlStatus = GNSS_WORKER_AD_DL_STATE_FAILED;
        LOG_ERROR("Trigger XTRA INT DL fail");
    }
    else
    {
        app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

        app_utils_timedwait_on_signal(
            _StateData.gnssWorkerSmXTRAIntDLCompleteSignal, _StateData.gnssWorkerSmMutex, MILLIS_TO_TICKS(120000));
        
        app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
        LOG_INFO("XTRA_INT DL Complete");
    }
}

// TRIGGER SINGLESHOT SESSION
static void _gnss_worker_sm_request_singleshot(
    qapi_Location_Power_Level_t maxPower, qapi_Location_Accuracy_Level_t minAccuracy,
    uint32 allowPriorityInversion)
{
    int ntpflag = 0;
    int xtraflag = 0;
    int xtraIntflag = 0;
    
    //Check and Update NTP
    if (_gnss_worker_sm_is_ntp_idle_state())
    {
        if (_gnss_worker_sm_is_ntp_update_required())
        {
            ntpflag = 1;
            // _gnss_worker_sm_trigger_ntp_update();
        }

        tx_thread_sleep(MILLIS_TO_TICKS(1000));
    }

    if (maxPower == QAPI_LOCATION_POWER_HIGH)
    {
        //Check and Trigger XTRA
        if (_gnss_worker_sm_is_xtra_idle_state())
        {
            if (_gnss_worker_sm_is_xtra_dl_required())
            {
                xtraflag = 1;
                // _gnss_worker_sm_trigger_xtra_dl();
            }

            tx_thread_sleep(MILLIS_TO_TICKS(1000));
        }

        //Check and Trigger XTRA INT
        if (_gnss_worker_sm_is_xtra_int_idle_state())
        {
            if (_gnss_worker_sm_is_xtra_int_update_required())
            {
                xtraIntflag = 1;
                //_gnss_worker_sm_trigger_xtra_int_update();
            }

            tx_thread_sleep(MILLIS_TO_TICKS(1000));
        }
    }

    if (ntpflag)
    {
        _gnss_worker_sm_trigger_ntp_update();
    }

    if(xtraflag)
    {
        _gnss_worker_sm_trigger_xtra_dl();
    }

    if(xtraIntflag)
    {
      _gnss_worker_sm_trigger_xtra_int_update();
    }

    if (maxPower == QAPI_LOCATION_POWER_HIGH)
    {
        gnss_worker_utils_set_gnss_high_priority();
    }

    tx_thread_sleep(MILLIS_TO_TICKS(2000));

    LOG_DEBUG("Triggering Singleshot request with [%s] [%s] [timeout %d sec]",
              app_utils_power_level_str(maxPower), app_utils_accuracy_level_str(minAccuracy),
              SINGLESHOT_SESSION_TIMEOUT_SEC);

    _StateData.ongoingRequestParams.maxPower = maxPower;
    _StateData.ongoingRequestParams.minAccuracy = minAccuracy;
    _StateData.ongoingRequestParams.allowPriorityInversion = allowPriorityInversion;

    qapi_loc_wrapper_get_single_shot(maxPower, minAccuracy, SINGLESHOT_SESSION_TIMEOUT_SEC * 1000);

    if (maxPower == QAPI_LOCATION_POWER_HIGH)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_WAITING_FOR_GNSS_LOAD);
    }
    else
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_WAITING_FOR_NON_GNSS_POSITION);
    }

}

// CANCEL SINGLESHOT SESSION
static void _gnss_worker_sm_cancel_singleshot()
{
    LOG_DEBUG("Canceling Singleshot Session");

    qapi_loc_wrapper_cancel_single_shot();
}

/***************************************************************************
    STATE CHANGE/HANDLING METHODS
****************************************************************************/
static int _gnss_worker_sm_state_idle(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_IDLE;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    // LOG_INFO("entered _gnss_worker_sm_state_idle, event = %d, status = %d",
    //     event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        gnss_worker_utils_restore_default_app_priority();
        _StateData.backoffCount = 0;

        // handle any queued request
        if (_StateData.isRequestQueuedForIdleState)
        {
            LOG_DEBUG("Trigger queued request in IDLE state");
            _StateData.isRequestQueuedForIdleState = 0;
            _gnss_worker_sm_request_singleshot(
                _StateData.queuedRequestParams.maxPower,
                _StateData.queuedRequestParams.minAccuracy,
                _StateData.queuedRequestParams.allowPriorityInversion);
        }

        ret = 0;
    }
    else if (event == GNSS_WORKER_EVENT_SINGLESHOT_REQUEST)
    {
        LOG_DEBUG("Trigger new request in IDLE state");
        _gnss_worker_sm_request_singleshot(
            _StateData.newRequestParams.maxPower,
            _StateData.newRequestParams.minAccuracy,
            _StateData.newRequestParams.allowPriorityInversion);
        ret = 0;
    }
    else if (event == GNSS_WORKER_EVENT_REPORT_DELAY_TIMER_EXPIRY)
    {
        LOG_DEBUG("Trigger delayed report request in IDLE state");
        _gnss_worker_sm_request_singleshot(
            _StateData.delayedRequestParams.maxPower,
            _StateData.delayedRequestParams.minAccuracy,
            _StateData.delayedRequestParams.allowPriorityInversion);
        ret = 0;
    }
    else if (event == GNSS_WORKER_EVENT_POSITION_REFRESH_TIMER_EXPIRY)
    {
        LOG_DEBUG("Trigger position refresh request in IDLE state");
        _gnss_worker_sm_request_singleshot(
            _StateData.newRequestParams.maxPower,
            _StateData.newRequestParams.minAccuracy,
            _StateData.newRequestParams.allowPriorityInversion);
        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_waiting_for_gnss_load(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_WAITING_FOR_GNSS_LOAD;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        // if we don't see a load event soon, means wwan is high priority
        if (AD_DL_COMPLETE_WAIT_ENABLED)
        {
            _gnss_worker_sm_start_ad_dl_complete_timer(AD_DL_COMPLETE_WAIT_SEC);
        }

        ret = 0;
    }

    // Transitions
    if (event == GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE)
    {
        ret = 0;
        switch (status)
        {
        case QAPI_LOCATION_ENGINE_STATUS_LOAD_FAILED:
        case QAPI_LOCATION_ENGINE_STATUS_UNLOADED:
            if (gnss_worker_utils_is_gnss_high_priority())
            {
                LOG_DEBUG("Load failed in GNSS High priority, GNSS Engine blocked for AD DL.");
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION);
            }
            else
            {
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY);
            }
            break;

        case QAPI_LOCATION_ENGINE_STATUS_LOAD_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_LOADED);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_THROTTLED:
            LOG_ERROR("XTRA Throttling not expected in production, please check if Modem XTRA is somehow deleted ?");
            break;

        case QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_START:
        case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_START:
        case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_START:
        case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_START:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_WAITING_FOR_AD_DL_COMPLETE);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_FIX_GENERATED);
            break;

        default:
            ret = -1;
            break;
        }
    }

    if (event == GNSS_WORKER_EVENT_AD_DL_COMPLETE_TIMER_EXPIRY)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY);
        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_waiting_for_ad_dl_complete(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_WAITING_FOR_AD_DL_COMPLETE;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        _gnss_worker_sm_cancel_ad_dl_complete_timer();
        ret = 0;
    }

    // Transitions
    if (event == GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE)
    {
        ret = 0;
        switch (status)
        {
        case QAPI_LOCATION_ENGINE_STATUS_LOAD_FAILED:
        case QAPI_LOCATION_ENGINE_STATUS_UNLOADED:
            if (_gnss_worker_sm_is_xtra_or_ntp_dl_ongoing() ||
                _gnss_worker_sm_is_xtra_or_ntp_dl_failed())
            {
                LOG_DEBUG("Load failed while AD DL incomplete, GNSS Engine blocked for AD DL.");
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION);
            }
            else if (gnss_worker_utils_is_gnss_high_priority())
            {
                LOG_DEBUG("Load failed in GNSS High priority, GNSS Engine blocked for AD DL.");
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION);
            }
            else
            {
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY);
            }
            break;

        case QAPI_LOCATION_ENGINE_STATUS_LOAD_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_LOADED);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_FAILED:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_FIX_GENERATED);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_THROTTLED:
            LOG_ERROR("XTRA Throttling not expected in production, please check if Modem XTRA is somehow deleted ?");
            break;

        case QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_SUCCESS:
        case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_SUCCESS:
        case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_SUCCESS:
        case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_SUCCESS:
            if (!_gnss_worker_sm_is_any_ad_dl_ongoing() &&
                !_gnss_worker_sm_is_any_ad_dl_failed())
            {
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_AD_DL_COMPLETE);
            }
            break;

        default:
            ret = -1;
            break;
        }
    }

    if (event == GNSS_WORKER_EVENT_SINGLESHOT_TIMEOUT)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION);
        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_ad_dl_complete(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_AD_DL_COMPLETE;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        ret = 0;

        // if we don't see a load event soon, means wwan is high priority
        if (AD_DL_COMPLETE_WAIT_ENABLED)
        {
            _gnss_worker_sm_start_ad_dl_complete_timer(AD_DL_COMPLETE_WAIT_SEC);
        }
    }

    // Transitions
    if (event == GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE)
    {
        ret = 0;
        switch (status)
        {
        case QAPI_LOCATION_ENGINE_STATUS_LOAD_FAILED:
        case QAPI_LOCATION_ENGINE_STATUS_UNLOADED:
            if (gnss_worker_utils_is_gnss_high_priority())
            {
                LOG_DEBUG("Load failed in GNSS High priority, GNSS Engine blocked for AD DL.");
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION);
            }
            else
            {
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY);
            }
            break;

        case QAPI_LOCATION_ENGINE_STATUS_LOAD_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_LOADED);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_FIX_GENERATED);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_THROTTLED:
            LOG_ERROR("XTRA Throttling not expected in production, please check if Modem XTRA is somehow deleted ?");
            break;

        case QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_START:
        case QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_START:
        case QAPI_LOCATION_ENGINE_STATUS_NTP_DL_START:
        case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_START:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_WAITING_FOR_AD_DL_COMPLETE);
            break;

        default:
            ret = -1;
            break;
        }
    }

    if (event == GNSS_WORKER_EVENT_SINGLESHOT_TIMEOUT)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY);
        ret = 0;
    }

    if (event == GNSS_WORKER_EVENT_AD_DL_COMPLETE_TIMER_EXPIRY)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY);
        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_gnss_loaded(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_GNSS_LOADED;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        // nothing to do on entry
        ret = 0;
    }

    // Transitions
    if (event == GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE)
    {
        ret = 0;
        switch (status)
        {
        case QAPI_LOCATION_ENGINE_STATUS_WEAK_GNSS_SIGNALS:
        case QAPI_LOCATION_ENGINE_STATUS_FIX_FAILED:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WEAK_GNSS_SIGNALS);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_UNLOADED:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_FIX_GENERATED);
            break;

        default:
            ret = -1;
            break;
        }
    }

    if (event == GNSS_WORKER_EVENT_SINGLESHOT_TIMEOUT)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_BACKOFF_WEAK_GNSS_SIGNALS);
        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_backoff_bad_wwan_connection(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        LOG_DEBUG("Backoff for %d seconds.", BACKOFF_DELAY_SEC);
        _gnss_worker_sm_start_backoff_timer(BACKOFF_DELAY_SEC);
        ret = 0;
    }

    if (event == GNSS_WORKER_EVENT_BACKOFF_TIMER_EXPIRY)
    {
        if (_StateData.backoffCount > 0)
        {
            LOG_DEBUG("Backoff ongoing for bad wwan..");
        }
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_RETRY);
        ret = 0;
    }

    // Transitions
    if (event == GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE)
    {
        ret = 0;
        switch (status)
        {
        case QAPI_LOCATION_ENGINE_STATUS_LOAD_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_LOADED);
            break;

        default:
            ret = -1;
            break;
        }
    }

    return ret;
}

static int _gnss_worker_sm_state_backoff_weak_gnss_signals(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_BACKOFF_WEAK_GNSS_SIGNALS;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        LOG_DEBUG("Backoff for %d seconds.", BACKOFF_DELAY_SEC);
        _gnss_worker_sm_start_backoff_timer(BACKOFF_DELAY_SEC);
        qapi_loc_wrapper_start_motion_tracking(QAPI_LOCATION_MOTION_TYPE_MOVE_ABSOLUTE);
        ret = 0;
    }

    if (event == GNSS_WORKER_EVENT_BACKOFF_TIMER_EXPIRY ||
        event == GNSS_WORKER_EVENT_ABSOLUTE_MOTION)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_RETRY);
        qapi_loc_wrapper_stop_motion_tracking();
        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_backoff_wwan_high_priority(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        LOG_DEBUG("Backoff for %d seconds.", BACKOFF_DELAY_SEC);
        _gnss_worker_sm_start_backoff_timer(BACKOFF_DELAY_SEC);
        ret = 0;
    }

    if (event == GNSS_WORKER_EVENT_BACKOFF_TIMER_EXPIRY)
    {
        // Check if we can switch priority to gnss
        if (_StateData.ongoingRequestParams.allowPriorityInversion)
        {
            gnss_worker_utils_set_gnss_high_priority();
        }

        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_RETRY);

        ret = 0;
    }

    if (event == GNSS_WORKER_EVENT_WWAN_SLEEP_CB &&
        _StateData.wwanSleepDuration >= MIN_WWAN_SLEEP_FOR_GNSS_RETRY_SEC)
    {
        LOG_DEBUG("WWAN Sleep CB (%d sec) in Backoff WWAN High Priority state, retry right away.",
                  _StateData.wwanSleepDuration);
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_RETRY);
    }

    return ret;
}

static int _gnss_worker_sm_state_gnss_retry(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_GNSS_RETRY;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        // if max retries done, try lower power mode or send to idle state
        if (_StateData.backoffCount > MAX_BACKOFF_COUNT)
        {
            if (_StateData.ongoingRequestParams.maxPower == QAPI_LOCATION_POWER_HIGH)
            {
                LOG_DEBUG("Fallback from High power mode to Medium power mode");
                _StateData.ongoingRequestParams.maxPower = QAPI_LOCATION_POWER_MED;
            }
            else if (_StateData.ongoingRequestParams.maxPower == QAPI_LOCATION_POWER_MED)
            {
                LOG_DEBUG("Fallback from Medium power mode to Low power mode");
                _StateData.ongoingRequestParams.maxPower = QAPI_LOCATION_POWER_LOW;
            }
            else
            {
                LOG_DEBUG("Max retries (%d) done, going back to idle.", MAX_BACKOFF_COUNT);
                _gnss_worker_sm_cancel_singleshot();
                _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_IDLE);
                return 0;
            }
        }
        else
        {
            _StateData.backoffCount++;
        }

        // Cancel if session is running and restart
        _gnss_worker_sm_cancel_singleshot();
        _gnss_worker_sm_request_singleshot(
            _StateData.ongoingRequestParams.maxPower,
            _StateData.ongoingRequestParams.minAccuracy,
            _StateData.ongoingRequestParams.allowPriorityInversion);

        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_waiting_for_non_gnss_position(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_WAITING_FOR_NON_GNSS_POSITION;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    // LOG_INFO("entered _gnss_worker_sm_state_waiting_for_non_gnss_position, event = %d, status = %d",
    //     event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        // nothing to do on entry
        ret = 0;
    }

    // Transitions
    if (event == GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE)
    {
        ret = 0;
        switch (status)
        {
        case QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_FAILED:
        case QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_FAILED:
        case QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_FAILED:
        case QAPI_LOCATION_ENGINE_STATUS_POS_ACC_CHECK_FAILED:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_RETRY);
            break;

        case QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS:
            _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_FIX_GENERATED);
            break;

        default:
            ret = -1;
            break;
        }
    }

    if (event == GNSS_WORKER_EVENT_SINGLESHOT_TIMEOUT)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_GNSS_RETRY);
        ret = 0;
    }

    return ret;
}

static int _gnss_worker_sm_state_gnss_fix_generated(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    _StateData.state = GNSS_WORKER_STATE_GNSS_FIX_GENERATED;
    gnss_worker_utils_log_state_event(_StateData.state, event, status);

    // LOG_INFO("entered _gnss_worker_sm_state_gnss_fix_generated, event = %d, status = %d",
    //     event, status);

    int ret = -1;

    if (event == GNSS_WORKER_EVENT_STATE_ENTRY)
    {
        _gnss_worker_sm_enter_state(GNSS_WORKER_STATE_IDLE);
        ret = 0;
    }

    return ret;
}

/*  ---- STATE HANDLING METHODS COMPLETE ----  */

static int _gnss_worker_sm_report_event(
    gnss_worker_event event, qapi_Location_Meta_Data_Engine_Status_t status)
{
    int ret = -1;

    // Let's update AD DL status before event handling
    _gnss_worker_sm_update_dl_ongoing(status);

    switch (_StateData.state)
    {
    case GNSS_WORKER_STATE_IDLE:
        ret = _gnss_worker_sm_state_idle(event, status);
        break;

    case GNSS_WORKER_STATE_WAITING_FOR_GNSS_LOAD:
        ret = _gnss_worker_sm_state_waiting_for_gnss_load(event, status);
        break;

    case GNSS_WORKER_STATE_WAITING_FOR_AD_DL_COMPLETE:
        ret = _gnss_worker_sm_state_waiting_for_ad_dl_complete(event, status);
        break;

    case GNSS_WORKER_STATE_AD_DL_COMPLETE:
        ret = _gnss_worker_sm_state_ad_dl_complete(event, status);
        break;

    case GNSS_WORKER_STATE_GNSS_LOADED:
        ret = _gnss_worker_sm_state_gnss_loaded(event, status);
        break;

    case GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION:
        ret = _gnss_worker_sm_state_backoff_bad_wwan_connection(event, status);
        break;

    case GNSS_WORKER_STATE_BACKOFF_WEAK_GNSS_SIGNALS:
        ret = _gnss_worker_sm_state_backoff_weak_gnss_signals(event, status);
        break;

    case GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY:
        ret = _gnss_worker_sm_state_backoff_wwan_high_priority(event, status);
        break;

    case GNSS_WORKER_STATE_GNSS_RETRY:
        ret = _gnss_worker_sm_state_gnss_retry(event, status);
        break;

    case GNSS_WORKER_STATE_WAITING_FOR_NON_GNSS_POSITION:
        ret = _gnss_worker_sm_state_waiting_for_non_gnss_position(event, status);
        break;

    case GNSS_WORKER_STATE_GNSS_FIX_GENERATED:
        ret = _gnss_worker_sm_state_gnss_fix_generated(event, status);
        break;

    default:
        break;
    }

    return ret;
}

static void _gnss_worker_sm_enter_state(gnss_worker_state state)
{
    switch (state)
    {
    case GNSS_WORKER_STATE_IDLE:
        _gnss_worker_sm_state_idle(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_WAITING_FOR_GNSS_LOAD:
        _gnss_worker_sm_state_waiting_for_gnss_load(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_WAITING_FOR_AD_DL_COMPLETE:
        _gnss_worker_sm_state_waiting_for_ad_dl_complete(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_AD_DL_COMPLETE:
        _gnss_worker_sm_state_ad_dl_complete(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_GNSS_LOADED:
        _gnss_worker_sm_state_gnss_loaded(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_BACKOFF_BAD_WWAN_CONNECTION:
        _gnss_worker_sm_state_backoff_bad_wwan_connection(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_BACKOFF_WEAK_GNSS_SIGNALS:
        _gnss_worker_sm_state_backoff_weak_gnss_signals(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_BACKOFF_WWAN_HIGH_PRIORITY:
        _gnss_worker_sm_state_backoff_wwan_high_priority(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_GNSS_FIX_GENERATED:
        _gnss_worker_sm_state_gnss_fix_generated(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_WAITING_FOR_NON_GNSS_POSITION:
        _gnss_worker_sm_state_waiting_for_non_gnss_position(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    case GNSS_WORKER_STATE_GNSS_RETRY:
        _gnss_worker_sm_state_gnss_retry(GNSS_WORKER_EVENT_STATE_ENTRY, 0);
        break;
    default:
        LOG_ERROR("Invalid state requested %d", state);
    }
}

/***************************************************************************
    EXTERNAL METHODS
****************************************************************************/
// INIT
void gnss_worker_sm_init()
{
    if (_StateData.isLocInitDone)
    {
        LOG_ERROR("GNSS worker SM init already done.");
        return;
    }

    memset(&_StateData, 0, sizeof(_StateData));

    app_utils_init_signal(&_StateData.gnssWorkerSmSingleshotSignal, "_StateData.gnssWorkerSmSingleshotSignal");
    app_utils_init_signal(&_StateData.gnssWorkerSmNTPStatusSignal, "_StateData.gnssWorkerSmNTPStatusSignal");
    app_utils_init_signal(&_StateData.gnssWorkerSmXTRAStatusSignal, "_StateData.gnssWorkerSmXTRAStatusSignal");
    app_utils_init_signal(&_StateData.gnssWorkerSmXTRAIntStatusSignal, "_StateData.gnssWorkerSmXTRAIntStatusSignal");

    app_utils_init_signal(&_StateData.gnssWorkerSmNTPDLCompleteSignal, "_StateData.gnssWorkerSmNTPDLCompleteSignal");
    app_utils_init_signal(&_StateData.gnssWorkerSmXTRADLCompleteSignal, "_StateData.gnssWorkerSmXTRADLCompleteSignal");
    app_utils_init_signal(&_StateData.gnssWorkerSmXTRAIntDLCompleteSignal, "_StateData.gnssWorkerSmXTRAIntDLCompleteSignal");
    
    app_utils_mutex_init(&_StateData.gnssWorkerSmMutex, "_StateData.gnssWorkerSmMutex");

    qapi_loc_wrapper_setup();
    qapi_loc_wrapper_init();

    qapi_loc_wrapper_set_gnss_constellation_config(DEFAULT_CONSTELLATION_MASK);

    if (WWAN_SLEEP_EVENT_ENABLED)
    {
        gnss_worker_utils_set_wwan_sleep_callback();
    }

    _StateData.isLocInitDone = 1;
    _StateData.state = GNSS_WORKER_STATE_IDLE;
}

// REQUEST HANDLER :: Best Available Location
void gnss_worker_sm_request_best_avail_location()
{
    _StateData.isBestAvailRequestPending = 1;
    qapi_loc_wrapper_get_best_available_position();
}

void gnss_worker_sm_request_delayed_location(
    uint32 reportDelaySec, qapi_Location_Power_Level_t maxPower,
    qapi_Location_Accuracy_Level_t minAccuracy, uint32 allowPriorityInversion)
{
    // If there is a previous request running delay timer, cancel it
    if (_StateData.isReportDelayTimerRunning)
    {
        _gnss_worker_sm_cancel_report_delay_timer();
    }

    // // Cancel running refresh timer
    // if (_StateData.isPositionRefreshTimerRunning)
    // {
    //     _gnss_worker_sm_cancel_position_refresh_timer();
    // }

    // Fetch the best available position
    _gnss_worker_sm_get_best_avail_loc_sync();

    //Reset AD DL States
    _StateData.ntpDlState = GNSS_WORKER_AD_DL_STATE_IDLE;
    _StateData.xtraDlState = GNSS_WORKER_AD_DL_STATE_IDLE;
    _StateData.xtraIntDlStatus = GNSS_WORKER_AD_DL_STATE_IDLE; 

    // Save incoming parameters
    _StateData.newRequestParams.maxPower = maxPower;
    _StateData.newRequestParams.minAccuracy = minAccuracy;
    _StateData.newRequestParams.allowPriorityInversion = allowPriorityInversion;

    // Whether to start session right away
    if (_StateData.bestAvailLocation.size == 0 ||
        _StateData.bestAvailLocation.accuracy > BEST_AVAIL_ACC_THRESHOLD_METERS ||
        reportDelaySec < (SINGLESHOT_SESSION_TIMEOUT_SEC + MIN_REPORT_DELAY_TIMER_DURATION_SEC))
    {
        int ret = _gnss_worker_sm_report_event(GNSS_WORKER_EVENT_SINGLESHOT_REQUEST, 0);
        if (ret != 0)
        {
            if (_StateData.isRequestQueuedForIdleState)
            {
                LOG_DEBUG("Ignoring new location request in current state [%s], one request already queued",
                          app_utils_gnss_worker_state_str(_StateData.state));
            }
            else
            {
                _StateData.queuedRequestParams = _StateData.newRequestParams;
                _StateData.isRequestQueuedForIdleState = 1;
                LOG_DEBUG("Queuing new location request (for idle state), not handled in current state [%s]",
                          app_utils_gnss_worker_state_str(_StateData.state));
            }
        }
    }
    else
    {
        _gnss_worker_sm_start_report_delay_timer(
            (reportDelaySec - (SINGLESHOT_SESSION_TIMEOUT_SEC + MIN_REPORT_DELAY_TIMER_DURATION_SEC)),
            maxPower, minAccuracy, allowPriorityInversion);
    }

    // // Start position refresh timer aligned with the report delay
    // if (reportDelaySec > MIN_REPORT_DELAY_TIMER_DURATION_SEC &&
    //         reportDelaySec > POSITION_REFRESH_INTERVAL_SEC)
    // {
    //     _gnss_worker_sm_start_position_refresh_timer(
    //             reportDelaySec - POSITION_REFRESH_INTERVAL_SEC);
    // }
}

/***************************************************************************
    Event handlers called after context switch from GNSS Worker thread
****************************************************************************/
void gnss_worker_sm_singleshot_cb(
    qapi_Location_t location, qapi_Location_Error_t err)
{
    if (QAPI_LOCATION_ERROR_SUCCESS == err)
    {
        if (_StateData.isBestAvailRequestPending)
        {
            gnss_worker_report_best_avail_location(location);
            _StateData.isBestAvailRequestPending = 0;
        }
        else
        {
            gnss_worker_report_singleshot_location(location);
        }
    }

    if (QAPI_LOCATION_ERROR_TIMEOUT == err ||
        QAPI_LOCATION_ERROR_TIMEOUT_ACC_CHECK_FAILED == err)
    {
        _gnss_worker_sm_report_event(
            GNSS_WORKER_EVENT_SINGLESHOT_TIMEOUT, 0);
    }
}

void gnss_worker_sm_singleshot_gtp_cb(gtp_data_t gtpdata)
{
    gnss_worker_report_singleshot_gtpdata(gtpdata);

    // Due to the unpredicted engine status report, send engine status to complete the state machine.
    
    LOG_INFO("_gnss_worker_sm_report_event(%d, %d)", 
        GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE, QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS);
    
    _gnss_worker_sm_report_event(
            GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE, QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS);
}

void gnss_worker_sm_meta_data_cb(qapi_Location_Meta_Data_t metaData)
{
    LOG_INFO("gnss_worker_sm_meta_data_cb flags %p engineStatus %d",
             metaData.flags, metaData.engineStatus);

    if ((metaData.flags & QAPI_LOCATION_META_DATA_HAS_ENGINE_STATUS) &&
        metaData.engineStatus != 0)
    {
        _gnss_worker_sm_report_event(
            GNSS_WORKER_EVENT_ENGINE_STATUS_UPDATE, metaData.engineStatus);
    }
}

void gnss_worker_sm_motion_info_cb(qapi_Location_Motion_Info_t motionInfo)
{
    if (motionInfo.motionType == QAPI_LOCATION_MOTION_TYPE_MOVE_ABSOLUTE)
    {
        _gnss_worker_sm_report_event(
            GNSS_WORKER_EVENT_ABSOLUTE_MOTION, 0);
    }
}

void gnss_worker_sm_wwan_sleep_cb(uint32 sleepDuration)
{
    _StateData.wwanSleepDuration = sleepDuration;
    _gnss_worker_sm_report_event(
        GNSS_WORKER_EVENT_WWAN_SLEEP_CB, 0);
}

/***************************************************************************
    Out of context calls - not in GNSS Worker thread context
****************************************************************************/
// This one only used for synchronized waiting for best avail position
// This one gets invoked in location callback context
void gnss_worker_sm_singleshot_cb_in_loc_context(
    qapi_Location_t location, qapi_Location_Error_t err)
{
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

    // In case this is meant to be sent back for get best avail request
    if (QAPI_LOCATION_ERROR_SUCCESS == err && _StateData.isBestAvailSyncRequestPending)
    {
        memcpy(&_StateData.bestAvailLocation, &location, sizeof(qapi_Location_t));
    }

    // Set the singleshot callback signal
    // Passing NULL mutex since we are doing mutex get/put here directly
    app_utils_set_signal(
        _StateData.gnssWorkerSmSingleshotSignal, _StateData.gnssWorkerSmMutex);

    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
}

// This one only used for synchronized waiting for NTP Update Status
// This one gets invoked in meta callback context
void gnss_worker_sm_ntp_status_cb_meta_data_context(
    qapi_Time_AD_Status_t *ptimeAdStatus)
{
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

    // memcpy(&_StateData.timeAdStatus, &timeAdStatus, sizeof(qapi_Time_AD_Status_t));
    // memscpy(&_StateData.timeAdStatus, sizeof(qapi_Time_AD_Status_t), &(metaData.adStatusInfo.timeAdStatus), sizeof(qapi_Time_AD_Status_t));
   
    _StateData.timeAdStatus.size = ptimeAdStatus->size;
    _StateData.timeAdStatus.tuncMs = ptimeAdStatus->tuncMs;

    // Set the NTP Status callback signal
    // Passing NULL mutex since we are doing mutex get/put here directly
    app_utils_set_signal(
        _StateData.gnssWorkerSmNTPStatusSignal, _StateData.gnssWorkerSmMutex);

    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
}

// This one only used for synchronized waiting for XTRA Status
// This one gets invoked in meta callback context
void gnss_worker_sm_xtra_status_cb_meta_data_context(
    qapi_Xtra_AD_Status_t *pxtraAdStatus)
{
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);

    // memcpy(&_StateData.xtraAdStatus, &xtraAdStatus, sizeof(qapi_Xtra_AD_Status_t));
    // memscpy(&_StateData.xtraAdStatus, sizeof(qapi_Xtra_AD_Status_t), &(metaData.adStatusInfo.xtraAdStatus), sizeof(qapi_Xtra_AD_Status_t));
    
    _StateData.xtraAdStatus.size = pxtraAdStatus->size;
    _StateData.xtraAdStatus.xtraDataStartTime = pxtraAdStatus->xtraDataStartTime;
    _StateData.xtraAdStatus.xtraFileDurationHrs = pxtraAdStatus->xtraFileDurationHrs;
    _StateData.xtraAdStatus.xtraFileGenerationTime = pxtraAdStatus->xtraFileGenerationTime;

    // if(_StateData.xtraAdStatus.xtraDataStartTime == pxtraAdStatus->xtraDataStartTime)
    // {
    //     LOG_INFO("xtraDataStartTime copy success");
    // }

    // if(_StateData.xtraAdStatus.xtraFileDurationHrs == pxtraAdStatus->xtraFileDurationHrs)
    // {
    //     LOG_INFO("xtraFileDurationHrs copy success");
    // }

    // if(_StateData.xtraAdStatus.xtraFileGenerationTime == pxtraAdStatus->xtraFileGenerationTime)
    // {
    //     LOG_INFO("xtraFileGenerationTime copy success");
    // }

    // Set the XTRA Status callback signal
    // Passing NULL mutex since we are doing mutex get/put here directly
    app_utils_set_signal(
        _StateData.gnssWorkerSmXTRAStatusSignal, _StateData.gnssWorkerSmMutex);

    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
}

// This one only used for synchronized waiting for XTRA INT AD Update Status
// This one gets invoked in meta callback context
void gnss_worker_sm_xtra_int_status_cb_meta_data_context(
    qapi_Xtra_Int_AD_Status_t *pxtraIntAdStatus)
{
    app_utils_mutex_get(_StateData.gnssWorkerSmMutex);
    // memcpy(&_StateData.xtraIntAdStatus, &xtraInt, sizeof(qapi_Xtra_Int_AD_Status_t));
    // memscpy(&_StateData.xtraIntAdStatus, sizeof(qapi_Xtra_Int_AD_Status_t), &(metaData.adStatusInfo.xtraIntAdStatus), sizeof(qapi_Xtra_Int_AD_Status_t));

    _StateData.xtraIntAdStatus.size = pxtraIntAdStatus->size;
    _StateData.xtraIntAdStatus.lastXtraIntDlSuccess = pxtraIntAdStatus->lastXtraIntDlSuccess;
    
    // if(_StateData.xtraIntAdStatus.lastXtraIntDlSuccess == pxtraIntAdStatus->lastXtraIntDlSuccess)
    // {
    //     LOG_INFO("XTRA Int Params Copy Success");
    // }
    
    // LOG_INFO("gnss_worker_sm_xtra_int_status_cb_meta_data_context XTRA INT Status, size =  %d", pxtraIntAdStatus->size);
    // LOG_INFO("gnss_worker_sm_xtra_int_status_cb_meta_data_context XTRA INT Status, size =  %d", _StateData.xtraIntAdStatus.size);
    // LOG_INFO("gnss_worker_sm_xtra_int_status_cb_meta_data_context XTRA INT Status, lastXtraIntDlSuccess =  %llu", pxtraIntAdStatus->lastXtraIntDlSuccess);
    // LOG_INFO("gnss_worker_sm_xtra_int_status_cb_meta_data_context XTRA INT Status, lastXtraIntDlSuccess =  %llu", _StateData.xtraIntAdStatus.lastXtraIntDlSuccess);
    
    // Set the XTRA INT AD Status callback signal
    // Passing NULL mutex since we are doing mutex get/put here directly
    app_utils_set_signal(
            _StateData.gnssWorkerSmXTRAIntStatusSignal, _StateData.gnssWorkerSmMutex);

    app_utils_mutex_put(_StateData.gnssWorkerSmMutex);
}

void gnss_worker_sm_engine_status_cb_meta_data_context(
    qapi_Location_Meta_Data_t metaData)
{
    if((metaData.engineStatus == QAPI_LOCATION_ENGINE_STATUS_NTP_DL_SUCCESS) || (metaData.engineStatus == QAPI_LOCATION_ENGINE_STATUS_NTP_DL_FAILED))
    {
        app_utils_set_signal(
            _StateData.gnssWorkerSmNTPDLCompleteSignal, _StateData.gnssWorkerSmMutex);
    }

    if((metaData.engineStatus == QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_SUCCESS) || (metaData.engineStatus == QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_FAILED))
    {
        app_utils_set_signal(
            _StateData.gnssWorkerSmXTRADLCompleteSignal, _StateData.gnssWorkerSmMutex);
    }

    if((metaData.engineStatus == QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_SUCCESS) || (metaData.engineStatus == QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_FAILED))
    {
        app_utils_set_signal(
            _StateData.gnssWorkerSmXTRAIntDLCompleteSignal, _StateData.gnssWorkerSmMutex);
    }
}

void gnss_worker_sm_report_delay_timer_cb_in_timer_context(uint32 cbData)
{
    LOG_INFO("Report delay timer expiry");

    gnss_worker_send_msg_process_cb(_gnss_worker_sm_report_delay_timer_cb, cbData);
}

void gnss_worker_sm_backoff_timer_cb_in_timer_context(uint32 cbData)
{
    //LOG_INFO("Backoff timer expiry");

    gnss_worker_send_msg_process_cb(_gnss_worker_sm_backoff_timer_cb, cbData);
}

void gnss_worker_sm_position_refresh_timer_cb_in_timer_context(uint32 cbData)
{
    //LOG_INFO("Position Refresh timer expiry");

    gnss_worker_send_msg_process_cb(_gnss_worker_sm_position_refresh_timer_cb, cbData);
}

void gnss_worker_sm_ad_dl_complete_timer_cb_in_timer_context(uint32 cbData)
{
    LOG_INFO("AD DL Complete timer expiry");

    gnss_worker_send_msg_process_cb(_gnss_worker_sm_ad_dl_complete_timer_cb, cbData);
}
