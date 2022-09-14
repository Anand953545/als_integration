#ifndef DATA_MGR_DEVICE_INFO_H_
#define DATA_MGR_DEVICE_INFO_H_

#include <stdint.h>

void data_mgr_init_device_info(void);
void data_mgr_release_device_info(void);
int data_mgr_reset_device_info(void);
int data_mgr_param_get_service_status(void);
uint8_t* data_mgr_get_device_serial_number(void);
int data_mgr_set_wwan_high_priority(void);

#endif