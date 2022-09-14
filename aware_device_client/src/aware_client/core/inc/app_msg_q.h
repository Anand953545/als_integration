#ifndef __APP_MSG_Q_H__
#define __APP_MSG_Q_H__

#include <stdlib.h>

typedef enum
{
  APP_MSG_Q_SUCCESS,
  APP_MSG_Q_FAILURE_GENERAL,
  APP_MSG_Q_INVALID_PARAMETER,
  APP_MSG_Q_INVALID_HANDLE,
  APP_MSG_Q_UNAVAILABLE_RESOURCE,
  APP_MSG_Q_INSUFFICIENT_BUFFER,
} app_msq_q_err_type;

app_msq_q_err_type app_msg_q_init(void** msg_q_data);
const void* app_msg_q_init2(void);
app_msq_q_err_type app_msg_q_destroy(void** msg_q_data);
app_msq_q_err_type app_msg_q_snd(const void* msg_q_data, void* msg_obj, void (*dealloc)(void*));
app_msq_q_err_type app_msg_q_rcv(const void* msg_q_data, void** msg_obj);
app_msq_q_err_type app_msg_q_flush(void* msg_q_data);

#endif /* __APP_MSG_Q_H__ */
