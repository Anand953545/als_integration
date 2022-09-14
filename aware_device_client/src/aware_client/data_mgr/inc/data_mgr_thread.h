#ifndef __DATA_MGR_THREAD_H__
#define __DATA_MGR_THREAD_H__

#include <stdint.h>
#include <stdio.h>

#include "qapi_location.h"
#include "app_types.h"

#define LEFT_SHIFT_OP(N)		(1 << (N))

typedef enum DSS_Net_Evt_TYPE
{
	DSS_EVT_INVALID_E = 0x00,   /**< Invalid event. */
	DSS_EVT_NET_IS_CONN_E,      /**< Call connected. */
	DSS_EVT_NET_NO_NET_E,       /**< Call disconnected. */
	DSS_EVT_NET_RECONFIGURED_E, /**< Call reconfigured. */
	DSS_EVT_NET_NEWADDR_E,      /**< New address generated. */
	DSS_EVT_NET_DELADDR_E,      /**< Delete generated. */
	DSS_EVT_NIPD_DL_DATA_E,
	DSS_EVT_MAX_E
} DSS_Net_Evt_Type_e;

typedef enum DSS_SIG_EVENTS
{
	DSS_SIG_EVT_INV_E		= LEFT_SHIFT_OP(0),
	DSS_SIG_EVT_NO_CONN_E	= LEFT_SHIFT_OP(1),
	DSS_SIG_EVT_CONN_E		= LEFT_SHIFT_OP(2),
	DSS_SIG_EVT_DIS_E		= LEFT_SHIFT_OP(3),
	DSS_SIG_EVT_EXIT_E		= LEFT_SHIFT_OP(4),
	DSS_SIG_EVT_MAX_E		= LEFT_SHIFT_OP(5)
} DSS_Signal_Evt_e;

// Messages that can be posted to this thread
typedef enum
{
    DATA_MGR_MSG_ID_MIN,

    // Incoming requests to this thread
    DATA_MGR_MSG_ID_PERIODIC_CHECKIN,

    DATA_MGR_MSG_ID_MAX
} data_mgr_msg_id;

void data_mgr_task_entry(unsigned long args);
char* data_mgr_get_task_name();
int data_mgr_get_task_id();

void data_mgr_report_scripted_location(void);
void data_mgr_publish_activation_message(void);

int data_mgr_thread_send_msg_periodic_checkin(uint8* payload, size_t payload_len);

#endif