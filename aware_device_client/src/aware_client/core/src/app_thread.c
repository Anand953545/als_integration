#include <stdio.h>
#include "qapi_timer.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "qflog_utils.h"
#include "app_msg_q.h"
#include "app_thread.h"
#include "app_utils_misc.h"

#include "aware_log.h"

#define LOCATION_STACK_SIZE (16384)

static AppThreadContext threadPool[APP_THREAD_ID_MAX];
static boolean isModuleInit = 0;

/*-------------------------------------------------------------------------*/
/**
  @brief    Deallocate application message thread
  @param    msg		Application Message 
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void app_thread_msg_dealloc(void *msg)
{
    AppMsg *payload = (AppMsg *)msg;

    if(NULL == payload)
    {
        return;
    }

    if (payload->msg)
    {
        app_utils_byte_release(payload->msg);
    }

    app_utils_byte_release(payload);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Reset context of application thread
  @param    threadId	Application thread Id from where context need to be reset 
  @return   0 on success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
static int app_thread_context_reset(app_thread_id threadId)
{
    int ret = -1;

    if(threadId >= APP_THREAD_ID_MAX)
    {
        LOG_ERROR("Invalid thread id %d", threadId);
        return -1;
    }

    threadPool[threadId].handle = NULL;
    threadPool[threadId].msgQ   = NULL;
    threadPool[threadId].bValid = 0;

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Deinit context of application thread
  @param    threadId	Application thread Id from where context need to be deinit 
  @return   0 on success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
static int app_thread_context_deinit(app_thread_id threadId)
{
    int ret = -1;
    AppThreadContext *context = NULL;

    if(threadId >= APP_THREAD_ID_MAX)
    {
        LOG_ERROR("Invalid thread id %d", threadId);
        return -1;
    }

    context = &threadPool[threadId];

    if (context->handle)
    {
#ifdef QAPI_TXM_MODULE
        txm_module_object_deallocate(context->handle);
#endif
    }

    if(context->msgQ)
    {
        app_msg_q_flush((void *)context->msgQ);
        app_msg_q_destroy((void **)&(context->msgQ));
    }

    context->bValid = 0;
    app_thread_context_reset(threadId);
    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Initialize context of application thread
  @param    threadId	Application thread Id from where context need to be init 
  @return   0 on success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
static int app_thread_context_init(app_thread_id threadId)
{

    int ret = -1;
    AppThreadContext *context = NULL;

    if(threadId >= APP_THREAD_ID_MAX)
    {
        LOG_ERROR("Invalid thread id %d", threadId);
        return -1;
    }

    context = &threadPool[threadId];

    do
    {

#ifdef QAPI_TXM_MODULE
        if (TX_SUCCESS != (ret = txm_module_object_allocate(
                (void**)&(context->handle), sizeof(TX_THREAD))))
        {
            LOG_ERROR("Handle allocation failed err %d",ret);
            break;
        }
#endif

        if(NULL == (context->msgQ = app_msg_q_init2()))
        {
            LOG_ERROR("msgQ allocation failed");
            ret = -1;
            break;
        }

        context->bValid = 1;
    } while(0);

    if (ret != 0)
    {
        app_thread_context_deinit(threadId);
    }

    return ret;
}

/**************************************************************************
 * PUBLIC FUNCTIONS
 **************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Initialize application thread module
 */
/*--------------------------------------------------------------------------*/
void app_thread_module_init()
{
    int i = 0;

    if(isModuleInit)
    {
        LOG_ERROR("Module already initialized");
        return;
    }

    for(i = 0; i<APP_THREAD_ID_MAX; i++)
    {
        app_thread_context_reset(i);
    }

    isModuleInit = 1;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Creates application thread
  @param    threadName	    Name of the application thread
  @param    entryFunction	Name of the thread entry function
  @param    threadId	    Application thread Id
  @return   0 on success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
int app_thread_create(
        const char *threadName, threadEntry entryFunction,
        const app_thread_id threadId)
{
    int ret = -1;
    char *stackPtr = NULL;
    AppThreadContext *context = NULL;

    /*Param Check*/
    if((threadId >= APP_THREAD_ID_MAX) ||
            (NULL == entryFunction) || (NULL == threadName))
    {
        LOG_ERROR("Invalid param");
        return -1;
    }

    context = &threadPool[threadId];

    if(0 != context->bValid)
    {
        LOG_ERROR("Thread id allready in use");
        return -1;
    }

    do
    {
        /* Initialise the context for the thread */
        if (0 != (ret = app_thread_context_init(threadId)))
        {
            LOG_ERROR("context init failed err %d",ret);
            break;
        }

        /* Allocate the stack*/
        if (TX_SUCCESS != (ret =
                app_utils_byte_allocate((VOID **)&stackPtr, LOCATION_STACK_SIZE)))
        {
            LOG_ERROR("Stack allocation failed err %d",ret);
            break;
        }

        /* Create the application thread */
        if (TX_SUCCESS != (ret = tx_thread_create(
                                       (context->handle),
                                       (char*)threadName,
                                       entryFunction,
                                       (ULONG)threadId,
                                       stackPtr,
                                       LOCATION_STACK_SIZE,
                                       150,
                                       150,
                                       TX_NO_TIME_SLICE,
                                       TX_AUTO_START)))
        {
            LOG_ERROR("Thread creation failed err %d",ret);
            break;
        }
    } while(0);

    if (ret != TX_SUCCESS)
    {
        if(stackPtr)
        {
            app_utils_byte_release(stackPtr);
        }
        app_thread_context_deinit(threadId);
    }
    return ret;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete application thread
  @param    threadId	Application thread Id to delete
  @return   0 on success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
int app_thread_destroy(app_thread_id threadId)
{
    int ret = -1;
    AppThreadContext *context = NULL;

    if(threadId >= APP_THREAD_ID_MAX)
    {
        LOG_ERROR("Invalid thread id %d", threadId);
        return -1;
    }

    context = &threadPool[threadId];

    if(0 == context->bValid)
    {
        LOG_ERROR("Thread already destroyed %d", (context->bValid));
        return -1;
    }

    if(TX_SUCCESS != (ret = tx_thread_terminate(context->handle)))
    {
        LOG_ERROR("Thread termination failed err %d", ret);
        return ret;
    }

    if(TX_SUCCESS != (ret = tx_thread_delete(context->handle)))
    {
        LOG_ERROR("Thread delete failed err %d", ret);
        return ret;
    }

    app_thread_context_deinit(threadId);
    context->bValid = 0;

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Send message to application thread
  @param    dstThreadId 	Destination application thread Id
  @param    msgId       	Message Id
  @param    msg     	    Message to application thread
  @param    msgSize     	Message size
  @return   0 on success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
int app_thread_send_msg(app_thread_id dstThreadId, int msgId, void *msg, size_t msgSize)
{
    int ret = -1;
    AppThreadContext *context = NULL;
    AppMsg *payload = NULL;

    if(dstThreadId >= APP_THREAD_ID_MAX)
    {
        LOG_ERROR("Invalid thread id %d", dstThreadId);
        return -1;
    }

    context = &threadPool[dstThreadId];

    if(0 == context->bValid)
    {
        LOG_ERROR("Invalid context %d", (context->bValid));
        return -1;
    }

    do
    {
        /* Allocate payload */
        if (TX_SUCCESS != (ret =
                app_utils_byte_allocate((void **)&payload, sizeof(*payload))))
        {
            LOG_ERROR("Payload allocation failed err %d",ret);
            break;
        }
        else
        {
            payload->msg = NULL;
            payload->msgSize = 0;
            payload->msgId= 0;
        }

        /* Allocate msg */
        if (msg != NULL && msgSize > 0)
        {
            if (TX_SUCCESS != (ret = app_utils_byte_allocate((VOID **)&(payload->msg), msgSize))) {
                LOG_ERROR("Msg allocation failed err %d",ret);
                break;
            }
        }
    } while(0);

    if(0 != ret)
    {
        app_thread_msg_dealloc(payload);
        return ret;
    }

    payload->msgId = msgId;

    if (msg != NULL && msgSize > 0)
    {
        memcpy(payload->msg, msg, msgSize);
        payload->msgSize = msgSize;
    }

    LOG_DEBUG("Sending message MsgId %d to TID %d", msgId, dstThreadId);

    if(0 != (ret = app_msg_q_snd(context->msgQ, payload, app_thread_msg_dealloc)))
    {
        LOG_ERROR("Couldnot send the message %d",ret);
        app_thread_msg_dealloc(payload);
        return -1;
    }

    return ret;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Receive message to application thread
  @param    threadId	Application thread Id
  @param    payload 	Payload message
  @return   0 on success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
int app_thread_rcv_msg(app_thread_id threadId, void **payload)
{
    int ret = -1;
    AppThreadContext *context = NULL;

    if(threadId >= APP_THREAD_ID_MAX)
    {
        LOG_ERROR("Invalid thread id %d", threadId);
        return -1;
    }

    context = &threadPool[threadId];

    if(0 == context->bValid)
    {
        LOG_ERROR("Invalid context %d", (context->bValid));
        return -1;
    }

    if(0 != (ret = app_msg_q_rcv((void *)context->msgQ, payload)))
    {
        LOG_ERROR("Could not receive the message %d",ret);
        return -1;
    }

    LOG_DEBUG("Message received for TID%d", threadId);
    return ret;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get context of application thread
  @param    threadId	Application thread Id
 */
/*--------------------------------------------------------------------------*/
AppThreadContext app_utils_get_context(app_thread_id threadId)
{
    return threadPool[threadId];
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Sleep time in milliseconds
  @param    millis  time in milliseconds
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void app_utils_sleep_ms(uint32 millis)
{
    // Not possible to sleep for less than 15 millis
    if (millis < 15)
    {
        millis = 15;
    }
    tx_thread_sleep(MILLIS_TO_TICKS(millis));
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Sleep time with message in milliseconds
  @param    millis  time in milliseconds
  @param    msg     message string
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void app_utils_sleep_ms_with_log(uint32 millis, const char* msg)
{
    // Not possible to sleep for less than 10 millis
    if (millis < 15)
    {
        millis = 15;
    }

    uint32 seconds = millis / 1000;
    uint32 milliseconds = millis % 1000;
    for (uint32 i = 0; i < seconds; i++)
    {
        tx_thread_sleep(MILLIS_TO_TICKS(1000));
        LOG_INFO("[%s] Slept for %d/%d seconds..", msg, i+1, seconds);
    }
    if (milliseconds > 15)
    {
        tx_thread_sleep(MILLIS_TO_TICKS(milliseconds));
    }
}

/**************************************************************************
    Signal Utilities
***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Initialize signal application utils
  @param    signal          signal to search
  @param    signalName      Name of the signal
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
int app_utils_init_signal(TX_EVENT_FLAGS_GROUP** signal, const char* signalName)
{

#ifdef QAPI_TXM_MODULE
    txm_module_object_allocate((void**)signal, sizeof(TX_EVENT_FLAGS_GROUP));
#endif

    tx_event_flags_create(*signal, (char*)signalName);
    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Deinitialize signal application utils
  @param    signal      signal to search
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
int app_utils_deinit_signal(TX_EVENT_FLAGS_GROUP* signal)
{
    tx_event_flags_delete(signal);

#ifdef QAPI_TXM_MODULE
    txm_module_object_deallocate(signal);
#endif

    return 0;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Wait for the mutex signal
  @param    signal      signal to search
  @param    mutex       mutex object
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
uint32_t app_utils_wait_on_signal(TX_EVENT_FLAGS_GROUP* signal, TX_MUTEX* mutex)
{
    return app_utils_timedwait_on_signal(signal, mutex, TX_WAIT_FOREVER);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Wait untill given time to get mutex signal
  @param    signal      signal to search
  @param    mutex       mutex object
  @param    waitForTicks Number of timer ticks to wait
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
uint32_t app_utils_timedwait_on_signal(
        TX_EVENT_FLAGS_GROUP* signal, TX_MUTEX* mutex, uint32_t waitForTicks)
{
    ULONG        setSignal = 0;
    UINT         old_threshold, dummy;
    TX_THREAD*   thread;

    /* Making the operation atomic. Find the current thread and
     * Raise its preemption threshold so it does not get de-scheduled. */
    thread = tx_thread_identify();
    tx_thread_preemption_change(thread, 0, &old_threshold);

    app_utils_mutex_put(mutex);
    uint32_t ret = tx_event_flags_get(
                       signal,
                       1,
                       TX_OR_CLEAR,
                       &setSignal,
                       waitForTicks);

    /* Restore original preemption threshold and lock the Mutex*/
    tx_thread_preemption_change(thread, old_threshold, &dummy);
    app_utils_mutex_get(mutex);

    return ret;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Set signal to the mutex
  @param    signal      Signal to search
  @param    mutex       mutex object
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void app_utils_set_signal(TX_EVENT_FLAGS_GROUP* signal, TX_MUTEX* mutex)
{
    app_utils_mutex_get(mutex);
    tx_event_flags_set(signal, 1, TX_OR);
    app_utils_mutex_put(mutex);
}

/**************************************************************************
    Mutex Utilities
***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Initialize mutex
  @param    mutex       mutex object
  @param    name_ptr    name of mutex
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
int app_utils_mutex_init(TX_MUTEX** mutex, CHAR *name_ptr)
{
    int ret = -1;
    if(NULL == mutex)
    {
        LOG_ERROR("NULL mutex in Init");
        return -1;
    }

#ifdef QAPI_TXM_MODULE
    if(0 != txm_module_object_allocate((void**)mutex, sizeof(TX_MUTEX)))
    {
        LOG_ERROR("Object aloocation for mutex failed");
        return -1;
    }
#endif

    if(0 != tx_mutex_create(*mutex, name_ptr, TX_NO_INHERIT))
    {
        LOG_DEBUG("mutex %s creation failed", name_ptr);
#ifdef QAPI_TXM_MODULE
        txm_module_object_deallocate(mutex);
#endif
        return -1;
    }

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get mutex
  @param    mutex  mutex object
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
int app_utils_mutex_get(TX_MUTEX* mutex)
{
    if(NULL == mutex)
    {
        LOG_ERROR("NULL mutex in get");
        return -1;
    }
    return tx_mutex_get(mutex, TX_WAIT_FOREVER);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    release mutex
  @param    mutex  mutex object
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
int app_utils_mutex_put(TX_MUTEX* mutex)
{
    if(NULL == mutex)
    {
        LOG_ERROR("NULL mutex in put");
        return -1;
    }
    return tx_mutex_put(mutex);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    delete mutex
  @param    mutex  mutex object
  @return   0 on success
 */
/*--------------------------------------------------------------------------*/
int app_utils_mutex_delete(TX_MUTEX* mutex)
{
    if(mutex)
    {
        LOG_ERROR("NULL mutex in delete");
        return -1;
    }
    return tx_mutex_delete(mutex);
}
