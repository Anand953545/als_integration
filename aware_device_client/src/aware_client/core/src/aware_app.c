#include "aware_app.h"
#include "aware_log.h"
#include "data_mgr.h"
#include "aware_health_check.h"
#include "app_context.h"

#include "app_thread.h"
#include "app_utils_misc.h"
#include "aware_main_thread.h"
#include "gnss_worker_thread.h"
#include "data_mgr_thread.h"
#include "fota_thread.h"
#include "sensor_mgr_thread.h"
#include "aware_app_state.h"
#include "aware_psm.h"

app_context_t app_ctx;

/*----------------------------------------------------------------------------*/
/**
  @brief    Aware dam application entry function
  @return   Success or failure
 */
/*--------------------------------------------------------------------------*/
int aware_dam_app_start()
{
    int ret = 0;

    aware_log_init(AWARE_LOG_DEBUG);

    // Some wait to allow for logging to initialize on bootup
    app_utils_sleep_ms_with_log(20 * 1000, "Bootup Delay");

#ifdef RUN_UNIT_TESTS
	//MU_RUN_SUITE(aware_test_suite);
	//MU_REPORT();
#endif

    LOG_INFO("[aware_app]: Starting application");
    LOG_INFO("[aware_app]: App Build Date and Time %s:%s\n", __DATE__, __TIME__);

    // Initialize platform utilities
    app_utils_byte_create_pool();
    app_thread_module_init();

    //app_ctx.app_mode = APP_MODE_HEALTHCHECK;
    set_app_mode(APP_MODE_HEALTHCHECK);
    ret = perform_health_check(&app_ctx);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: perform_health_check : failed\n");
        goto error;
    }

    //ret = aware_psm_init();
    //if(ret != 0) {
    //    LOG_ERROR("[aware_app]: failed  to  initialize PSM\n");
    //    goto error;
    //}

    //aware_vote_for_psm();

    ret = device_unique_config_create(&app_ctx.dhu_cfg);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: device_unique_config_create : failed\n");
        goto error;
    }

    ret = device_general_config_create(&app_ctx.dhg_cfg);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: device_general_config_create : failed\n");
        goto error;
    }

    ret = device_static_common_create(&app_ctx.dhc_cfg);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: device_static_common_create : failed\n");
        goto error;
    }

    ret = app_config_create(&app_ctx.app_cfg);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: app_config_create : failed\n");
        goto error;
    }

    ret = runtime_config_create(&app_ctx.runtime_cfg);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: app_config_create : failed\n");
        goto error;
    }


    //load ALS sensor config
	ret = sensor_config_create(&app_ctx.als_sensor_cfg, 1);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: app_config_create : failed\n");
        goto error;
    }

    //load pressure sensor config
	ret = sensor_config_create(&app_ctx.pressure_sensor_cfg, 2);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: app_config_create : failed\n");
        goto error;
    }

	//load humidity sensor config
    ret = sensor_config_create(&app_ctx.humidity_sensor_cfg, 3);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: app_config_create : failed\n");
        goto error;
    }

    //load temperature sensor config
	ret = sensor_config_create(&app_ctx.temperature_sensor_cfg, 4);
    if(ret != 0) {
        LOG_ERROR("[aware_app]: app_config_create : failed\n");
        goto error;
    }


    set_app_mode(APP_MODE_CHARGING);
    set_fota_state(FOTA_NORAMAL_OPERATION);

    if(app_ctx.runtime_cfg.last_device_state == DEVICE_STATE_UPGRADE)
    {    
        send_fota_trigger();
        set_device_state(DEVICE_STATE_APP_READY);
        fota_version_update(&app_ctx.dhc_cfg);
    }

    // Create GNSS Worker Thread
    LOG_INFO("Creating Location thread");
    if(0 != (ret = app_thread_create(
                        gnss_worker_get_task_name(),
                        gnss_worker_task_entry,
                        gnss_worker_get_task_id())))
    {
        LOG_ERROR("GNSS Worker thread creation failed (%d)", ret);
        return ret;
    }

    // Create CONTROLLER Thread
    LOG_INFO("Creating Aware Main Thread");
    if(0 != (ret = app_thread_create(
                        aware_main_thread_get_task_name(),
                        aware_main_thread_task_entry,
                        aware_main_thread_get_task_id())))
    {
        LOG_ERROR("Aware Main Thread creation failed (%d)", ret);
        return ret;
    }

    // Create Fota Thread
    LOG_INFO("Creating Fota Thread");
    if(0 != (ret = app_thread_create(
                        fota_thread_get_task_name(),
                        fota_thread_task_entry,
                        fota_thread_get_task_id())))
    {
        LOG_ERROR("Fota Thread creation failed (%d)", ret);
        return ret;
    }

    // Create Data Mgr Thread
    LOG_INFO("Creating Data Mgr Thread");
    if(0 != (ret = app_thread_create(
                        data_mgr_get_task_name(),
                        data_mgr_task_entry,
                        data_mgr_get_task_id())))
    {
        LOG_ERROR("Data Mgr Thread creation failed (%d)", ret);
        return ret;
    }

    // //skip collecting sensor data on demo mode
    // if(!app_ctx.dhg_cfg.demo_mode)
    // {
        // Create Sensor Mgr Thread
         LOG_INFO("Creating Sensor Mgr Thread");
         if(0 != (ret = app_thread_create(
                         sensor_mgr_thread_get_task_name(),
                             sensor_mgr_thread_task_entry,
                             sensor_mgr_thread_get_task_id())))
         {
             LOG_ERROR("Data Mgr Thread creation failed (%d)", ret);
             return ret;
         }
    // }

    app_utils_sleep_ms(500);

    aware_main_thread_send_msg_init();

error:
    LOG_INFO("Entry thread done.");

    return ret;
}