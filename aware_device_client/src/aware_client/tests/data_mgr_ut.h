#ifndef _DATA_MGR_UT_H_
#define _DATA_MGR_UT_H_

#include "minunit.h"

//DATA_MGR_DEVICE_INFO
MU_TEST(data_mgr_set_wwan_high_priority_success);
MU_TEST(data_mgr_get_device_serial_number_success);


//DATA_MGR_DSS
MU_TEST(is_valid_ip_address_success);

//DATA_MGR_THREAD
MU_TEST(dss_netctrl_start_success);
MU_TEST(dss_netctrl_stop_success);
MU_TEST(data_mgr_get_task_name_success);
MU_TEST(data_mgr_get_task_id_success);

//DATA_MGR
MU_TEST(data_mgr_connect_coap_server_success);
MU_TEST(data_mgr_disconnect_coap_server_success);
#endif