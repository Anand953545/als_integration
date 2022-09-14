#include "fota_dam_download.h"
#include "stringl.h"
#include "aware_log.h"

#include "qapi_fs.h"
#undef true
#undef false
#include "qapi_dss.h"
#include "qapi_netservices.h"
#include "qapi_httpc.h"
#include "app_utils_misc.h"
#include "aware_utils.h"

#include "qapi_fs_types.h"
#include "qapi_timer.h"
#include "qapi_httpc.h"
#include "fota_dam_swapping.h"
#include "app_context.h"
#include "aware_app_state.h"

qapi_Net_HTTPc_handle_t http_handle = NULL;
TX_EVENT_FLAGS_GROUP *http_release_handle;

static int dl_file_handle = 0;
static boolean file_opened = FALSE;

DSS_Lib_Status_e http_netctl_lib_status = DSS_LIB_STAT_INVALID_E;
extern uint8 http_datacall_status;
static http_session_policy_t http_session_policy;

#define DOWNLOAD_FILE_PATH "/datatx/fota_image/aware_dam_app_v2.bin"

#define HTTP_MAX_DATA_RETRY (3)
#define HTPP_MAX_CONNECT_RETRY (6)

#define RELEASE_FLAG 0x00001
#define RE_DOWNLOAD_FLAG 0x00010

int dam_download(char* host, uint16 port, char* file_name);
int dam_download_process(qapi_Net_HTTPc_handle_t http_handler, char *host, uint16 port, char *file);
void dam_download_release(void);

#ifdef HTTPS_SUPPORT

/* *.pem format */
#define HTTPS_CA_PEM			"/datatx/fota_cacert.pem"
#define HTTPS_CERT_PEM			"/datatx/fota_client_cert.pem"
#define HTTPS_PREKEY_PEM		"/datatx/fota_client_key.pem"

/* *.bin format */
#define HTTPS_CA_BIN			"fota_cacert.bin"
#define HTTPS_CERT_BIN			"fota_clientcert.bin"
#define HTTPS_PREKEY_BIN		"fota_clientkey.bin"

uint32_t ssl_ctx_id = 0;		/* http or https identification */
qapi_Net_SSL_Obj_Hdl_t ssl_obj_hdl = 0;

#endif	/* QUECTEL_HTTPS_SUPPORT */

#ifdef HTTPS_SUPPORT

/*-------------------------------------------------------------------------*/
/**
  @brief Http store read from efs file
  @param name  Name
  @param buffer_ptr Buffer pointer to string
  @param buffer_size Size of the string
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int http_store_read_from_EFS_file(const char *name, void **buffer_ptr, size_t *buffer_size)
{
	int bytes_read;
	int efs_fd = -1;
	struct qapi_FS_Stat_Type_s stat;
	uint8 *file_buf = NULL;
	stat.st_size = 0;

	if ((!name) || (!buffer_ptr) ||(!buffer_size))
	{
		LOG_INFO("Reading SSL from EFS file failed!");
		return QAPI_ERROR;
	}

	if (qapi_FS_Open(name, QAPI_FS_O_RDONLY_E, &efs_fd) < 0)
	{
		LOG_INFO("Opening EFS EFS file %s failed", name);
		return QAPI_ERROR;
	}

	if (qapi_FS_Stat_With_Handle(efs_fd, &stat) < 0) 
	{
		LOG_INFO("Reading EFS file %s failed", name);
		return QAPI_ERROR;
	}

	LOG_INFO("Reading EFS file size %d ", stat.st_size);
	
	tx_byte_allocate(app_utils_get_byte_pool(), (VOID **) &file_buf, stat.st_size, TX_NO_WAIT);
	if (file_buf  == NULL) 
	{
		LOG_INFO("SSL_cert_store: QAPI_ERR_NO_MEMORY ");
		return QAPI_ERR_NO_MEMORY;
	}

	qapi_FS_Read(efs_fd, file_buf, stat.st_size, &bytes_read);
	if ((bytes_read < 0) || (bytes_read != stat.st_size)) 
	{
		tx_byte_release(file_buf);
		LOG_INFO("SSL_cert_store: Reading EFS file error");
		return QAPI_ERROR;
	}

	*buffer_ptr = file_buf;
	*buffer_size = stat.st_size;

	qapi_FS_Close(efs_fd);

	return QAPI_OK;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Http ssl connection object configuration
  @param ssl  Config parameters
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
qapi_Status_t http_ssl_conn_obj_config(SSL_INST *ssl)
{
	qapi_Status_t ret_val = QAPI_OK;
	
	/* default eight encryption suites */
	ssl->config.cipher[0] = QAPI_NET_TLS_RSA_WITH_AES_128_CBC_SHA;
	ssl->config.cipher[1] = QAPI_NET_TLS_RSA_WITH_AES_256_CBC_SHA;
	ssl->config.cipher[2] = QAPI_NET_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA;
	ssl->config.cipher[3] = QAPI_NET_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA;
	ssl->config.cipher[4] = QAPI_NET_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA;
	ssl->config.cipher[5] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA;
	ssl->config.cipher[6] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA;
	ssl->config.cipher[7] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384;
	
	ssl->config.max_Frag_Len = 4096;
	ssl->config.max_Frag_Len_Neg_Disable = 0;
	ssl->config.protocol = QAPI_NET_SSL_PROTOCOL_TLS_1_2;
	ssl->config.verify.domain = 0;
	ssl->config.verify.match_Name[0] = '\0';
	ssl->config.verify.send_Alert = 0;
	ssl->config.verify.time_Validity = 0;	/* Don't check certification expiration */

	return ret_val;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Http ssl verify method
  @param verify_method  Verify method
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int http_ssl_verify_method(ssl_verify_method_e *verify_method)
{
	int ca_fd = -1;
	int cert_fd = -1;
	int key_fd = -1;
	qapi_FS_Status_t fs_stat = QAPI_OK;

	*verify_method = QHTTPS_SSL_NO_METHOD;

	/* check CAList is exist or not */
	fs_stat = qapi_FS_Open(HTTPS_CA_PEM, QAPI_FS_O_RDONLY_E, &ca_fd);
	if ((fs_stat != QAPI_OK) || (ca_fd < 0))
	{
		LOG_INFO("Cannot find or open CAList file in EFS");
		*verify_method = QHTTPS_SSL_NO_METHOD;
	}
	else
	{
		*verify_method = QHTTPS_SSL_ONEWAY_METHOD;
		
		/* check client certificate is exist or not */
		fs_stat = qapi_FS_Open(HTTPS_CERT_PEM, QAPI_FS_O_RDONLY_E, &cert_fd);
		if ((fs_stat != QAPI_OK) || (cert_fd < 0))
		{
			LOG_INFO("Cannot find or open Cert file in EFS");
			*verify_method = QHTTPS_SSL_ONEWAY_METHOD;
		}
		else
		{
			*verify_method = QHTTPS_SSL_TWOWAY_METHOD;
			
			/* if client certificate is exist, client private key must be existed */
			fs_stat = qapi_FS_Open(HTTPS_PREKEY_PEM, QAPI_FS_O_RDONLY_E, &key_fd);
			if ((fs_stat != QAPI_OK) || (key_fd < 0))
			{
				LOG_INFO("Cannot find or open key file in EFS");
				return -1;	//miss client key 
			}
			else
			{
				qapi_FS_Close(key_fd);
			}
			
			qapi_FS_Close(cert_fd);
		}

		qapi_FS_Close(ca_fd);
	}

	return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Http ssl certificate load
  @param ssl Loading parameters
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
qapi_Status_t http_ssl_cert_load(SSL_INST *ssl)
{
	qapi_Status_t result = QAPI_OK;

	int32_t ret_val = 0;

#if 0	
	int cert_data_buf_len;
	char *cert_data_buf;
#endif

    uint8_t *cert_Buf = NULL; 
    uint32_t cert_Size = 0;
	uint8_t *key_Buf = NULL;
	uint32_t key_Size = 0;

//	int32 cert_size = 0;
	qapi_Net_SSL_Cert_Info_t cert_info;
	qapi_NET_SSL_CA_Info_t calist_info[QAPI_NET_SSL_MAX_CA_LIST];

	ssl_verify_method_e verify_method;

	if (ssl_ctx_id)
	{
		LOG_INFO("Start https process");

		if (0 != http_ssl_verify_method(&verify_method))
		{
			LOG_INFO("Miss certificates in EFS, error return");
			return QAPI_ERROR;
		}
		
		switch (verify_method)
		{
			case QHTTPS_SSL_TWOWAY_METHOD:
			{
				/* Read the client certificate information */
			    ret_val = http_store_read_from_EFS_file((char *)HTTPS_CERT_PEM, (void **)&cert_Buf, (size_t *)&cert_Size);
				LOG_INFO("Read %s, result %d", HTTPS_CERT_PEM, ret_val);
			    if (QAPI_OK != ret_val) 
			    {
			         LOG_INFO("ERROR: Reading client certificate from EFS failed!! ");
			         result = QAPI_ERROR;
			    }

			    /* Read the client key information */
			    ret_val = http_store_read_from_EFS_file((char *)HTTPS_PREKEY_PEM, (void **)&key_Buf, (size_t *)&key_Size);
				LOG_INFO("Read %s, result %d", HTTPS_PREKEY_PEM, ret_val);
			    if (QAPI_OK != ret_val) 
			    {
			         LOG_INFO("ERROR: Reading key information from EFS failed!!");
			         result = QAPI_ERROR;
			    }

				/* Update the client certificate information */
				cert_info.cert_Type = QAPI_NET_SSL_CERTIFICATE_E; 
			    cert_info.info.cert.cert_Buf = cert_Buf;
			    cert_info.info.cert.cert_Size = cert_Size;
			    cert_info.info.cert.key_Buf = key_Buf;
			    cert_info.info.cert.key_Size = key_Size;
			    //cert_info.info.cert.pass_Key = https_fota_pass;

				/* Convert and store the certificate */ 
				result = qapi_Net_SSL_Cert_Convert_And_Store(&cert_info, HTTPS_CERT_BIN);
				LOG_INFO("%s qapi_Net_SSL_Cert_Convert_And_Store: %d", HTTPS_CERT_BIN, result);
				
				if (result == QAPI_OK)
				{
					if (qapi_Net_SSL_Cert_Load(ssl->sslCtx, QAPI_NET_SSL_CERTIFICATE_E, HTTPS_CERT_BIN) < 0)
					{
						LOG_INFO("ERROR: Unable to load client cert from FLASH");
						//return QCLI_STATUS_ERROR_E;
					}
				}

				/* continue to load CAList */
			}
			case QHTTPS_SSL_ONEWAY_METHOD:
			{
				/* Store CA List */
				ret_val = http_store_read_from_EFS_file(HTTPS_CA_PEM, (void **)&calist_info[0].ca_Buf, (size_t *)&calist_info[0].ca_Size);
				LOG_INFO("Read %s, result %d", HTTPS_CA_PEM, ret_val);

			    if (QAPI_OK != ret_val) 
			    {
			         LOG_INFO("ERROR: Reading ca information from EFS failed!!");
			         result = QAPI_ERROR;
			    }

				cert_info.info.ca_List.ca_Info[0] = &calist_info[0];
			    cert_info.info.ca_List.ca_Cnt = 1;
				cert_info.cert_Type = QAPI_NET_SSL_CA_LIST_E;
				
				result = qapi_Net_SSL_Cert_Convert_And_Store(&cert_info, HTTPS_CA_BIN);
				//result = qapi_Net_SSL_Cert_Store(HTTPS_CA_BIN, QAPI_NET_SSL_CA_LIST_E, cert_data_buf, cert_data_buf_len);
				LOG_INFO("%s qapi_Net_SSL_Cert_Convert_And_Store: %d", HTTPS_CA_BIN, result);
				
				if (result == QAPI_OK)
				{
					if (qapi_Net_SSL_Cert_Load(ssl->sslCtx, QAPI_NET_SSL_CA_LIST_E, HTTPS_CA_BIN) < 0)
					{
						LOG_INFO("ERROR: Unable to load CA from FLASH");
						//return QCLI_STATUS_ERROR_E;
					}
				}

				break;
			}			
			default:
			{
				LOG_INFO("Don't need to verify certifications");
				break;
			}
		}
	}
	
#if 0	/* not use */	
	/* Store PSK */
	cert_info.cert_Type = QAPI_NET_SSL_PSK_TABLE_E;
	result = qapi_Net_SSL_Cert_Convert_And_Store(&cert_info, HTTPS_PREKEY_BIN);
	LOG_INFO("%s convert and store failed: %d", HTTPS_PREKEY_BIN, result);
	//result = qapi_Net_SSL_Cert_Store(HTTPS_FOTA_CLIENT_KEY, QAPI_NET_SSL_PSK_TABLE_E, cert_data_buf, cert_data_buf_len);
	if (result == QAPI_OK)
	{
		if (qapi_Net_SSL_Cert_Load(ssl->sslCtx, QAPI_NET_SSL_PSK_TABLE_E, HTTPS_PREKEY_BIN) < 0)
		{
			LOG_INFO("ERROR: Unable to load PSK from FLASH");
			//return QCLI_STATUS_ERROR_E;
		}
	}
#endif
	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Http ssl configuration
  @param ssl Configuration parameters
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int http_ssl_config(SSL_INST *ssl)
{
	int ret_val = 0;	
	qapi_Status_t result = QAPI_OK;
	qapi_Net_SSL_Role_t role = QAPI_NET_SSL_CLIENT_E;	/* TLS Client object creation. */

	memset(ssl, 0, sizeof(SSL_INST));	
	ssl->role = role;
	ssl->sslCtx = qapi_Net_SSL_Obj_New(role);
	if (ssl->sslCtx == QAPI_NET_SSL_INVALID_HANDLE)
	{
		LOG_INFO("ERROR: Unable to create SSL context");
		return -1;
	}
	else
	{
		ssl_obj_hdl = ssl->sslCtx;
	}
	LOG_INFO("--->http ssl config: New SSL Object ssl->sslCtx - %lu", ssl->sslCtx);

	/* TLS Configuration of a Connection Object */
	result = http_ssl_conn_obj_config(ssl);
	
	/* 
	 * allocate memory and read the certificate from certificate server or EFS.
	 * Once cert_data_buf filled with valid SSL certificate, Call QAPI to Store and Load 
	 */
	result = http_ssl_cert_load(ssl);
	if (result != QAPI_OK)
	{
		LOG_INFO("http_ssl_cert_load return error");
		ret_val = -1;
	}
	
	return ret_val;
}
#endif	/* HTTPS_SUPPORT */


/*-------------------------------------------------------------------------*/
/**
  @brief Http string copy
  @param dst  Destinationn string
  @param src Source string
  @param siz Length of the string to copy from src to dst
  @return Count of bytes copied to string
 */
/*--------------------------------------------------------------------------*/
int32 http_strlcpy
(
char *dst, 
const char *src,
int32  siz
)
{
	char  *d = dst;
	const char *s = src;
	int32 n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) 
   	{
		while (--n != 0) 
		{
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) 
	{
		if (siz != 0)
			*d = '\0';
	
		while (*s++)
		{
		}
	}

	return(s - src - 1);	/* count does not include NUL */
}


/*-------------------------------------------------------------------------*/
/**
  @brief Decode the URL
  @param url URL which needs to be decoded
  @param url_length Length of the URL
  @param host Pointer to the host address string
  @param port Pointer to the port number string
  @param file_name Poiner to file name
  @return True or False
 */
/*--------------------------------------------------------------------------*/
boolean   decode_url
(
	uint8            *url, 
	uint32           *url_length, 
	char             *host,
	uint16           *port,
	uint8            *file_name
)
{
    uint8      hstr[9];
    uint32    i;
    uint8    *phostnamehead;
    uint8    *phostnameTail;
    boolean      ret = FALSE;
    uint8     url_type;
    uint8   *puri= url;
    uint32   datalen = *url_length;
    *port = HTTP_DEFAULT_PORT;
    do
    {
        memset(hstr,0,9);

        if((*url_length) < 8)
        {
            break;
        }

        memcpy(hstr,url,8);

        for(i=0;i<8;i++)
        {
            hstr[i] = lower((int32)hstr[i]);
        }
        if(strncmp((char *)hstr,(char*)"https://",8) == 0)
        {
            puri = url + 8;
            datalen -= 8;
            url_type = 1;
#ifdef HTTPS_SUPPORT
			ssl_ctx_id = 1;
#endif
        }
        else if(strncmp((char *)hstr,(char*)"http://",7) == 0)
        {
            puri = url+7;
            datalen -= 7;
            url_type = 0;
#ifdef HTTPS_SUPPORT
			ssl_ctx_id = 0;
#endif
        }
        else
        {  
            break;
        }
       
        if(url_type == 1)
        {
            *port = HTTPS_DEFAULT_PORT;
        }
        else
        {
             *port = HTTP_DEFAULT_PORT;  
        }
        i=0;

	 if(puri[i] != '[') //chris: normal url  http://220.180.239.201:80/index.html  or http://www.baidu.com
	 {
	 	 	/* parse host name */
        	phostnamehead = puri;
        	phostnameTail = puri;
		
        	while(i<datalen && puri[i] != '/' && puri[i] != ':')
        	{
            		i++;
            		phostnameTail++;
        	}
	 }
	 else //ipv6 url:http://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]:80/index.html  or http://[1080:0:0:0:8:800:200C:417A]/index.html
	 {
		datalen -= 1;
		puri += 1;
		
	 	phostnamehead = puri;
		phostnameTail = puri;
	 	while(i < datalen && puri[i] != ']')
	 	{
	 		i++;
			phostnameTail++;
	 	}
		i++;
	 }
        hstr[0] = *phostnameTail;
        *phostnameTail = '\0';
        if(strlen((char*)phostnamehead)  == 0 || strlen((char*)phostnamehead) >= HTTP_DOMAIN_NAME_LENGTH)
        {
            //LOG_INFO("%s", __FUNCTION__);
             break;
        }
        http_strlcpy(host,  (char*)phostnamehead, (strlen((char*)phostnamehead)+1));
        
        *phostnameTail = hstr[0];

        /* IP address URL without port */
        if(datalen >= i)
            datalen -= i;
        else
            datalen = 0;
        if(datalen == 0)
        {
            url[0] = '/';
            *url_length = 1;
            ret = TRUE;
            break;
        }

		/* get port */
		puri+=i;
		i = 0;
		
        if(*puri==':')
        {
            datalen -= 1;
            puri++;
            phostnamehead = puri;
            phostnameTail = puri;

            while(*puri !='/' && i<datalen ){
                puri++;
                i++;
            }		
            
            phostnameTail = puri;
            hstr[0] = *phostnameTail;
            *phostnameTail = '\0';

            *port = aware_atoi((char *)phostnamehead);
            *phostnameTail = hstr[0];
        }

        if(datalen >= i)
            datalen -= i;
        else
            datalen = 0;
        if(datalen == 0)
        {
             url[0] = '/';
            *url_length = 1;
            ret = TRUE;
            break;
        }

		/* get resouce URI */
        if(*puri=='/')
        {
            i=0;
            while(i<datalen)
            {
                url[i]=puri[i];
                i++;
            }		
            *url_length= datalen;
            *(url+datalen)='\0';
            http_strlcpy((char*)file_name, (char*)url, strlen((char*)url)+1);
        }
        else
        {
            url[0] = '/';
            *url_length= 1;
        }
        ret = TRUE;
        
    }while(FALSE);
    if(ret == FALSE)
    {
    	 url[0]= '\0';
		*url_length = 0;
		memset((void *)host, 0, HTTP_DOMAIN_NAME_LENGTH);
    }
		
    return ret;

}

void dam_download_session_release(void)
{
	/* stop http client first */
	qapi_Net_HTTPc_Stop();

	if (NULL != http_handle)
	{
		qapi_Net_HTTPc_Disconnect(http_handle);
		qapi_Net_HTTPc_Free_sess(http_handle);
	}
	#ifdef HTTPS_SUPPORT
	if (0 != ssl_obj_hdl)
	{
		qapi_Net_SSL_Obj_Free(ssl_obj_hdl);
	}
    #endif	/* HTTPS_SUPPORT */
}


/*-------------------------------------------------------------------------*/
/**
  @brief Release the dam download session
  @return Void
 */
/*--------------------------------------------------------------------------*/
void dam_download_release(void)
{
	if(file_opened){
    	qapi_FS_Close(dl_file_handle);
		file_opened = FALSE;
	}

	memset(&http_session_policy, 0, sizeof(http_session_policy));
	dam_download_session_release();

	if(http_release_handle) {
		tx_event_flags_delete(http_release_handle);
	}
}


/*-------------------------------------------------------------------------*/
/**
  @brief Write update data to file
  @param handle  File descriptor 
  @param src Buffer data
  @param size Size of the buffer
  @return TRUE or FALSE
 */
/*--------------------------------------------------------------------------*/

boolean write_update_file(int32 handle, char* src, uint32 size)
{
    uint32 len;
    boolean ret_val = FALSE;

	ret_val = qapi_FS_Write(handle, src, size, &len);
	if(size == len && ret_val == QAPI_OK)
	{
        return TRUE;
	}
	else
	{
        return FALSE;
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief Dam download callback
  @return Void
 */
/*--------------------------------------------------------------------------*/

void dam_download_cb(void* arg, int32 state, void* http_resp)
{
	int status=QAPI_OK;
	boolean ret_val = FALSE;
	qapi_Net_HTTPc_Response_t * resp = NULL;

	resp = (qapi_Net_HTTPc_Response_t *)http_resp;
	http_session_policy.session_state = HTTP_SESSION_DOWNLOADING;
	LOG_INFO("[Fota Thread]: dam_download_cb:%x,%ld,len:%lu,code:%lu,%x",arg,state,resp->length,resp->resp_Code,resp->data);

	if( resp->resp_Code >= 200 && resp->resp_Code < 300)
	{
		if(resp->data != NULL && state >= 0)
		{
			http_session_policy.last_pos += resp->length;
			http_session_policy.reason_code = state;
			http_session_policy.data_retry = 0;

			if(file_opened == FALSE)
			{
				ret_val = qapi_FS_Open(DOWNLOAD_FILE_PATH, QAPI_FS_O_WRONLY_E | QAPI_FS_O_CREAT_E | QAPI_FS_O_TRUNC_E, &dl_file_handle);

				if(ret_val == 0)
				{
					LOG_INFO("[Fota Thread]: open %s success",DOWNLOAD_FILE_PATH);
					file_opened = TRUE;
				}  
				else
				{
					LOG_INFO("[Fota Thread]: open failed:%d",ret_val);
				}
			}
			if (file_opened == TRUE)
			{
				ret_val = write_update_file(dl_file_handle, (char*)resp->data, resp->length);
				if(ret_val != TRUE)
				{
					LOG_INFO("[Fota Thread]: write_update_file failed");
				}
			}
			if (state == 0)
			{
				LOG_INFO("[Fota Thread]: download success");
				set_fota_state(FOTA_READY_TO_APPLY);
				
				// status = setup_dam_ota_update_dir();
				// if(status!=QAPI_OK)
				// {
				// 	LOG_INFO("dir not initialised");
				// }
				http_session_policy.session_state = HTTP_SESSION_DL_FIN;
				status = runtime_config_update(&app_ctx.runtime_cfg, RUNTIME_CONFIG_LAST_DEVICE_STATE, DEVICE_STATE_UPGRADE);
				if(status != QAPI_OK)
				{
					LOG_INFO("[Fota Thread]: fota file upgrade flag failed");
				}
				status = swap_dam_file();
				if(status != QAPI_OK)
				{
					LOG_INFO("[Fota Thread]: fota file swapping failed");
				}
			}  
		}
		else if(resp->data == NULL && state == 0)
		{
			http_session_policy.reason_code = state;
			http_session_policy.data_retry = 0;
			LOG_INFO("[Fota Thread]: download success");
			http_session_policy.session_state = HTTP_SESSION_DL_FIN;
			set_fota_state(FOTA_READY_TO_APPLY);
			// status = initial_fota_dir();
			// if(status!=QAPI_OK)
			// {
			// 	LOG_INFO("dir not initialised");
			// }
			status = runtime_config_update(&app_ctx.runtime_cfg, RUNTIME_CONFIG_LAST_DEVICE_STATE, DEVICE_STATE_UPGRADE);
			if(status != QAPI_OK)
			{
				LOG_INFO("[Fota Thread]: fota file upgrade flag failed");
			}
			status = swap_dam_file();
			if(status != QAPI_OK)
			{
				LOG_INFO("[Fota Thread]: fota file swapping failed");
			}
		}
		else if (resp->data == NULL && state > 0)
		{
			//do nothing, wait and reveive next packet
			http_session_policy.data_retry = 0;
			LOG_INFO("no data received, continue...");
			http_session_policy.session_state = HTTP_SESSION_DOWNLOADING;
		}    
		else
		{
			/* data retransmission */
			if (http_session_policy.data_retry++ < HTTP_MAX_DATA_RETRY)
			{
				LOG_INFO("[Fota Thread]: download timeout, try again - %d", http_session_policy.data_retry);
				tx_event_flags_set(http_release_handle, RE_DOWNLOAD_FLAG, TX_OR);
			}
			else
			{
				http_session_policy.session_state = HTTP_SESSION_DL_FAIL;
			}
		}
	}
	else if(resp->resp_Code == 0)
	{
		if (http_session_policy.data_retry++ < HTTP_MAX_DATA_RETRY)
		{
			LOG_INFO("[Fota Thread]: rsp code = 0, try again - %d", http_session_policy.data_retry);
			tx_event_flags_set(http_release_handle, RE_DOWNLOAD_FLAG, TX_OR);
		}
		else
		{
			http_session_policy.session_state = HTTP_SESSION_DL_FAIL;
		}
	}
	else
	{
		/* if response code is not 2xx, don't need try again */
		http_session_policy.session_state = HTTP_SESSION_DL_FAIL;
	}

	if (http_session_policy.session_state != HTTP_SESSION_DOWNLOADING)
	{
		tx_event_flags_set(http_release_handle, RELEASE_FLAG,TX_OR);
	}
}



/*-------------------------------------------------------------------------*/
/**
  @brief Dam download API
  @param host  host name
  @param port Port number
  @param file_name File name
  @return    0: No error
			-1: create SSL context failed
			-2: SSL config failed
			-3: create HTTP session failed
			-4: HTTP connect failed
 			-5: HTPP request failed
 */
/*--------------------------------------------------------------------------*/
int dam_download(char* host, uint16 port, char* file_name)
{
	qapi_Status_t ret = -1;
	uint8 reconnect_count = 0;

#ifdef HTTPS_SUPPORT
	SSL_INST http_ssl;
#endif

	/* Start HTTPc service */
	qapi_Net_HTTPc_Start();
	memset(&http_session_policy, 0, sizeof(http_session_policy_t));
#ifdef HTTPS_SUPPORT
	if (ssl_ctx_id)
	{
		/* config object and certificate */
		if (0 != http_ssl_config(&http_ssl))
		{
			LOG_INFO("ERROR: Config ssl object and certificates error");
		}
		/* @Justice_20190413 qapi_Net_HTTPc_Connect" will internally take care of creating SSL connection 
		* corresponding to HTTP connection. And creating SSL connection using "qapi_Net_SSL_Con_New" which 
		* is not necessary. (refer to TX3.0.1 P8101-35 Doc) */
#if 0
		/* create a SSL session */
		ssl_obj_hdl = http_ssl.sslCtx;
		http_ssl.sslCon = qapi_Net_SSL_Con_New(http_ssl.sslCtx, QAPI_NET_SSL_TLS_E);
		if (http_ssl.sslCon == QAPI_NET_SSL_INVALID_HANDLE)
		{
			LOG_INFO("ERROR: Unable to create SSL context");
			return -1;
		}

		if (QAPI_OK != qapi_Net_SSL_Configure(http_ssl.sslCon, &http_ssl.config))
		{
			LOG_INFO("ERROR: SSL configure failed");
			return -2;
		}
#endif

		http_handle = qapi_Net_HTTPc_New_sess(20000, http_ssl.sslCtx, dam_download_cb, NULL, 1024, 1024);
		ret = qapi_Net_HTTPc_Configure_SSL(http_handle, &http_ssl.config);
		if(ret != QAPI_OK)
		{
			LOG_INFO("Start HTTPS connection, SSL config ret[%d]", ret);
			return -2;
		}
	}
	else
	{
		http_handle = qapi_Net_HTTPc_New_sess(20000, 0, dam_download_cb, NULL, 1024, 1024);
	}
#else
	http_handle = qapi_Net_HTTPc_New_sess(20000, 0, dam_download_cb, NULL, 1024, 1024);
#endif /* HTTPS_SUPPORT */

	http_session_policy.session_state = HTTP_SESSION_INIT;

	if ( http_handle == NULL)
	{
		LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_New_sess ERROR");
		return -3;
	}

	qapi_Net_HTTPc_Pass_Pool_Ptr(http_handle, app_utils_get_byte_pool());
    
	do
	{
		ret = qapi_Net_HTTPc_Connect(http_handle, host, port);
		if (ret != QAPI_OK)
		{
			reconnect_count++;
			LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Connect ERROR ret:%d, re-connect times:%d",ret,reconnect_count);
			if(reconnect_count >= HTPP_MAX_CONNECT_RETRY)
			{
				return -4;
			}
			qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
		}
		else
		{
			LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Connect success :%d", ret);
			break;
		}
	} while (1);

	http_session_policy.session_state = HTTP_SESSION_CONN;

	/* get response content */
	ret = qapi_Net_HTTPc_Request(http_handle, QAPI_NET_HTTP_CLIENT_GET_E, file_name); 

	if (ret != QAPI_OK)
	{
		LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Request ERROR :%d",ret);
		return -5;
	}

	return 0;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Dam download process
  @param http_handler  
  @param host Host name
  @param port Port number
  @param file File name
  @return 0: No error
		 -1: input http handler is null
		 -2: add header field failed
		 -3: HTTP connect failed
		 -4: HTPP request failed
 */
/*--------------------------------------------------------------------------*/
int dam_download_process(qapi_Net_HTTPc_handle_t http_handler, char *host, uint16 port, char *file)
{
    uint8 reconn_counter = 0;
	qapi_Status_t ret = QAPI_ERROR;
	char range_buffer[32] = {0};

	if (http_handler == NULL)
	{
	    LOG_INFO("[Fota Thread]: http_handler is null");
		return -1;
	}
	
	/* Reconnection may failed with network Latency, please try again */
    do
    {
		/* add range filed to re-download data from last breakpoint */
		memset(range_buffer, 0, sizeof(range_buffer));
		http_session_policy.start_pos = http_session_policy.last_pos;
		snprintf(range_buffer, sizeof(range_buffer), "bytes=%lu-", http_session_policy.start_pos);
		
		LOG_INFO("[Fota Thread]: http_download: %s", range_buffer);
		
		ret = qapi_Net_HTTPc_Add_Header_Field(http_handler, "Range", range_buffer);
		if (ret != QAPI_OK)
		{
			LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Add_Header_Field content-type ERROR :%d", ret);
            return -2;
		}

	    ret = qapi_Net_HTTPc_Connect(http_handler, host, port);
	    if (ret != QAPI_OK)
	    {
	        reconn_counter++;
			LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Connect ERROR :%d,reconn_counter:%d",ret, reconn_counter);
			if(reconn_counter >= HTPP_MAX_CONNECT_RETRY)
			{
				return -3;
			}
			qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
	    }
	    else
	    {
	    	LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Connect success :%d", ret);
			break;
	    }

		LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Connect re-content times:%d", reconn_counter);
    } while (1);
    
    /* get response content */
    ret = qapi_Net_HTTPc_Request(http_handler, QAPI_NET_HTTP_CLIENT_GET_E, (char*)file); 
    if (ret != QAPI_OK)
    {
        LOG_INFO("[Fota Thread]: qapi_Net_HTTPc_Request ERROR :%d",ret);
        return -4;
    }

    return 0;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Prepare for dam download
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int prepare_for_dam_download()
{
	int status=QAPI_OK;
	int batt_level=0;
	uint32_t total_free_bytes=0;
	uint32_t free_blocks=0;
	uint32_t block_size=0;
	struct qapi_FS_Statvfs_Type_s s_file_statvfs;

	status = get_battery_level(&batt_level);
	if(status == QAPI_OK)
	{
		if(batt_level > LOW_BATTERY_LEVEL)
		{
			status = QAPI_OK;
			LOG_INFO("[Fota Thread]: Battery level status ok: %d%%",batt_level);
		}else{
			status = QAPI_ERROR;	
			LOG_INFO("[Fota Thread]: Battery level status: bad: %d%%",batt_level);
		}
	} else {

		status = QAPI_ERROR;
		LOG_INFO("[Fota Thread]: Battery not attached");
	}
	
	status = get_free_space_in_EFS(&s_file_statvfs);
	if(status == QAPI_OK)
	{
		//total_free_bytes=(s_file_statvfs.f_bsize * s_file_statvfs.f_bfree);
		free_blocks = s_file_statvfs.f_bfree;
        block_size = s_file_statvfs.f_bsize;
        total_free_bytes = (free_blocks*block_size);	
		LOG_INFO("[Fota Thread]: Total number of free blocks! %ld",free_blocks);
		LOG_INFO("[Fota Thread]: blocks size! %ld",block_size);
		LOG_INFO("[Fota Thread]: Total number of free bytes available on EFS! %ld",total_free_bytes);
	}
	
	return status;
} 


/*-------------------------------------------------------------------------*/
/**
  @brief Start fota dam download
  @param host Host name
  @param port Port number
  @param file_name File name
  @return ESUCCESS or EFAILURE
 */
/*--------------------------------------------------------------------------*/
int start_fota_dam_download(char* host, uint16 port, char* file_name)
{
	uint32 session_pocess_sig = 0;
	int result = 0;
	int status=QAPI_OK;

#ifdef QAPI_TXM_MODULE
	txm_module_object_allocate((void**)&http_release_handle, sizeof(TX_EVENT_FLAGS_GROUP));
#endif
	
	status = setup_dam_ota_update_dir();
	if(status!=QAPI_OK)
	{
		LOG_INFO("[Fota Thread]: dir not initialised");
	}
	
	/* intialize/create the directory*/
	/* check for sufficient battery storage and battery confirm current*/
	if((result = prepare_for_dam_download()) == QAPI_OK)
	{
		LOG_INFO("[Fota Thread]: Battery status and free space ok");
		//return result;
	}
	
	tx_event_flags_create(http_release_handle, "http_release_event");
	tx_event_flags_set(http_release_handle, 0x0, TX_AND);

	set_app_mode(APP_MODE_DOWNLOADING);
	set_fota_state(FOTA_FILE_DOWNLOAD);

	result = dam_download(host, port, file_name);
	if(result < 0)
	{
		dam_download_release();
		return -1;
	}

	while (1)
	{
		tx_event_flags_get(http_release_handle, RELEASE_FLAG | RE_DOWNLOAD_FLAG, TX_OR_CLEAR, &session_pocess_sig, TX_WAIT_FOREVER);
		LOG_INFO("[Fota Thread]: @release_sig [0x%x]", session_pocess_sig);

		if (session_pocess_sig & RELEASE_FLAG)
		{
			dam_download_release();
			break;
		}
		else if (session_pocess_sig & RE_DOWNLOAD_FLAG)
		{
			/* trigger next download process */
			LOG_INFO("[Fota Thread]: Re-download process again, current pos: %d, try - %d", http_session_policy.last_pos, http_session_policy.data_retry);

			result = dam_download_process(http_handle, host, port, file_name);
			if (result < 0)
			{
				dam_download_release();
				break;
			}
		}
	}

	return result;
}