#include "aware_device_info.h"
#include "qapi_device_info.h"
#include "app_utils_misc.h"
#include "aware_log.h"


/*-------------------------------------------------------------------------*/
/**
  @brief    Gets the aware device information
  @param    info   Type of info required about
  @return   Device information
 */
/*--------------------------------------------------------------------------*/
char* get_aware_device_info(aware_device_info_t info)
{
	qapi_Device_Info_Hndl_t device_info_hndl;
	qapi_Device_Info_t device_resp;
    char* device_info = NULL;

    qapi_Status_t result = qapi_Device_Info_Init_v2(&device_info_hndl);
    if (QAPI_OK != result)
    {
        LOG_ERROR("qapi_Device_Info_Init_v2 failed");
        return NULL;
    }

    memset(&device_resp, 0, sizeof(qapi_Device_Info_t));
    
    switch(info)
    {
        case AWARE_DEVICE_INFO_IMEI:
            result = qapi_Device_Info_Get_v2(device_info_hndl, QAPI_DEVICE_INFO_IMEI_E, &device_resp);
            if((result == QAPI_OK) && (device_resp.u.valuebuf.len != 0))
            {
                if(device_resp.info_type == QAPI_DEVICE_INFO_TYPE_BUFFER_E) {
                    uint32_t msgSize = device_resp.u.valuebuf.len;
                    if (TX_SUCCESS == (result = app_utils_byte_allocate((void **)&device_info, msgSize))) {
                        strlcpy(device_info, device_resp.u.valuebuf.buf, msgSize);
                    }
                }
            }
        break;
        default:
        break;
    }

    if(device_info_hndl)
        qapi_Device_Info_Release_v2(device_info_hndl);

    return device_info;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the sim card status description
  @return   Sim status
 */
/*--------------------------------------------------------------------------*/
int get_aware_device_sim_status(void)
{
    sim_state sim_status = QAPI_DEVICE_INFO_SIM_STATE_UNKNOWN;

	qapi_Device_Info_Hndl_t hndl;
	qapi_Device_Info_t info;
    
	qapi_Status_t status = qapi_Device_Info_Init_v2(&hndl);
	if (QAPI_OK == status) {
		status = qapi_Device_Info_Get_v2(hndl, QAPI_DEVICE_INFO_SIM_STATE_E, &info);
		if (QAPI_OK == status)
		{
            sim_status = info.u.valueint;
		}
		qapi_Device_Info_Release_v2(hndl);
	}

    return sim_status;
}