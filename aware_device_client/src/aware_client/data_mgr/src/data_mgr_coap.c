#include "qapi_fs.h"
#undef true
#undef false
#include "qapi_dss.h"
#include "qapi_dnsc.h"
#include "qapi_ns_utils.h"
#include "data_mgr_dss.h"
#include "data_mgr_coap.h"
#include "aware_log.h"
#include "stringl.h"

#include <ctype.h>

#include "txm_module.h"
#include "data_mgr_queue.h"
#include "app_utils_misc.h"
#include "aware_app.h"
#include "aware_main_thread.h"
#include "aware_app_state.h"

/*******************************************************************************
 * DATA_MGR coap security files
 * *****************************************************************************/
#define MAX_CERT_NAME_LEN QAPI_NET_SSL_MAX_CERT_NAME_LEN 

#define DTLS_COAP_CLIENT_PSK        "/datatx/coap_client.psk"
#define DTLS_COAP_CLIENT_CRT        "/datatx/coap_cert.crt"
#define DTLS_COAP_CLIENT_KEY        "/datatx/coap_key.key"

#define DTLS_COAP_CLIENT_PSK_BIN    "coap_key.bin"
#define DTLS_COAP_CLIENT_CERT_BIN   "coap_cert.bin"

#ifdef QAPI_TXM_MODULE
TX_EVENT_FLAGS_GROUP *coap_signal_handle;
#else
TX_EVENT_FLAGS_GROUP _coap_signal_handle; 
TX_EVENT_FLAGS_GROUP *coap_signal_handle = &_coap_signal_handle;
#endif

url_info coap_url_info;

enum coap_events
{
    COAP_MSG_ACK = 0x01
};

int is_scheme_char(int);

extern qapi_DSS_Hndl_t data_mgr_dss_handle;

uint32_t received_sigs = 0;

uint8_t *cert_Buf = NULL; 
uint32_t cert_Size = 0;
uint8_t *key_Buf = NULL; 
uint32_t key_Size = 0;

int coap_configure_connection(qapi_Coap_Connection_Cfg_t * cfg);
void coap_response_message_cb(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Transaction_t * transaction, qapi_Coap_Packet_t* message);
int32_t coap_session_cb(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Packet_t* coap_message, void* clientData);

int coap_message_init(qapi_Coap_Session_Hdl_t*, qapi_Coap_Packet_t*, qapi_Coap_Method_t);
int coap_message_add_payload(qapi_Coap_Session_Hdl_t*, qapi_Coap_Packet_t*, payload_type_t payload_type, const void* payload, size_t payload_len);
int coap_configure_ssl(qapi_Net_SSL_Config_t* ssl_config,	qapi_Net_SSL_Protocol_t protocol, uint8_t ssl_mode);
int read_from_efs(const char *name, void **buffer_ptr, size_t *buffer_size);
int coap_destroy_session(qapi_Coap_Session_Hdl_t*);
int coap_close_connection(qapi_Coap_Session_Hdl_t*);
int coap_message_add_content_type(qapi_Coap_Session_Hdl_t*, qapi_Coap_Packet_t*, int);
int coap_message_add_uri_path(qapi_Coap_Session_Hdl_t*, qapi_Coap_Packet_t*, const char*);
int coap_message_add_query_string(qapi_Coap_Session_Hdl_t*, qapi_Coap_Packet_t*, char*);

int is_scheme_char(int c)
{
    return (!isalpha(c) && '+' != c && '-' != c && '.' != c) ? 0 : 1;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    URL parsing
  @param    url_string   url string
  @param	purl	Structure to store url info
  @return   0 on success, -1 on failure
 */
/*--------------------------------------------------------------------------*/
int parse_url(char *url_string, url_info* purl)
{
	int scheme_flag = 0;
    char *tmpstr;
    char *curstr;
    int len;

    if ( NULL == purl ) 
    {
        return -1;
    }

    curstr = url_string;
    
//	/* Read scheme */
//    tmpstr = strchr(curstr, ':');
//    /* Get the scheme length */
//    len = tmpstr - curstr;
//    /* Check restrictions */
//    for ( int i = 0; i < len; i++ ) {
//        scheme_flag = !is_scheme_char(curstr[i])? 0:1;
//    }
//	if(scheme_flag)
//	{
//		strlcpy(purl->scheme, curstr, len);
//		purl->scheme[len] = '\0';
//	}
//    /* Skip ':' */
//    tmpstr++;
//    curstr = tmpstr;

    tmpstr = curstr;
    while ( '\0' != *tmpstr )
    {
		if ( ':' == *tmpstr || '/' == *tmpstr ) 
		{
			/* Port number is specified. */
			break;
		}
        tmpstr++;
    }
    len = tmpstr - curstr;
	len = len + 1;

    (void)strlcpy(purl->host, curstr, len);
    purl->host[len] = '\0';
    curstr = tmpstr;

	if ( ':' == *curstr )
	{
        curstr++;
        /* Read port number */
        tmpstr = curstr;
        while ( '\0' != *tmpstr && '/' != *tmpstr )
        {
           tmpstr++;
        }
        len = tmpstr - curstr;
        purl->port = atoi(curstr);
        curstr = tmpstr;
    }
    
	if ( '\0' == *curstr ) 
	{
        return 0;
    }
    
    if ( '/' != *curstr ) {
        return -1;
    }
    //curstr++;

    tmpstr = curstr;
    while ( '\0' != *tmpstr && '#' != *tmpstr  && '?' != *tmpstr )
    {
        tmpstr++;
    }
    len = tmpstr - curstr;
	len = len + 1;
    strlcpy(purl->path, curstr, len);
    purl->path[len] = '\0';
    curstr = tmpstr;

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Creates the coap session
  @param    coap_session_handle Coap Session Handle
  @return   SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_create_session(qapi_Coap_Session_Hdl_t* coap_session_handle)
{
	if(!coap_signal_handle)
	{
#ifdef QAPI_TXM_MODULE
		if(TX_SUCCESS != txm_module_object_allocate((void **)&coap_signal_handle, sizeof(TX_EVENT_FLAGS_GROUP))) 
		{
			return -1;
		}
#endif
		/* Initialize coap event flags */
		tx_event_flags_create(coap_signal_handle, "coap_events"); 

		/*Clearing signals*/
		tx_event_flags_set(coap_signal_handle, 0x0, TX_AND);
	}

    int result = -1;
	qapi_Coap_Session_Info_t session_params = {0};

	session_params.coap_transaction_timeout=QAPI_COAP_RESPONSE_TIMEOUT; 
	session_params.coap_max_retransmits=QAPI_COAP_MAX_RETRANSMIT;
	session_params.coap_default_maxage=QAPI_COAP_DEFAULT_MAX_AGE;
	session_params.coap_ack_random_factor=QAPI_COAP_ACK_RANDOM_FACTOR;
	session_params.coap_max_latency=100;
	session_params.clientData=NULL;
	session_params.cb = coap_session_cb;

    if(QAPI_OK != qapi_Coap_Create_Session(&(*coap_session_handle), &session_params))
    {
        LOG_ERROR("[data_mgr]: coap create session failed\n");
        return result;
    }

#ifdef QAPI_TXM_MODULE
    result = qapi_Coap_Pass_Pool_Ptr(*coap_session_handle, app_utils_get_byte_pool());
    if(QAPI_OK != result)
    {
        LOG_ERROR("[data_mgr]: coap pass pool ptr failed\n");
        return result;
    }
#endif

    return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Creates the coap connection
  @param    coap_session_handle Coap Session Handle
  @return   SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_create_connection(qapi_Coap_Session_Hdl_t* coap_session_handle)
{
    int result = -1;
	qapi_Coap_Connection_Cfg_t connection_params = {0};

	result = coap_configure_connection(&connection_params);
    if(QAPI_OK != result)
    {
		return result;
	}

    /* Connect to CoAP server */
    result = qapi_Coap_Create_Connection(*coap_session_handle, &connection_params);
    if(QAPI_OK != result)
    {
		print_coap_connection_status(result);
        return result;
    }

    return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Closes the coap session
  @param    coap_session_handle Coap Session Handle
  @return   SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_destroy_session(qapi_Coap_Session_Hdl_t* coap_session_handle)
{
	int result = qapi_Coap_Destroy_Session(*coap_session_handle);

    return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Closes the coap connection
  @param    coap_session_handle Coap Session Handle
  @return   SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_close_connection(qapi_Coap_Session_Hdl_t* coap_session_handle)
{
	int result = qapi_Coap_Close_Connection(*coap_session_handle);

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Releases the coap session, connection handle
  @param    coap_session_handle Coap Session Handle
  @return   SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_cleanup(qapi_Coap_Session_Hdl_t* coap_session_handle)
{
	int status = 0;
	/* Initialize coap event flags */
    //tx_event_flags_delete(coap_signal_handle); 

	status = coap_close_connection(coap_session_handle);
    if(status != QAPI_OK)
    {
      LOG_INFO("[data_mgr]: coap_close_connection failed\n");
    }

	status = coap_destroy_session(coap_session_handle);
    if(status != QAPI_OK)
    {
      LOG_INFO("[data_mgr]: coap_destroy_session failed\n");
    }

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Configures coap ssl parameters
  @param 	ssl_config SSL Configuration
  @param 	protocol SSL Protocol (TLS/DTLS)
  @param 	ssl_mode Security Mode (PSK/PKI)
  @return   Void
 */
/*--------------------------------------------------------------------------*/
int coap_configure_ssl
(
	qapi_Net_SSL_Config_t*  ssl_config, 
	qapi_Net_SSL_Protocol_t protocol, 
	uint8_t                 ssl_mode
)
{
	int status = QAPI_OK;

	memset(ssl_config, 0, sizeof(qapi_Net_SSL_Config_t));

	if(ssl_mode == 1)
	{
		/* No Sec Authentication */
	}
	/* a. chose cipher suites, applicatoin need chose cipher sutes per requirement */
	else if(ssl_mode == 2)
	{
		ssl_config->cipher[0] = QAPI_NET_TLS_PSK_WITH_AES_128_CCM_8;
		ssl_config->cipher[1] = QAPI_NET_TLS_PSK_WITH_AES_128_CBC_SHA;
		ssl_config->cipher[2] = QAPI_NET_TLS_PSK_WITH_AES_256_CBC_SHA;
		ssl_config->cipher[3] = QAPI_NET_TLS_PSK_WITH_AES_128_GCM_SHA256;
		ssl_config->cipher[4] = QAPI_NET_TLS_PSK_WITH_AES_256_GCM_SHA384;
		ssl_config->cipher[5] = QAPI_NET_TLS_PSK_WITH_AES_128_CBC_SHA256;
		ssl_config->cipher[6] = QAPI_NET_TLS_PSK_WITH_AES_256_CBC_SHA384;
	}
	else if(ssl_mode == 3)
	{
		ssl_config->cipher[0] = QAPI_NET_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256;
		ssl_config->cipher[1] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384;
		ssl_config->cipher[2] = QAPI_NET_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256;
		ssl_config->cipher[3] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384;
	}
	else {
		status = QAPI_ERROR;
	}

	/* b. set protocol, TLS1.2 or DTLS1.2 */
	if(protocol == QAPI_NET_SSL_DTLS_E)
		ssl_config->protocol  = QAPI_NET_SSL_PROTOCOL_DTLS_1_2;
	else
		ssl_config->protocol  = QAPI_NET_SSL_PROTOCOL_TLS_1_2;

	/* c. set verify policy */
	ssl_config->verify.domain        = FALSE; /* verify certificate commonName against the peer's domain name. */
	ssl_config->verify.send_Alert    = TRUE; /* let low layer to send a fatal alert on detection of an untrusted certificate. */
	ssl_config->verify.match_Name[0] = '\0'; /* don't check the common name or altDNSNames of the certificate*/  
	/* set verify certificate time validity, 
	if set as TRUE, need ensure that UE time is be set correctly
	(in case GSM, network may not sync the network time to UE, 
	then UE time must be set by AT commands or some other API before setup SSL connect)*/
	ssl_config->verify.time_Validity = FALSE; /*normally it should be set as TRUE;*/

	/* d. set max max fragement negotion (RFC 6066)*/
	ssl_config->max_Frag_Len             = 512;
	ssl_config->max_Frag_Len_Neg_Disable = TRUE;

	/* e. set SNI Name */
	ssl_config->sni_Name_Size = 0;
	ssl_config->sni_Name      = 0;

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Configures coap connection parameters
  @param 	cfg CoAP Connection Configuration
  @return   Void
 */
/*--------------------------------------------------------------------------*/
int coap_configure_connection(qapi_Coap_Connection_Cfg_t* cfg)
{
	int result = QAPI_OK;
	qapi_Net_SSL_Config_t ssl_config;

	LOG_INFO("[data_mgr]: coap_configure_connection: start");

	if(!cfg)
	{
		LOG_ERROR("[data_mgr]: coap_configure_connection: null cfg passed\n");
		return -1;
	}

	if(app_ctx.dhc_cfg.security_method == 1)
	{
		cfg->sec_Mode =	QAPI_COAP_MODE_NONE;
		cfg->sec_info = NULL;
		cfg->ssl_cfg = NULL;
		cfg->dst_port = 5683;

		LOG_INFO("[data_mgr]: coap_dam_config mode: No auth");
	}
	else if(app_ctx.dhc_cfg.security_method == 2)
	{
		/* Initialize the ssl config */
		result = coap_configure_ssl(&ssl_config, QAPI_NET_SSL_DTLS_E, 2);
		
		qapi_Net_SSL_Cert_Info_t cert_info;	
		qapi_Coap_Sec_Info_t sec_info;

		memset(&cert_info, 0, sizeof(cert_info));
		memset(&sec_info, 0, sizeof(sec_info));

		/* Read PSK information */
		result = read_from_efs(DTLS_COAP_CLIENT_PSK, (void **)&key_Buf, (size_t *)&key_Size);
		LOG_INFO("[data_mgr]: Read %s, result %d\r\n", DTLS_COAP_CLIENT_PSK, result);

		if (QAPI_OK != result) 
		{
			LOG_ERROR("[data_mgr]: Reading PSK information from EFS failed!!");
			result = QAPI_ERROR;
		}

		/* Update the client psk information */
		cert_info.cert_Type = QAPI_NET_SSL_PSK_TABLE_E; 
		cert_info.info.psk_Tbl.psk_Buf = key_Buf;
		cert_info.info.psk_Tbl.psk_Size = key_Size;

		result = qapi_Net_SSL_Cert_Convert_And_Store(&cert_info, DTLS_COAP_CLIENT_PSK_BIN);
	    if (QAPI_OK != result) 
        {
             LOG_ERROR("[data_mgr]: convert/store for %s failed.", DTLS_COAP_CLIENT_PSK_BIN);
             result = QAPI_ERROR;
        }
		else
		{
        	LOG_INFO("[data_mgr]: %s qapi_Net_SSL_Cert_Convert_And_Store: %d", DTLS_COAP_CLIENT_PSK_BIN, result);
		}

		//assign the psk bin location
		sec_info.psk = (qapi_Net_SSL_PSKTable_t)DTLS_COAP_CLIENT_PSK_BIN;

		cfg->sec_Mode =	QAPI_COAP_MODE_PSK;
		cfg->sec_info = &sec_info;
		cfg->ssl_cfg = &ssl_config;
		cfg->dst_port = 5684;
		LOG_INFO("[data_mgr]: coap config mode: PSK");
	}
	else if(app_ctx.dhc_cfg.security_method == 3)
	{
		/*    initialize the ssl config		*/
		coap_configure_ssl(&ssl_config, QAPI_NET_SSL_DTLS_E, 3);

		qapi_Net_SSL_Cert_Info_t cert_info;
		qapi_Coap_Sec_Info_t sec_info;
	
  		memset(&cert_info, 0, sizeof(cert_info));
		memset(&sec_info, 0, sizeof(sec_info));

		/* Read PKI certificate information */
		result = read_from_efs(DTLS_COAP_CLIENT_CRT, (void **)&cert_Buf, (size_t *)&cert_Size);
		LOG_INFO("[data_mgr]: Read %s, result %d", DTLS_COAP_CLIENT_CRT, result);

		if (QAPI_OK != result) {
			LOG_ERROR("[data_mgr]: ERROR: Reading certificate information from EFS failed!!");
			result = QAPI_ERROR;
		}

        /* Read the client key information */
        result = read_from_efs((char *)DTLS_COAP_CLIENT_KEY, (void **)&key_Buf, (size_t *)&key_Size);
        LOG_INFO("[data_mgr]: Read %s, result %d", DTLS_COAP_CLIENT_KEY, result);
        if (QAPI_OK != result) {
             LOG_ERROR("[data_mgr]: ERROR: Reading key information from EFS failed!!");
             result = QAPI_ERROR;
        }

        /* Update the client certificate information */
		cert_info.cert_Type = QAPI_NET_SSL_CERTIFICATE_E;
		cert_info.info.cert.cert_Size = cert_Size;
		cert_info.info.cert.cert_Buf  = cert_Buf;
		cert_info.info.cert.key_Size  = key_Size;
		cert_info.info.cert.key_Buf   = key_Buf;

        /* Convert and store the certificate */ 
        result = qapi_Net_SSL_Cert_Convert_And_Store(&cert_info, DTLS_COAP_CLIENT_CERT_BIN);
        if (QAPI_OK != result) 
        {
			LOG_ERROR("[data_mgr]: convert/store for %s failed.", DTLS_COAP_CLIENT_CERT_BIN);
			result = QAPI_ERROR;
        }
		else
		{
        	LOG_INFO("[data_mgr]: %s qapi_Net_SSL_Cert_Convert_And_Store: %d", DTLS_COAP_CLIENT_CERT_BIN, result);
		}

		//assign the certficate bin location
		sec_info.cert = (qapi_Net_SSL_Cert_t)DTLS_COAP_CLIENT_CERT_BIN;

		cfg->sec_Mode =	QAPI_COAP_MODE_CERTIFICATE;
		cfg->sec_info = &sec_info;
		cfg->ssl_cfg = &ssl_config;
		cfg->dst_port = 5684;
		LOG_INFO("[data_mgr]: coap config mode: PKI");
	} else {
		LOG_ERROR("[data_mgr]: coap_configure_connection: invalid security authentication method\n");
		result = QAPI_ERROR;
	}

	cfg->proto = QAPI_COAP_PROTOCOL_UDP;
	cfg->resumption_enabled = FALSE;
	cfg->session_resumption_timeout = 0;

    char coap_server_ip[16] = {0};

	if(is_valid_ip_address(app_ctx.dhc_cfg.aware_url))
	{
		LOG_INFO("[data_mgr]: coap server IP: %s", app_ctx.dhc_cfg.aware_url);
		cfg->dst_host = app_ctx.dhc_cfg.aware_url;
	}
	else
	{
		LOG_INFO("[data_mgr]: Aware Service Discovery");
		LOG_INFO("[data_mgr]: coap server endpoint: %s", app_ctx.dhc_cfg.aware_url);
		parse_url(app_ctx.dhc_cfg.aware_url, &coap_url_info);
		get_ip_from_url(data_mgr_dss_handle, coap_url_info.host, coap_server_ip);
		LOG_INFO("[data_mgr]: coap server IP: %s", coap_server_ip);
		LOG_INFO("[data_mgr]: ***********************");
		if(is_valid_ip_address(coap_server_ip))
			cfg->dst_host = coap_server_ip;
		else
			cfg->dst_host = "";
	}
	
	cfg->src_host = NULL;
	cfg->src_port = 0;

	cfg->family_type = AF_INET;
	cfg->callHndl = NULL;

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Coap session callback function
  @param 	hdl CoAP Session Handle
  @param 	coap_message CoAP message
  @param 	clientData Client Data
  @return   SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int32_t coap_session_cb(
	qapi_Coap_Session_Hdl_t hdl,
	qapi_Coap_Packet_t * coap_message,
	void * clientData
	)
{
	uint8_t uriPath[15];
	uint8_t uri_path_len = 0;
	qapi_Multi_Option_t* path;
	uint8_t message_code = 0;
	qapi_Coap_Message_Type_t message_type = QAPI_COAP_TYPE_CON;
	qapi_Coap_Status_t coap_response_code = QAPI_NO_ERROR;
	qapi_Coap_Status_t result = QAPI_NO_ERROR;

	qapi_Coap_Packet_t *response = {0};
	qapi_Coap_Message_Params_t message_param = {0};

	if (hdl == NULL||  coap_message == NULL ) 
	{ 
		LOG_ERROR("[data_mgr]: passed argument is NULL\n" );
		coap_response_code =QAPI_COAP_IGNORE;
		return coap_response_code;
	}
	else
	{
		LOG_INFO("[data_mgr]: coapMessage->type %d" , coap_message->type);
		LOG_INFO("[data_mgr]: coapMessage->code %d" , coap_message->code);
	}

	message_type =  coap_message->type;
	switch(message_type)
	{
	case QAPI_COAP_TYPE_CON :
	case QAPI_COAP_TYPE_NON :

		message_code = coap_message->code;
		
		switch (message_code)
		{
		case QAPI_COAP_GET : 
			LOG_INFO("[data_mgr]: Got Get request from server\n");
			LOG_INFO("[data_mgr]: Payload in recieved response %s\n", coap_message->payload);
			LOG_INFO("[data_mgr]: Payload length in recieved response %d\n", coap_message->payload_len);
			LOG_INFO("[data_mgr]: uri path in recieved response %s\n", coap_message->uri_path->data);
			LOG_INFO("[data_mgr]: uri path length in recieved response %d\n", coap_message->uri_path->len);
			break;
		case QAPI_COAP_POST :
			LOG_INFO("[data_mgr]: Got post request from server\n");
			LOG_INFO("[data_mgr]: Payload in recieved response %s\n", coap_message->payload);
			LOG_INFO("[data_mgr]: Payload length in recieved response %d\n", coap_message->payload_len);
			LOG_INFO("[data_mgr]: uri path in recieved response %s\n", coap_message->uri_path->data);
			LOG_INFO("[data_mgr]: uri path length in recieved response %d\n", coap_message->uri_path->len);
			break;
		case QAPI_COAP_PUT :
			LOG_INFO("[data_mgr]: Got PUT request from server\n");
			LOG_INFO("[data_mgr]: Payload in recieved response %s\n", coap_message->payload);
			LOG_INFO("[data_mgr]: Payload length in recieved response %d\n", coap_message->payload_len);
			LOG_INFO("[data_mgr]: uri path in recieved response %s\n", uriPath);
			LOG_INFO("[data_mgr]: uri path length in recieved response %d\n", uri_path_len);
			break;
		case QAPI_COAP_DELETE :
			LOG_INFO("[data_mgr]: Got delete request from server\n");
			break;
			
		case QAPI_CREATED_2_01 :
		case QAPI_DELETED_2_02 :
		case QAPI_VALID_2_03   :
		case QAPI_CHANGED_2_04 :
		case QAPI_CONTENT_2_05 :
		case QAPI_CONTINUE_2_31 :
			break;
			
		case QAPI_BAD_REQUEST_4_00 :
		case QAPI_UNAUTHORIZED_4_01 :
		case QAPI_BAD_OPTION_4_02 :
		case QAPI_FORBIDDEN_4_03 :
		case QAPI_NOT_FOUND_4_04 :
		case QAPI_METHOD_NOT_ALLOWED_4_05 :
		case QAPI_NOT_ACCEPTABLE_4_06 :
		case QAPI_PRECONDITION_FAILED_4_12 :
		case QAPI_REQUEST_ENTITY_TOO_LARGE_4_13 :
		case QAPI_UNSUPPORTED_MEDIA_TYPE_4_15 :
			LOG_INFO("[data_mgr]: Got response with client error response code \n");
			break;
			
		case QAPI_INTERNAL_SERVER_ERROR_5_00 :
		case QAPI_NOT_IMPLEMENTED_5_01 :
		case QAPI_BAD_GATEWAY_5_02 :
		case QAPI_SERVICE_UNAVAILABLE_5_03 :
		case QAPI_GATEWAY_TIMEOUT_5_04 :
		case QAPI_PROXYING_NOT_SUPPORTED_5_05 :
		case QAPI_MEMORY_ALLOCATION_ERROR :
			LOG_INFO("[data_mgr]: Got response with server error response code \n");
			break;
			
		default : 
			LOG_INFO("[data_mgr]: Got unsupported coap message code from server\n");
			coap_response_code = QAPI_COAP_IGNORE;
		}
		
		break;
	case QAPI_COAP_TYPE_ACK :
		LOG_INFO("[data_mgr]: message type : ACK , message code : %u\n",coap_message->code);
		break;
	case QAPI_COAP_TYPE_RST :
		LOG_INFO("[data_mgr]: message type : RST , message code : %u\n",coap_message->code);
		break;
		default : 
		LOG_INFO("[data_mgr]: Message code unknown\n");
		coap_response_code =QAPI_COAP_IGNORE;
	}

end:
	if(coap_message != NULL && message_type != QAPI_COAP_TYPE_RST)
	{
		if (qapi_Coap_Free_Message(hdl , coap_message) != QAPI_OK)
		{
			LOG_INFO("[data_mgr]: coap_session_cb : qapi_Coap_Free_Message returned error\n" );
			coap_response_code =QAPI_COAP_IGNORE;
		}
	}
	
	return coap_response_code;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Transaction callback funtion for CoAP Req/Response .
			This Callback gets called when we recieve ACK for CON messages
  @param 	hdl CoAP Session Handle
  @param 	transaction CoAP transaction handle
  @param 	message CoAP message
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void coap_response_message_cb(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Transaction_t * transaction,
qapi_Coap_Packet_t* message)
{
	qapi_Coap_Transaction_t * transact;
	qapi_Coap_Packet_t * coapMessage = NULL;
	qapi_Coap_Status_t result;

	if((hdl == NULL) || (transaction == NULL))	{
		LOG_ERROR("[data_mgr]: hdl == NULL) || (transaction == NULL)\n" );
		return ;
	}

	transact = (qapi_Coap_Transaction_t *)transaction;
	coapMessage = (qapi_Coap_Packet_t *)message;

	/* set COAP ACK flag */
    data_mgr_signal_set(coap_signal_handle, COAP_MSG_ACK);

	LOG_INFO("[data_mgr]: Reply recevied from Server");

	if ((NULL == coapMessage || QAPI_FORBIDDEN_4_03 == coapMessage->code || QAPI_BAD_REQUEST_4_00 == coapMessage->code )) {
		if (coapMessage == NULL) {
			LOG_ERROR("[data_mgr]: recevied NULL response from Server");
		} else {
			LOG_ERROR("[data_mgr]: Received 403 forbidden reponse");
		}
	} else { 
		if (NULL != coapMessage && QAPI_COAP_TYPE_RST != coapMessage->type)
		{
			char * payload = app_utils_mem_alloc(coapMessage->payload_len+1);

			strlcpy(payload, (char*)coapMessage->payload, coapMessage->payload_len+1);

			LOG_INFO("[data_mgr]: Received NON null and non rst reponse from server");
			LOG_INFO("[data_mgr]: coapMessage->type %d" , coapMessage->type);
			LOG_INFO("[data_mgr]: coapMessage->code %d" , coapMessage->code);
			LOG_INFO("[data_mgr]: transact->ack_received %d" , transact->ack_received);
			LOG_INFO("[data_mgr]: transact->mID %lu" , transact->mID);
			LOG_INFO("[data_mgr]: coapMessage->payload_len %d" , coapMessage->payload_len );
			LOG_INFO("[data_mgr]: coapMessage->payload %s" , payload);

			if(coapMessage->payload_len > 22)
			{
				aware_main_thread_send_msg_config_update(payload, coapMessage->payload_len);
			}

			app_utils_mem_free(payload);
		}
		else
		{
			LOG_ERROR("[data_mgr]: Received QAPI_COAP_TYPE_RST message");
			return;
		}
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief Reads coap connection security information from psk/pki files
  @param name File to be read from device EFS file system
  @param buffer_ptr buffer to copy the file contents
  @param buffer_size content size
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int read_from_efs(const char *name, void **buffer_ptr, size_t *buffer_size)
{
	int bytes_read;
	int efs_fd = -1;
	struct qapi_FS_Stat_Type_s stat; 
	uint8 *file_buf = NULL;
	stat.st_size = 0;

	if ((!name) || (!buffer_ptr) ||(!buffer_size))
	{
		LOG_ERROR("[data_mgr]: Reading SSL from EFS file failed!\n");
		return QAPI_ERROR;
	}

	if (qapi_FS_Open(name, QAPI_FS_O_RDONLY_E, &efs_fd) < 0)
	{
		LOG_ERROR("[data_mgr]: Opening EFS file %s failed\n", name);
		return QAPI_ERROR;
	}

	if (qapi_FS_Stat_With_Handle(efs_fd, &stat) < 0) 
	{
		LOG_ERROR("[data_mgr]: Getting EFS file size failed\n");
		return QAPI_ERROR;
	}

	LOG_INFO("[data_mgr]: Reading EFS file size %d \n", stat.st_size);
	
	tx_byte_allocate(app_utils_get_byte_pool(), (VOID **) &file_buf, stat.st_size, TX_NO_WAIT);
	if (file_buf  == NULL) 
	{
		LOG_ERROR("[data_mgr]: SSL_cert_store: QAPI_ERR_NO_MEMORY \n");
		return QAPI_ERR_NO_MEMORY;
	}

	qapi_FS_Read(efs_fd, file_buf, stat.st_size, &bytes_read);
	if ((bytes_read < 0) || (bytes_read != stat.st_size)) 
	{
		tx_byte_release(file_buf);
		LOG_ERROR("[data_mgr]: SSL_cert_store: Reading EFS file error\n");
		return QAPI_ERROR;
	}

	*buffer_ptr = file_buf;
	*buffer_size = stat.st_size;

	qapi_FS_Close(efs_fd);
	//qapi_FS_Unlink(name);

	return QAPI_OK;
}

int coap_message_add_block2_header(qapi_Coap_Session_Hdl_t* coap_session_handle, qapi_Coap_Packet_t* coap_message, int pkt_number)
{
	int result = QAPI_ERROR;

	qapi_Coap_Block_Params_t params;

	params.num = pkt_number;
	params.more = 0;
	params.size = 024;	/* Set blocksize you want in downlink */
	
	result = qapi_Coap_Set_Header(coap_session_handle, coap_message, QAPI_COAP_BLOCK2, &params,sizeof(qapi_Coap_Block_Params_t));
	if(QAPI_OK != result) {
		LOG_ERROR("[data_mgr]: coap message add query string header failed");
	}

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief This function adds a custom coap header attributes based on input parameters
  @param coap_session_handle CoAP Session Handle
  @param coap_message CoAP message
  @param header_string CoAP header parameter
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_message_add_query_string(qapi_Coap_Session_Hdl_t* coap_session_handle, qapi_Coap_Packet_t* coap_message, char* query_string)
{
	int result = QAPI_ERROR;
	int query_length = strlen(query_string);

	result = qapi_Coap_Set_Header(coap_session_handle, coap_message, QAPI_COAP_URI_QUERY, (void *)query_string, query_length);

	if(QAPI_OK != result) {
		LOG_ERROR("[data_mgr]: coap message add query string failed");
	}
	else
	{
		LOG_INFO("[data_mgr]: coap message add query string success");
	}

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief This function adds uri path coap header
  @param coap_session_handle CoAP Session Handle
  @param coap_message CoAP message
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_message_add_uri_path(qapi_Coap_Session_Hdl_t* coap_session_handle, qapi_Coap_Packet_t* coap_message, const char* resource_path)
{
	int result = QAPI_ERROR;
	char* uri_path;
	int uri_length = strlen(resource_path)+1;

	if(uri_length > 1) {
		/* Setting uri_path and uri_length */
		uri_path = (char*)app_utils_mem_alloc(uri_length * sizeof(char));
		if (uri_path == NULL)
		{
			LOG_ERROR("[data_mgr]: Memory allocation failed for uri_path \n");
			return -1;
		}

		memset(uri_path,'\0',uri_length);
		strlcpy(uri_path, resource_path, uri_length);
  		LOG_INFO("[data_mgr]: uri_path: %s uri_length : %d", uri_path, uri_length);

		result = qapi_Coap_Set_Header(coap_session_handle, coap_message, QAPI_COAP_URI_PATH, (void *)uri_path, uri_length);

		if(QAPI_OK != result) {
			LOG_ERROR("[data_mgr]: coap message add uri path failed");
		}
		else
		{
			LOG_INFO("[data_mgr]: coap message add uri path success");
		}

		app_utils_mem_free(uri_path);
	}

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief This function adds content type coap header
  @param coap_session_handle CoAP Session Handle
  @param coap_message CoAP message
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_message_add_content_type(qapi_Coap_Session_Hdl_t* coap_session_handle, qapi_Coap_Packet_t* coap_message, int content_format)
{
	int result = -1;
  	qapi_Coap_Header_type      header_type;

	header_type = QAPI_COAP_CONTENT_TYPE;
	result = qapi_Coap_Set_Header(coap_session_handle, coap_message, header_type, (void *)&content_format, sizeof(int));

	if(QAPI_OK != result)
	{
		LOG_ERROR("[data_mgr]: coap message add content type failed");
	}
	else
	{
		LOG_INFO("[data_mgr]: coap message add content type success");
	}

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Initializes the coap message
  @param coap_session_handle CoAP Session Handle
  @param coap_message CoAP message
  @param method_type Method type
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_message_init(qapi_Coap_Session_Hdl_t* coap_session_handle, qapi_Coap_Packet_t* coap_message, qapi_Coap_Method_t method_type)
{
	int result = QAPI_ERROR;
  	qapi_Coap_Message_Type_t msg_type = QAPI_COAP_TYPE_CON; 

	result = qapi_Coap_Init_Message(coap_session_handle, &coap_message, msg_type, method_type);

	if(QAPI_OK != result){
		LOG_ERROR("[data_mgr]: coap_message_init failed");
	} else {
		LOG_INFO("[data_mgr]: coap_message_init success");
	}

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief This function will add payload to the message handle and returns success of failure status
  @param coap_session_handle CoAP Session Handle
  @param coap_message CoAP message
  @param payload Payload to be added to the message
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_message_add_payload(qapi_Coap_Session_Hdl_t* coap_session_handle, qapi_Coap_Packet_t* coap_message, payload_type_t payload_type, const void* payload, size_t payload_len)
{
	int result = -1;

	if(payload_type == PAYLOAD_TYPE_CBOR_LIKE)
	{
		size_t encoded_size = 0;

		char* encoded_msg = base64_encode((const unsigned char *)payload, payload_len, &encoded_size);
		
		result = qapi_Coap_Set_Payload(coap_session_handle, coap_message, encoded_msg, encoded_size);

		app_utils_mem_free(encoded_msg);
	} else {
		result = qapi_Coap_Set_Payload(coap_session_handle, coap_message, payload, payload_len);
	}

	if(QAPI_OK != result)
	{
		LOG_ERROR("[data_mgr]: coap_message_add_payload failed");
	} else {
		LOG_INFO("[data_mgr]: coap_message_add_payload success");
	}

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief This function publishes the message to coap server and returns success of failure status
  @param coap_session_handle CoAP Session Handle
  @param device_id Device Identifier
  @param event_type Event type(telemetry/alert)
  @param msg Message Payload
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_send_post_message(qapi_Coap_Session_Hdl_t* coap_session_handle, const char* device_id, const char* event_type, payload_type_t payload_type, const void* msg, size_t msg_len)
{
	qapi_Coap_Message_Params_t message_param  	= {0};
	qapi_Coap_Packet_t         *coap_message    = {0};

	int result = QAPI_ERROR;
	uint8* payload = NULL;
	char query_string[128];
	char uri_path[128];

	if(msg_len  > 0) {
		payload = app_utils_mem_alloc(msg_len);
		memset(payload,'\0', msg_len);
		memscpy(payload, msg_len, msg, msg_len);
	}

	/* Clear ACK flag */
	data_mgr_signal_clear(coap_signal_handle, COAP_MSG_ACK);

	result = qapi_Coap_Init_Message(coap_session_handle, &coap_message, QAPI_COAP_TYPE_CON, QAPI_COAP_POST);
	if(QAPI_OK != result) {
		LOG_ERROR("[data_mgr]: coap_send_message failed \n");
		goto error;
	}

	snprintf(uri_path, strlen(coap_url_info.path)+1, "%s", coap_url_info.path);
	result = coap_message_add_uri_path(coap_session_handle, coap_message, uri_path);
	if(QAPI_OK != result) {
		goto error;
	}

	result = coap_message_add_content_type(coap_session_handle, coap_message, QAPI_TEXT_PLAIN);
	if(QAPI_OK != result) {
		goto error;
	}

	if(get_app_state() == APP_STATE_UNPROVISIONED) {
		snprintf(query_string, 20, "tn=onboarding&ct=pb");
	} else {
		if(payload_type == PAYLOAD_TYPE_CBOR_LIKE)
			snprintf(query_string, strlen(device_id) + 21, "id=%s&tn=s.s&ct=BASE64", device_id);
		else
			snprintf(query_string, 13, "tn=s.s&ct=pb");
	}

	LOG_INFO("[data_mgr]: coap message add query string %s", query_string);
	result = coap_message_add_query_string(coap_session_handle, coap_message, query_string);
	if(QAPI_OK != result) {
		goto error;
	}

	message_param.msg_cb = coap_response_message_cb;

	if(msg_len > 0)
	{
		LOG_INFO("[data_mgr]: coap message add payload");
		result = coap_message_add_payload(coap_session_handle, coap_message, payload_type, payload, msg_len);
		if(QAPI_OK != result) {
			goto error;
		}
	}

	LOG_INFO("[data_mgr]: coap send message");

	result = qapi_Coap_Send_Message(coap_session_handle, coap_message, &message_param);
	LOG_DEBUG("[data_mgr]: coap_send_message end with result %d\n", result);
	if(QAPI_OK != result) {
		goto error;
	}

	LOG_INFO("[data_mgr]: waiting for coap_send_message ack\n");
	received_sigs = data_mgr_signal_wait(coap_signal_handle, COAP_MSG_ACK, DM_SIGNAL_ATTR_WAIT_ANY);

	if(received_sigs & COAP_MSG_ACK)
	{
		LOG_INFO("[data_mgr]: received ack for coap post message\n");
	}

error:
	LOG_INFO("[data_mgr]: cleanup coap message and payload");
	app_utils_mem_free(payload);
	qapi_Coap_Free_Message(coap_session_handle, coap_message);
	
	return result;
}


/*-------------------------------------------------------------------------*/
/**
  @brief This function sends a get message to coap server and returns success of failure status
  @param coap_session_handle CoAP Session Handle
  @param device_id Device Identifier
  @param event_type Event type(telemetry/alert)
  @param msg Message Payload
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int coap_send_get_block_message(qapi_Coap_Session_Hdl_t* coap_session_handle, const char* device_id, qapi_Coap_Transaction_Callback_t msg_txn_cb)
{
	qapi_Coap_Message_Params_t message_param  = {0};
	qapi_Coap_Packet_t         *coap_message  = {0};
	int result = QAPI_ERROR;
	char query_string[65];
	char uri_path[128];

	/* Clear ACK flag */
	data_mgr_signal_clear(coap_signal_handle, COAP_MSG_ACK);

	result = qapi_Coap_Init_Message(coap_session_handle, &coap_message, QAPI_COAP_TYPE_CON, QAPI_COAP_GET);
	if(QAPI_OK != result) {
		LOG_ERROR("[data_mgr]: coap_send_message failed");
		goto error;
	}

	LOG_INFO("[data_mgr]: coap message add uri path header");
	result = coap_message_add_uri_path(coap_session_handle, coap_message, coap_url_info.path);
	if(QAPI_OK != result) {
		goto error;
	}

	LOG_INFO("[data_mgr]: coap message add content type header");
	result = coap_message_add_content_type(coap_session_handle, coap_message, QAPI_TEXT_PLAIN);
	if(QAPI_OK != result) {
		goto error;
	}

	snprintf(query_string, strlen(device_id) + 15, "dt=TRACKER&id=%s", device_id);

	LOG_INFO("[data_mgr]: coap message query string %s", query_string);
	result = coap_message_add_query_string(coap_session_handle, coap_message, query_string);
	if(QAPI_OK != result) {
		goto error;
	}

	message_param.msg_cb = msg_txn_cb;

	LOG_INFO("[data_mgr]: coap send message");

	result = qapi_Coap_Send_Message(coap_session_handle, coap_message, &message_param);
	LOG_INFO("[data_mgr]: coap_send_message end with result %d\n", result);
	if(QAPI_OK != result) {
		goto error;
	}

	LOG_INFO("[data_mgr]: waiting for coap_send_message ack");
	received_sigs = data_mgr_signal_wait(coap_signal_handle, COAP_MSG_ACK, DM_SIGNAL_ATTR_WAIT_ANY);

	if(received_sigs & COAP_MSG_ACK) {
		LOG_INFO("[data_mgr]: received ack for coap post message\n");
	}

error:
	qapi_Coap_Free_Message(coap_session_handle, coap_message);

	return result;
}

/**
 * @brief prints the coap connection status description
 * 
 * @param result_code coap connection result input
**/
void print_coap_connection_status(int result_code)
{
	switch(result_code)
	{
		case QAPI_NET_COAP_EINVALID_PARAM:
			LOG_INFO("[data_mgr]: CoAP Connection Failed: QAPI_NET_COAP_EINVALID_PARAM.");
		break;
		case QAPI_NET_COAP_ENOMEM:
			LOG_INFO("[data_mgr]: CoAP Connection Failed: QAPI_NET_COAP_ENOMEM.");
		break;
		case QAPI_NET_COAP_HANDLE_NOT_FOUND:
			LOG_INFO("[data_mgr]: CoAP Connection Failed: QAPI_NET_COAP_HANDLE_NOT_FOUND.");
		break;
		case QAPI_NET_COAP_CONNECTION_CREATION_FAILED:
			LOG_INFO("[data_mgr]: CoAP Connection Failed: QAPI_NET_COAP_CONNECTION_CREATION_FAILED.");
		break;
		case QAPI_NET_COAP_CONNECTION_CREATION_FAILED_TCP_ERROR:
			LOG_INFO("[data_mgr]: CoAP Connection Failed: QAPI_NET_COAP_CONNECTION_CREATION_FAILED_TCP_ERROR.");
		break;
		case QAPI_NET_COAP_CONNECTION_CREATION_FAILED_SSL_ERROR:
			LOG_INFO("[data_mgr]: CoAP Connection Failed: QAPI_NET_COAP_CONNECTION_CREATION_FAILED_SSL_ERROR.");
		break;
		case QAPI_NET_COAP_TRANSACTION_CALLBACK_NOT_SET:
			LOG_INFO("[data_mgr]: CoAP Connection Failed: QAPI_NET_COAP_TRANSACTION_CALLBACK_NOT_SET.");
		break;
	}
}