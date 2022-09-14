/*================================================================================== 
  Diag user space functions                     

       Copyright (c) 2017-2018 QUALCOMM Technologies Incorporated.
                       All Rights Reserved.
                QUALCOMM Proprietary and Confidential.
==================================================================================*/

/* $Header: //components/rel/core.tx/6.0/services/diag/LSM/qapi/src/qapi_diag_user.c#1 $ */

/*==========================================================================
when       who     what, where, why
--------   ---    ----------------------------------------------------------
01/08/18   gn     Fixed KW error (initialized local variable rc) 
08/30/17   gn     Created
===========================================================================*/

#include "qapi_diag.h"

#ifdef FEATURE_MMU_ENABLED  
/*===========================================================================

FUNCTION diag_user_space_dispatcher

DESCRIPTION
   Diag function in user space that dispatches user space callbacks
===========================================================================*/
uint32_t diag_user_space_dispatcher(uint32_t cb_id, void *app_cb, uint32_t cb_param)
{
	uint32_t rc = 0;

	uint32_t (*user_cb) (uint32_t);
	switch(cb_id)
	{
	  case QC_DIAG_CALLBACK_CMD_RSP_ID:
	    /* Call the user callback with the length of the command written to client's input buffer */
		user_cb = (uint32_t(*)(uint32_t))app_cb;
		rc = (user_cb)((uint32_t)cb_param);		
		break;
	  default:
	    break;
	}		
	return rc;
}
#endif

