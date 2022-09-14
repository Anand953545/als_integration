/*==============================================================================

qapi_custom_cb_TSENS_dispatcher

GENERAL DESCRIPTION
This file contains definition of User callback dispatcher.


Copyright (c) 2020  by Qualcomm Technologies, Inc.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.tx/6.0/hwengines/tsens/qapi/src/qapi_tsens_user.c#1 $
$DateTime: 2020/03/23 03:20:41 $ $Author: pwbldsvc $
2009/01/13 04:17:02 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ------------------------------------------------------------
03/04/20  cs      Initial version

==============================================================================*/

#include "qapi_tsens.h"


#ifdef QAPI_TXM_MODULE

/* CALLBACK DISPATCHER */
int qapi_custom_cb_TSENS_dispatcher(UINT cb_id, void *app_cb, 
									 UINT cb_param1, UINT cb_param2, 
									 UINT cb_param3, UINT cb_param4,
									 UINT cb_param5, UINT cb_param6,
									 UINT cb_param7, UINT cb_param8)
{
	QAPI_Tsens_Threshold_Cb_Type  pfn_app_cb1;
	
	if(cb_id == TXM_QAPI_TSENS_SET_TRESHOLD_CB)
	{
		pfn_app_cb1 = (QAPI_Tsens_Threshold_Cb_Type)app_cb;
		(pfn_app_cb1)((void *)cb_param1, (const qapi_TSENS_CallbackPayloadType_t *)cb_param2);
		
	}	
	return 0;
}

#endif
