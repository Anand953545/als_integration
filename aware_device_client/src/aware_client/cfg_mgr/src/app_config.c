#include "app_config.h"
#include "stringl.h"

#include "qapi_fs.h"
#include "aware_log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
#include "config_parser.h"
#include "app_utils_misc.h"

int app_config_load(app_config_t*);

/*-------------------------------------------------------------------------*/
/**
  @brief    Create application config file
  @param    app_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int app_config_create(app_config_t* app_cfg)
{
	int status = 0;

	if (app_cfg) {
		status = app_config_load(app_cfg);
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Load application config data to structure variable
  @param    app_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int app_config_load(app_config_t* app_cfg)
{
	qapi_Status_t status = QAPI_OK;
	
	if (NULL != app_cfg)
	{
		dict_t* d = config_parser_load(AWARE_APPS_CONFIG_FILE_PATH);

		app_cfg->report_state_transitions = config_parser_get_int(d, "rs");
		app_cfg->reporting_method = config_parser_get_int(d, "rb");
		app_cfg->journey_normal_checkin_period = config_parser_get_int(d, "ci.j.p");
		app_cfg->journey_lowbattery_checkin_period = config_parser_get_int(d, "ci.j.lb");
		app_cfg->active_normal_checkin_period = config_parser_get_int(d, "ci.i.p");
		app_cfg->active_lowbattery_checkin_period = config_parser_get_int(d, "ci.i.lb");
		app_cfg->active_sensor_enabled = config_parser_get_int(d, "ci.i.sc");
		app_cfg->factory_normal_checkin_period = config_parser_get_int(d, "ci.f.p");
		app_cfg->factory_lowbattery_checkin_period = config_parser_get_int(d, "ci.f.lb");
		app_cfg->factory_sensor_enabled = config_parser_get_int(d, "ci.f.sc");
		app_cfg->healthcheck_seconds_duration = config_parser_get_int(d, "hc.d");
		app_cfg->healthcheck_reportinterval_seconds = config_parser_get_int(d, "hc.ri");
		app_cfg->healthcheck_fence = config_parser_get_int(d, "hc.la");
		app_cfg->healthcheck_location_tech = config_parser_get_int(d, "hc.l");
		app_cfg->healthcheck_transmission = config_parser_get_int(d, "hc.t");
		app_cfg->healthcheck_sensor_enabled = config_parser_get_int(d, "hc.sc");
		app_cfg->threshold_low_battery = config_parser_get_int(d, "th.lb");
		app_cfg->threshold_low_storage = config_parser_get_int(d, "th.ls");
		app_cfg->threshold_low_ram = config_parser_get_int(d, "th.lr");
		app_cfg->sensor_diag_level = config_parser_get_string(d, "d.l");
		app_cfg->last_numBytes = config_parser_get_int(d, "d.lb");
		app_cfg->retention_duration_insec = config_parser_get_int(d, "d.s");
		app_cfg->system_diag_system = config_parser_get_int(d, "d.sd");
		app_cfg->system_diag_level = config_parser_get_int(d, "d.sl");

		config_parser_free(d);

		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
		LOG_INFO("[cfg_mgr]:               a.c.conf");
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
		LOG_INFO("[cfg_mgr]: report state transition: %d", app_cfg->report_state_transitions);
		LOG_INFO("[cfg_mgr]: reporting method: %d", app_cfg->reporting_method);
		LOG_INFO("[cfg_mgr]: journey normal checkin period: %d", app_cfg->journey_normal_checkin_period);
		LOG_INFO("[cfg_mgr]: journey lowbattery checkin period: %d", app_cfg->journey_lowbattery_checkin_period);
		LOG_INFO("[cfg_mgr]: active normal check in period: %d", app_cfg->active_normal_checkin_period);
		LOG_INFO("[cfg_mgr]: active lowbattery check in period: %d", app_cfg->active_lowbattery_checkin_period);
		LOG_INFO("[cfg_mgr]: active sensor enabled: %d", app_cfg->active_sensor_enabled);
		LOG_INFO("[cfg_mgr]: factory normal check in period: %d", app_cfg->factory_normal_checkin_period);
		LOG_INFO("[cfg_mgr]: factory lowbattery check in period: %d", app_cfg->factory_lowbattery_checkin_period);
		LOG_INFO("[cfg_mgr]: factory sensor enabled: %d", app_cfg->factory_sensor_enabled);
		LOG_INFO("[cfg_mgr]: health check seconds: %d", app_cfg->healthcheck_seconds_duration);
		LOG_INFO("[cfg_mgr]: health check reporting interval seconds: %d", app_cfg->healthcheck_reportinterval_seconds);
		LOG_INFO("[cfg_mgr]: health check fence: %d", app_cfg->healthcheck_fence);
		LOG_INFO("[cfg_mgr]: health check location tech: %d", app_cfg->healthcheck_location_tech);
		LOG_INFO("[cfg_mgr]: health check transmission: %d", app_cfg->healthcheck_transmission);
		LOG_INFO("[cfg_mgr]: health check sensor enabled: %d", app_cfg->healthcheck_sensor_enabled);
		LOG_INFO("[cfg_mgr]: threshold low battery: %d", app_cfg->threshold_low_battery);
		LOG_INFO("[cfg_mgr]: threshold low storage: %d", app_cfg->threshold_low_storage);
		LOG_INFO("[cfg_mgr]: threshold low ram: %d", app_cfg->threshold_low_ram);
		LOG_INFO("[cfg_mgr]: sensor diag level: %s", app_cfg->sensor_diag_level);
		LOG_INFO("[cfg_mgr]: last num bytes: %d", app_cfg->last_numBytes);
		LOG_INFO("[cfg_mgr]: retention during in sec: %d", app_cfg->retention_duration_insec);
		LOG_INFO("[cfg_mgr]: system diag sytem: %d", app_cfg->system_diag_system);
		LOG_INFO("[cfg_mgr]: system diag level: %d", app_cfg->system_diag_level);
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}

int app_config_update(void* self, char* key, char* valuestring)
{
	int ret = -1;

	app_config_t* app_cfg;
	app_cfg =(app_config_t*)self;

	//aware url update
	if(strcasecmp(key, AWARE_ACTIVE_NORMAL_CHECKIN_PERIOD_CONFIG_KEY) == 0) {
		dict_t* d = config_parser_load(AWARE_APPS_CONFIG_FILE_PATH);

		config_parser_set_string(d, "ci.i.p", valuestring);
		app_cfg->active_normal_checkin_period = atoi(valuestring);
		LOG_INFO("[cfg_mgr]: normal checkin period: %d", app_cfg->active_normal_checkin_period);

		config_parser_save(d, AWARE_APPS_CONFIG_FILE_PATH);
		config_parser_free(d);
	}

	return 0;
}