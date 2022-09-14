#include "app_thread.h"
#include "app_utils_misc.h"
#include "fota_thread.h"

#include "aware_log.h"
#include "aware_utils.h"
#include "aware_app.h"
#include "fota_dam_download.h"
#include "stringl.h"
#include "data_mgr.h"
#undef true
#undef false
#include "data_mgr_dss.h"

/* Thread Constants */
static app_thread_id _FotaTaskId = APP_THREAD_ID_FOTA;
static char* _FotaTaskName = "fota_thread";

//#define FOTA_URL "https://aware-dam.s3.amazonaws.com/aware_dam_app_v1.bin"
#define FOTA_URL "http://aware-dam.s3.amazonaws.com/aware_dam_app_v1.bin"



/*-------------------------------------------------------------------------*/
/**
  @brief Get task Id
  @return Task id
 */
/*--------------------------------------------------------------------------*/
int fota_thread_get_task_id()
{
    return _FotaTaskId;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Get task name
  @return Task name
 */
/*--------------------------------------------------------------------------*/
char* fota_thread_get_task_name()
{
    return _FotaTaskName;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Convenience functions to send messages to this fota thread
  @return Void
 */
/*--------------------------------------------------------------------------*/
void fota_thread_send_msg_init()
{
    app_thread_send_msg(
            _FotaTaskId,
            FOTA_THREAD_MSG_ID_INIT, NULL, 0);
}


/*-------------------------------------------------------------------------*/
/**
  @brief Send messages dam download
  @return Void
 */
/*--------------------------------------------------------------------------*/
static void fota_thread_send_msg_dam_download()
{
    app_thread_send_msg(
            _FotaTaskId,
            FOTA_THREAD_MSG_ID_DAM_DOWNLOAD, NULL, 0);
}


/*-------------------------------------------------------------------------*/
/**
  @brief Fota thread Message init handler
  @param dataPtr pointer to msg
  @param dataSize msg length
  @return Void
 */
/*--------------------------------------------------------------------------*/
static void _fota_thread_handle_msg_init(void* dataPtr, size_t dataSize)
{
    LOG_DEBUG("Fota message Init");
}


/*-------------------------------------------------------------------------*/
/**
  @brief Fota thread dam download handler
  @param dataPtr pointer to msg
  @param dataSize msg length
  @return Void
 */
/*--------------------------------------------------------------------------*/
static int _fota_thread_handle_dam_download(void* dataPtr, size_t dataSize)
{
	int result = 0;
	uint16 port;
	uint32 len = 0;
	uint8 file_name[HTTP_MAX_PATH_LENGTH+1] = {0};
	char host[HTTP_DOMAIN_NAME_LENGTH+1] = {0};
    uint8 buffer[300] = {0};

    //strlcpy((char*)buffer, "http://104.211.226.181:8081/aware_dam_app_v1.bin", 49);
    strlcpy((char*)buffer, "http://34.207.127.61:8080/aware_dam_app_v1.bin", 47);
    len = strlen((char*)buffer);

    decode_url((uint8*)buffer, &len, host, &port, file_name);
    LOG_INFO("[Fota Thread]: host:%s, port:%d, source:%s", host, port, file_name);

	LOG_INFO("[Fota Thread]: start downloading from host : %s, port: %d, filename: %s", host, port, file_name);
	result = start_fota_dam_download(host, port, (char*)file_name);
    if(0 != result) {
        LOG_ERROR("[Fota Thread]: Fota DAM download failed %d", result);
    }

    return result;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Fota thread message processing
  @param msgId Message id
  @param msgPtr pointer to msg
  @param msgSize msg length
  @return Void
 */
/*--------------------------------------------------------------------------*/
static void _fota_thread_process_msg(int msgId, void *msgPtr, size_t msgSize)
{
    LOG_DEBUG("[Fota Thread]: Fota Thread received message %d", msgId);

    // Make sure to free any data pointer within the message
    // As part of handling the message..
    switch (msgId)
    {
        case FOTA_THREAD_MSG_ID_INIT:
            _fota_thread_handle_msg_init(msgPtr, msgSize);
            break;

        case FOTA_THREAD_MSG_ID_DAM_DOWNLOAD:
            if(QAPI_OK == data_mgr_set_wwan_high_priority()) {
				if(QAPI_OK == data_mgr_start_datacall()) {
					dss_show_nw_info();
                    _fota_thread_handle_dam_download(msgPtr, msgSize);
					data_mgr_stop_datacall();
				}
			}
            break;

        default :
            LOG_ERROR("[Fota Thread]: Invalid msgId %d", msgId);
    }

    LOG_DEBUG("[Fota Thread]: Fota Thread message processing complete");
}


/*-------------------------------------------------------------------------*/
/**
  @brief This is the entry point for Fota task
  @return Void
 */
/*--------------------------------------------------------------------------*/
void fota_thread_task_entry(ULONG args)
{
    AppMsg *payload = NULL;

    LOG_INFO("[Fota Thread]: Fota Thread started. TCB %x TID %d", tx_thread_identify(), args);

    while(0 == app_thread_rcv_msg(args, (void **)&payload))
    {
        _fota_thread_process_msg(payload->msgId, payload->msg, payload->msgSize);
        if (payload->msg != NULL)
        {
            app_utils_byte_release(payload->msg);
        }
        app_utils_byte_release(payload);
    }
}