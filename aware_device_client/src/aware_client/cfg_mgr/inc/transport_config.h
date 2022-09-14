#ifndef TRANSPORT_CONFIG_H_
#define TRANSPORT_CONFIG_H_

#include <stdint.h>

typedef struct
{
	uint16_t transport_capabilities_mask;
	uint8_t protocol;
	uint8_t no_service_timeout;
	uint8_t qos_retry_count;
	uint8_t qos_retry_timer;
	uint16_t max_bytes_per_message;
	uint8_t ipv6;
} transport_config_t;

int transport_config_create(transport_config_t*);

#endif /* TRANSPORT_CONFIG_H_ */