/*======================================================================
                        qapi_gpioint_user.c 

GENERAL DESCRIPTION
  User interface for the GPIOInt module.


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
 * 03/06/2020   ssambu   Initial Version
 ======================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "qapi_gpioint.h"

#ifdef  QAPI_TXM_MODULE     // USER_MODE_DEFS

void qapi_custom_cb_gpioint_dispatcher(UINT cb_id, void *app_cb, 
                                       UINT cb_param1)
{
  qapi_GPIOINT_CB_t pfn_app_cb1;
  if(cb_id == TXM_QAPI_GPIOINT_REGISTER_INTERRUPT_CB) // custom cb-type1
  {
    pfn_app_cb1 = (qapi_GPIOINT_CB_t)app_cb;
    (pfn_app_cb1)((qapi_GPIOINT_Callback_Data_t)(cb_param1));
  }
}

#endif
