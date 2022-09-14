#include "sensor_config.h"
#include "aware_log.h"
#include "stringl.h"
#include "qapi_fs.h"
#include "aware_utils.h"
#include "dictionary.h"
#include "config_parser.h"
#include "cfg_mgr.h"
#include "qapi_sensor_mgr.h"
#include "sensor_mgr_utils.h"

int sensor_config_load(sensor_config_t *sensor_cfg, uint8_t sensor_id);

/*-------------------------------------------------------------------------*/
/**
  @brief    Create sensor config file
  @param    sensor_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int sensor_config_create(sensor_config_t *sensor_cfg, uint8_t sensor_id)
{
	if (sensor_cfg) {
		sensor_config_load(sensor_cfg, sensor_id);
	}

	return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Load sensor config data to structure variable
  @param    sensor_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int sensor_config_load(sensor_config_t *sensor_cfg, uint8_t sensor_id)
{
	qapi_Status_t status = QAPI_OK;
    char sensor_id_buffer[12] = {0};

	if (NULL != sensor_cfg)
	{
		dict_t* d = config_parser_load(AWARE_DEVICE_SENSOR_CONFIG_FILE_PATH);

        switch (sensor_id)
		{
			case QAPI_SENSOR_MGR_SENSOR_ID_ALS:
				sensor_cfg->sensor_id = config_parser_get_int(d, "c.1.id");
				sensor_cfg->alert_threshold_high = config_parser_get_float(d, "c.1.g.lt.1");
				sensor_cfg->alert_threshold_low = config_parser_get_float(d, "c.1.g.lr.1");
				sensor_cfg->pre_alert_threshold_high = get_pre_alert_threshold_high(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);	
				sensor_cfg->pre_alert_threshold_low = get_pre_alert_threshold_low(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);

				sensor_cfg->hysteresis_duration_valid = 1;		
				sensor_cfg->hysteresis_duration = config_parser_get_float(d, "c.1.g.hy");
				LOG_INFO("[cfg_mgr]:               ALS Sensor Config");
				break;
			case QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE:
				sensor_cfg->sensor_id = config_parser_get_int(d, "c.2.id");

				sensor_cfg->alert_threshold_high = config_parser_get_float(d, "c.2.g.ht.1");
				sensor_cfg->alert_threshold_low = config_parser_get_float(d, "c.2.g.lt.1");

				sensor_cfg->pre_alert_threshold_high = get_pre_alert_threshold_high(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);
				sensor_cfg->pre_alert_threshold_low = get_pre_alert_threshold_low(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);

				sensor_cfg->hysteresis_duration_valid = 1;
				sensor_cfg->hysteresis_duration = config_parser_get_float(d, "c.2.g.hy");
				LOG_INFO("[cfg_mgr]:               Pressure Sensor Config");
				break;
			case QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY:
				sensor_cfg->sensor_id = config_parser_get_int(d, "c.3.id"); 

				sensor_cfg->alert_threshold_high = config_parser_get_float(d, "c.3.g.ht.1");
				sensor_cfg->alert_threshold_low = config_parser_get_float(d, "c.3.g.lt.1");

				sensor_cfg->pre_alert_threshold_high = get_humidity_pre_alert_threshold_high(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);
				sensor_cfg->pre_alert_threshold_low = get_humidity_pre_alert_threshold_low(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);

				sensor_cfg->hysteresis_duration_valid = 1;
				sensor_cfg->hysteresis_duration = config_parser_get_float(d, "c.3.g.hy");
				LOG_INFO("[cfg_mgr]:               Humidity Sensor Config");
				break;
			case QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE:
				sensor_cfg->sensor_id = config_parser_get_int(d, "c.4.id");

				sensor_cfg->alert_threshold_high = config_parser_get_float(d, "c.4.g.ht.1");
				sensor_cfg->alert_threshold_low = config_parser_get_float(d, "c.4.g.lt.1");

				sensor_cfg->pre_alert_threshold_high = get_pre_alert_threshold_high(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);
				sensor_cfg->pre_alert_threshold_low = get_pre_alert_threshold_low(sensor_cfg->alert_threshold_high, sensor_cfg->alert_threshold_low);

				sensor_cfg->hysteresis_duration_valid = 1;
				sensor_cfg->hysteresis_duration = config_parser_get_float(d, "c.4.g.hy");

				LOG_INFO("[cfg_mgr]:               Temperature Sensor Config");
				break;	
			default:
				break;
		}

		sensor_cfg->sensor_capabilities_mask = config_parser_get_bitmask(d, "c");
		sensor_cfg->measurement_period_valid = 1;
		//for demo, reporting interval is considered as sampling period
		sensor_cfg->measurement_period = config_parser_get_int(d, "c.g.r");

		sensor_cfg->high_perf_measurement_period_valid = 1;
		sensor_cfg->high_perf_measurement_period = config_parser_get_int(d, "c.g.rh.1");

		sensor_cfg->operating_mode_valid = 1;
		sensor_cfg->operating_mode = 1;

		sensor_cfg->perf_mode_valid = 1;
		sensor_cfg->perf_mode = 1;

		sensor_cfg->policy_dim = 1;

		config_parser_free(d);

		LOG_INFO("[cfg_mgr]: ------------------------------------------------");	
		LOG_INFO("[cfg_mgr]: Sensor Capabilities Mask: %d", sensor_cfg->sensor_capabilities_mask);
		LOG_INFO("[cfg_mgr]: Sensor Id: %d", sensor_cfg->sensor_id);
		LOG_INFO("[cfg_mgr]: Sample Rate: %d", sensor_cfg->measurement_period);
		LOG_INFO("[cfg_mgr]: Pre Alert Threshold High: %d", sensor_cfg->pre_alert_threshold_high);
		LOG_INFO("[cfg_mgr]: Pre Alert Threshold Low: %d", sensor_cfg->pre_alert_threshold_low);
		LOG_INFO("[cfg_mgr]: Alert Threshold High: %d", sensor_cfg->alert_threshold_high);
		LOG_INFO("[cfg_mgr]: Alert Threshold Low: %d", sensor_cfg->alert_threshold_low);
		LOG_INFO("[cfg_mgr]: Hysteresis Duration: %d", sensor_cfg->hysteresis_duration);
		LOG_INFO("[cfg_mgr]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}