#include "sensor_mgr.h"
#include "qapi_sensor_mgr.h"
#include "app_utils_misc.h"
#include "aware_log.h"
#include "app_context.h"

qapi_Sensor_Mgr_Hndl_t sensor_handle;

extern TX_EVENT_FLAGS_GROUP *sensor_mgr_signal;

signed int sensor_value = 0;
unsigned int samples_available_to_read = 0;
unsigned int sample_dimensions_available = 0;

/**********************************************************************************
*	function name:sensor_ind_callback
*	Decription: This is asynchronous call back function for sensors
*   @param[in] qapi_Sensor_Mgr_Hndl_t - Sensor manager handler
*   @param[in] qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t - Sensor data indication message.

*********************************************************************************/
void sensor_ind_callback(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl,const qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *ind_msg)
{
    LOG_INFO("[sensor_mgr]: ==sensor_ind_callback:entry()===");
	
	///...checking sensor handler status
    if((sensor_mgr_hndl != sensor_handle) || ind_msg == NULL)
    {
        LOG_INFO("[sensor_mgr]: sensor_ind_callback:Invalid Params");
		//return;
    }
	
	//...Msg IG for sensor INDication
	if(ind_msg->type!= QAPI_SENSOR_MGR_SENSOR_IND_E)
	{
	   LOG_INFO("[sensor_mgr]: sensor_ind_callback:invalid type");
	   //return;
	}
	else
	{
		//...Sensor data indication message- IF Policy met mask valid.	
		if(ind_msg->ind.ind_data->policy_met_sensor_mask_valid)
	  	{
			LOG_INFO("[sensor_mgr]: sensor_ind_callback:policy_met_sensor_mask_valid is SET");

			LOG_INFO("[sensor_mgr]: sensor_ind_callback:ind_msg->ind.ind_data->policy_met_sensor_mask %d",ind_msg->ind.ind_data->policy_met_sensor_mask);

			if((ind_msg->ind.ind_data->policy_met_sensor_mask & QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK)==QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:policy_met_sensor_mask for QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK");
			//   return;
			}
			else if((ind_msg->ind.ind_data->policy_met_sensor_mask & QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:policy_met_sensor_mask for QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK");
			}
			else if((ind_msg->ind.ind_data->policy_met_sensor_mask & QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:policy_met_sensor_mask for QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK");
			}
			else if((ind_msg->ind.ind_data->policy_met_sensor_mask & QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:policy_met_sensor_mask for QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK");
			}
			else
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:policy_met_sensor_mask  is valid");
			}
	  	}
		else
	  	{
			LOG_INFO("[sensor_mgr]: sensor_ind_callback:policy_met_sensor_mask_valid is NOT SET");
			//  return;
	  	}
		
		//...Sensor data indication message- Batch full mask valid.	  
		if(ind_msg->ind.ind_data->batch_full_sensor_mask_valid)
	  	{
			LOG_INFO("[sensor_mgr]: sensor_ind_callback:batch_full_sensor_mask_valid is SET");
			LOG_INFO("[sensor_mgr]: sensor_ind_callback:ind_msg->ind.ind_data->batch_full_sensor_mask %d",ind_msg->ind.ind_data->batch_full_sensor_mask);
		   
			if((ind_msg->ind.ind_data->batch_full_sensor_mask  & QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:batch_full_sensor_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK");
			//   return;
			}
			else if((ind_msg->ind.ind_data->batch_full_sensor_mask & QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:batch_full_sensor_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK");
			}
			else if((ind_msg->ind.ind_data->batch_full_sensor_mask & QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:batch_full_sensor_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK");
			}
			else if((ind_msg->ind.ind_data->batch_full_sensor_mask & QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:batch_full_sensor_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK");
			}
			else
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:batch_full_sensor_mask_valid  is valid");
			}
	   	}
		else
	   	{
			LOG_INFO("[sensor_mgr]: sensor_ind_callback:batch_full_sensor_mask_valid is NOT SET");
			// return;
	   	}
		
		//...Sensor data indication message- Bad state Sensor mask valid   
	   	if(ind_msg->ind.ind_data->sensor_fail_cond_met_mask_valid)
	  	{
	  	  LOG_INFO("[sensor_mgr]: sensor_ind_callback:sensor_fail_cond_met_mask_valid is SET");
	   
		   LOG_INFO("[sensor_mgr]: sensor_ind_callback:ind_msg->ind.ind_data->sensor_fail_cond_met_mask %d",ind_msg->ind.ind_data->sensor_fail_cond_met_mask);
			if((ind_msg->ind.ind_data->sensor_fail_cond_met_mask & QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:sensor_fail_cond_met_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK");
			//	 return;
			}
			else if((ind_msg->ind.ind_data->sensor_fail_cond_met_mask & QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:sensor_fail_cond_met_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK");
			}
			else if((ind_msg->ind.ind_data->sensor_fail_cond_met_mask & QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:sensor_fail_cond_met_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK");
			}
			else if((ind_msg->ind.ind_data->sensor_fail_cond_met_mask & QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK) == QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK)
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:sensor_fail_cond_met_mask_valid for QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK");
			}
			else
			{
				LOG_INFO("[sensor_mgr]: sensor_ind_callback:sensor_fail_cond_met_mask_valid  is valid");
			}
	   	}
		else
	   	{
			LOG_INFO("[sensor_mgr]: sensor_ind_callback:sensor_fail_cond_met_mask_valid is NOT SET");
			// return;
	   	}
	}

	tx_event_flags_set(sensor_mgr_signal, SENSOR_MGR_CALL_BACK_SET, TX_OR);
}

void sensor_state_id(int sensor_num)
{
	///... It will print the logs for sensor state after call back	
	if(sensor_num == QAPI_SENSOR_MGR_SENSOR_STATE_MIN_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id %d for :QAPI_SENSOR_MGR_SENSOR_STATE_MIN_E",sensor_num);
	}
	else if(sensor_num == QAPI_SENSOR_MGR_MEASUREMENT_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id %d for :QAPI_SENSOR_MGR_MEASUREMENT_E",sensor_num);
	}
	else if(sensor_num == QAPI_SENSOR_MGR_PRE_ALERT_LOW_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id  %d for :QAPI_SENSOR_MGR_PRE_ALERT_LOW_E",sensor_num);
	}
	else if(sensor_num == QAPI_SENSOR_MGR_PRE_ALERT_HIGH_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id %d for :QAPI_SENSOR_MGR_PRE_ALERT_HIGH_E",sensor_num);
	}
	else if(sensor_num == QAPI_SENSOR_MGR_OUT_OF_RANGE_LOW_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id  %d for :QAPI_SENSOR_MGR_OUT_OF_RANGE_LOW_E",sensor_num);
	}
	else if(sensor_num == QAPI_SENSOR_MGR_OUT_OF_RANGE_HIGH_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id %d for :QAPI_SENSOR_MGR_OUT_OF_RANGE_HIGH_E",sensor_num);
	}
	else if(sensor_num == QAPI_SENSOR_MGR_OPERATION_RANGE_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id  %d for :QAPI_SENSOR_MGR_OPERATION_RANGE_E",sensor_num);
	}
	else if(sensor_num == QAPI_SENSOR_MGR_SENSOR_STATE_MAX_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id  %d for :QAPI_SENSOR_MGR_SENSOR_STATE_MAX_E",sensor_num);
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_state_id %d for :Default Id not valid",sensor_num);
	}
}

/*********************************************************************************
*	function name:sensor_event_id
*	Decription: Prints the specific sensor event in text format,after readin the sensor data
*   @param[in]         sensor ID
*********************************************************************************/
void sensor_event_id(int event_id)
{
	///...It will print the sensor event occured, after call back and when we read sensor data	
	if(event_id == QAPI_SENSOR_MGR_SENSOR_STATE_MIN_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id %d for :QAPI_SENSOR_MGR_SENSOR_STATE_MIN_E",event_id);
	}
	else if(event_id == QAPI_SENSOR_MGR_MEASUREMENT_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id %d for :QAPI_SENSOR_MGR_MEASUREMENT_E",event_id);
	}
	else if(event_id == QAPI_SENSOR_MGR_PRE_ALERT_LOW_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id  %d for :QAPI_SENSOR_MGR_PRE_ALERT_LOW_E",event_id);
	}
	else if(event_id == QAPI_SENSOR_MGR_PRE_ALERT_HIGH_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id %d for :QAPI_SENSOR_MGR_PRE_ALERT_HIGH_E",event_id);
	}
	else if(event_id == QAPI_SENSOR_MGR_OUT_OF_RANGE_LOW_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id  %d for :QAPI_SENSOR_MGR_OUT_OF_RANGE_LOW_E",event_id);
	}
	else if(event_id == QAPI_SENSOR_MGR_OUT_OF_RANGE_HIGH_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id %d for :QAPI_SENSOR_MGR_OUT_OF_RANGE_HIGH_E",event_id);
	}
	else if(event_id == QAPI_SENSOR_MGR_OPERATION_RANGE_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id  %d for :QAPI_SENSOR_MGR_OPERATION_RANGE_E",event_id);
	}
	else if(event_id == QAPI_SENSOR_MGR_SENSOR_STATE_MAX_E)
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id  %d for :QAPI_SENSOR_MGR_SENSOR_STATE_MAX_E",event_id);
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_event_id %d for :Default Id not valid",event_id);
	}
}

/*********************************************************************************
*	function name:check_sensor_id
*	Decription: Prints the respective senor_num senor ID
*   @param[in] int        sensor ID.
*********************************************************************************/
void check_sensor_id(int sensor_id)
{
	if(sensor_id == QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE)
	{
		LOG_INFO("[sensor_mgr]: check_sensor_id %d for :QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK",sensor_id);
	}
	else if(sensor_id == QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE)
	{
		LOG_INFO("[sensor_mgr]: check_sensor_id %d for :QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK",sensor_id);
	}
	else if(sensor_id == QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY)
	{
		LOG_INFO("[sensor_mgr]: check_sensor_id  %d for :QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK",sensor_id);
	}
	else if(sensor_id == QAPI_SENSOR_MGR_SENSOR_ID_ALS)
	{
		LOG_INFO("[sensor_mgr]: check_sensor_id  %d for :QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK",sensor_id);
	}
	else
	{
		LOG_INFO("[sensor_mgr]: check_sensor_id %d for :Default Id not valid",sensor_id);
	}
}

/**********************************************************************************
*	function name:sensor_mgr_init
*	Decription: Creates a new sensor manager context.
*   			Initlize the sensor related structures and handlers
*   @param[in] cbk         void.
*********************************************************************************/
int sensor_mgr_init(void)
{
	int ret = QAPI_ERROR;
	
	ret = qapi_Sensor_Mgr_Init(&sensor_handle);
	if(ret != QAPI_OK)
	{
        LOG_INFO("[sensor_mgr]: sensor_init : qapi_Sensor_Mgr_Init failed!ret = %d", ret);
        return 1;
	}

	ret = qapi_Sensor_Mgr_Pass_Pool_Ptr(sensor_handle, app_utils_get_byte_pool());
	if(ret != QAPI_OK)
	{
        LOG_INFO("[sensor_mgr]: sensor_init : qapi_Sensor_Mgr_Pass_Pool_Ptr failed! ret = %d", ret);
        sensor_mgr_release();
        return 1;
	}

	ret = qapi_Sensor_Mgr_Set_Callback(sensor_handle, QAPI_SENSOR_MGR_SENSOR_IND_E, sensor_ind_callback);
	if(ret != QAPI_OK)
	{
        LOG_INFO("[sensor_mgr]: sensor_init : qapi_Sensor_Mgr_Set_Callback failed! ret = %d", ret);
        return 1;
	}

	return 0;
}

/**********************************************************************************
*	function name:release_sensor_handler
*	Decription: De-init a sensor manager context.
*   @param[in] cbk         void.
*********************************************************************************/
void sensor_mgr_release(void)
{
	qapi_Status_t ret= QAPI_ERROR;
	LOG_INFO("[sensor_mgr]: sensor_mgr_release entry()");

	ret = qapi_Sensor_Mgr_Release(sensor_handle);
	if(ret != QAPI_OK)
	{
		LOG_INFO("[sensor_mgr]: sensor_init: qapi_Sensor_Mgr_Release FAIL");
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_init: qapi_Sensor_Mgr_Release Success");
	}
}

/*********************************************************************************
*	function name:sensor_get_data
*	Decription: Creates a new sensor manager context.
*   			Initlize the sensor related structures and handlers
*	@dependencies qapi_Sensor_Mgr_Init().
*   @param[in] cbk         void.
*********************************************************************************/
void sensor_get_data(sensor_data_t* sensor_data) 
{
	int ret =-1;

	qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t *get_data_req;
	qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t *get_data_resp;

	sensor_data->als = 0;
	sensor_data->temperature = 0;
	sensor_data->humidity = 0;
	sensor_data->pressure = 0;

	LOG_INFO("[sensor_mgr]: =====================sensor_get_data:entry()====================");

	///...Allocate memory for request ptr.
	get_data_req=(qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t));
	
	if(NULL == get_data_req)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: Alloc qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t memory fail");
		return; 
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_get_data: qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t));
	}

	///...Clear the strucuture to zeros.	
	memset(get_data_req,0,sizeof(qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t));

	///...Request sensor data type
	get_data_req->type = QAPI_SENSOR_MGR_GET_SENSOR_DATA_REQ_E;

	//.Allocate memory for configuration structure
  	get_data_req->req.get_sensor_data =(qapi_Sensor_Mgr_Sensor_Data_Req_t *)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Data_Req_t));
 	
	if(NULL == get_data_req->req.get_sensor_data)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: Alloc get_data_req->req.get_sensor_data memory fail");
		
		///...release the memories of outer structure ie., above allocated pointers
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req releasing memory----------");
		app_utils_mem_free(get_data_req);

		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: get_data_req->req.get_sensor_data success");
		LOG_INFO("[sensor_mgr]: sensor_get_data: size of qapi_Sensor_Mgr_Sensor_Data_Req_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Data_Req_t));
	}

	///...Masking the sensor ID mask
	get_data_req->req.get_sensor_data->sensor_id_mask = QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK | QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK | QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK | QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK ;

	///...Must be set true , for logging mode
	get_data_req->req.get_sensor_data->is_logging_mode_valid = 1;

	///...logging mode passing zero ,needs to check with dev team.
	get_data_req->req.get_sensor_data->is_logging_mode = 0;// 1- fetching the data for logging and QMI wil not re-flash the global variables, for multiple times call
								// 0- will delete the global variables data and gives details, call only once after call back,

	///...Allocate memory for response ptr.
	get_data_resp =(qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t));
	if(NULL == get_data_resp)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: Alloc qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t memory fail");
		
		///...release the memories of outer structure ie., above allocated pointers
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req->req.get_sensor_data releasing memory----------");
		app_utils_mem_free(get_data_req->req.get_sensor_data);
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req releasing memory----------");
		app_utils_mem_free(get_data_req);

		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t memory success");
	}

	///...Clear the strucuture to zeros.
	memset(get_data_resp,0,sizeof(qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t));

	///...Request respose type to collect data
	get_data_resp->type = QAPI_SENSOR_MGR_GET_SENSOR_DATA_RESP_E;

	///...Allocate memory for internal structure
	get_data_resp->resp.get_sensor_data_rsp = (qapi_Sensor_Mgr_Sensor_Data_Resp_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Data_Resp_t));
	
	if(get_data_resp->resp.get_sensor_data_rsp == NULL)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: Alloc qapi_Sensor_Mgr_Sensor_Data_Resp_t memory fail");
		
		///...release the memories of outer structure ie., above allocated pointers
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_resp releasing memory----------");
		app_utils_mem_free(get_data_resp);

		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req->req.get_sensor_data releasing memory----------");
		app_utils_mem_free(get_data_req->req.get_sensor_data);

		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req releasing memory----------");
		app_utils_mem_free(get_data_req);

		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: qapi_Sensor_Mgr_Sensor_Data_Resp_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_get_data: size of qapi_Sensor_Mgr_Sensor_Data_Resp_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Data_Resp_t));
	}

	LOG_INFO("[sensor_mgr]: sensor_get_data:----CALLING qapi_Sensor_Mgr_Get_Sensor_Data");

	///...call the get sensor data with handler, request structue and pass the response structure
	ret = qapi_Sensor_Mgr_Get_Sensor_Data(sensor_handle,get_data_req,get_data_resp);

	if(ret!=QAPI_OK)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data:qapi_Sensor_Mgr_Get_Sensor_Data failed to get");
		
		///...release the memories of outer structure ie., above allocated pointers
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_resp->resp.get_sensor_data_rsp releasing memory----------");
		app_utils_mem_free(get_data_resp->resp.get_sensor_data_rsp);

		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_resp releasing memory----------");
		app_utils_mem_free(get_data_resp);

		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req->req.get_sensor_data releasing memory----------");
		app_utils_mem_free(get_data_req->req.get_sensor_data);

		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req releasing memory----------");
		app_utils_mem_free(get_data_req); 
		
        return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: qapi_Sensor_Mgr_Get_Sensor_Data succeeded to get");
	}

	LOG_INFO("[sensor_mgr]: //////------/////////  Get_Sensor_Data  ///////-------/////////");	
	
	LOG_INFO("[sensor_mgr]: sensor_get_data: sensor_info_valid = %d",get_data_resp->resp.get_sensor_data_rsp->sensor_info_valid);
	
	///...no. of sensors data that got on structure
	LOG_INFO("[sensor_mgr]: sensor_get_data: sensor_info_len = %d",get_data_resp->resp.get_sensor_data_rsp->sensor_info_len);

    int no_of_sensors = get_data_resp->resp.get_sensor_data_rsp->sensor_info_len;
    for(int sensor_idx =0; sensor_idx < no_of_sensors; sensor_idx++)
	{
		///...Sensor ID 
		LOG_INFO("[sensor_mgr]: sensor_get_data: sensor_id = %d",get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sensor_id);

		///...get sensor ID
		sensor_value = get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sensor_id;
		check_sensor_id(sensor_value);

		///..Sensor sample batch length
		LOG_INFO("[sensor_mgr]: sensor_get_data: sample_batch_len = %d",get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch_len);

		///...get the length of array 
		samples_available_to_read = get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch_len;

		///..Sensor samples that we got
		for(int i =0; i< samples_available_to_read;i++)
		{
			LOG_INFO("[sensor_mgr]: sensor_get_data: sample_batch[%d].sample_dim(no.of sensor Dim) = %d",i,
				get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch[i].sample_dim);

			///... for motion sensors dimensional array might be 2/3 so for that we needs to print/read samples for every dimension.
			//..if sensor is multi-demesnsional sensor we needs to array for each dimension.
			///... to print the sample values for no. of dimensions ,For now we are processing for only single dimension sensor
			LOG_INFO("[sensor_mgr]: sensor_get_data: sample_batch[%d].sample[0] = %d",i,
				(int)get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch[i].sample[0]);

			switch (sensor_value)
			{
				case QAPI_SENSOR_MGR_SENSOR_ID_ALS:
					sensor_data->als = get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch[i].sample[0];
					break;
				case QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE:
					sensor_data->temperature = get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch[i].sample[0];
					break;
				case QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE:
					sensor_data->pressure = get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch[i].sample[0];
					break;
				case QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY:
					sensor_data->humidity = get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch[i].sample[0];
					break;
				default:
					break;
			}

			///..Sensor timestamp that logged
			LOG_INFO("[sensor_mgr]: sensor_get_data: sample_batch[%d].timestamp = %d",i,
				get_data_resp->resp.get_sensor_data_rsp->sensor_data[sensor_idx].sample_batch[i].timestamp);
		}

		///...Algorithm info valid
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_info_valid = %d",get_data_resp->resp.get_sensor_data_rsp->algo_info_valid);

		///...Number of sensors whose algorithm info is contained in response.
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_info_len = %d",get_data_resp->resp.get_sensor_data_rsp->algo_info_len);

		///...Sensor ID that mask
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_data[0].sensor_id = %d",get_data_resp->resp.get_sensor_data_rsp->algo_data[0].sensor_id);

		///...get the sensor ID
		sensor_value = get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].sensor_id;
		check_sensor_id(sensor_value);

		///...sensor event mes/High_pre_alt/high_alt/low_thr/pre_low_thr
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_data[0].event_type = %d",get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].event_type);
		sensor_value = get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].event_type;

		sensor_event_id(sensor_value);

		///...sensor state mes/High_pre_alt/high_alt/low_thr/pre_low_thr
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_data[0].sensor_state = %d",get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].sensor_state);

		///...Get the sensor state
		sensor_value = get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].sensor_state;

		sensor_state_id(sensor_value);

		///...High Performance -Indicates time remaining (in mins) until sensor measurements breach threshold
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_data[0].ttc = %d",(int)get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].ttc);

		///..High Performance: Indicates rate of change of sensor measurements
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_data[0].slope = %d",(int)get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].slope);

		///...High Performance mesaurement perodicity
		LOG_INFO("[sensor_mgr]: sensor_get_data: algo_data[0].high_perf_measurement_period = %d",get_data_resp->resp.get_sensor_data_rsp->algo_data[sensor_idx].high_perf_measurement_period);

	}

	///..making flag false to stop print logs in output file
	///...releasing memory for inner structure first
	if((get_data_resp != NULL)&&(get_data_resp->resp.get_sensor_data_rsp != NULL))
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_resp->resp.get_sensor_data_rsp releasing memory----------");
		app_utils_mem_free(get_data_resp->resp.get_sensor_data_rsp);
	}

	///...Releasing memory for outer structure
	if(get_data_resp != NULL)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_resp releasing memory----------");
		app_utils_mem_free(get_data_resp);
	}

	///...Releasing the memory
	if((get_data_req != NULL)&&(get_data_req->req.get_sensor_data != NULL))
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req->req.get_sensor_data releasing memory----------");
		app_utils_mem_free(get_data_req->req.get_sensor_data);
	}

	if(get_data_req != NULL)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_data: - ----------- get_data_req releasing memory----------");
		app_utils_mem_free(get_data_req);
	}

    return;
}

/*********************************************************************************
*	function name:sensor_set_config
*	Decription: Creates a new sensor manager context.
*   			Initlize the sensor related structures and handlers
*	@dependencies qapi_Sensor_Mgr_Init().
*   @param[in] cbk         void.
*********************************************************************************/
void sensor_set_config(sensor_config_t* sensor_cfg) 
{
	qapi_Status_t ret = QAPI_ERROR;

	qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t *Set_Req_ptr = NULL;
	qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t *Set_Rsp_ptr = NULL;

	//...Select no. of sensors to configure
	unsigned long int sensor_cfg_len = 1;

	LOG_INFO("[sensor_mgr]: =====================sensor_set_config:  entry()====================");
	LOG_INFO("[sensor_mgr]: *****************************************************************************");

	Set_Req_ptr=(qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t));
	
	if(NULL == Set_Req_ptr)
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: Alloc qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t memory fail");
		///...returning directly
		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_set_config:  size of qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t));
	}

    memset(Set_Req_ptr,0,sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t));
	
	//.condifure the type for set/get
  	Set_Req_ptr->type=QAPI_SENSOR_MGR_SET_CONFIG_REQ_E;

	//.Allocate memory for configuration structure
  	Set_Req_ptr->req.set_cfg=(qapi_Sensor_Mgr_Sensor_Set_Config_Policy_t *)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Set_Config_Policy_t));
 	
	if(NULL == Set_Req_ptr->req.set_cfg)
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: Alloc configs->req.set_cfg memory fail");
		
		///...Releasing the above structure pointers first
		app_utils_mem_free(Set_Req_ptr);

		return; 
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: configs->req.set_cfg memory success");
		LOG_INFO("[sensor_mgr]: sensor_set_config: sizeof qapi_Sensor_Mgr_Sensor_Set_Config_Policy_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Set_Config_Policy_t));
	}

	memset(Set_Req_ptr->req.set_cfg,0,sizeof(qapi_Sensor_Mgr_Sensor_Set_Config_Policy_t));

	//..Set the sensor ID needs to set	
	Set_Req_ptr->req.set_cfg->sensor_id = sensor_cfg->sensor_id;
	LOG_INFO("[sensor_mgr]: sensor_set_config: Sensior ID %d",Set_Req_ptr->req.set_cfg->sensor_id);

	//..needs to understand purpose of policy dimension (no of sensors might be)//////////////////////////////////////////////
	Set_Req_ptr->req.set_cfg->sensor_policy_dim=sensor_cfg->policy_dim;//config_ptr->sensor_policy_dim;
	LOG_INFO("[sensor_mgr]: sensor_set_config: Sensior policy dim = %d",Set_Req_ptr->req.set_cfg->sensor_policy_dim);

	//..Set the threashold values  
	Set_Req_ptr->req.set_cfg->policy_cond[0].alert_threshold_high=sensor_cfg->alert_threshold_high;
	Set_Req_ptr->req.set_cfg->policy_cond[0].alert_threshold_low=sensor_cfg->alert_threshold_low;
	Set_Req_ptr->req.set_cfg->policy_cond[0].pre_alert_threshold_high=sensor_cfg->pre_alert_threshold_high;
	Set_Req_ptr->req.set_cfg->policy_cond[0].pre_alert_threshold_low=sensor_cfg->pre_alert_threshold_low;

	LOG_INFO("[sensor_mgr]: sensor_set_config: alert_threshold_high = %d",(int)Set_Req_ptr->req.set_cfg->policy_cond[0].alert_threshold_high);
	LOG_INFO("[sensor_mgr]: sensor_set_config: alert_threshold_low = %d",(int)Set_Req_ptr->req.set_cfg->policy_cond[0].alert_threshold_low);
	LOG_INFO("[sensor_mgr]: sensor_set_config: pre_alert_threshold_high = %d",(int)Set_Req_ptr->req.set_cfg->policy_cond[0].pre_alert_threshold_high);
	LOG_INFO("[sensor_mgr]: sensor_set_config: pre_alert_threshold_low = %d",(int)Set_Req_ptr->req.set_cfg->policy_cond[0].pre_alert_threshold_low);

	//..Must be set to true to enable or disable the sensor
	Set_Req_ptr->req.set_cfg->operating_mode_valid=sensor_cfg->operating_mode_valid;
	LOG_INFO("[sensor_mgr]: sensor_set_config: operating mode valid = %d",Set_Req_ptr->req.set_cfg->operating_mode_valid);

	//..to enable - set 1 or else disable - set 0 
	Set_Req_ptr->req.set_cfg->operating_mode=sensor_cfg->operating_mode;
	LOG_INFO("[sensor_mgr]: sensor_set_config: Operating mode = %d",Set_Req_ptr->req.set_cfg->operating_mode);

	//..Must be set to true to enable or disable the sensor performance
	Set_Req_ptr->req.set_cfg->perf_mode_valid=sensor_cfg->perf_mode_valid;
	LOG_INFO("[sensor_mgr]: sensor_set_config: Perf_mode_valid = %d",Set_Req_ptr->req.set_cfg->perf_mode_valid);
	Set_Req_ptr->req.set_cfg->perf_mode=sensor_cfg->perf_mode;
	LOG_INFO("[sensor_mgr]: sensor_set_config: perf_mode = %d",Set_Req_ptr->req.set_cfg->perf_mode);

	//..Must be set to true to enable or disable the sensor hysteresis(lags) with value 
	Set_Req_ptr->req.set_cfg->hysteresis_duration_valid=sensor_cfg->hysteresis_duration_valid;
	LOG_INFO("[sensor_mgr]: sensor_set_config: hysteresis_duration_valid = %d",Set_Req_ptr->req.set_cfg->hysteresis_duration_valid);
	Set_Req_ptr->req.set_cfg->hysteresis_duration=sensor_cfg->hysteresis_duration;
	LOG_INFO("[sensor_mgr]: sensor_set_config: hysteresis_duration = %d",Set_Req_ptr->req.set_cfg->hysteresis_duration);

	//.  Must be set to true to enable or disable the sensor measurement period is being passed
	Set_Req_ptr->req.set_cfg->high_perf_measurement_period_valid=sensor_cfg->high_perf_measurement_period_valid;
	LOG_INFO("[sensor_mgr]: sensor_set_config: high_perf_measurement_period_valid = %d",Set_Req_ptr->req.set_cfg->high_perf_measurement_period_valid);
	//.required to set the min or max samples for reference ,Needs to check with dev team
	Set_Req_ptr->req.set_cfg->high_perf_measurement_period=sensor_cfg->high_perf_measurement_period;
	LOG_INFO("[sensor_mgr]: sensor_set_config: high_perf_measurement_period = %d",Set_Req_ptr->req.set_cfg->high_perf_measurement_period);

	//.Polling time of sensors during high performance mode in secs 60/900/30
	Set_Req_ptr->req.set_cfg->measurement_period_valid=sensor_cfg->measurement_period_valid;
	LOG_INFO("[sensor_mgr]: sensor_set_config: measurement_period_valid = %d",Set_Req_ptr->req.set_cfg->measurement_period_valid);
	Set_Req_ptr->req.set_cfg->measurement_period=sensor_cfg->measurement_period;
	LOG_INFO("[sensor_mgr]: sensor_set_config: measurement_period = %d",Set_Req_ptr->req.set_cfg->measurement_period);
 
	//...Set the response sructure ,Allocate memory for response structure
    Set_Rsp_ptr=(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t));
    
	if(NULL == Set_Rsp_ptr)
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: Alloc qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t memory fail");
		
		///...Releasing the above structure pointers first
		app_utils_mem_free(Set_Req_ptr->req.set_cfg);
		app_utils_mem_free(Set_Req_ptr);

		return; 
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_set_config: size of qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t));
	}

    memset(Set_Rsp_ptr,0,sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t));

    //...Set the response type 
    Set_Rsp_ptr->type=QAPI_SENSOR_MGR_SET_CONFIG_RESP_E;

	//...Allocate memory for internal response structure 
    Set_Rsp_ptr->resp.set_cfg_resp=(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t));
    
	if(NULL == Set_Rsp_ptr->resp.set_cfg_resp)
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: Alloc qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t memory fail");
		
		///...Releasing the above structure pointers first
		app_utils_mem_free(Set_Rsp_ptr);
		app_utils_mem_free(Set_Req_ptr->req.set_cfg);
		app_utils_mem_free(Set_Req_ptr);

		return; 
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_set_config: size of qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t));
	}

    memset(Set_Rsp_ptr->resp.set_cfg_resp,0,sizeof(qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t));
    //resp->resp.set_cfg_resp->sensor_resp_len=16; 

    //...call the QAPI with request and response structure
	ret= qapi_Sensor_Mgr_Set_Sensor_Config(sensor_handle,Set_Req_ptr,1,Set_Rsp_ptr);
	
	if(ret!=QAPI_OK)
	{
		LOG_INFO("[sensor_mgr]: sensor_init:qapi_Sensor_Mgr_Set_Sensor_Config failed to set");
        
		///...Releasing the above structure pointers first
		app_utils_mem_free(Set_Rsp_ptr->resp.set_cfg_resp);
		app_utils_mem_free(Set_Rsp_ptr);
		app_utils_mem_free(Set_Req_ptr->req.set_cfg);
		app_utils_mem_free(Set_Req_ptr);

		return; 
	}
	else
	{
		LOG_INFO("[sensor_mgr]: qapi_Sensor_Mgr_Set_Sensor_Config succeeded to set");
	}

	//...On calling SET QAPi succesfully we will get the details in response structure
	LOG_INFO("[sensor_mgr]: sensor_set_config:- ----------- SET Response structure details ----------");

	//...It will give the details of no. of sensors that has been configured
	LOG_INFO("[sensor_mgr]: sensor_set_config: sensor_resp_len = %d",Set_Rsp_ptr->resp.set_cfg_resp->sensor_resp_len);

	///...check the sensor_lngth is greater than 0 .Just to avoid printing if any garbage values. 
	if(Set_Rsp_ptr->resp.set_cfg_resp->sensor_resp_len > 0)
	{
		//...It will give the details of sensor ID that has been configured
		LOG_INFO("[sensor_mgr]: sensor_set_config: sensor_id = %d",Set_Rsp_ptr->resp.set_cfg_resp->sensor_resp[0].sensor_id);

		//...It will give the details success/failure of cofiguration that has been configured
		LOG_INFO("[sensor_mgr]: sensor_set_config: Result = %d",Set_Rsp_ptr->resp.set_cfg_resp->sensor_resp[0].result);

		//...It will give the details success/failure of cofiguration that has been configured
		LOG_INFO("[sensor_mgr]: sensor_set_config: error_cfg = %d",Set_Rsp_ptr->resp.set_cfg_resp->sensor_resp[0].error_cfg);

		//...It will give the details success/failure of cofiguration that has been configured
		LOG_INFO("[sensor_mgr]: sensor_set_config: error_type = %d",Set_Rsp_ptr->resp.set_cfg_resp->sensor_resp[0].error_type);
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_set_config: sensor_resp_len is less than zero and failed to set the configuration");
	}

    LOG_INFO("[sensor_mgr]: *******************************************************");
    LOG_INFO("[sensor_mgr]: wait for 5 sec:");
    qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true); 

	///...Releasing the memories , inner response structure is de-allocated first	
	if((Set_Rsp_ptr != NULL)&&(Set_Rsp_ptr->resp.set_cfg_resp != NULL))
	{
		app_utils_mem_free(Set_Rsp_ptr->resp.set_cfg_resp);
	}

	///,,,Releasing memorie ,outer structure 
	if(Set_Rsp_ptr != NULL)
	{
		app_utils_mem_free(Set_Rsp_ptr);
	}

	///...Releasing the memories , inner Request structure is de-allocated first
	if((Set_Req_ptr != NULL)&&(Set_Req_ptr->req.set_cfg != NULL))
	{
		app_utils_mem_free(Set_Req_ptr->req.set_cfg);
	}

	///...Releasing the memories , inner response structure is de-allocated first	
	if(Set_Req_ptr != NULL)
	{
		app_utils_mem_free(Set_Req_ptr);
	}
	
	return;
}

/*******************************************************************************
*	function name:sensor_get_config
*	Decription:Gets sensor configuration. API can be used to get for single/multiple sensors.
*   @dependencies qapi_Sensor_Mgr_Init().
*   @param[in] cbk         void.
********************************************************************************/
void sensor_get_config(void)
{
	qapi_Status_t ret= QAPI_ERROR;
	uint32_t u32_sensors_list;
	uint32_t u32_sensor_policy_dimension;
	uint32_t u32_pre_alt_th_high;
	uint32_t u32_pre_alt_th_low;
	uint32_t u32_alt_th_high;
	uint32_t u32_alt_th_low;
	uint32_t u32_high_performance_measurement_period;
	uint32_t u32_measurement_period;

	uint8_t u8_operating_mode;
	uint8_t u8_performance_mode;
	uint8_t u8_hysteresis_duration;
	
	qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t *configs_get = NULL;
	qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t *resp_get = NULL;

	LOG_INFO("[sensor_mgr]: sensor_get_config : entry");

	configs_get=(qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t));

	if(NULL == configs_get)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :Alloc qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t memory fail");
		///...returning directly
		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_get_config :size of qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t));
	}

	memset(configs_get,0,sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t));

	//...pass the get request type to get sensor details
	configs_get->type= QAPI_SENSOR_MGR_GET_CONFIG_REQ_E;
	//... Allocate the memory fot the struture
	configs_get->req.get_cfg=(qapi_Sensor_Mgr_Sensor_Config_Get_Req_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Req_t));
	
	if(NULL == configs_get->req.get_cfg)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :Alloc qapi_Sensor_Mgr_Sensor_Config_Get_Req_t memory fail");
		///...Releasing the above structure pointers ,to free the memoery
		app_utils_mem_free(configs_get);
		
		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :Alloc qapi_Sensor_Mgr_Sensor_Config_Get_Req_t memory Success");
		LOG_INFO("[sensor_mgr]: sensor_get_config :size of qapi_Sensor_Mgr_Sensor_Config_Get_Req_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Req_t));
	}

	//...init the structure with zero
	memset(configs_get->req.get_cfg,0,sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Req_t));
	//...pass the sensor ID to fetch sensor information
	configs_get->req.get_cfg->sensor_id= QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK;//QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE;

	//..alocate the memory for response sturctue
	resp_get=(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t));
	
	if(NULL == resp_get)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :Alloc qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t memory fail");
		
		///...Releasing the above structure pointers ,to free the memoery
		app_utils_mem_free(configs_get->req.get_cfg);
		app_utils_mem_free(configs_get);
		
		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_get_config : size of qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t));
	}

	memset(resp_get,0,sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t));

	//...request the response config infor from sesor manager
	resp_get->type=QAPI_SENSOR_MGR_GET_CONFIG_RESP_E;
	
	//...Allocate the memory for response config structure
	resp_get->resp.get_cfg_resp=(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t*)app_utils_mem_alloc(sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t));
	
	if(NULL == resp_get->resp.get_cfg_resp)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :Alloc qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t memory fail");
		
		///...Releasing the above structure pointers ,to free the memoery
		app_utils_mem_free(resp_get);
		app_utils_mem_free(configs_get->req.get_cfg);
		app_utils_mem_free(configs_get);
		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t memory success");
		LOG_INFO("[sensor_mgr]: sensor_get_config :size of qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t %d",sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t));
	}

	memset(resp_get->resp.get_cfg_resp,0,sizeof(qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t));

	//...Request the no. of sensors information required to fetch
	//resp_get->resp.get_cfg_resp->sensor_config_list_len=1; //
	//...call the qapi with sensor manager handler	, request structure and response structure
	ret= qapi_Sensor_Mgr_Get_Sensor_Config(sensor_handle,configs_get,resp_get);
	
	if(ret!=QAPI_OK)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config:qapi_Sensor_Mgr_Get_Sensor_Config failed to get");
		
		///...Releasing the above structure pointers ,to free the memoery
		app_utils_mem_free(resp_get->resp.get_cfg_resp);
		app_utils_mem_free(resp_get);
		app_utils_mem_free(configs_get->req.get_cfg);
		app_utils_mem_free(configs_get);
		
		return;
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config: qapi_Sensor_Mgr_Get_Sensor_Config succeeded to get");
	}

	//... Check the no of sensors list inforamtion that got request and response is same.	
	u32_sensors_list = resp_get->resp.get_cfg_resp->sensor_config_list_len;
	LOG_INFO("[sensor_mgr]:  sensor_get_config :sensors_list = %d",u32_sensors_list);

	////....sensor list (length > 0) , just to avoid reading the garbage value
	if(u32_sensors_list >0)
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :sensors_ID = %d",resp_get->resp.get_cfg_resp->sensor_config_list[0].sensor_id);

		u32_sensor_policy_dimension = resp_get->resp.get_cfg_resp->sensor_config_list[0].sensor_policy_dim;
		LOG_INFO("[sensor_mgr]: sensor_get_config :u32_sensor_policy_dimension = %d",u32_sensor_policy_dimension);

		u32_alt_th_high = resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].alert_threshold_high;
		
		u32_alt_th_low = resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].alert_threshold_low;

		u32_pre_alt_th_high = resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].pre_alert_threshold_high;

		u32_pre_alt_th_low = resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].pre_alert_threshold_low;
		
		LOG_INFO("[sensor_mgr]: sensor_get_config :u32_alt_th_high = %d",(int)resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].alert_threshold_high);
		LOG_INFO("[sensor_mgr]: sensor_get_config :u32_alt_th_low = %d",(int)resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].alert_threshold_low);
		LOG_INFO("[sensor_mgr]: sensor_get_config :u32_pre_alt_th_high = %d",(int)resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].pre_alert_threshold_high);
		LOG_INFO("[sensor_mgr]: sensor_get_config :u32_pre_alt_th_low = %d",(int)resp_get->resp.get_cfg_resp->sensor_config_list[0].policy_cond[0].pre_alert_threshold_low);
		
		//..operating mode
		u8_operating_mode = resp_get->resp.get_cfg_resp->sensor_config_list[0].operating_mode;
		u8_performance_mode = resp_get->resp.get_cfg_resp->sensor_config_list[0].perf_mode;
		u8_hysteresis_duration = resp_get->resp.get_cfg_resp->sensor_config_list[0].hysteresis_duration;
		u32_high_performance_measurement_period = resp_get->resp.get_cfg_resp->sensor_config_list[0].high_perf_measurement_period;
		u32_measurement_period  = resp_get->resp.get_cfg_resp->sensor_config_list[0].measurement_period;

		LOG_INFO("[sensor_mgr]: sensor_get_config :u8_operating_mode = %d",u8_operating_mode);
		LOG_INFO("[sensor_mgr]: sensor_get_config :u8_performance_mode = %d",u8_performance_mode);
		LOG_INFO("[sensor_mgr]: sensor_get_config :u8_hysteresis_duration = %d",u8_hysteresis_duration);
		LOG_INFO("[sensor_mgr]: sensor_get_config :u32_high_performance_measurement_period = %d",u32_high_performance_measurement_period);
		LOG_INFO("[sensor_mgr]: sensor_get_config :u32_measurement_period = %d",u32_measurement_period);
	}
	else
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config :sensor_config_list_len is zero and failed to get details =");
	}

	LOG_INFO("[sensor_mgr]: app_thread : *******************************************************");
    LOG_INFO("[sensor_mgr]: app_thread wait for 5 sec:");
    qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true); 

	///...releasein memory for inner response structure first
	if((resp_get != NULL)&&(resp_get->resp.get_cfg_resp != NULL))
	{
		LOG_INFO("[sensor_mgr]: sensor_get_config:- ----------- resp_get->resp.get_cfg_resp releasing memory----------");
		app_utils_mem_free(resp_get->resp.get_cfg_resp);
	}

	if(resp_get != NULL)
	{
		app_utils_mem_free(resp_get);
	}
	
	///...releaing allocated memory
	if((configs_get != NULL)&&(configs_get->req.get_cfg != NULL))
	{
		app_utils_mem_free(configs_get->req.get_cfg);
	}

	if(configs_get != NULL)
	{
		app_utils_mem_free(configs_get);
	}

	return;
}