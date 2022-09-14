#include "version_config.h"
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

int version_config_load(version_config_t*);

/*-------------------------------------------------------------------------*/
/**
  @brief    Create version config file
  @param    version_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int version_config_create(version_config_t* version_cfg)
{
	int status = 0;

	if (version_cfg) {
		status = version_config_load(version_cfg);
	}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @@brief   Load version config data to structure variable
  @param    version_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int version_config_load(version_config_t* version_cfg)
{
	qapi_Status_t status = QAPI_OK;
	if (NULL != version_cfg)
	{
		dict_t* d = config_parser_load(AWARE_DEVICE_VERSION_CONFIG_FILE_PATH);

		version_cfg->modem_software_version = config_parser_get_string(d, "m.X");
		version_cfg->apps_software_version = config_parser_get_string(d, "a.X");
		version_cfg->client_software_version = config_parser_get_string(d, "c.X");
		version_cfg->expire_after_hours = config_parser_get_int(d, "u.xp");
		version_cfg->time_available = config_parser_get_int(d, "u.tm");
		version_cfg->pkg_id = config_parser_get_int(d, "u.id");
		version_cfg->package_type = config_parser_get_int(d, "u.ty");
		version_cfg->file_count = config_parser_get_int(d, "u.fc");
		version_cfg->zipped = config_parser_get_int(d, "u.zp");
		version_cfg->manifest_file = config_parser_get_string(d, "u.mn");
		version_cfg->image_software_version = config_parser_get_string(d, "u.<f>.sv");
		version_cfg->size_ini_bytes = config_parser_get_int(d, "u.<f>.sz");
		version_cfg->crc = config_parser_get_int(d, "u.<f>.crc");
		version_cfg->url_to_file_on_server = config_parser_get_string(d, "u.<f>.url");

		config_parser_free(d);

		LOG_INFO("[aware_app]:               v.conf");
		LOG_INFO("[aware_app]: ------------------------------------------------");

		LOG_INFO("[aware_app]: Modem X software version : %s", version_cfg->modem_software_version);
		LOG_INFO("[aware_app]: Apps X software version : %s", version_cfg->apps_software_version);
		LOG_INFO("[aware_app]: Client X software version: %s", version_cfg->client_software_version);
		LOG_INFO("[aware_app]: Expire after hours : %d", version_cfg->expire_after_hours);
		LOG_INFO("[aware_app]: Time available : %d", version_cfg->time_available);
		LOG_INFO("[aware_app]: Package ID :" "%" PRId64, version_cfg->pkg_id);
		LOG_INFO("[aware_app]: Package type : %d", version_cfg->package_type);
		LOG_INFO("[aware_app]: File count : %d", version_cfg->file_count);
		LOG_INFO("[aware_app]: Zipped : %d", version_cfg->zipped);
		LOG_INFO("[aware_app]: Manifest file : %s", version_cfg->manifest_file);
		LOG_INFO("[aware_app]: Software version of image : %s", version_cfg->image_software_version);
		LOG_INFO("[aware_app]: Size in bytes : %d", version_cfg->size_ini_bytes);
		LOG_INFO("[aware_app]: CRC :" "%" PRId64, version_cfg->crc);
		LOG_INFO("[aware_app]: URL to file on server : %s", version_cfg->url_to_file_on_server);
		
		LOG_INFO("[aware_app]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}

int version_config_update(void* self, uint8_t* data)
{
	//TODO: Add support for coap resource discovery
	return 0;
}