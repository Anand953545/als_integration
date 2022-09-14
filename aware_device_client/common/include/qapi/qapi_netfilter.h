/* Copyright (c) 2016-2021 Qualcomm Technologies, Inc. and/or its subsidiaries.
 All rights reserved.
 Confidential - Qualcomm Technologies, Inc. - May Contain Trade Secrets
*/

/*
!! IMPORTANT NOTE: "DATA SERVICES" VERSION CATEGORY. 
*/

/**
  @file qapi_netfilter.h
  Networking Utilities (NF).
*/


#ifndef _QAPI_NET_FILTER_H_
#define _QAPI_NET_FILTER_H_

#include "stdint.h"
#include "qapi_net_status.h"
#include "qapi_types.h"
#include "qapi_data_txm_base.h"


#define  TXM_QAPI_ADD_STACK_FILTER                          TXM_QAPI_NET_BASE + 25
#define  TXM_QAPI_REMOVE_STACK_FILTER                       TXM_QAPI_NET_BASE + 26

/** @addtogroup qapi_net_services_datatypes
@{ */

/** Mask passes information to stack filtering layer on which the type of filter is being added.
 */
typedef uint64_t filter_mask_t;

#define FILTER_MASK_IP_ALLOW                            0x00000000     /** IP filter allow. */
#define FILTER_MASK_PORT_ALLOW                          0x00000001     /** Port filter allow. */


#define FILTER_MASK_IP_DISALLOW                         0x00000003     /** IP filter disallow. */
#define FILTER_MASK_PORT_DISALLOW                       0x00000004     /** Port filter disallow. */
 


/** Mask passes information to stack IP filtering layer to determine address type
   used to add filters at IP layer.
 */
typedef uint64_t ip_filter_mask_t;

#define IPFILTER_MASK_V4                          0x00000000   /** IPv4 filter. */
#define IPFILTER_MASK_V6                          0x00000001   /** IPv6 filter. */


/** IPv4 address that must be filtered at the IP Layer. */

typedef struct in_addr_f
{
   uint32_t s_addr;  /**< IPv4 address in network order. */

}ip4_addr_f;


/** IPv6 address that must be filtered at IP Layer. */

typedef struct in6_addr_s
{
   uint8_t  s_addr[16]; /**< 128-bit IPv6 address. */
   
} ip6_addr_f;



/** IP address filter information. */
typedef struct ip_filter_params
{

   ip_filter_mask_t mask;         /**< IP filter mask. */

   union
   {
      ip4_addr_f v4_addr;     /**< IPv4 address. */
      ip6_addr_f v6_addr;     /**< IPv6 address. */
   }f;

}ip_filter_params_t;


/** Port filter information to be passed to stack filtering layer. */

typedef struct port_filter_params
{
  uint16_t port;       /**< Port. */
  uint16_t range;       /**< Range. */

}port_filter_params_t;


/** Filtering parameters to be passed to stack filtering layer. */
typedef struct filter_params
{
   filter_mask_t  mask;       /**< Filter mask. */
   
   port_filter_params_t pinfo;	  /**< Port filter information. */
   ip_filter_params_t   ainfo;    /**< Address filter information. */

}filter_params_t;

/** @} */ /* end_addtogroup qapi_net_services_datatypes */

#ifdef  QAPI_TXM_MODULE     // USER_MODE_DEFS

#define qapi_add_stack_filter(a)                         ((UINT)         (_txm_module_system_call12)(TXM_QAPI_ADD_STACK_FILTER     , (ULONG) a, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_remove_stack_filter(a)                      ((UINT)         (_txm_module_system_call12)(TXM_QAPI_REMOVE_STACK_FILTER  , (ULONG) a, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))


#else



/** @ingroup qapi_add_stack_filter

 * @versiontable{2.0,2.45,
 * Data\_Services 1.10.0  &  Introduced. @tblendline
 * }
 *   Adds filter information to the stack filtering layer.
 *   Packets matching the filters will be dropped.

 *
 * @param[in] filter_info  Filter parameters to be added.
 *
 *
 * @return 
 * On success, 0. \n
 * On error, -1. 

 * Possible errors:\n
 * - EINVAL: Invalid arguments passed.
 * - ENOBUFS: No Memory
 */

qapi_Status_t qapi_add_stack_filter(filter_params_t * filter_info);



/** @ingroup qapi_remove_stack_filter

 * @versiontable{2.0,2.45,
 * Data\_Services 1.10.0  &  Introduced. @tblendline
 * }
 *   Removes filter information to the stack filtering layer 
 *   Packets matching the added filters will be dropped.

 * @param[in] filter_info  Filter parameters to be removed.

 * @return
  * On success, 0. \n
 * On error, -1. 

 * Possible errors:\n
 * - EINVAL: Invalid arguments passed.
 * - ENOBUFS: No Memory
 */

qapi_Status_t qapi_remove_stack_filter(filter_params_t * filter_info);

#endif
#endif /* _QAPI_NET_FILTER_H_ */


