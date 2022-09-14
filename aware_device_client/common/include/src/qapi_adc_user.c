
/*======================================================================
                        qapi_adc_user.c

GENERAL DESCRIPTION
  Contains User level Cb translation 

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2020  by Qualcomm Technologies, Inc.  All Rights Reserved.
 ======================================================================*/
/*======================================================================
 *
 *                       EDIT HISTORY FOR FILE
 *
 *   This section contains comments describing changes made to the
 *   module. Notice that changes are listed in reverse chronological
 *   order.
 *
 *
 *
 *
 * when         who     what, where, why
 * ----------   ---     ------------------------------------------------
 * 03/09/2020   KS      Create.
 ======================================================================*/
// This should be on top. Do not move

#ifdef QAPI_TXM_MODULE
#include "qapi_adc.h"


int qapi_custom_cb_ADC_dispatcher(UINT cb_id, void *app_cb, 
									 UINT cb_param1, UINT cb_param2, 
									 UINT cb_param3, UINT cb_param4,
									 UINT cb_param5, UINT cb_param6,
									 UINT cb_param7, UINT cb_param8)
{
	qapi_AdcTM_Threshold_Cb_Type  pfn_app_cb1;
	
	if(cb_id == TXM_QAPI_ADC_AMP_THRESHOLD_CB)
	{
		pfn_app_cb1 = (qapi_AdcTM_Threshold_Cb_Type)app_cb;
		(pfn_app_cb1)((void *) cb_param1, (qapi_ADC_Threshold_Result_t *)&cb_param2);
		
	}	
	if(cb_id == TXM_QAPI_ADC_SET_TOLERENCE_CB)
	{
	    pfn_app_cb1 = (qapi_AdcTM_Threshold_Cb_Type)app_cb;
	    (pfn_app_cb1)((void *) cb_param1, (qapi_ADC_Threshold_Result_t *)&cb_param2);
		
	}
	return 0;
}

#endif


