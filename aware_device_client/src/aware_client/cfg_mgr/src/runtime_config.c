#include "runtime_config.h"
#include "stringl.h"
#include <stdlib.h>

#include "qapi_fs.h"
#include "aware_log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
#include "aware_app.h"
#include "config_parser.h"
#include "app_utils_misc.h"

#define FOTA_UPGRADE_DONE	'6'
#define FOTA_UPGRADE_NONE	'3'

int runtime_config_load(runtime_config_t*);

/*-------------------------------------------------------------------------*/
/**
  @brief    Create run time config file
  @param    runtime_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int runtime_config_create(runtime_config_t* runtime_cfg)
{
	int status = 0;

	if (runtime_cfg) {
		status = runtime_config_load(runtime_cfg);
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Load runtime config data to structure variable
  @param    runtime_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int runtime_config_load(runtime_config_t* runtime_cfg)
{
	qapi_Status_t status = QAPI_OK;
	if (NULL != runtime_cfg)
	{
		dict_t* d = config_parser_load(AWARE_RUNTIME_CONFIG_FILE_PATH);

		runtime_cfg->last_device_state = config_parser_get_int(d, "st.dl");
		runtime_cfg->last_app_state = config_parser_get_int(d, "st.al");
		runtime_cfg->init_error = config_parser_get_int(d, "st.e");
		runtime_cfg->awake_timeout = config_parser_get_int(d, "st.s");
		runtime_cfg->curr_segment = config_parser_get_int(d, "j.cs");
		runtime_cfg->last_fix_time_ms = config_parser_get_int(d, "j.lf");
		runtime_cfg->last_check_time_ms = config_parser_get_int(d, "j.lc");
		runtime_cfg->start_time_ms = config_parser_get_int(d, "j.st");
		runtime_cfg->stop_time = config_parser_get_int(d, "hc.st");
		runtime_cfg->num_check_ins = config_parser_get_int(d, "hc.nc");
		runtime_cfg->fence_complete = config_parser_get_boolean(d, "hc.f.c");
		runtime_cfg->loc_tech_bm = config_parser_get_int(d, "hc.l.c");
		runtime_cfg->trans_already_checked = config_parser_get_int(d, "hc.t");
		runtime_cfg->sensor_already_checked = config_parser_get_int(d, "hc.s");

		config_parser_free(d);

		LOG_INFO("[cfg_mgr]:               a.r.p.conf");
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");	
		LOG_INFO("[cfg_mgr]: device state: %d", runtime_cfg->last_device_state);
		LOG_INFO("[cfg_mgr]: device app: %d", runtime_cfg->last_app_state);
		LOG_INFO("[cfg_mgr]: init error: %d", runtime_cfg->init_error);
		LOG_INFO("[cfg_mgr]: awaketimeout: %d", runtime_cfg->awake_timeout);
		LOG_INFO("[cfg_mgr]: segment: %d", runtime_cfg->curr_segment);
		LOG_INFO("[cfg_mgr]: last fix: %d", runtime_cfg->last_fix_time_ms);
		LOG_INFO("[cfg_mgr]: last checkintime: %d", runtime_cfg->last_check_time_ms);
		LOG_INFO("[cfg_mgr]: start time in ms: %d", runtime_cfg->start_time_ms);
		LOG_INFO("[cfg_mgr]: stop time: %d", runtime_cfg->stop_time);
		LOG_INFO("[cfg_mgr]: checkin: %d", runtime_cfg->num_check_ins);
		LOG_INFO("[cfg_mgr]: fence complete: %d", runtime_cfg->fence_complete);
		LOG_INFO("[cfg_mgr]: loc already: %d", runtime_cfg->loc_tech_bm);
		LOG_INFO("[cfg_mgr]: bm already checked: %d", runtime_cfg->trans_already_checked);
		LOG_INFO("[cfg_mgr]: sensor checked: %d", runtime_cfg->sensor_already_checked);
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    update runtime config data to structure variable
  @param    runtime_cfg		Pointer to structure to get relevant data
  @param	update_flag		value to be updated
  @param	last_state		runtime config last device state
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int runtime_config_update(runtime_config_t* runtime_cfg, runtime_config_enum_t last_state, uint8_t update_flag)
{
	qapi_Status_t status = QAPI_OK;

	if (NULL != runtime_cfg)
	{
		dict_t* d = config_parser_load(AWARE_RUNTIME_CONFIG_FILE_PATH);

		switch (last_state)
		{
			case RUNTIME_CONFIG_LAST_DEVICE_STATE:
				config_parser_set_int(d, "st.dl", update_flag);
				runtime_cfg->last_device_state = config_parser_get_int(d, "st.dl");
				LOG_INFO("[cfg_mgr]: device state: %d", runtime_cfg->last_device_state);
				config_parser_save(d,AWARE_RUNTIME_CONFIG_FILE_PATH);	
							
				break;
			case RUNTIME_CONFIG_LAST_APP_STATE:
				config_parser_set_int(d, "st.al", update_flag);
				runtime_cfg->last_app_state = config_parser_get_int(d, "st.al");
				LOG_INFO("[cfg_mgr]: device app: %d", runtime_cfg->last_app_state);
				config_parser_save(d,AWARE_RUNTIME_CONFIG_FILE_PATH);
				break;
			default:
				break;
		}

		config_parser_free(d);
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}