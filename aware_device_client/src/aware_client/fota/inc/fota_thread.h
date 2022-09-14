#ifndef __FOTA_THREAD_H__
#define __FOTA_THREAD_H__

#include <stdint.h>

typedef enum
{
    FOTA_THREAD_MSG_ID_MIN,

    FOTA_THREAD_MSG_ID_INIT,
    FOTA_THREAD_MSG_ID_DAM_DOWNLOAD,

    FOTA_THREAD_MSG_ID_MAX
} FotaThreadMsgId;

typedef struct
{
    char* argName;
    char* argValue;

} FotaThreadCmdArg;

typedef struct
{
    uint32_t cmdIndex;
    char* cmdName;

    // Array of command arguments
    uint32_t cmdArgCount;
    FotaThreadCmdArg* cmdArgArray;

} FotaThreadCmd;

typedef struct
{
    uint32_t cmdCount;
    FotaThreadCmd* cmdArray;

} FotaThreadCmdArray;

void fota_thread_task_entry(unsigned long args);
char* fota_thread_get_task_name(void);
int fota_thread_get_task_id(void);

// Send messages to this thread, which will be processed after a
// context switch to this thread.
void fota_thread_send_msg_init(void);
void fota_thread_send_msg_dam_download(void);

void send_fota_trigger(void);
#endif /* __FOTA_THREAD_H__ */
