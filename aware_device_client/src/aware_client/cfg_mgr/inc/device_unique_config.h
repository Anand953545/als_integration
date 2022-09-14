#ifndef DEVICE_HARDCODED_UNIQUE_CONFIG_H_
#define DEVICE_HARDCODED_UNIQUE_CONFIG_H_

#include <stdint.h>

typedef struct
{
	char* device_id;
	char* serial_number;
	char* hw_id;
	char* device_type;
} device_unique_config_t;

int device_unique_config_update(void*, uint8_t*);

int device_unique_config_create(device_unique_config_t*);

#endif