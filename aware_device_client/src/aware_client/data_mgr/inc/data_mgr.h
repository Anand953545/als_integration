#ifndef DATA_MGR_API_H_
#define DATA_MGR_API_H_

#include <stdint.h>

typedef enum
{
    PAYLOAD_TYPE_CBOR_LIKE = 0,
    PAYLOAD_TYPE_PB
} payload_type_t;

/**
 * @brief Initialize DATA_MGR will create the data_mgr_thread and load mqtt configuration files and initializes the callbacks
 *        Or, return immediately when there is a invalid parameter passed
 * @param message_callback Invoked when message is receivied from server : Should not be NULL
 * @param prof_update_callback Invoked when OEM profile updation is required : Should not be NULL
 * @return ESUCCESS or FAILURE
 */
int data_mgr_initialize(void);

/**
 * @brief Publish event to coap server
 * @param device_id device identifier
 * @param event_type event type ( telemetry/alert )
 * @param pub_msg Message to be sent
 * @return ESUCCESS or EFAILURE
 */

int data_mgr_publish_event(const char *device_id, const char* event_type, payload_type_t payload_type, const void* payload, size_t payload_length);

/**
 * @brief Connect coap server endpoint
 * @return Success/failure
 */
int data_mgr_connect_coap_server(void);

/**
 * @brief Disconnect coap server endpoint
 * @return Success/failure
 */
int data_mgr_disconnect_coap_server(void);

extern int data_mgr_set_wwan_high_priority(void);

extern uint8_t* data_mgr_get_device_serial_number(void);

void publish_pending_messages(void);

#endif