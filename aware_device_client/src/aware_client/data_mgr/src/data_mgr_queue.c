/*
 * @file data_mgr_queue.c
 * @brief DATA_MGR Queue implementations
 */
  
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "qapi_timer.h"
#include "app_utils_misc.h"

#include "data_mgr.h"
#include "data_mgr_dss.h"
#include "data_mgr_queue.h"
#include "aware_log.h"
#include "aware_app.h"
#include "stringl.h"

/**Maximum queue lenght*/
#define DATA_MGR_MAX_QUEUE_SIZE 10

/**DATA_MGR Publish queue*/
static queue_t data_mgr_pub_queue;


/*-------------------------------------------------------------------------*/
/**
  @brief Initialize queue
  @param q Queue pointer
  @return Void
 */
/*--------------------------------------------------------------------------*/
void qe_init(queue_t *q)
{
  if(q != NULL) {
    q->cnt  = 0;
    q->head = NULL;
    q->tail = NULL;

#ifdef QAPI_TXM_MODULE
    if(TX_SUCCESS != txm_module_object_allocate((void **)&q->qmutex, sizeof(TX_MUTEX))) 
    {
      return;
    }
    tx_mutex_create(q->qmutex, "data_mgr_queue", TX_INHERIT);
#else
    tx_mutex_create(&q->qmutex, "data_mgr_queue", TX_INHERIT);
#endif
  }
}


/*-------------------------------------------------------------------------*/
/**
  @brief Destroy queue
  @param q Queue pointer
  @return Void
 */
/*--------------------------------------------------------------------------*/
void qe_destroy(queue_t *q)
{
  if(q != NULL) {
    q->cnt  = 0;
    q->head = NULL;
    q->tail = NULL;
#ifdef QAPI_TXM_MODULE
    tx_mutex_delete(q->qmutex);
#else
    tx_mutex_delete(&q->qmutex);
#endif
  }
}


/*-------------------------------------------------------------------------*/
/**
  @brief Enqueue item to queue
  @param q Queue pointer 
  @param ptr Item link
  @param item Item
  @return Void
 */
/*--------------------------------------------------------------------------*/
void qe_enque(queue_t *q, dm_queue_item_t *item)
{
  if((q != NULL) && (item != NULL)) {
#ifdef QAPI_TXM_MODULE
    tx_mutex_get(q->qmutex, TX_WAIT_FOREVER);
#else
    tx_mutex_get(&q->qmutex, TX_WAIT_FOREVER);
#endif

    item->next = NULL;

    if(q->head == NULL)
      q->head = item;
    else 
      q->tail->next = item;

    q->tail = item;
    
    q->cnt = q->cnt + 1;
#ifdef QAPI_TXM_MODULE
    tx_mutex_put(q->qmutex);
#else
    tx_mutex_put(&q->qmutex);
#endif
  }
}


/*-------------------------------------------------------------------------*/
/**
  @brief Dequeue item from queue
  @param q Queue pointer 
  @param ptr Item link
  @return Void
 */
/*--------------------------------------------------------------------------*/
void qe_deque(queue_t *q, dm_queue_item_t *item)
{ 
  dm_queue_item_t *nptr = NULL;
  dm_queue_item_t *pptr = NULL;

  if((q != NULL) && (item != NULL)) {
#ifdef QAPI_TXM_MODULE
    tx_mutex_get(q->qmutex, TX_WAIT_FOREVER);
#else
    tx_mutex_get(&q->qmutex, TX_WAIT_FOREVER);
#endif

    nptr = q->head;
    
    if(q->cnt != 0) {
      while(nptr) {
        if(nptr == item) {
          /* found our item; dequeue it */
          if(pptr)
            pptr->next = nptr->next;
          else /* item was at head of queqe */
            q->head = nptr->next;

          /* fix queue tail pointer if needed */
          if(q->tail == item)
            q->tail = pptr;

          /* fix queue counters */
          q->cnt--;
          break;
        }
        pptr = nptr;
        nptr = nptr->next;
      }
    }
#ifdef QAPI_TXM_MODULE
    tx_mutex_put(q->qmutex);
#else
    tx_mutex_put(&q->qmutex);
#endif
  }
}


/*-------------------------------------------------------------------------*/
/**
  @brief Get first item from queue
  @param q Queue pointer
  @return first item
 */
/*--------------------------------------------------------------------------*/
dm_queue_item_t* qe_check(queue_t *q)
{
  dm_queue_item_t *item = NULL;
  if(q != NULL) {
#ifdef QAPI_TXM_MODULE
    tx_mutex_get(q->qmutex, TX_WAIT_FOREVER);
#else
    tx_mutex_get(&q->qmutex, TX_WAIT_FOREVER);
#endif

    item = q->head;

#ifdef QAPI_TXM_MODULE
    tx_mutex_put(q->qmutex);
#else
    tx_mutex_put(&q->qmutex);
#endif

  } 
  return item;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Get number of items in the queue
  @param q Queue pointer
  @return Number of items in the queue
 */
/*--------------------------------------------------------------------------*/
unsigned int qe_cnt(queue_t *q)
{
  int count = -1;
  
  if(q != NULL) {
#ifdef QAPI_TXM_MODULE
    tx_mutex_get(q->qmutex, TX_WAIT_FOREVER);
#else
    tx_mutex_get(&q->qmutex, TX_WAIT_FOREVER);
#endif

    count = q->cnt;

#ifdef QAPI_TXM_MODULE
    tx_mutex_put(q->qmutex);
#else
    tx_mutex_put(&q->qmutex);
#endif

  }
  return count;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Get next item from link
  @param q Queue pointer
  @param link Item link
  @return Number of items in the queue
 */
/*--------------------------------------------------------------------------*/
dm_queue_item_t* qe_next(queue_t *q, dm_queue_item_t *link)
{
  dm_queue_item_t *item = NULL;
  if((q != NULL) && (link != NULL)) {
#ifdef QAPI_TXM_MODULE
    tx_mutex_get(q->qmutex, TX_WAIT_FOREVER);
#else
    tx_mutex_get(&q->qmutex, TX_WAIT_FOREVER);
#endif

    item = link->next;

#ifdef QAPI_TXM_MODULE
    tx_mutex_put(q->qmutex);
#else
    tx_mutex_put(&q->qmutex);
#endif
  }
  return item;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Initialize Publish queue
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_queue_init()
{
  LOG_INFO("[data_mgr]: data_mgr_queue_init : Entry\n");
  qe_init(&data_mgr_pub_queue);
}


/*-------------------------------------------------------------------------*/
/**
  @brief Destroy Publish queue
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_queue_destroy()
{
  dm_queue_item_t *item = qe_check(&data_mgr_pub_queue);
  dm_queue_item_t *next = item;
  
  while(item != NULL) 
  {
    next = qe_next(&data_mgr_pub_queue, item);
    data_mgr_queue_remove_pub_msg(item);
    item = next;
  }
  qe_destroy(&data_mgr_pub_queue);
}


/*-------------------------------------------------------------------------*/
/**
  @brief Enqueue a publish message
  @param msgId Message ID
  @param msg Publish message
  @param msg_len Message length
  @param topic Publish topic
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int data_mgr_queue_enqueue_msg(char* msgId, char *msg, int32_t msg_len)
{

  char *new_msg = NULL;
  dm_queue_item_t *new_item = NULL;
   
  if(qe_cnt(&data_mgr_pub_queue) >= DATA_MGR_MAX_QUEUE_SIZE)
  {
    LOG_ERROR("[data_mgr]: data_mgr_queue_enqueue_pub_msg : Publish queue is full\n");
    return EFAILURE;
  }
  
  new_item  = app_utils_mem_alloc(sizeof(dm_queue_item_t));
  new_msg   = app_utils_mem_alloc(msg_len + 1);
  
  if(new_item == NULL || new_msg == NULL) 
  {
    if(new_item != NULL) app_utils_mem_free(new_item);
    if(new_msg != NULL) app_utils_mem_free(new_msg);
    LOG_ERROR("[data_mgr]: data_mgr_queue_enqueue_pub_msg : Alloc failed\n");
    return EFAILURE;
  }
  
  memscpy(new_msg, msg_len + 1, msg, msg_len + 1);
  
  new_item->msg = new_msg;
  strlcpy(new_item->msgId, (const char *)msgId, sizeof(new_item->msgId));
  new_item->timestamp = tx_time_get() / 100;
   
  qe_enque(&data_mgr_pub_queue, new_item);
  
  LOG_INFO("[data_mgr]: data_mgr_queue_enqueue_pub_msg : Added to queue %d item\n", qe_cnt(&data_mgr_pub_queue));
  
  return ESUCCESS;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Dequeue a item from queue, free associated memory
  @param item Item to be deleted
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_queue_remove_pub_msg(dm_queue_item_t *item)
{
  app_utils_mem_free(item->msg);
  qe_deque(&data_mgr_pub_queue, item);
  app_utils_mem_free(item);
  LOG_INFO("[data_mgr]: data_mgr_queue_remove_pub_msg : Removed from queue, %d items left\n", qe_cnt(&data_mgr_pub_queue));
}


/*-------------------------------------------------------------------------*/
/**
  @brief Process publish queue, delete item from queue if validity expires 
         or publish message if connected to server
  @return ESUCCESS or EFAILURE
*/
/*--------------------------------------------------------------------------*/
int data_mgr_queue_process()
{
  uint32_t current_time = 0;
  dm_queue_item_t *item = qe_check(&data_mgr_pub_queue);
  dm_queue_item_t *next_link;
  
  while(item != NULL) 
  { 
    next_link = qe_next(&data_mgr_pub_queue, item);
   
//    if(QAPI_OK == data_mgr_publish_event(app_ctx.dhu_cfg.device_id, "telemetry", item->msg))
//      data_mgr_queue_remove_pub_msg(item);

    qapi_Timer_Sleep(100, QAPI_TIMER_UNIT_MSEC, true);

    item = next_link;
  }

  return qe_cnt(&data_mgr_pub_queue);
}
