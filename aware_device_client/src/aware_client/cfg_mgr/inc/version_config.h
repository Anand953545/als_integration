#ifndef VERSION_CONFIG_H_
#define VERSION_CONFIG_H_

#include <stdint.h>

#define AWARE_DEVICE_VERSION_CONFIG_FILE_PATH "/datatx/v.conf"

typedef struct
{
	char* modem_software_version;
	char* apps_software_version;
	char* client_software_version;
    uint16_t expire_after_hours;
	uint32_t time_available;
	uint64_t pkg_id;			//need to add data type as 128 bits
	uint8_t package_type;
	uint8_t file_count;
	uint8_t zipped;				//need to add data type as bool
	char* manifest_file;
	char* image_software_version;
	uint32_t size_ini_bytes;
	uint64_t crc;
	char* url_to_file_on_server;
} version_config_t;

int version_config_update(void*, uint8_t*);

int version_config_create(version_config_t*);

#endif