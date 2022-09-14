#ifndef __APP_THREAD_H__
#define __APP_THREAD_H__

#include "txm_module.h"
#include <stdint.h>

typedef void (*threadEntry)(unsigned long args);
typedef void (*msgHandler)(void *args, size_t size);

typedef enum
{
    APP_THREAD_ID_MIN,
	APP_THREAD_ID_AWARE_MAIN,
	APP_THREAD_ID_GNSS_WORKER,
	APP_THREAD_ID_SENSOR_MGR,
	APP_THREAD_ID_DATA_MGR,
	APP_THREAD_ID_FOTA,
	APP_THREAD_ID_MAX
} app_thread_id;

typedef struct {
	TX_THREAD *handle;
	const void *msgQ;
	uint8_t bValid;
} AppThreadContext;

typedef struct {
	void *msg;
	size_t msgSize;
	msgHandler handler;
	int msgId;
} AppMsg;

void app_thread_module_init(void);
AppThreadContext app_get_context(app_thread_id threadId);
int app_thread_create(const char *threadName, threadEntry entryFunction, const app_thread_id threadId);
int app_thread_destroy(app_thread_id threadId);
int app_thread_send_msg(app_thread_id dstThreadId, int msgId, void *msg, size_t msgSize);
int app_thread_rcv_msg(app_thread_id threadId, void **payload);

#endif /* __APP_THREAD_H__ */
