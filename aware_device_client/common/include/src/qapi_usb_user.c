/*==============================================================================

qapi_USB_user_cb_dispatcher

GENERAL DESCRIPTION
This file contains definition of User callback dispatcher.


Copyright (c) 2020  by Qualcomm Technologies, Inc.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.tx/6.0/wiredconnectivity/hsusb/driver/src/adapt_layers/qapi_usb_user.c#1 $
$DateTime: 2020/03/23 03:02:38 $ $Author: pwbldsvc $
2009/01/13 04:17:02 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ------------------------------------------------------------
03/04/20  cs      Initial version

==============================================================================*/

#include "qapi_usb.h"


#ifdef QAPI_TXM_MODULE

/* CALLBACK DISPATCHER */
int qapi_USB_user_cb_dispatcher(UINT cb_id, void *app_cb,
                                     UINT cb_param1, UINT cb_param2,
                                     UINT cb_param3, UINT cb_param4,
                                     UINT cb_param5, UINT cb_param6,
                                     UINT cb_param7, UINT cb_param8)
{
  void (*pfn_app_cb1) (void);
  void (*pfn_app_cb2) (void);

  if(cb_id == TXM_QAPI_USB_RX_CB) // app_rx_cb
  {
      pfn_app_cb1 = (void (*)(void))app_cb;
      (pfn_app_cb1)();
  }
  else if(cb_id == TXM_QAPI_USB_DISC_CB) // app_disconnect_cb
  {
      pfn_app_cb2 = (void (*)(void))app_cb;
      (pfn_app_cb2)();
  }

  return 0;
}

#endif
