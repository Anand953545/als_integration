#ifndef DEVICE_HARDCODED_GENERAL_CONFIG_H_
#define DEVICE_HARDCODED_GENERAL_CONFIG_H_

#include <stdint.h>

typedef struct
{
	char* device_hw_version;
	char* device_model;
	char* batch_id;
    char* manufacturer_id;
    uint8_t demo_mode;
    uint16_t sensor_capabilities_mask;
    uint16_t transport_capabilities_mask;
    uint16_t location_capabilities_mask;
	uint16_t security_capabilities_mask;
} device_general_config_t;

int device_general_config_update(void*, uint8_t*);

int device_general_config_create(device_general_config_t*);

#endif