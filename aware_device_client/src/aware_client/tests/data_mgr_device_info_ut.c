#include "data_mgr_device_info_ut.h"
#include "data_mgr_device_info.h"

void test_setup(void) {
    /* Test Data Initialization */
}

void test_teardown(void) {
	/* Test Data Cleanup */
}

MU_TEST(data_mgr_set_wwan_high_priority_success) {
	int status = data_mgr_set_wwan_high_priority();
    mu_check(status == 0);
}

MU_TEST(data_mgr_reset_device_info_success) {
	int status = data_mgr_reset_device_info();
    mu_check(status != 0);
}

MU_TEST(data_mgr_param_get_service_status_success) {
	int status = data_mgr_param_get_service_status();
    mu_check(status != 0);
}

MU_TEST(data_mgr_get_device_serial_number_success) {
	uint8_t* status = data_mgr_get_device_serial_number();
    mu_check(status != NULL);
}