#ifndef _CORE_UT_H_
#define _CORE_UT_H_

#include "minunit.h"


//APP_THREAD.C
MU_TEST(app_thread_destroy_success);


//APP_UTILS_MEM.C
MU_TEST(app_utils_byte_create_pool_success);
MU_TEST(app_utils_get_byte_pool_success);

//AWARE_UTILS.C	
MU_TEST(get_current_time_in_milliseconds_success);
MU_TEST(aware_atoi_success);

//QAPI_LOC_WRAPPER.C
MU_TEST(qapi_loc_wrapper_init_success);
MU_TEST(qapi_loc_wrapper_deinit_success);
MU_TEST(qapi_loc_wrapper_cancel_single_shot_success);
MU_TEST(qapi_loc_wrapper_tracking_stop_success);
MU_TEST(qapi_loc_wrapper_get_best_available_position_success);
MU_TEST(qapi_loc_wrapper_stop_motion_tracking_success);


//FOTA_DAM_DOWNLOAD.C
MU_TEST(prepare_for_dam_download_success);



#endif