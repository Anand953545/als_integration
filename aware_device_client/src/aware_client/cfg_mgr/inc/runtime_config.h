#ifndef RUN_TIME_CONFIG_H_
#define RUN_TIME_CONFIG_H_

#include <stdint.h>

typedef enum {
    RUNTIME_CONFIG_LAST_DEVICE_STATE = 0,
    RUNTIME_CONFIG_LAST_APP_STATE

} runtime_config_enum_t;

typedef struct
{
	uint8_t last_device_state;
    uint8_t last_app_state;
    uint8_t init_error;         //needs to check
    uint16_t awake_timeout;
    uint8_t curr_segment;
    uint32_t last_fix_time_ms;
    uint32_t last_check_time_ms;
    uint32_t start_time_ms;
    uint32_t stop_time;
    uint8_t num_check_ins;
    uint8_t fence_complete;
    uint8_t loc_tech_bm;
    uint8_t trans_already_checked;
    uint8_t sensor_already_checked;   
} runtime_config_t;


int runtime_fota_file_update(runtime_config_t*, uint8_t );
int runtime_config_put_cb(void*, uint8_t*);
int runtime_config_get_cb(void*, uint8_t*);

int runtime_config_create(runtime_config_t*);
int runtime_config_update(runtime_config_t*, runtime_config_enum_t, uint8_t);

#endif