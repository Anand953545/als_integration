#ifndef AWARE_APP_STATE_H_
#define AWARE_APP_STATE_H_

#include "aware_app.h"

void set_fota_state(fota_state_t fota_state);
void set_device_state(device_state_t device_state);
void set_app_state(app_state_t app_state);
void set_app_mode(app_mode_t app_mode);
int get_app_state(void);
int get_device_state(void);

#endif