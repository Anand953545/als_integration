/* Copyright (c) 2016-2021 Qualcomm Technologies, Inc. and/or its subsidiaries.
   All rights reserved.
   Confidential - Qualcomm Technologies, Inc. - May Contain Trade Secrets
*/

/* Copyright (c) 2015 Qualcomm Atheros, Inc.
   All rights reserved.
   Qualcomm Atheros Confidential and Proprietary.
*/

/*
!! IMPORTANT NOTE: "DATA SERVICES" VERSION CATEGORY. 
*/

/**
 * @file qapi_netprofile.h
 *
 * @addtogroup qapi_networking_netprofile
 * @{
 *
 * @details The Network Profile service provides a collection of API functions
 * that allow the application to configure the networking sub-system in terms
 * of performance versus memory consumption. A larger buffer pool would allow
 * higher possible throughput and a smaller buffer pool would allow the system
 * to run with a smaller memory footprint. There are 3 predefined profiles that
 * the application can select from, or it can define its own custom profile.
 * It is also possible to configure the custom profile, as well as choose the
 * one of the predefined profiles using Device Configuration.
 *
 * @}
 */

#ifndef _QAPI_NETBUF_PROFILE_H_
#define _QAPI_NETBUF_PROFILE_H_

#include "qapi_data_txm_base.h"
#include "qapi_status.h"

/* Net QAPI REQUEST ID DEFINES */
#define  TXM_QAPI_NET_PROFILE_SET_CUSTOM                TXM_QAPI_NET_BASE + 18
#define  TXM_QAPI_NET_PROFILE_SET_ACTIVE                TXM_QAPI_NET_BASE + 19
#define  TXM_QAPI_NET_PROFILE_GET_ACTIVE                TXM_QAPI_NET_BASE + 20

/** @addtogroup qapi_networking_netprofile
@{ */

/**
* @details The predefined netbuf pool ID. #QAPI_NET_PROFILE_CUST_E updates the customized netbuf pool.
*/
typedef enum {
    QAPI_NET_PROFILE_PREF_E = 1,
    /**< Maximum throughput (performance) profile. */
    QAPI_NET_PROFILE_BESTEFFORT_E,
    /**< Performance/Memory Trade-off (Best effort) profile. */
    QAPI_NET_PROFILE_MEMOPT_E,
    /**< Minimum memory usage profile. */
    QAPI_NET_PROFILE_CUST_E,
    /**< Custom buffer pool profile. @newpage */
    QAPI_NET_PROFILE_MAX = QAPI_NET_PROFILE_CUST_E,
} qapi_Net_Profile_Type_t;

/**
*  Holds a netbuf pool entity. Each netbuf entry is the number of buffers and
*  its size.
*/
typedef struct {
   int32_t buf_Num;
   /**< Initial number of buffers in the queue. */
   int32_t buf_Size;
   /**< Size of each buffer. */
} qapi_Net_Profile_Custom_Pool_t;


#ifdef  QAPI_TXM_MODULE     // USER_MODE_DEFS

#define qapi_Net_Profile_Set_Custom(a,b)                     ((qapi_Status_t) (_txm_module_system_call12)(TXM_QAPI_NET_PROFILE_SET_CUSTOM              , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Net_Profile_Set_Active(a)                       ((qapi_Status_t) (_txm_module_system_call12)(TXM_QAPI_NET_PROFILE_SET_ACTIVE              , (ULONG) a, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Net_Profile_Get_Active(a,b,c)                   ((qapi_Status_t) (_txm_module_system_call12)(TXM_QAPI_NET_PROFILE_GET_ACTIVE              , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))

#else

#ifdef QAPI_NET_PROFILE_DYNAMIC_SETTING
/**
* Sets customized netbuf pool.
*
* @details Call this function to configure custom netbuf pool.
*
* @param [in] *pNetBuf of type  #qapi_Net_Profile_Custom_Pool_t. Holds netbuf pool entries.
* @param [in] net_bufq_size     Specifies size of netbuf pool.
*
* @return       0 if operation succeeded, -1 otherwise.
*/
qapi_Status_t qapi_Net_Profile_Set_Custom(qapi_Net_Profile_Custom_Pool_t *pNet_buf, uint8_t net_bufq_size);

/**
*  Sets active profile. Profile can be from predefined netbuf profiles.
*
* @details Call this switch from one netbuf pool profile to another.
*
* @param [in] profile  Return profile ID of type #qapi_Net_Profile_Type_t.
*
* @return       0 if operation succeeded, -1 otherwise.
*/
qapi_Status_t qapi_Net_Profile_Set_Active(qapi_Net_Profile_Type_t profile);

#endif /* QAPI_NET_PROFILE_DYNAMIC_SETTING */

/**
* Get active profile.
*
* @details Call this function to get active profile.
*
* @param [in] pNet_buf       Memory to hold the return netbuf pool.
* @param [in] net_bufq_size  Number entries in the current pool.
* @param [in] profile        Return profile ID of type #qapi_Net_Profile_Type_t.
*
* @return       0 if operation succeeded, -1 otherwise.
*/
qapi_Status_t qapi_Net_Profile_Get_Active(qapi_Net_Profile_Custom_Pool_t **pNet_buf, uint8_t *net_bufq_size, qapi_Net_Profile_Type_t *profile);

/** @} */

#endif

#endif /* _QAPI_NETBUF_PROFILE_H_ */
