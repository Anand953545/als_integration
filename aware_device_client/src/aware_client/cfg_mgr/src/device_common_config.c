#include "device_common_config.h"
#include "runtime_config.h"
#include "stringl.h"

#include "qapi_fs.h"
#include "aware_log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
#include "config_parser.h"
#include "app_utils_misc.h"
#include "cJSON.h"
#include "app_context.h"
#include "aware_app.h"
#include "aware_app_state.h"

int device_static_common_load(device_common_config_t*);

/*-------------------------------------------------------------------------*/
/**
  @brief    Create device static common config file
  @param    device_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int device_static_common_create(device_common_config_t* device_cfg)
{
	int status = 0;

	if (device_cfg) {
		status = device_static_common_load(device_cfg);
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Load device static common config data to structure variable
  @param    dev_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int device_static_common_load(device_common_config_t* dev_cfg)
{
	qapi_Status_t status = QAPI_OK;

	if (NULL != dev_cfg)
	{
		dict_t* d = config_parser_load(AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);

		
		dev_cfg->security_capabilities_mask = config_parser_get_bitmask(d, "sc.c");
		dev_cfg->security_method = config_parser_get_int(d, "sc.t");
		dev_cfg->sw_version = config_parser_get_string(d, "sw.v");
		dev_cfg->dss_apn = config_parser_get_string(d, "cn.apn");
		dev_cfg->aware_url = config_parser_get_string(d, "u.1");
		dev_cfg->aware_alt_url = config_parser_get_string(d, "u.2");
		dev_cfg->ntp_server_url = config_parser_get_string(d, "u.3");
		dev_cfg->operation_start_time = config_parser_get_string(d, "v.1");
		dev_cfg->operation_until_time = config_parser_get_string(d, "v.2");
		dev_cfg->nw_wait_timer = config_parser_get_string(d, "v.3");

		config_parser_free(d);

		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
		LOG_INFO("[cfg_mgr]:               d.s.conf");
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
		LOG_INFO("[cfg_mgr]: Security capabilities: %d", dev_cfg->security_capabilities_mask);
		LOG_INFO("[cfg_mgr]: security_method: %d", dev_cfg->security_method);
		LOG_INFO("[cfg_mgr]: sw_version: %s", dev_cfg->sw_version);
		LOG_INFO("[cfg_mgr]: dss_apn: %s", dev_cfg->dss_apn);
		LOG_INFO("[cfg_mgr]: aware_url: %s", dev_cfg->aware_url);
		LOG_INFO("[cfg_mgr]: aware_alt_url: %s", dev_cfg->aware_alt_url);
		LOG_INFO("[cfg_mgr]: ntp_server_url: %s", dev_cfg->ntp_server_url);
		LOG_INFO("[cfg_mgr]: operation_start_time: %s", dev_cfg->operation_start_time);
		LOG_INFO("[cfg_mgr]: operation_until_time: %s", dev_cfg->operation_until_time);
		LOG_INFO("[cfg_mgr]: nw_wait_timer: %s", dev_cfg->nw_wait_timer);
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
  @brief    Load  fota version update data to device static common config file
  @param    dev_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int fota_version_update(device_common_config_t* dev_cfg)
{
	qapi_Status_t status = QAPI_OK;

	if (NULL != dev_cfg)
	{
		dict_t* d = config_parser_load(AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);
		config_parser_set_string(d, "sw.v", "1.1.0");
		dev_cfg->sw_version = config_parser_get_string(d, "sw.v");
		LOG_INFO("[cfg_mgr]: sw_version: %s", dev_cfg->sw_version);
		config_parser_save(d,AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);
		config_parser_free(d);
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    updates the device static configuratioin
  @param    self		Pointer to structure to get relevant data
  @param    char		Pointer to dictionary key
  @param    self		Pointer to the value to be updated
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int device_static_common_update(void* self, char* key, char* valuestring)
{
	int ret = -1;

	device_common_config_t* dev_cfg;
	dev_cfg=(device_common_config_t*)self;

	//aware url update
	if(strcasecmp(key, AWARE_ENDPOINT_URL_CONFIG_KEY) == 0) {
		dict_t* d = config_parser_load(AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);
		snprintf(dev_cfg->aware_url,strlen(valuestring)+9,"%s/message",valuestring);

		config_parser_set_string(d, "u.1", dev_cfg->aware_url);
		dev_cfg->aware_url = config_parser_get_string(d, "u.1");
		LOG_INFO("[cfg_mgr]: aware_url: %s", dev_cfg->aware_url);

		config_parser_save(d, AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);
		config_parser_free(d);
	}

	set_app_state(APP_STATE_ACTIVE);

	if(app_ctx.runtime_cfg.last_device_state == DEVICE_STATE_UPGRADE)
		set_device_state(DEVICE_STATE_APP_READY);

	return ret;
}