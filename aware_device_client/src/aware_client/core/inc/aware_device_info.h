#ifndef AWARE_DEVICE_INFO_H_
#define AWARE_DEVICE_INFO_H_

typedef enum {
    AWARE_DEVICE_INFO_IMEI,
    AWARE_DEVICE_INFO_IMSI,
    AWARE_DEVICE_INFO_ICCID
} aware_device_info_t;

char* get_aware_device_info(aware_device_info_t info);
int get_aware_device_sim_status(void);

#endif /* AWARE_DEVICE_INFO_H_ */