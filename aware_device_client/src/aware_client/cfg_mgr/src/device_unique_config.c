#include "device_unique_config.h"
#include "stringl.h"

#include "qapi_fs.h"
#include "aware_log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
#include "config_parser.h"
#include "app_utils_misc.h"

int device_unique_config_load(device_unique_config_t*);


/*-------------------------------------------------------------------------*/
/**
  @brief    Create device unique config file
  @param    device_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int device_unique_config_create(device_unique_config_t* device_cfg)
{
	int status = 0;

	if (device_cfg) {
		status = device_unique_config_load(device_cfg);
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Load device unique config data to structure variable
  @param    dev_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int device_unique_config_load(device_unique_config_t* dev_cfg)
{
	qapi_Status_t status = QAPI_OK;
	if (NULL != dev_cfg)
	{
		dict_t* d = config_parser_load(AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH);

		dev_cfg->device_id = config_parser_get_string(d, "ai");
		dev_cfg->serial_number = config_parser_get_string(d, "sn");
		dev_cfg->hw_id = config_parser_get_string(d, "hi");
		dev_cfg->device_type = config_parser_get_string(d, "dt");

		config_parser_free(d);

		LOG_INFO("[cfg_mgr]:               d.h.u.conf");
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");	
		LOG_INFO("[cfg_mgr]: Device Id: %s", dev_cfg->device_id);
		LOG_INFO("[cfg_mgr]: Serial number: %s", dev_cfg->serial_number);
		LOG_INFO("[cfg_mgr]: Hardware id: %s", dev_cfg->hw_id);
		LOG_INFO("[cfg_mgr]: Device type: %s", dev_cfg->device_type);
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}
	return status;
}

int device_unique_config_update(void* self, uint8_t* data)
{
	//TODO: Add support for device unique config update
	return 0;
}
