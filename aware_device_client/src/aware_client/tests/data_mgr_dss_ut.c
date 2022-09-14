
#include "data_mgr_dss_ut.h"
#include "data_mgr_dss.h"

void test_setup(void) {
    /* Test Data Initialization */
}

void test_teardown(void) {
	/* Test Data Cleanup */
}

MU_TEST(data_mgr_param_get_service_status_success)
{
	int status = data_mgr_param_get_service_status();
    mu_check(status == 0);
}

MU_TEST(data_mgr_dss_status_check_success)
{
	int status = data_mgr_dss_status_check();
    mu_check(status == 0);
}

MU_TEST(is_valid_ip_address_success)
{
	int status = is_valid_ip_address("0.0.0.0");
    mu_check(status == 0);
}

MU_TEST(data_mgr_dss_start_datacall_success)
{
	int status = data_mgr_dss_start_datacall();
    mu_check(status == 0);
}

MU_TEST(data_mgr_dss_stop_datacall_success)
{
	int status = data_mgr_dss_stop_datacall();
    mu_check(status == 0);
}