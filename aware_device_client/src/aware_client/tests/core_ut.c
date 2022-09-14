#include "core_ut.h"
#include "app_thread.h"
#include "app_utils_misc.h"
#include "aware_utils.h"
#include "fota_dam_download.h"



//APP_THREAD.C

MU_TEST(app_thread_destroy_success) 
{
	int status = app_thread_destroy(0);
    mu_check(status != 0);
}




//APP_UTILS_MEM.C
MU_TEST(app_utils_byte_create_pool_success) 
{
	int status = app_utils_byte_create_pool();
    mu_check(status == 0);
}
	
MU_TEST(app_utils_get_byte_pool_success) 
{
	TX_BYTE_POOL *status = app_utils_get_byte_pool();
    mu_check(status != NULL);	
}
	

//AWARE_UTILS.C	
MU_TEST(get_current_time_in_milliseconds_success) 
{
	int status = get_current_time_in_milliseconds();
    mu_check(status != 0);
}
	
MU_TEST(aware_atoi_success) 
{
	int status = aware_atoi("a");
    mu_check(status != 0);
}
	
	
//QAPI_LOC_WRAPPER.C
	
MU_TEST(qapi_loc_wrapper_init_success) 
{
	int status = qapi_loc_wrapper_init();
    mu_check(status == 0);
}
	
MU_TEST(qapi_loc_wrapper_deinit_success) 
{
	int status = qapi_loc_wrapper_deinit();
    mu_check(status == 0);
}
	
MU_TEST(qapi_loc_wrapper_cancel_single_shot_success) 
{
	int status = qapi_loc_wrapper_cancel_single_shot();
    mu_check(status == 0);
}
	
MU_TEST(qapi_loc_wrapper_tracking_stop_success) 
{
	int status = qapi_loc_wrapper_tracking_stop();
    mu_check(status == 0);
}
	
MU_TEST(qapi_loc_wrapper_get_best_available_position_success) 
{
	int status = qapi_loc_wrapper_get_best_available_position();
    mu_check(status == 0);
}
	
MU_TEST(qapi_loc_wrapper_stop_motion_tracking_success) 
{
	int status = qapi_loc_wrapper_stop_motion_tracking();
    mu_check(status == 0);
}
	
//FOTA_DAM_DOWNLOAD.C
MU_TEST(prepare_for_dam_download_success) 
{
	int status = prepare_for_dam_download();
    mu_check(status == 0);
}
