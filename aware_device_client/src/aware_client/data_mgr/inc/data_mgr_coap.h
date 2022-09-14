
#ifndef DATA_MGR_COAP_H_
#define DATA_MGR_COAP_H_

#include "qapi_coap.h"
#include "data_mgr.h"

int coap_create_session(qapi_Coap_Session_Hdl_t*);
int coap_create_connection(qapi_Coap_Session_Hdl_t*);
int coap_send_post_message(qapi_Coap_Session_Hdl_t*, const char*, const char*, payload_type_t, const void*, size_t);
int coap_cleanup(qapi_Coap_Session_Hdl_t*);
int coap_send_get_block_message(qapi_Coap_Session_Hdl_t*, const char*, qapi_Coap_Transaction_Callback_t);
void print_coap_connection_status(int result_code);

/* Aware endpoint URL info */
typedef struct
{
  char scheme[10];
  char host[128];
  int port;
  char path[64];
} url_info;

/**
 * @brief Parses the fully qualified domain name
 * 
 * @param url_string The URL string
 * @return success or failure
**/
int parse_url(char *url_string, url_info*);

extern char* base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);
#endif