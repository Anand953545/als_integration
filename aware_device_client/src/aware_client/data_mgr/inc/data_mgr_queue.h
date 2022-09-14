/*
 * @file data_mgr_queue.h
 * @brief DATA_MGR Queue header
 */
 
#ifndef DATA_MGR_QUEUE_H_
#define DATA_MGR_QUEUE_H_


typedef struct dm_queue_item_s
{
  struct dm_queue_item_s *next;         /*  next item field */
  char msgId[10];                       /* message Id */
  char *msg;                            /*message*/
  uint32_t timestamp;                   /*Timestamp in secs*/
}dm_queue_item_t;

/* queue header */
typedef struct queue   
{
  dm_queue_item_t *head;   /* first element in queue */
  dm_queue_item_t *tail;
#ifdef QAPI_TXM_MODULE
  TX_MUTEX *qmutex;     /*queue mutex*/
#else
  TX_MUTEX qmutex;
#endif
  unsigned int cnt;    /* number of elements in queue */
} queue_t;




/************************************************************************
 * DATA_MGR Queue APIs
 ************************************************************************/
 
void data_mgr_queue_init(void);

void data_mgr_queue_destroy(void);

int data_mgr_queue_enqueue_msg(char* msgId, char *msg, int32_t msg_len);

void data_mgr_queue_remove_pub_msg(dm_queue_item_t *item);

int data_mgr_queue_process(void);

#endif
