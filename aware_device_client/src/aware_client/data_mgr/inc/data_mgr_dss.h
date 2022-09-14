/*
 * @file data_mgr_intr.h
 * @brief DATA_MGR Internal header
 */
 
#ifndef DATA_MGR_DSS_H_
#define DATA_MGR_DSS_H_

#include "qapi_dss.h"
#include "qapi_diag.h"
#include "app_types.h"

#define DM_APN_SZ 32        /*APN string size*/

#define INET_ADDRSTRLEN 16

#define INITIAL_DELAY 2     /*Initial retry delay in secs*/
#define MAX_DELAY (10 * 60) /*Maximum delay in secs*/
#define EXPONENTIAL_INC 0.3 /*Retry delay increment*/

/************************************************************************
 *  Signal/events utility functions
 * **********************************************************************/

#define DM_SIGNAL_ATTR_WAIT_ANY    0x00000001
#define DM_SIGNAL_ATTR_WAIT_ALL    0x00000002
#define DM_SIGNAL_ATTR_CLEAR_MASK  0x00000004

void data_mgr_signal_init(TX_EVENT_FLAGS_GROUP *signal);

void data_mgr_signal_destroy(TX_EVENT_FLAGS_GROUP *signal);

uint32 data_mgr_signal_wait(TX_EVENT_FLAGS_GROUP *signal, uint32 mask, uint32 attribute);

int data_mgr_signal_wait_timed(TX_EVENT_FLAGS_GROUP *signal,
                         uint32 mask,
                         uint32 attribute,
                         uint32 *curr_signals,
                         uint32 timeout);
                         
void data_mgr_signal_set(TX_EVENT_FLAGS_GROUP *signal, uint32 mask);

void data_mgr_signal_clear(TX_EVENT_FLAGS_GROUP *signal, uint32 mask);

uint32 data_mgr_signal_get(TX_EVENT_FLAGS_GROUP *signal);

char coap_local_ip[16];

/************************************************************************
 *  Misc utility functions
 * **********************************************************************/

void data_mgr_util_int_to_str(char* str, int num);

int get_ip_from_url(qapi_DSS_Hndl_t dss_hndl, const char *hostname, char* resolved_ip_address);

int is_valid_ip_address(char *server_ip);

int32_t tcp_inet_ntoa(const qapi_DSS_Addr_t inaddr, uint8_t *buf, int32_t buflen);

/*Return values*/
#define ESUCCESS 0
#define EFAILURE -1

int data_mgr_start_datacall(void);
int data_mgr_stop_datacall(void);
void dss_show_nw_info(void);

#endif
