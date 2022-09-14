#include "aware_app_state.h"
#include "aware_log.h"

extern app_context_t app_ctx;

/*-------------------------------------------------------------------------*/
/**
  @brief    Prints fota status
  @param    fota_state   Fota status
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void set_fota_state(fota_state_t fota_state)
{
	app_ctx.fota_state = fota_state;

	switch(fota_state)
	{
		case FOTA_FILE_DOWNLOAD:
			LOG_INFO("[aware_app]:fota state: FOTA_FILE_DOWNLOAD");
			break;
		case FOTA_READY_TO_APPLY:
			LOG_INFO("[aware_app]:fota state: FOTA_READY_TO_APPLY");
			break;
    	case FOTA_APPLYING:
			LOG_INFO("[aware_app]:fota state: FOTA_APPLYING");
			break;
    	case FOTA_NORAMAL_OPERATION:
			LOG_INFO("[aware_app]:fota state: FOTA_NORAMAL_OPERATION");
			break;
    	case FOTA_UPGRADE:
			LOG_INFO("[aware_app]:fota state: FOTA_UPGRADE");
		    break;
	}
}

int get_app_state(void)
{
	return app_ctx.runtime_cfg.last_app_state;
}

int get_device_state(void)
{
	return app_ctx.runtime_cfg.last_device_state;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Prints device status
  @param    device_state   Device status
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void set_device_state(device_state_t device_state)
{
	if(device_state != app_ctx.device_state) {
		//update device state in runtime config file
		runtime_config_update(&app_ctx.runtime_cfg, RUNTIME_CONFIG_LAST_DEVICE_STATE, device_state);
	}

	app_ctx.device_state = device_state;

	switch(device_state)
	{
		case DEVICE_STATE_ERROR:
			 LOG_INFO("[aware_app]:device state: ERROR");
			 break;
		case DEVICE_STATE_POWERING_UP:
			LOG_INFO("[aware_app]:device state: POWERING_UP");
			break;	
		case DEVICE_STATE_WAKING_UP:
			LOG_INFO("[aware_app]:device state: WAKING_UP");
			break;	
    	case DEVICE_STATE_APP_READY:
			LOG_INFO("[aware_app]:device state: APP_READY");
			break;
		case DEVICE_STATE_GOING_TO_SLEEP:
			LOG_INFO("[aware_app]:device state: GOING_TO_SLEEP");
			break;
		case DEVICE_STATE_POWERING_DOWN:
			LOG_INFO("[aware_app]:device state: POWERING_DOWN");
			break;
		case DEVICE_STATE_UPGRADE:
			LOG_INFO("[aware_app]:device state: UPGRADE");
			break;
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Prints app status
  @param    app_state   App status
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void set_app_state(app_state_t app_state)
{
	if(app_state != app_ctx.app_state) {
		//update app state in runtime config file
		runtime_config_update(&app_ctx.runtime_cfg, RUNTIME_CONFIG_LAST_APP_STATE, app_state);
	}

	app_ctx.app_state = app_state;

	switch(app_state)
	{
		case APP_STATE_INACTIVE:
			 	LOG_INFO("[aware_app]:app state:INACTIVE");
			 break;
		case APP_STATE_UNPROVISIONED:
				LOG_INFO("[aware_app]:app state:UNPROVISIONED");
			 break;
		case APP_STATE_PROVISIONED_BUT_NOT_IN_SERVICE:
			 	LOG_INFO("[aware_app]:app state:PROVISIONED_BUT_NOT_IN_SERVICE");
			 break;
    	case APP_STATE_ACTIVE:
				LOG_INFO("[aware_app]:app state:ACTIVE");
			 break;
    	case APP_STATE_ACTIVE_JOURNEY:
				LOG_INFO("[aware_app]:app state:ACTIVE_JOURNEY");
			 break;
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Prints app mode in use
  @param    app_state   App mode
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void set_app_mode(app_mode_t app_mode)
{
	app_ctx.app_mode=app_mode;
	switch(app_mode)
	{
		case APP_MODE_CHARGING:
			LOG_INFO("[aware_app]:app mode:CHARGING");
			break;
		case APP_MODE_TETHERED:
			LOG_INFO("[aware_app]:app mode:TETHERED");
			break;
		case APP_MODE_LOWPOWER:
			LOG_INFO("[aware_app]:app mode:LOWPOWER");	
			break;
		case APP_MODE_DOWNLOADING:
			LOG_INFO("[aware_app]:app mode:DOWNLOADING");
			break;
		case APP_MODE_HEALTHCHECK:
			LOG_INFO("[aware_app]:app mode:HEALTHCHECK");
			break;
		case APP_MODE_FARADAYCAGE:
			LOG_INFO("[aware_app]:app mode:FARADAYCAGE");
			break;
	}
}