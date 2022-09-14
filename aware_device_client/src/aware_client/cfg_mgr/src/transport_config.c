#include "transport_config.h"
#include "cfg_mgr.h"
#include "aware_log.h"
#include "config_parser.h"

int transport_config_load(transport_config_t *transport_cfg);

/*-------------------------------------------------------------------------*/
/**
  @brief    Create transport config file
  @param    transport_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int transport_config_create(transport_config_t *transport_cfg)
{
	if (transport_cfg) {
		transport_config_load(transport_cfg);
	}

	return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Load transport config data to structure variable
  @param    dev_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int transport_config_load(transport_config_t* transport_cfg)
{
	qapi_Status_t status = QAPI_OK;

	if (NULL != transport_cfg)
	{
		dict_t* d = config_parser_load(AWARE_DEVICE_TRANSPORT_CONFIG_FILE_PATH);

		transport_cfg->transport_capabilities_mask = config_parser_get_bitmask(d, "c");
		transport_cfg->protocol = config_parser_get_int(d, "v.1");
		transport_cfg->no_service_timeout = config_parser_get_int(d, "v.2");
		transport_cfg->qos_retry_count = config_parser_get_int(d, "c.1.qr");
		transport_cfg->qos_retry_timer = config_parser_get_int(d, "c.1.qt");
		transport_cfg->max_bytes_per_message = config_parser_get_int(d, "c.1.mb");
		transport_cfg->ipv6 = config_parser_get_boolean(d, "c.1.ipv6");

		config_parser_free(d);

		LOG_INFO("[cfg_mgr]:               t.conf");
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");	
		LOG_INFO("[cfg_mgr]: Transport Bitmask: %d", transport_cfg->transport_capabilities_mask);
		LOG_INFO("[cfg_mgr]: Protocol: %d", transport_cfg->protocol);
		LOG_INFO("[cfg_mgr]: No Service Timeout: %d", transport_cfg->no_service_timeout);
		LOG_INFO("[cfg_mgr]: QoS Retries: %d", transport_cfg->qos_retry_count);
		LOG_INFO("[cfg_mgr]: QoS Retry Timer: %d", transport_cfg->qos_retry_timer);
		LOG_INFO("[cfg_mgr]: Max bytes per message: %d", transport_cfg->max_bytes_per_message);
		LOG_INFO("[cfg_mgr]: IPv6: %d", transport_cfg->ipv6);
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}