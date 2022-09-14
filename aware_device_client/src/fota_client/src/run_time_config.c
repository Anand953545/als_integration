#include "run_time_config.h"
#include "stringl.h"
#include <stdlib.h>

#include "qapi_fs.h"
#include "log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
//#include "aware_app.h"
#include "config_parser.h"
#include "app_utils_misc.h"

#define FOTA_UPGRADE_DONE	'6'
#define FOTA_UPGRADE_NONE	'3'

int run_time_config_load(run_time_config_t*);

int run_time_config_create(run_time_config_t* run_time_cfg)
{
	int status = 0;

	if (run_time_cfg) {
		status = run_time_config_load(run_time_cfg);
	}

	return status;
}

int run_time_config_load(run_time_config_t* run_time_cfg)
{
	qapi_Status_t status = QAPI_OK;
	if (NULL != run_time_cfg)
	{
		dict_t* d = config_parser_load(AWARE_RUNTIME_CONFIG_FILE_PATH);

		run_time_cfg->last_device_state = config_parser_get_int(d, "st.dl");
		run_time_cfg->last_app_state = config_parser_get_int(d, "st.al");
		run_time_cfg->init_error = config_parser_get_int(d, "st.e");
		run_time_cfg->awake_timeout = config_parser_get_int(d, "st.s");
		run_time_cfg->curr_segment = config_parser_get_int(d, "j.cs");
		run_time_cfg->last_fix_time_ms = config_parser_get_int(d, "j.lf");
		run_time_cfg->last_check_time_ms = config_parser_get_int(d, "j.lc");
		run_time_cfg->start_time_ms = config_parser_get_int(d, "j.st");
		run_time_cfg->stop_time = config_parser_get_int(d, "hc.st");
		run_time_cfg->num_check_ins = config_parser_get_int(d, "hc.nc");
		run_time_cfg->fence_complete = config_parser_get_boolean(d, "hc.f.c");
		run_time_cfg->loc_tech_bm = config_parser_get_int(d, "hc.l.c");
		run_time_cfg->trans_already_checked = config_parser_get_int(d, "hc.t");
		run_time_cfg->sensor_already_checked = config_parser_get_int(d, "hc.s");

		config_parser_free(d);

		LOG_INFO("[aware_app]:               a.r.p.conf");
		LOG_INFO("[aware_app]: ------------------------------------------------");	
		LOG_INFO("[aware_app]: device state: %d", run_time_cfg->last_device_state);
		LOG_INFO("[aware_app]: app state: %d", run_time_cfg->last_app_state);
		LOG_INFO("[aware_app]: init error: %d", run_time_cfg->init_error);
		LOG_INFO("[aware_app]: awaketimeout: %d", run_time_cfg->awake_timeout);
		LOG_INFO("[aware_app]: segment: %d", run_time_cfg->curr_segment);
		LOG_INFO("[aware_app]: last fix: %d", run_time_cfg->last_fix_time_ms);
		LOG_INFO("[aware_app]: last checkintime: %d", run_time_cfg->last_check_time_ms);
		LOG_INFO("[aware_app]: start time in ms: %d", run_time_cfg->start_time_ms);
		LOG_INFO("[aware_app]: stop time: %d", run_time_cfg->stop_time);
		LOG_INFO("[aware_app]: checkin: %d", run_time_cfg->num_check_ins);
		LOG_INFO("[aware_app]: fence complete: %d", run_time_cfg->fence_complete);
		LOG_INFO("[aware_app]: loc already: %d", run_time_cfg->loc_tech_bm);
		LOG_INFO("[aware_app]: bm already checked: %d", run_time_cfg->trans_already_checked);
		LOG_INFO("[aware_app]: sensor checked: %d", run_time_cfg->sensor_already_checked);
		LOG_INFO("[aware_app]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}

int run_time_config_update(run_time_config_t* run_time_cfg, uint8_t update_flag, run_time_config_enum_t last_state)
{
	qapi_Status_t status = QAPI_OK;
	
	if (NULL != run_time_cfg)
	{
		dict_t* d = config_parser_load(AWARE_RUNTIME_CONFIG_FILE_PATH);

		switch (last_state)
		{
		case RUN_TIME_CONFIG_LAST_DEVICE_STATE:
			config_parser_set_int(d, "st.dl", update_flag);
			run_time_cfg->last_device_state = config_parser_get_int(d, "st.dl");
			LOG_INFO("[aware_app]: device state: %d", run_time_cfg->last_device_state);
			config_parser_save(d,AWARE_RUNTIME_CONFIG_FILE_PATH);	
						
			break;
		case RUN_TIME_CONFIG_LAST_APP_STATE:
			config_parser_set_int(d, "st.al", update_flag);
			run_time_cfg->last_app_state = config_parser_get_int(d, "st.al");
			LOG_INFO("[aware_app]: device app: %d", run_time_cfg->last_app_state);
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

int run_time_config_put_cb(void* self, uint8_t* data)
{
	//TODO: Add support for coap resource discovery
	return 0;
}

int run_time_config_get_cb(void* self, uint8_t* data)
{
	//TODO: Add support for coap resource discovery
	return 0;
}
