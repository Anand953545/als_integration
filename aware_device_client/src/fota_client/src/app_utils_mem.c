#include "qapi_fs.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "app_utils_misc.h"
#include "qapi_diag.h"
#include "log.h"
//#include "aware_log.h"

#define LOCATION_BYTE_POOL_SIZE (30720*8)

static UCHAR freeMemoryLocation[LOCATION_BYTE_POOL_SIZE];
static TX_BYTE_POOL *bytePoolLocation = NULL;
static boolean isInitialised = FALSE;

int32 app_utils_byte_create_pool() {

    int32 ret = -1;

    if(TRUE == isInitialised) {
        return 0;
    }

    do {
#ifdef QAPI_TXM_MODULE
        if(0 != (ret = txm_module_object_allocate((void**)&bytePoolLocation, sizeof(TX_BYTE_POOL)))) {
            LOG_ERROR("txm_module_object_allocate failed err %d", ret);
            break;
        }
#endif
        if(0 != (tx_byte_pool_create(bytePoolLocation, "Location application pool", freeMemoryLocation, LOCATION_BYTE_POOL_SIZE))) {
            LOG_ERROR("tx_byte_pool_create failed err %d", ret);
            break;
        }
    } while(0);

    if(0 != ret) {
        if(bytePoolLocation) {
#ifdef QAPI_TXM_MODULE
            txm_module_object_deallocate(bytePoolLocation);
#endif
        }
        LOG_ERROR("Module initialization failed %d", ret);
    } else {
        isInitialised = TRUE;
    }

    return ret;
}

int32 app_utils_byte_allocate(void** memory_ptr, uint32 memory_size) {

    if(!isInitialised && 0 != app_utils_byte_create_pool()) {
        return -1;
    }
    return (TX_SUCCESS == tx_byte_allocate(
            bytePoolLocation, memory_ptr, memory_size, TX_NO_WAIT))? 0: -1;
}

int32 app_utils_byte_release(void* memory_ptr) {

    if(NULL != memory_ptr) {
        return (TX_SUCCESS == tx_byte_release(memory_ptr))? 0: -1;
    } else {
        LOG_ERROR("NULL memory pointer");
    }

    return -1;
}

TX_BYTE_POOL *app_utils_get_byte_pool() {

    int ret = 0;
    if(!isInitialised && (0 != (ret = app_utils_byte_create_pool()))) {
        LOG_INFO("Memory pool err %d", ret);
        return NULL;
    }
    return bytePoolLocation;
}

/**
 * @brief Alloc memory 
 * @param sz Size of memory needed
 */
void* app_utils_mem_alloc(size_t sz)
{
  void *ptr = NULL;
 
  tx_byte_allocate(app_utils_get_byte_pool(), (VOID **)&ptr, sz, TX_NO_WAIT);

  if(ptr != NULL)
    memset(ptr, 0, sz);

  return ptr;
}

/**
 * @brief Free memory
 * @param ptr Memory pointer
 */
void app_utils_mem_free(void *ptr)
{
  if(ptr != NULL)
    tx_byte_release(ptr);
}

/**
 * @brief Copy bytes from src to dst
 * @param dst Destination pointer
 * @param dst_size Destination size
 * @param src Source pointer
 * @param src_size Source size
 * @return 
 */

size_t memscpy(void *dst, size_t dst_size, const void *src, size_t src_size)
{
  UINT  copy_size = 0;
  if(dst != NULL && src != NULL) {
    copy_size = (dst_size <= src_size)? dst_size : src_size;
    memcpy(dst, src, copy_size);
  }
  return copy_size;
}