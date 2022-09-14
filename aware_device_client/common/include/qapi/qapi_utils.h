#ifndef QAPI_UTILS_H
#define QAPI_UTILS_H

/**
 * @file qapi_utils.h
 * @brief QAPI UTILS Functional Overview
 * @details This file provides QAPI wrapper over UTILITY APIs.
*/
/*
 * Copyright (C) 2022 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2022-03-16   sna   created file for qapi support.

===========================================================================*/


#include "qapi_txm_base.h"  
#include "txm_module.h"


#define  TXM_QAPI_FLASH_COUNTER              TXM_QAPI_UTILS_BASE

#ifdef  QAPI_TXM_MODULE

#define qapi_utils_efs_log_writecount(subsys, wrt_cnt, blk_cnt)\
    ((qapi_FS_Status_t) (_txm_module_system_call12)(TXM_QAPI_FLASH_COUNTER,\
    (ULONG) subsys,(ULONG) wrt_cnt, (ULONG) blk_cnt, (ULONG) 0,(ULONG) 0,\
    (ULONG) 0,(ULONG) 0,(ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, \
    (ULONG) 0))


#elif defined QAPI_TXM_SOURCE_CODE


typedef int32              qapi_utils_Status_t;

/*============================================================================
                qapi_utils_efs_log_writecount
============================================================================*/
/**
 *
 *@brief   QAPI to get the number of flash page writes in EFS partition
 *
 *@details This API returns the number of flash page writes in the EFS partition
 *         for the specified subsystem from current bootup till the point of
 *         invocation. The count is taken from the EFS log region and the number
 *         of blocks in the log region is also returned in the parameter passed.
 *         The log region is updated in a circular fashion. The current rate of
 *         flash writes can be calculated based on the write count and the number
 *         of pages (no. of pages = block count * no. of pages per block).
 *         The rate of flash page writes combined with the P/E cycles for the flash
 *         can give an indication of the flash life and if the rate is going very
 *         high that it affects the flash life, the application can take necessary steps
 *
 *  @param[in] subsys  Subsystem to get write counters from which EFS region
 *			         	0.APPS EFS Region
 *			         	1.MPSS EFS Region
 *  @param[in] write_count Argument that gets returns the number of writes.
 *
 *  @param[out] num_block  It will returns the number of block present.
 *			        	   
 *@return Returns QAPI_OK on success and -ve error code is returned on failure.
 *
 * @note1hang If this API is called from user space with MMU enabled,
 *            the parameters must be user space addresses, otherwise
 *            an invalid parameter error will be returned.
 */

int qapi_utils_efs_log_writecount(int subsys,uint32 *write_count,uint32 *num_block);


int qapi_utils_handler(UINT id, UINT a, UINT b, UINT c, UINT d, UINT e,
                UINT f, UINT g, UINT h, UINT i, UINT j, UINT k, UINT l);

#endif
#endif
