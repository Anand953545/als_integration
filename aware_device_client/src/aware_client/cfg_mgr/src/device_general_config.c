#include "device_general_config.h"
#include "stringl.h"

#include "qapi_fs.h"
#include "aware_log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
#include "config_parser.h"
#include "app_utils_misc.h"

int device_general_config_load(device_general_config_t*);

/*-------------------------------------------------------------------------*/
/**
  @brief    Create device general config file
  @param    device_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int device_general_config_create(device_general_config_t* device_cfg)
{
	int status = 0;

	if (device_cfg) {
		status = device_general_config_load(device_cfg);
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @@brief   Load device general config data to structure variable
  @param    dev_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int device_general_config_load(device_general_config_t* dev_cfg)
{
	qapi_Status_t status = QAPI_OK;
	if (NULL != dev_cfg)
	{
		dict_t* d = config_parser_load(AWARE_DEVICE_HC_GENERAL_CONFIG_FILE_PATH);

		dev_cfg->device_hw_version = config_parser_get_string(d, "v");
		dev_cfg->device_model = config_parser_get_string(d, "m");
		dev_cfg->batch_id = config_parser_get_string(d, "b");
		dev_cfg->manufacturer_id = config_parser_get_string(d, "mi");
		dev_cfg->demo_mode = config_parser_get_boolean(d, "dm");
		dev_cfg->sensor_capabilities_mask = config_parser_get_bitmask(d, "s.c");
		dev_cfg->transport_capabilities_mask = config_parser_get_bitmask(d, "tc.c");
		dev_cfg->location_capabilities_mask = config_parser_get_bitmask(d, "l.c");
		dev_cfg->security_capabilities_mask = config_parser_get_bitmask(d, "sc.c");

		config_parser_free(d);

		LOG_INFO("[cfg_mgr]:               d.h.g.conf");
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");

		LOG_INFO("[cfg_mgr]: device hw version: %s", dev_cfg->device_hw_version);
		LOG_INFO("[cfg_mgr]: device model: %s", dev_cfg->device_model);
		LOG_INFO("[cfg_mgr]: batch id: %s", dev_cfg->batch_id);
		LOG_INFO("[cfg_mgr]: manufacturer id: %s", dev_cfg->manufacturer_id);
		LOG_INFO("[aware_app]: demo mode: %d", dev_cfg->demo_mode);
		LOG_INFO("[cfg_mgr]: Sensor capabilities: %d", dev_cfg->sensor_capabilities_mask);
		LOG_INFO("[cfg_mgr]: Transport capabilities: %d", dev_cfg->transport_capabilities_mask);
		LOG_INFO("[cfg_mgr]: Location capabilities: %d", dev_cfg->location_capabilities_mask);
		LOG_INFO("[cfg_mgr]: Security capabilities: %d", dev_cfg->security_capabilities_mask);
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}

int device_general_config_update(void* self, uint8_t* data)
{
	//TODO: Add support for general config update
	return 0;
}