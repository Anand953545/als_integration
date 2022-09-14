/**
 * @file aware_psm_intf.c
 * @brief PSM interface for Tracker APP
 */
#include <stdarg.h>

#include "aware_psm.h"
#include "aware_log.h"
#include "aware_app.h"
#include "app_utils_misc.h"

bool psm_inited = false;
static aware_psm_context_t aware_psm_ctx;
extern TX_EVENT_FLAGS_GROUP *psm_signal_handle;

/*-------------------------------------------------------------------------*/
/**
  @brief    PSM  callback
  @param    psm_status  Structure of PSM status message type.
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_psm_cb(psm_status_msg_type *psm_status)
{
  if(psm_status)
  {
    if(psm_status->status != PSM_STATUS_HEALTH_CHECK &&
       psm_status->status != PSM_STATUS_NONE)
    {
      LOG_INFO(" aware_psm_cb:PSM STATUS:    %d", psm_status->status);
      LOG_INFO(" aware_psm_cb:REJECT REASON: %d\n", psm_status->reason);
    }

    switch(psm_status->status)
    {
      // special handling for some cases like modem loaded, not loaded etc.
      case PSM_STATUS_MODEM_LOADED:
      {
          aware_psm_ctx.is_modem_loaded = TRUE;
      }
      break;

      case PSM_STATUS_MODEM_NOT_LOADED:
      {
          aware_psm_ctx.is_modem_loaded = FALSE;
      }
      break;

      case PSM_STATUS_REJECT:
      {
        aware_psm_ctx.psm_msg.status = psm_status->status;
        aware_psm_ctx.psm_msg.reason = psm_status->reason;
      
        switch(psm_status->reason)
        {
          case PSM_REJECT_REASON_NOT_ENABLED:
          {
              aware_psm_ctx.psm_enabled = FALSE;
          }
          break;

          case PSM_REJECT_REASON_MODEM_NOT_READY:
          break;

          case PSM_REJECT_REASON_DURATION_TOO_SHORT:
          case PSM_REJECT_REASON_INCORRECT_USAGE:
          {
              /* Cancel vote for PSM and state */
              aware_cancel_vote_for_psm();
          }
          break;
        }
      }
      break;

      case PSM_STATUS_READY:
      case PSM_STATUS_NOT_READY:
      case PSM_STATUS_COMPLETE:
      case PSM_STATUS_DISCONNECTED:
      break;

      case PSM_STATUS_FEATURE_ENABLED:
      {
          aware_psm_ctx.psm_enabled = TRUE;
      }
      break;

      case PSM_STATUS_FEATURE_DISABLED:
      {
          aware_psm_ctx.psm_enabled = FALSE;
      }
      break;

      case PSM_STATUS_HEALTH_CHECK:
      {
        //tx_event_flags_set(psm_signal_handle, PSM_HEALTH_CHECK, TX_OR);
        //qapi_PSM_Client_Enter_Backoff(client_id);
      }
      break;

      case PSM_STATUS_NW_OOS:
      case PSM_STATUS_NW_LIMITED_SERVICE:
      {
        //tx_event_flags_set(psm_signal_handle, PSM_ENTER_BACKOFF, TX_OR);
      }
      break;

      default:
      break;
    }
  }
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Initialization of PSM
  @return   Void
 */
/*--------------------------------------------------------------------------*/
int aware_psm_init()
{
  qapi_PSM_Status_t  result = QAPI_ERROR;

  if(psm_inited == FALSE)
  {
    aware_psm_ctx.psm_enabled = TRUE;
    aware_psm_ctx.psm_wakeup_type = PSM_WAKEUP_MEASUREMENT_NW_ACCESS;
  
    result = qapi_PSM_Client_Register(&aware_psm_ctx.client_id, aware_psm_cb, &aware_psm_ctx.psm_msg);

    if (result == QAPI_OK)
    {
      LOG_INFO("Aware client registered with PSM: client id is %d\n", aware_psm_ctx.client_id);
    
      aware_psm_ctx.psm_registered = TRUE;
      psm_inited = TRUE;
    }
  }

  return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Deinitialization of PSM
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_psm_deinit()
{
  qapi_PSM_Status_t  result = QAPI_OK;

  if(psm_inited == TRUE)
  {
    result = qapi_PSM_Client_Unregister(aware_psm_ctx.client_id);
    LOG_INFO("qapi_PSM_Client_Unregister: Result: %s\n", result ? "FAIL":"SUCCESS");

    psm_inited = FALSE;
  }
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Selecting PSM
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_vote_for_psm()
{
  qapi_PSM_Status_t  result = QAPI_OK;
  psm_info_type      psm_info;
  memset(&psm_info, 0, sizeof(psm_info));

  psm_info.psm_time_info.time_format_flag     = PSM_TIME_IN_SECS;

  psm_info.active_time_in_secs = 60;
  psm_info.psm_wakeup_type = aware_psm_ctx.psm_wakeup_type;
  psm_info.psm_time_info.psm_duration_in_secs = 300;

  LOG_INFO("aware_vote_for_psm:  Duration in secs : %d \n", psm_info.psm_time_info.psm_duration_in_secs);
  result = qapi_PSM_Client_Enter_Psm(aware_psm_ctx.client_id, &psm_info);

  LOG_INFO("qapi_PSM_Client_Enter_Psm: Result: %s\n", result ? "FAIL":"SUCCESS");
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Calculate the PSM time
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void  aware_compute_psm_time()
{
  int i;
  uint64_t min_psm_wakeup = 0x7FFFFFFF;
  uint64_t expiry_time;
  uint64_t sensor_min_psm;
  uint64_t geofenceIn_expire;
  uint64_t timeactivity_expire;
  uint64_t geofenceIn_psm_time;
  uint64_t timeactivity_psm_time;
  qapi_time_get_t cur_time;

  qapi_time_get(QAPI_TIME_SECS, &cur_time);

  aware_psm_ctx.psm_wakeup_time = min_psm_wakeup;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    deselecting PSM
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_cancel_vote_for_psm(void)
{
  qapi_PSM_Status_t  result = QAPI_OK;

  result = qapi_PSM_Client_Cancel_Psm(aware_psm_ctx.client_id);

  LOG_INFO("qapi_PSM_Client_Cancel_Psm: Result: %s\n", result ? "FAIL":"SUCCESS");
}