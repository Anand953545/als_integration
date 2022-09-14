#include <stdbool.h>
#include <stdint.h>

#include "data_mgr_device_info.h"
#include "qapi_device_info.h"
#include "aware_log.h"
#include "data_mgr_dss.h"
#include "app_utils_misc.h"

/** Global variable declarations */
static qapi_Device_Info_t device_resp;
static qapi_Device_Info_Hndl_t device_info_hndl;


/*-------------------------------------------------------------------------*/
/**
  @func data_mgr_init_device_info
  @brief API will init the client handlers
  @return void 
 */
/*--------------------------------------------------------------------------*/
void data_mgr_init_device_info()
{
    int result;
    LOG_INFO("[data_mgr]: data_mgr_init_device_info : Entry\n");

    result = qapi_Device_Info_Init_v2(&device_info_hndl);
    if(result != QAPI_OK)
    {
        LOG_INFO("[data_mgr]: data_mgr_init_device_info : qapi_Device_Info_Init_v2 failed. \n");
    }

#ifdef QAPI_TXM_MODULE
    result = qapi_Device_Info_Pass_Pool_Ptr(device_info_hndl, app_utils_get_byte_pool());
    if(result != QAPI_OK)
    {
        LOG_INFO("[data_mgr]: data_mgr_init_device_info : qapi_Device_Info_Pass_Pool_Ptr failed.. \n");
    }
#endif

}


/*-------------------------------------------------------------------------*/
/**
  @brief API will get device serial number
  @return Device serial number
 */
/*--------------------------------------------------------------------------*/
uint8_t* data_mgr_get_device_serial_number(void)
{
  qapi_Status_t ret = QAPI_OK;
  
  if (NULL == device_info_hndl)
  {
      qapi_Status_t res = qapi_Device_Info_Init_v2(&device_info_hndl);
      if (QAPI_OK != res)
      {
        LOG_ERROR("qapi_Device_Info_Init_v2 failed");
        return NULL;
      }
  }

  memset(&device_resp, 0, sizeof(qapi_Device_Info_t));
  
  ret = qapi_Device_Info_Get_v2(device_info_hndl, QAPI_DEVICE_INFO_IMEI_E, &device_resp);
  if((ret == QAPI_OK) && (device_resp.u.valuebuf.len != 0))
  {
    LOG_INFO("[data_mgr]: app_param_get_device_serial_number : qapi_Device_Info_Get_v2 successfull");
    return (uint8_t *)device_resp.u.valuebuf.buf;
  }
  else
  {
    LOG_INFO("[data_mgr]: app_param_get_device_serial_number : qapi_Device_Info_Get_v2 failed");
    return NULL;
  }
}


/*-------------------------------------------------------------------------*/
/**
  @brief API will set wwan to high priority
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int data_mgr_set_wwan_high_priority()
{
  uint64_t timeout = 1;
  int result = QAPI_ERROR;
  uint8_t retry_counter = 0;
  int priorityValue[1] = {0};
  //0 - GNSS, 1 - WWAN
  priorityValue[0] = 1;

  if (NULL == device_info_hndl)
  {
    qapi_Status_t res = qapi_Device_Info_Init_v2(&device_info_hndl);
    if (QAPI_OK != res)
    {
      LOG_ERROR("qapi_Device_Info_Init_v2 failed");
      return res;
    }
  }

  do {
    qapi_Timer_Sleep(timeout, QAPI_TIMER_UNIT_SEC, true);
    timeout = timeout * 2;
    result = qapi_Device_Info_Set(device_info_hndl, QAPI_DEVICE_INFO_SET_APP_PRIORITY_E, 1, priorityValue);
    if(QAPI_OK == result) {
      LOG_INFO("[data_mgr]: Set app priority to WWAN");
      break;
    }
    retry_counter++;
  } while((result != QAPI_OK) && (retry_counter < 3));

  return result;
}


/*-------------------------------------------------------------------------*/
/**
  @brief API will release the client handlers
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_release_device_info()
{
  qapi_Device_Info_Release_v2(device_info_hndl);
}


/*-------------------------------------------------------------------------*/
/**
  @brief API will reset the device information
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int data_mgr_reset_device_info()
{
  qapi_Status_t res = QAPI_OK;

  if (NULL == device_info_hndl)
  {
    res = qapi_Device_Info_Init_v2(&device_info_hndl);
    if (QAPI_OK != res)
    {
      LOG_ERROR("qapi_Device_Info_Init_v2 failed");
      return res;
    }
  }

  res = qapi_Device_Info_Reset_v2(device_info_hndl);
  if (QAPI_OK != res)
  {
    LOG_ERROR("qapi_Device_Info_Reset_v2 failed");
  }

  return res;
}


/*-------------------------------------------------------------------------*/
/**
  @brief API will get the service status of the primary slot
  @return The service status of the primary slot
 */
/*--------------------------------------------------------------------------*/
int data_mgr_param_get_service_status()
{
  qapi_Status_t ret = QAPI_OK;
  
  if (NULL == device_info_hndl)
  {
    ret = qapi_Device_Info_Init_v2(&device_info_hndl);
    if (QAPI_OK != ret)
    {
      LOG_ERROR("qapi_Device_Info_Init_v2 failed");
      return ret;
    }
  }

  memset(&device_resp, 0, sizeof(qapi_Device_Info_t));
 
  ret = qapi_Device_Info_Get_v2(device_info_hndl, QAPI_DEVICE_INFO_SERVICE_STATE_E, &device_resp);
  
  if(ret == QAPI_OK) 
  {
    return device_resp.u.valueint;
  }
  else
  {
    return QAPI_ERROR;
  }
}