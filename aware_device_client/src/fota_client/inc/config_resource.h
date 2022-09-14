#ifndef COAP_RESOURCE_H_
#define COAP_RESOURCE_H_

#include <stdint.h>

typedef	int (*get_cb)(void*, uint8_t*);
typedef	int (*put_cb)(void*, uint8_t*);
	
typedef struct
{
	char uri_path[10];

	get_cb getCb;
	put_cb putCb;

} config_resource_t;

#endif