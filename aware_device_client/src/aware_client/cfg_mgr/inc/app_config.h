#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#include <stdint.h>

typedef struct
{
	int report_state_transitions;
	int reporting_method;
  int journey_normal_checkin_period;
  int journey_lowbattery_checkin_period;
  int active_normal_checkin_period;
  int active_lowbattery_checkin_period;
  int active_sensor_enabled;
  int factory_normal_checkin_period;
  int factory_lowbattery_checkin_period;
  int factory_sensor_enabled;
  int healthcheck_seconds_duration;
  int healthcheck_reportinterval_seconds;
  int healthcheck_fence;
  int healthcheck_location_tech;
  int healthcheck_transmission;
  int healthcheck_sensor_enabled;
  int threshold_low_battery;
  int threshold_low_storage;
  int threshold_low_ram;
  char* sensor_diag_level;
  int last_numBytes;
  int retention_duration_insec;
  int system_diag_system;
  int system_diag_level;
} app_config_t;

int app_config_update(void*, char*, char*);

int app_config_create(app_config_t*);

#endif