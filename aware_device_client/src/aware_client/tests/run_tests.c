#include "run_tests.h"

#include "data_mgr_ut.h"
#include "cfg_mgr_ut.h"
#include "core_ut.h"


MU_TEST_SUITE(aware_test_suite) {
   /*DATA_MGR_UT*/	
	//DATA_MGR_DEVICE_INFO
	MU_RUN_TEST(data_mgr_set_wwan_high_priority_success);
	MU_RUN_TEST(data_mgr_get_device_serial_number_success);


	//DATA_MGR_DSS
	MU_RUN_TEST(is_valid_ip_address_success);

	//DATA_MGR_THREAD
	MU_RUN_TEST(dss_netctrl_start_success);
	MU_RUN_TEST(dss_netctrl_stop_success);
	MU_RUN_TEST(data_mgr_get_task_name_success);
	MU_RUN_TEST(data_mgr_get_task_id_success);

	//DATA_MGR
	MU_RUN_TEST(data_mgr_connect_coap_server_success);
	MU_RUN_TEST(data_mgr_disconnect_coap_server_success);
	
	
	/*CORE*/

	//APP_THREAD.C
	MU_RUN_TEST(app_thread_destroy_success);

	//APP_UTILS_MEM.C
	MU_RUN_TEST(app_utils_byte_create_pool_success);
	MU_RUN_TEST(app_utils_get_byte_pool_success);
	
	//AWARE_UTILS.C	
	MU_RUN_TEST(get_current_time_in_milliseconds_success);
	MU_RUN_TEST(aware_atoi_success);
	
	//QAPI_LOC_WRAPPER.C
	MU_RUN_TEST(qapi_loc_wrapper_init_success);
	MU_RUN_TEST(qapi_loc_wrapper_deinit_success);
	MU_RUN_TEST(qapi_loc_wrapper_cancel_single_shot_success);
	MU_RUN_TEST(qapi_loc_wrapper_tracking_stop_success);
	MU_RUN_TEST(qapi_loc_wrapper_get_best_available_position_success);
	MU_RUN_TEST(qapi_loc_wrapper_stop_motion_tracking_success);
	
	
	//FOTA_DAM_DOWNLOAD.C
	MU_RUN_TEST(prepare_for_dam_download_success);

	/*CFG_MGR*/
	//DICTIONARY.C
	MU_RUN_TEST(streq_success);
	MU_RUN_TEST(dictionary_has_success);
	MU_RUN_TEST(dictionary_get_success);
	MU_RUN_TEST(dictionary_update_success);
	MU_RUN_TEST(dictionary_len_success);
	MU_RUN_TEST(dictionary_keys_success);
	MU_RUN_TEST(dictionary_values_success);

	
	//CONFIG_PARSER.C
	MU_RUN_TEST(config_parser_get_string_success);
	MU_RUN_TEST(config_parser_has_key_success);


}