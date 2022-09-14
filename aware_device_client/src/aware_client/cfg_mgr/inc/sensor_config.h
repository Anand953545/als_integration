#ifndef SENSOR_CONFIG_H_
#define SENSOR_CONFIG_H_

#include <stdint.h>

typedef struct
{
	uint16_t sensor_capabilities_mask;
	/*sensor_id*/
	int sensor_id;
	/*sensor_policy_dim*/
	int policy_dim;
	/*operating_mode_valid*/	 
	int operating_mode_valid;
	/*operating_mode*/	
	int operating_mode;
	/*perf_mode_valid*/	
	int perf_mode_valid;
	/*perf_mode*/	
	int perf_mode;
	/*pre_alert_threshold_high*/
	int pre_alert_threshold_high;
	/*pre_alert_threshold_low*/
	int pre_alert_threshold_low;
	/*alert_threshold_high*/
	int alert_threshold_high;
	/*alert_threshold_low*/	
	int alert_threshold_low;
	/*high_perf_measurement_period_valid*/
	int high_perf_measurement_period_valid;
	/*high_perf_measurement_period*/
	int high_perf_measurement_period;
	/*measurement_period_valid*/
	int measurement_period_valid;
	/*measurement_period*/
	int measurement_period;
	/*hysteresis_duration*/
	int hysteresis_duration_valid;
	/*hysteresis_duration*/
	int hysteresis_duration;
} sensor_config_t;

int sensor_config_create(sensor_config_t *, uint8_t);

#endif