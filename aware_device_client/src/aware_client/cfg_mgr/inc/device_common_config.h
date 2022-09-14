#ifndef DEVICE_STATIC_COMMON_CONFIG_H_
#define DEVICE_STATIC_COMMON_CONFIG_H_

#include <stdint.h>

typedef struct
{
	uint8_t security_capabilities_mask;
	uint8_t security_method;
	char* sw_version;
	char* dss_apn;
    char* aware_url;
    char* aware_alt_url;
    char* ntp_server_url;
    char* operation_start_time;
    char* operation_until_time;
    char* nw_wait_timer;
} device_common_config_t;

int device_static_common_update(void*, char*, char*);

int device_static_common_create(device_common_config_t*);
int fota_version_update(device_common_config_t*);

#endif