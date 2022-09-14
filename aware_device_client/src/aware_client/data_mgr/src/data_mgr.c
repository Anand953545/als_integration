#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <math.h>

#include "qapi_timer.h"
#include "qapi_device_info.h"
#include "qapi_fs.h"

#include "data_mgr.h"
#include "aware_log.h"
#include "data_mgr_dss.h"
#include "data_mgr_coap.h"
#include "data_mgr_queue.h"
#include "data_mgr_device_info.h"
#include "app_utils_misc.h"

int retry_counter = 0;

static char coap_error_strings[9][46] = {
   "COAP_INVALID_PARAM",
   "COAP_NOMEM",
   "COAP_HANDLE_NOT_FOUND",
   "COAP_CONNECTION_CREATION_FAILED",
   "COAP_CONNECTION_CREATION_FAILED_TCP_ERROR",
   "COAP_CONNECTION_CREATION_FAILED_SSL_ERROR",
   "COAP_DTLS_RESUMPTION_FAILED_IN_COAP_SEND",
   "COAP_TRANSACTION_CALLBACK_NOT_SET"
};

/*DATA_MGR state*/
static bool data_mgr_running = false;

/*DATA_MGR Datacall state */
static bool dm_data_call_up = false;
static bool dm_coap_connection_up = false;

/*DATA_MGR CoAP session handle */
qapi_Coap_Session_Hdl_t coap_session_handle = NULL;

void coap_get_msg_txn_cb(qapi_Coap_Session_Hdl_t hdl,
qapi_Coap_Transaction_t * transaction,
qapi_Coap_Packet_t * message);

/*-------------------------------------------------------------------------*/
/**
  @brief Publish all pendinng messages
  @return Void
 */
/*--------------------------------------------------------------------------*/
void publish_pending_messages()
{
    if (0 == data_mgr_queue_process())
    {
      //wait for c2d messages
      tx_thread_sleep(MILLIS_TO_TICKS(30000));
    }
}

/*******************************************************************************
 * DATA_MGR API implementation
 * *****************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief Connects the coap server endpoint
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int data_mgr_connect_coap_server(void)
{
  int status = QAPI_OK;

  LOG_INFO("[data_mgr]: creating coap session");
  status = coap_create_session(&coap_session_handle);
	if (QAPI_OK != status)
	{
		LOG_ERROR("[data_mgr]: create coap session failed");
		return status;
	}

  LOG_INFO("[data_mgr]: creating coap connection");
  status = coap_create_connection(&coap_session_handle);
	if (QAPI_OK != status)
	{
		LOG_ERROR("[data_mgr]: create coap connection failed");
		return status;
	}

  dm_coap_connection_up = true;
  LOG_INFO("[data_mgr]: successfully connected to coap server");

  return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Disconnects the coap connection
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int data_mgr_disconnect_coap_server(void)
{
  int status = QAPI_OK;

  if(coap_session_handle)
  {
    LOG_INFO("[data_mgr]: closing coap session and connection\n");
    status = coap_cleanup(&coap_session_handle);
    if(status != QAPI_OK)
    {
      LOG_INFO("[data_mgr]: coap_close_connection failed\n");
    }
    return status;
  }

  return status;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Publish event to coap server
  @param device_id  Target device
  @param event_type Message type
  @param pub_msg Message payload
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int data_mgr_publish_event(const char *device_id, const char* event_type, payload_type_t payload_type, const void* pub_msg, size_t pub_msg_len)
{
  return coap_send_post_message(coap_session_handle, device_id, event_type, payload_type, pub_msg, pub_msg_len);
}


/*-------------------------------------------------------------------------*/
/**
  @brief Download fota file
  @param fota_file_path  Path from where file need to be downloaded
  @param device_id Device Id
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int data_mgr_coap_fota_file_download(const char* fota_file_path, const char* device_id)
{
  int result = 0, fd;
  int pkt_number = 0, payload_len = 0, bytes_written = 0;
  int received_sigs = 0;
  qapi_FS_Offset_t offset = 0, actual_offset;
  char payload[1024]; 			/* Pointer to store payload */
  boolean last_packet = false;
  
  if(device_id == NULL) {
    LOG_ERROR("[data_mgr]: data_mgr_publish_event : deviceId or message is NULL\n");
    return EFAILURE;
  }
  
  /* Create new file where you want to store payload in downlink */
  result = qapi_FS_Open(fota_file_path, QAPI_FS_O_RDWR_E | QAPI_FS_O_CREAT_E, &fd);

  if(result < 0)
  {
    LOG_INFO("coap_dam_sample_app_service_routine : Error opening %s file\n", fota_file_path);
    return EFAILURE;
  }

  result = qapi_FS_Truncate(fota_file_path, 0);

  if(result < 0)
  {
    LOG_INFO("coap_dam_sample_app_service_routine : qapi_FS_Truncate returned error ret =  %d" , result);
    return EFAILURE;
  }

  result = qapi_FS_Seek(fd, 0, 0, &offset);

  if(result < 0)
  {
    LOG_INFO("coap_dam_sample_app_service_routine : qapi_FS_Seek returned error ret =  %d" , result);
    return EFAILURE;
  }
  
  memset(payload, 0, 1024);

  do
  {
    result = coap_send_get_block_message(coap_session_handle, device_id, coap_get_msg_txn_cb);
    if(QAPI_OK != result)
    {
      LOG_INFO("CoAP Sample APP : coap_dam_sample_app_service_routine: qapi_Coap_Send_Message failure in downlink");
      break;
    }
    else
    {
      LOG_INFO("CoAP Sample APP : coap_dam_sample_app_service_routine: GET message sent Successfully");
    }

    /* Write received payload in file */
    payload_len = strlen(payload);
    if (payload_len != 0)
    {
      result = qapi_FS_Write(fd, payload, payload_len, &bytes_written);
      if(result < 0)
      {
        LOG_INFO("coap_dam_sample_app_service_routine : Error writing in file\n");
        break;
      }

      result = qapi_FS_Seek(fd, 0, QAPI_FS_SEEK_CUR_E, &actual_offset);

      if(result < 0)
      {
        LOG_INFO("coap_dam_sample_app_service_routine : Error updating file offset\n");
        break;
      }
    }

    pkt_number++;

  } while(!last_packet);

  return 0;
}

void coap_get_msg_txn_cb(qapi_Coap_Session_Hdl_t hdl,
qapi_Coap_Transaction_t * transaction,
qapi_Coap_Packet_t * message)
{

}

