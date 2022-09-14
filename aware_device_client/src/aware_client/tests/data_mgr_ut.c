#include "data_mgr_ut.h"
#include "data_mgr_device_info.h"
#include "data_mgr_dss.h"
#include "data_mgr_thread.h"
#include "data_mgr.h"

void test_setup(void) {
    /* Test Data Initialization */
}

void test_teardown(void) {
	/* Test Data Cleanup */
}


//DATA_MGR_DEVICE_INFO.C
MU_TEST(data_mgr_set_wwan_high_priority_success) 
{
	int status = data_mgr_set_wwan_high_priority();
    mu_check(status == 0);
}

MU_TEST(data_mgr_get_device_serial_number_success) 
{
	uint8_t* status = data_mgr_get_device_serial_number();
    mu_check(status != NULL);
}


// DATA_MGR_DSS.C
MU_TEST(is_valid_ip_address_success)
{
	int status = is_valid_ip_address("0.0.0.0");
    mu_check(status == 0);
}


//DATA_MGR_THREAD.C

MU_TEST(dss_netctrl_start_success)
{
	int status = dss_netctrl_start();
    mu_check(status == 0);
}

MU_TEST(dss_netctrl_stop_success)
{
	int status = dss_netctrl_stop();
    mu_check(status == 0);
}


MU_TEST(data_mgr_get_task_name_success)
{
	char* status = data_mgr_get_task_name();
    mu_check(status != NULL);
}

MU_TEST(data_mgr_get_task_id_success)
{
	int status = data_mgr_get_task_id();
    mu_check(status != NULL);
}


//DATA_MGR.C
MU_TEST(data_mgr_connect_coap_server_success)
{
	int status = data_mgr_get_task_id();
    mu_check(status == 0);
}

MU_TEST(data_mgr_disconnect_coap_server_success)
{
	int status = data_mgr_get_task_id();
    mu_check(status == 0);
}
