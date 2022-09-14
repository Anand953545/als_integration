#ifndef __APP_UTILS_LIST_H__
#define __APP_UTILS_LIST_H__

#include "qapi_types.h"

typedef enum
{
  LINKED_LIST_SUCCESS,
  LINKED_LIST_FAILURE_GENERAL,
  LINKED_LIST_INVALID_PARAMETER,
  LINKED_LIST_INVALID_HANDLE,
  LINKED_LIST_UNAVAILABLE_RESOURCE,
  LINKED_LIST_INSUFFICIENT_BUFFER,
} app_utils_list_err;

app_utils_list_err app_utils_list_init(void** list_data);
app_utils_list_err app_utils_list_destroy(void** list_data);
app_utils_list_err app_utils_list_add(void* list_data, void *data_obj, void (*dealloc)(void*));
app_utils_list_err app_utils_list_remove(void* list_data, void **data_obj);
int app_utils_list_empty(void* list_data);
app_utils_list_err app_utils_list_flush(void* list_data);
app_utils_list_err app_utils_list_search(void* list_data, void **data_p,
                                        boolean (*equal)(void* data_0, void* data),
                                        void* data_0, boolean rm_if_found);

#endif /* __APP_UTILS_LIST_H__ */
