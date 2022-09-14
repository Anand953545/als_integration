/** 
    @file  qapi_spi_user.c
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
#include "qapi_spi_master.h"

#if defined  QAPI_TXM_MODULE 

int qapi_spi_cb_uspace_dispatcher(UINT cb_id, void *app_cb,
									UINT cb_param1, UINT cb_param2,
									UINT cb_param3, UINT cb_param4,
									UINT cb_param5, UINT cb_param6,
									UINT cb_param7, UINT cb_param8)
{
        qapi_SPIM_Callback_Fn_t (*pfn_app_cb1) (uint32_t, void *);

        if(cb_id == TXM_QAPI_SPI_CB_NOTIFY) // custom cb-type1
        {
                pfn_app_cb1 = (qapi_SPIM_Callback_Fn_t (*)(uint32_t, void *))app_cb;
                (pfn_app_cb1)((uint32_t)cb_param1, (void *)cb_param2);

        }
		return 0;
}

#endif
