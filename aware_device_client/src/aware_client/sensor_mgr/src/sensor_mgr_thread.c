#include "app_thread.h"
#include "app_utils_misc.h"
#include "sensor_mgr.h"
#include "sensor_mgr.h"

#include "aware_log.h"
#include "aware_main_thread.h"
#include "app_context.h"
#include "aware_main_thread.h"
#include "app_context.h"

TX_EVENT_FLAGS_GROUP *sensor_mgr_signal;
extern app_context_t app_ctx;

/* Thread Constants */
static app_thread_id _SensorMgrTaskId = APP_THREAD_ID_SENSOR_MGR;
static char* _SensorMgrTaskName = "sensor_mgr";

/*-------------------------------------------------------------------------*/
/**
  @brief Get task Id
  @return Task id
 */
/*--------------------------------------------------------------------------*/
int sensor_mgr_thread_get_task_id()
{
    return _SensorMgrTaskId;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Get task name
  @return Task name
 */
/*--------------------------------------------------------------------------*/
char* sensor_mgr_thread_get_task_name()
{
    return _SensorMgrTaskName;
}

/*-------------------------------------------------------------------------*/
/**
  @brief This is the entry point for Sensor Mgr task
  @return Void
 */
/*--------------------------------------------------------------------------*/
void sensor_mgr_thread_task_entry(ULONG args)
{
    LOG_INFO("[sensor_mgr]: Sensor Mgr Thread started. TCB %x TID %d", tx_thread_identify(), args);
    
	sensor_mgr_init();

    ///... Create flags for app_sensor_signal_hdlr
    if(TX_SUCCESS != txm_module_object_allocate((void **)&sensor_mgr_signal, sizeof(TX_EVENT_FLAGS_GROUP))) 
    {
		LOG_INFO("[sensor_mgr]: failed to allocated module object for app_sensor_signal_hdlr \n");
		return;
    }
  
   	tx_event_flags_create(sensor_mgr_signal, "sensor_events"); 
	tx_event_flags_set(sensor_mgr_signal, 0x0, TX_AND);

    unsigned long received_signal = 0;
    sensor_data_t sensor_data;

	app_ctx.sensor_mgr_state = SENSOR_MGR_SET_CONFIG;
	
	while(1)
	{	
		if(app_ctx.sensor_mgr_state == SENSOR_MGR_SET_CONFIG)
		{
			sensor_set_config(&app_ctx.als_sensor_cfg);
			sensor_set_config(&app_ctx.temperature_sensor_cfg);
			sensor_set_config(&app_ctx.pressure_sensor_cfg);
			sensor_set_config(&app_ctx.humidity_sensor_cfg);
			app_ctx.sensor_mgr_state = SENSOR_MGR_WAIT_FOR_CALLBACK;
		}
		else if(app_ctx.sensor_mgr_state == SENSOR_MGR_GET_CONFIG)
		{
			sensor_get_config();
			app_ctx.sensor_mgr_state = SENSOR_MGR_WAIT_FOR_CALLBACK;
		}
		else if(app_ctx.sensor_mgr_state == SENSOR_MGR_WAIT_FOR_CALLBACK)
		{
		//	LOG_INFO("[sensor_mgr]: Wait for call back events & Measurement period %d......\n",app_ctx.temperature_sensor_cfg.measurement_period+5);
		//	qapi_Timer_Sleep(app_ctx.temperature_sensor_cfg.measurement_period+5, QAPI_TIMER_UNIT_SEC, true);

			LOG_INFO("[sensor_mgr]: Wait for call back events & Measurement period %d......\n",app_ctx.als_sensor_cfg.measurement_period+5);
			qapi_Timer_Sleep(app_ctx.als_sensor_cfg.measurement_period+5, QAPI_TIMER_UNIT_SEC, true);

			tx_event_flags_get(sensor_mgr_signal, SENSOR_MGR_CALL_BACK_SET, TX_OR_CLEAR, &received_signal, 100*2 /*TX_NO_WAIT*/ );
			if((received_signal & SENSOR_MGR_CALL_BACK_SET) == SENSOR_MGR_CALL_BACK_SET)
			{
				LOG_INFO("[sensor_mgr]: SENSOR_MGR_CALL_BACK_SET set\n");
				tx_event_flags_set(sensor_mgr_signal, ~SENSOR_MGR_CALL_BACK_SET, TX_AND);

				app_ctx.sensor_mgr_state = SENSOR_MGR_GET_SENSOR_DATA;
			}
		}
		else if(app_ctx.sensor_mgr_state == SENSOR_MGR_GET_SENSOR_DATA)
		{
			sensor_get_data(&sensor_data);
    		app_ctx.sensor_mgr_state = SENSOR_MGR_WAIT_FOR_CALLBACK;
            aware_main_thread_send_msg_report_sensor_data(sensor_data);
		}
	}
}