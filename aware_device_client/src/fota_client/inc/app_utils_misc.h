#ifndef __APP_UTILS_MISC_H__
#define __APP_UTILS_MISC_H__

//#include <stdarg.h>
#include "txm_module.h"
#include <stdint.h>
#include "app_types.h"
#include "qapi_location.h"

//#include "qapi_diag.h"

/* PLATFORM UTILITIES */
#define MILLIS_TO_TICKS(millis) (((millis)*15)/160)

/* SOCKET UTILITIES */
#define app_utils_htons(s) ((((s)>>8) & 0xff) | (((s) << 8) & 0xff00))
#define app_utils_htonl(s) ((((s) << 24) & 0xff000000) | (((s) << 8) & 0x00ff0000) | \
                          (((s) >> 8) & 0x0000ff00) | (((s) >> 24) & 0x000000ff))

/***************************************************************************
    Wait and Signal Utility
****************************************************************************/
int app_utils_init_signal(TX_EVENT_FLAGS_GROUP** signal, const char* signalName);
int app_utils_deinit_signal(TX_EVENT_FLAGS_GROUP* signal);
uint32_t app_utils_wait_on_signal(TX_EVENT_FLAGS_GROUP* signal, TX_MUTEX* mutex);
uint32_t app_utils_timedwait_on_signal(
        TX_EVENT_FLAGS_GROUP* signal, TX_MUTEX* mutex, uint32_t waitForTicks);
void app_utils_set_signal(TX_EVENT_FLAGS_GROUP* signal, TX_MUTEX* mutex);
void app_utils_sleep_ms(uint32 millis);
// Generates a log every second while sleeping
void app_utils_sleep_ms_with_log(uint32 millis, const char* msg);

/***************************************************************************
    Memory pool creation and allocation Utility
****************************************************************************/
int32 app_utils_byte_create_pool(void);
int32 app_utils_byte_allocate(VOID **memory_ptr, ULONG memory_size);
int32 app_utils_byte_release(VOID *memory_ptr);
TX_BYTE_POOL* app_utils_get_byte_pool(void);
void* app_utils_mem_alloc(size_t sz);
void app_utils_mem_free(void *ptr);

/***************************************************************************
    Mutex Utility
****************************************************************************/
int app_utils_mutex_init(TX_MUTEX** mutex, CHAR *name_ptr);
int app_utils_mutex_get(TX_MUTEX* mutex);
int app_utils_mutex_put(TX_MUTEX* mutex);
int app_utils_mutex_delete(TX_MUTEX* mutex);

/***************************************************************************
    String Utilities
****************************************************************************/
char * strdup_s(const char * s);
uint64_t app_utils_string_to_uint64(char* str, uint32 strLen);
int app_utils_is_string_uint(char* str, uint32 strLen);
float app_utils_string_to_float(char* str);
char* trim_white_spaces(char* source);
void print_byte_array(uint8* src, size_t len);
size_t memscpy(void *dst, size_t dst_size, const void *src, size_t src_size);

#endif /* __APP_UTILS_MISC_H__ */
