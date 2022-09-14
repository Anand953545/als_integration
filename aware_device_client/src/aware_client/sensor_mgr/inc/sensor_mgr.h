#ifndef SENSOR_MGR_H_
#define SENSOR_MGR_H_

#include "qapi_sensor_mgr.h"
#include "sensor_config.h"
#include "app_context.h"

typedef enum
{
    SENSOR_MGR_CALL_BACK_SET = (0x1<<1),
} sensor_mgr_events_t;

void check_sensor_id(int sensor_id);
int sensor_mgr_init(void);
void sensor_get_data(sensor_data_t*);
void sensor_get_config(void);
void sensor_set_config(sensor_config_t* sensor_cfg);
void sensor_state_id(int sensor_num);
void sensor_event_id(int event_id);
void sensor_mgr_release(void);

#endif