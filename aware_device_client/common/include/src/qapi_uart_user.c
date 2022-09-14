/** 
    @file  qapi_uart_user.c
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
#include "qapi_uart.h"

#if defined  QAPI_TXM_MODULE 

int qapi_uart_tx_user_cb_dispatcher (UINT cb_id, void *pfnUser_cb,
									UINT cb_param1, UINT cb_param2)
{
    qapi_UART_Callback_Fn_t pFnAppCb;
	pFnAppCb = (qapi_UART_Callback_Fn_t) pfnUser_cb;
	(pFnAppCb)(cb_param1, (void *) cb_param2);

    return 0;
}

int qapi_uart_rx_user_cb_dispatcher (UINT cb_id,void *pfnUser_cb, int cb_param1, int cb_param2)
 {
    qapi_UART_Callback_Fn_t pFnAppCb;
	pFnAppCb = (qapi_UART_Callback_Fn_t) pfnUser_cb;
	(pFnAppCb)(cb_param1, (void *) cb_param2);
 
	 return 0;
 }

#endif

