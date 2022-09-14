#ifndef CFG_MGR_H_
#define CFG_MGR_H_

#define AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH "/datatx/d.h.u.conf"
#define AWARE_DEVICE_HC_GENERAL_CONFIG_FILE_PATH "/datatx/d.h.g.conf"
#define AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH "/datatx/d.s.conf"
#define AWARE_SECURITY_FILE_PATH "/datatx/coap_client.psk"
#define AWARE_APPS_CONFIG_FILE_PATH "/datatx/a.c.conf"
#define AWARE_DEVICE_SENSOR_CONFIG_FILE_PATH "/datatx/s.conf"
#define AWARE_RUNTIME_CONFIG_FILE_PATH "/datatx/a.r.p.conf"
#define AWARE_DEVICE_TRANSPORT_CONFIG_FILE_PATH "/datatx/t.conf"

#define AWARE_ENDPOINT_URL_CONFIG_KEY "d.s.u.1"
#define AWARE_ACTIVE_NORMAL_CHECKIN_PERIOD_CONFIG_KEY "a.c.ci.i.p"

int cfg_mgr_apply_update(char* msg, int msg_length);

#endif