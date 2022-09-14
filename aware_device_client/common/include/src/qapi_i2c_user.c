/** 
    @file  qapi_i2c_user.c
 */
/*=============================================================================
            Copyright (c) 2020 Qualcomm Technologies, Incorporated.
                              All rights reserved.
              Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/
/*==================================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: 

when         who     what, where, why
----------   ---     -----------------------------------------------------------------
03/26/2020   ss    Initial version
==================================================================================*/
#include "qapi_i2c_master.h"

#if defined  QAPI_TXM_MODULE 

int i2c_module_cb_manager(UINT cb_id, void *app_cb, 
									 UINT cb_param1, UINT cb_param2, 
									 UINT cb_param3, UINT cb_param4,
									 UINT cb_param5, UINT cb_param6,
									 UINT cb_param7, UINT cb_param8)
{
	qapi_I2CM_Transfer_CB_t CB_Function;
	
	if(cb_id == TXM_QAPI_I2C_NOTIFY_CALLBACK)
	{
		CB_Function = (qapi_I2CM_Transfer_CB_t) app_cb;
		(CB_Function)(cb_param1, (void *) cb_param2);
	}
	return 0;
}

#endif

