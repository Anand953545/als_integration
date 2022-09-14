#ifndef RUN_TIME_CONFIG_H_
#define RUN_TIME_CONFIG_H_

#include <stdint.h>

typedef enum {
    RUN_TIME_CONFIG_LAST_DEVICE_STATE = 0,
    RUN_TIME_CONFIG_LAST_APP_STATE

} run_time_config_enum_t;

typedef struct
{
	uint8_t last_device_state;
    uint8_t last_app_state;
    uint8_t init_error;         //needs to check
    uint16_t awake_timeout;
    uint8_t curr_segment;
    uint64_t last_fix_time_ms;
    uint64_t last_check_time_ms;
    uint64_t start_time_ms;
    uint32_t stop_time;
    uint8_t num_check_ins;
    uint8_t fence_complete;
    uint8_t loc_tech_bm;
    uint8_t trans_already_checked;
    uint8_t sensor_already_checked;    
} run_time_config_t;


int run_time_fota_file_update(run_time_config_t*, uint8_t );
int run_time_config_put_cb(void*, uint8_t*);
int run_time_config_get_cb(void*, uint8_t*);

int run_time_config_create(run_time_config_t*);
int run_time_config_update(run_time_config_t*, uint8_t, run_time_config_enum_t);

#endif
