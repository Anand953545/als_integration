#include "data_mgr_thread.h"
#include "app_thread.h"

#include "data_mgr.h"
#include "data_mgr_dss.h"
#include "app_utils_misc.h"

#include "aware_log.h"
#include "aware_app.h"
#include "cJSON.h"
#include "qapi_fs.h"
#include "aware_utils.h"

qapi_DSS_Hndl_t data_mgr_dss_handle = NULL;
TX_EVENT_FLAGS_GROUP* data_mgr_dss_signal_handle;

uint8 datacall_status = DSS_EVT_INVALID_E;
static uint16_t fleet_route_array_index = 0;

/* Thread Constants */
static app_thread_id _DataMgrTaskId = APP_THREAD_ID_DATA_MGR;
static char* _DataMgrTaskName = "dataMgrTask";

static TX_MUTEX* _DataMgrMutex;
static TX_EVENT_FLAGS_GROUP* _DataMgrSMSignal;

#define DSS_ADDR_INFO_SIZE	5
#define DSS_ADDR_SIZE       16
#define GET_ADDR_INFO_MIN(a, b) ((a) > (b) ? (b) : (a))

#define AWARE_DEMO_ROUTE_CONFIG_PATH "/datatx/demo_route.conf"

char* get_scripted_location_json(void);


/*-------------------------------------------------------------------------*/
/**
  @brief Show new info
  @return Void
 */
/*--------------------------------------------------------------------------*/
void dss_show_nw_info(void)
{
	int i   = 0;
	int j 	= 0;
	unsigned int len = 0;
	uint8 buff[DSS_ADDR_SIZE] = {0}; 
	qapi_Status_t status;
	qapi_DSS_Addr_Info_t info_ptr[DSS_ADDR_INFO_SIZE];

	status = qapi_DSS_Get_IP_Addr_Count(data_mgr_dss_handle, &len);
	if (QAPI_ERROR == status)
	{
		LOG_INFO("Get IP address count error");
		return;
	}

	status = qapi_DSS_Get_IP_Addr(data_mgr_dss_handle, info_ptr, len);
	if (QAPI_ERROR == status)
	{
		LOG_INFO("Get IP address error");
		return;
	}

	j = GET_ADDR_INFO_MIN(len, DSS_ADDR_INFO_SIZE);
	
	for (i = 0; i < j; i++)
	{
		tcp_inet_ntoa(info_ptr[i].iface_addr_s, buff, DSS_ADDR_SIZE);
		LOG_INFO("static IP: %s", buff);

		memset(buff, 0, sizeof(buff));
		tcp_inet_ntoa(info_ptr[i].gtwy_addr_s, buff, DSS_ADDR_SIZE);
		LOG_INFO("Gateway IP: %s", buff);

		memset(buff, 0, sizeof(buff));
		tcp_inet_ntoa(info_ptr[i].dnsp_addr_s, buff, DSS_ADDR_SIZE);
		LOG_INFO("Primary DNS IP: %s", buff);

		memset(buff, 0, sizeof(buff));
		tcp_inet_ntoa(info_ptr[i].dnss_addr_s, buff, DSS_ADDR_SIZE);
		LOG_INFO("Second DNS IP: %s", buff);
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief Initializes the DSS net event callback for the specified operating mode.
  @param hndl 
  @param user_data 
  @param evt Event name
  @param payload_ptr Event payload sent with event callback
  @return Void
 */
/*--------------------------------------------------------------------------*/
static void dss_net_event_cb
( 
	qapi_DSS_Hndl_t 		hndl,
	void 				   *user_data,
	qapi_DSS_Net_Evt_t 		evt,
	qapi_DSS_Evt_Payload_t *payload_ptr 
)
{	switch (evt)
	{
		case QAPI_DSS_EVT_NET_NEWADDR_E:
		case QAPI_DSS_EVT_NET_IS_CONN_E:
		{
			LOG_INFO("Data Call Connected: DSS_EVT_NET_IS_CONN_E");
			/* Signal main task */
  			tx_event_flags_set(data_mgr_dss_signal_handle, DSS_SIG_EVT_CONN_E, TX_OR);
			datacall_status = DSS_EVT_NET_IS_CONN_E;
			
			break;
		}
		case QAPI_DSS_EVT_NET_DELADDR_E:
		case QAPI_DSS_EVT_NET_NO_NET_E:
		{
			LOG_INFO("Data Call Failed: DSS_EVT_NET_NO_NET_E");
			if (DSS_EVT_NET_IS_CONN_E == datacall_status)
			{
				datacall_status = DSS_EVT_NET_NO_NET_E;
                tx_event_flags_set(data_mgr_dss_signal_handle, DSS_SIG_EVT_EXIT_E, TX_OR);
			}
            else
            {
                tx_event_flags_set(data_mgr_dss_signal_handle, DSS_SIG_EVT_NO_CONN_E, TX_OR);
            }
			break;
		}
		default:
		{
			LOG_INFO("Data Call status is invalid.");
			//tx_event_flags_set(http_signal_handle, DSS_SIG_EVT_INV_E, TX_OR);
			datacall_status = DSS_EVT_INVALID_E;
			break;
		}
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief Initializes the DSS netctrl library for the specified operating mode.
  @return Success or failure
 */
/*--------------------------------------------------------------------------*/
static int dss_netctrl_init(void)
{
	int ret_val = 0;
	qapi_Status_t status = QAPI_OK;

	LOG_INFO("Initializes the DSS library");

	/* Initializes the DSS netctrl library */
	if (QAPI_OK == qapi_DSS_Init(QAPI_DSS_MODE_GENERAL))
	{
		//http_netctl_lib_status = DSS_LIB_STAT_SUCCESS_E;
		LOG_INFO("qapi_DSS_Init success");
	}
	else
	{
		/* @Note: netctrl library has been initialized */
		//http_netctl_lib_status = DSS_LIB_STAT_FAIL_E;
		LOG_INFO("DSS netctrl library has been initialized.");
	}
	
	/* Registering callback data_mgr_dss_handleR */
	do
	{
		LOG_INFO("Registering Callback data_mgr_dss_handle");
		
		/* Obtain data service handle */
		status = qapi_DSS_Get_Data_Srvc_Hndl(dss_net_event_cb, NULL, &data_mgr_dss_handle);
		LOG_INFO("data_mgr_dss_handle %d, status %d", data_mgr_dss_handle, status);
		
		if (NULL != data_mgr_dss_handle)
		{
			LOG_INFO("Registed data_mgr_dss_handler success");
			break;
		}

		/* Obtain data service handle failure, try again after 10ms */
		qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
	} while(1);

	return ret_val;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Set the Parameter for Data Call, such as APN and network tech.
  @return 0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
static int dss_set_data_param(void)
{
    qapi_DSS_Call_Param_Value_t param_info;
	
	memset(&param_info, 0, sizeof(param_info));

    if (NULL != data_mgr_dss_handle)
    {
        /* set data call param */
        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_RADIO_TECH_UNKNOWN;	//Automatic mode(or DSS_RADIO_TECH_LTE)
        LOG_INFO("Setting tech to Automatic");
        if(QAPI_DSS_SUCCESS != qapi_DSS_Set_Data_Call_Param(data_mgr_dss_handle, QAPI_DSS_CALL_INFO_TECH_PREF_E, &param_info))
		{
			LOG_ERROR("[data_mgr]: Failed to set QAPI_DSS_CALL_INFO_TECH_PREF_E parameter");
		}

		/* set IP version(IPv4 or IPv6) */
        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_IP_VERSION_4;
        LOG_INFO("Setting family to IPv%d", param_info.num_val);
        if(QAPI_DSS_SUCCESS != qapi_DSS_Set_Data_Call_Param(data_mgr_dss_handle, QAPI_DSS_CALL_INFO_IP_VERSION_E, &param_info))
		{
			LOG_ERROR("[data_mgr]: Failed to set QAPI_DSS_CALL_INFO_IP_VERSION_E parameter");
		}

		param_info.buf_val = NULL;
		param_info.num_val = 1;
		if(QAPI_DSS_SUCCESS != qapi_DSS_Set_Data_Call_Param(data_mgr_dss_handle, QAPI_DSS_CALL_INFO_UMTS_PROFILE_IDX_E, &param_info)) 
		{
			LOG_ERROR("[data_mgr]: Failed to set QAPI_DSS_CALL_INFO_UMTS_PROFILE_IDX_E parameter");
		}

		/* set apn */
		param_info.buf_val = app_ctx.dhc_cfg.dss_apn;
		param_info.num_val = strlen(app_ctx.dhc_cfg.dss_apn);
		LOG_INFO("Setting APN - %s", app_ctx.dhc_cfg.dss_apn);
        if(QAPI_DSS_SUCCESS != qapi_DSS_Set_Data_Call_Param(data_mgr_dss_handle, QAPI_DSS_CALL_INFO_APN_NAME_E, &param_info))
		{
			LOG_ERROR("[data_mgr]: Failed to set QAPI_DSS_CALL_INFO_APN_NAME_E parameter");
		}
    }
    else
    {
        LOG_INFO("Dss handler is NULL!!!\n");
		return -1;
    }
	
    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Start the DSS netctrl library, and startup data call.
  @return 0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
int dss_netctrl_start(void)
{
	int rc = 0;
	qapi_Status_t status = QAPI_OK;

	rc = dss_netctrl_init();
	if (0 == rc)
	{
		/* Get valid DSS handler and set the data call parameter */
		dss_set_data_param();
	}
	else
	{
		LOG_ERROR("quectel_dss_init fail.");
		return QAPI_ERROR;
	}

	LOG_INFO("qapi_DSS_Start_Data_Call start!!!.");
	status = qapi_DSS_Start_Data_Call(data_mgr_dss_handle);
	if (QAPI_OK == status)
	{
		LOG_INFO("Start Data service success.");
		return QAPI_OK;
	}
	else
	{
        LOG_ERROR("Start Data service failed.");
		return QAPI_ERROR;
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief Cleans up the DSS netctrl library and close data service.
  @return 0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
int dss_netctrl_stop(void)
{
	qapi_Status_t stat = QAPI_OK;
	
	if (data_mgr_dss_handle)
	{
		stat = qapi_DSS_Stop_Data_Call(data_mgr_dss_handle);
		if (QAPI_OK == stat)
		{
			LOG_INFO("Stop data call success");
		}
        stat = qapi_DSS_Rel_Data_Srvc_Hndl(data_mgr_dss_handle);
		if (QAPI_OK != stat)
		{
			LOG_INFO("Release data service handle failed:%d", stat);
		}
		data_mgr_dss_handle = NULL;
		qapi_DSS_Release(QAPI_DSS_MODE_GENERAL);
	}

	return stat;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Stop data call
  @return 0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
int data_mgr_stop_datacall(void)
{
	int ret = dss_netctrl_stop();

	return ret;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Start data call
  @return 0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
int data_mgr_start_datacall(void)
{
	uint32 dss_event = 0;
	int ret = dss_netctrl_start();
	if (ret != 0)
	{
		return ret;
	}

	tx_event_flags_get(data_mgr_dss_signal_handle, DSS_SIG_EVT_CONN_E|DSS_SIG_EVT_NO_CONN_E, TX_OR_CLEAR, &dss_event, TX_WAIT_FOREVER);
	if(dss_event & DSS_SIG_EVT_CONN_E)
	{
        LOG_INFO("data call success");
		ret = QAPI_OK;
    }
	else if(dss_event & DSS_SIG_EVT_NO_CONN_E)
	{
        LOG_ERROR("data call failed");
		ret = QAPI_ERROR;
    }

	return ret;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Get task name
  @return Task name
 */
/*--------------------------------------------------------------------------*/
char* data_mgr_get_task_name()
{
    return _DataMgrTaskName;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Get task Id
  @return Task Id
 */
/*--------------------------------------------------------------------------*/
int data_mgr_get_task_id()
{
    return _DataMgrTaskId;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Report scripted location
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_report_scripted_location(void)
{
    char* jsonMsg = get_scripted_location_json();
    size_t msgSize = strlen(jsonMsg);

    app_thread_send_msg(
            _DataMgrTaskId,
            DATA_MGR_MSG_ID_PERIODIC_CHECKIN,
            jsonMsg, msgSize);

	app_utils_mem_free(jsonMsg);
}

/*-------------------------------------------------------------------------*/
/**
  @brief Publish activation message
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_publish_activation_message(void)
{
    app_thread_send_msg(
            _DataMgrTaskId,
            DATA_MGR_MSG_ID_PERIODIC_CHECKIN,
            NULL, 0);
}

/*-------------------------------------------------------------------------*/
/**
  @brief Send periodic chekin message to thread
  @param payload Message Buffer pointer
  @param payload_len Length of message
  @return 0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
int data_mgr_thread_send_msg_periodic_checkin(uint8* payload, size_t payload_len)
{
	int status = 0;

    status = app_thread_send_msg(
            _DataMgrTaskId,
            DATA_MGR_MSG_ID_PERIODIC_CHECKIN,
            payload, payload_len);

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Process message
  @param msgId Message Id
  @param msgPtr Message Buffer pointer
  @param msgSize Length of message
  @return 0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
static void _data_mgr_process_msg(int msgId, void *msgPtr, size_t msgSize)
{
	int result = QAPI_ERROR;
	int retry_counter = 0;

    LOG_DEBUG("Data Mgr received message %d", msgId);

    // Make sure to free any data pointer within the message
    // As part of handling the message..
    switch (msgId)
    {
        case DATA_MGR_MSG_ID_PERIODIC_CHECKIN:
			if(!app_ctx.dhg_cfg.demo_mode) {
				//set priority to WWAN
				if(QAPI_OK != data_mgr_set_wwan_high_priority()) 
				{
					LOG_ERROR("[data_mgr]: Failed to switch app priority to WWAN");
				}
			}

			//establish data call with 3 retry attempts
			do {
				result = data_mgr_start_datacall();
				retry_counter++;
			} while((result != QAPI_OK) && (retry_counter < 3));

			//data call success?
			if(QAPI_OK == result && QAPI_OK == data_mgr_connect_coap_server()) {
				if(app_ctx.dhg_cfg.demo_mode)
					data_mgr_publish_event(app_ctx.dhu_cfg.device_id, "checkin", PAYLOAD_TYPE_CBOR_LIKE, msgPtr, msgSize);
				else
					data_mgr_publish_event(app_ctx.dhu_cfg.device_id, "checkin", PAYLOAD_TYPE_PB, msgPtr, msgSize);
				
				//wait for 2 secs, before disconnecting..
				qapi_Timer_Sleep(2, QAPI_TIMER_UNIT_SEC, true);
				data_mgr_disconnect_coap_server();
			}

			//release data call
			data_mgr_stop_datacall();
            break;

        default :
            LOG_ERROR("Invalid msgId %d", msgId);
    }

    LOG_DEBUG("Data Mgr message processing complete");
}

/*-------------------------------------------------------------------------*/
/**
  @brief This is the entry point for Data Mgr task
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_task_entry(ULONG args)
{
    AppMsg *payload = NULL;

#ifdef QAPI_TXM_MODULE
	/* Create event signal handle and clear signals */
	txm_module_object_allocate((void**)&data_mgr_dss_signal_handle, sizeof(TX_EVENT_FLAGS_GROUP));
#endif

	tx_event_flags_create(data_mgr_dss_signal_handle, "dss_signal_event");
	tx_event_flags_set(data_mgr_dss_signal_handle, 0x0, TX_AND);

    LOG_INFO("Data Mgr Thread started. TCB %x TID %d", tx_thread_identify(), args);

    // Initializations
    app_utils_mutex_init(&_DataMgrMutex, "_DataMgrMutex");

    while(0 == app_thread_rcv_msg(args, (void **)&payload))
    {
        _data_mgr_process_msg(payload->msgId, payload->msg, payload->msgSize);
        if (payload->msg != NULL)
        {
            app_utils_byte_release(payload->msg);
        }
        app_utils_byte_release(payload);
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief Get scripted location info in json format
  @return Location info
 */
/*--------------------------------------------------------------------------*/
char* get_scripted_location_json(void)
{
   int *fd, ret, read_bytes = 0;
   char buffer[256] = {0};
   char* msg = NULL;
   struct qapi_FS_Stat_Type_s file_stat = {0};

   size_t msg_len = sizeof(buffer);
   uint64_t time_ms = get_current_time_in_milliseconds();

   ret = qapi_FS_Open(AWARE_DEMO_ROUTE_CONFIG_PATH, QAPI_FS_O_RDONLY_E, &fd);
   if (ret != QAPI_OK)
   {
      LOG_ERROR("[data_mgr]: get_location_json_msg:  qapi_FS_Open failed with %d\n", ret);
      return NULL;
   }
      
   if (qapi_FS_Stat_With_Handle(fd, &file_stat) < 0) 
   {
      LOG_ERROR("[data_mgr]: get_location_json_msg: failed to get EFS file size\n");
      return NULL;
   }
   size_t size = file_stat.st_size;
   
   char *json_string = (char *)app_utils_mem_alloc(size + 1);
   if (NULL == json_string)
   {
      LOG_ERROR("[data_mgr]: get_location_json_msg: malloc failed!!!\n");
      return NULL;
   }

   ret = qapi_FS_Read(fd, (uint8 *)json_string, size, &read_bytes);
   if(ret != QAPI_OK)
   {
      LOG_ERROR("[data_mgr]: write_sensor_config_to_efs: failed to get EFS file size\n");
      return NULL;
   }
   qapi_FS_Close(fd);

   //null terminate the string
   json_string[size] = '\0';

   cJSON *arrayElement = NULL;
   cJSON *latObj = NULL;
   cJSON *longObj = NULL;
   cJSON *altObj = NULL;
   cJSON *alsObj = NULL;
   cJSON *tempObj = NULL;
   cJSON *humidObj = NULL;
   cJSON *presObj = NULL;

   cJSON *root = cJSON_Parse(json_string);
   
   //free memory
   app_utils_mem_free(json_string);
   
   cJSON *fleet_route = NULL;
   cJSON *iterator = NULL;

	fleet_route = cJSON_GetObjectItem(root, "fleet_route_0");
	if (!fleet_route) {
		cJSON_Delete(root);
      return NULL;
	}

   int array_count = cJSON_GetArraySize(fleet_route);
   LOG_INFO("[aware_app]: short drive data count: %d", array_count);

   if(array_count > 0)
   {
      //reset the array index for next drive from start position
      if(fleet_route_array_index >= array_count)
         fleet_route_array_index = 0;

      LOG_INFO("[aware_app]: array index: %d", fleet_route_array_index);
      arrayElement = cJSON_GetArrayItem(fleet_route, fleet_route_array_index);
      if(arrayElement)
      {
		latObj = cJSON_GetObjectItem(arrayElement, "lat");
		longObj = cJSON_GetObjectItem(arrayElement, "lon");
		altObj = cJSON_GetObjectItem(arrayElement, "alt");
		alsObj = cJSON_GetObjectItem(arrayElement, "als");
		tempObj = cJSON_GetObjectItem(arrayElement, "temp");
		presObj = cJSON_GetObjectItem(arrayElement, "pres");
		humidObj = cJSON_GetObjectItem(arrayElement, "humid");
		fleet_route_array_index++;

        snprintf(buffer, msg_len, "{\"als\":%d,\"temp\":%d,\"humid\":%d,\"pres\":%d,\"lat\":%d.%05d,\"lon\":%d.%05d,\"alt\":%d.%05d,\"time\":%llu}", alsObj->valueint, tempObj->valueint, humidObj->valueint, presObj->valueint, (int)latObj->valuedouble, (abs((int)(latObj->valuedouble * 100000))) % 100000, (int)longObj->valuedouble, (abs((int)(longObj->valuedouble * 100000))) % 100000, (int)altObj->valuedouble, (abs((int)(altObj->valuedouble * 100000))) % 100000, time_ms);
        LOG_INFO("[aware_app]: telemetry payload: %s", buffer);
      }
   }

   cJSON_Delete(root);

   msg = app_utils_mem_alloc(msg_len);
   if (NULL == msg)
   {
      LOG_ERROR("[data_mgr]: get_location_json_msg malloc failed!!!\n");
      return NULL;
   }

   strlcpy(msg, buffer, msg_len);

    return msg;
}