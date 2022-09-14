/*
 * @file data_mgr_dss.c
 * @brief DATA_MGR Datacall Handling
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "data_mgr_dss.h"
#include "qapi_socket.h"
#include "qapi_ns_gen_v4.h" 

#include "data_mgr.h"
#include "aware_log.h"
#include "stringl.h"
#include "aware_app.h"
#include "app_utils_misc.h"

#define 	DSS_ADDR_INFO_SIZE	  	5
#define 	GET_ADDR_INFO_MIN(a, b) ((a) > (b) ? (b) : (a))

/*-------------------------------------------------------------------------*/
/**
  @brief Utility interface to translate ip from "int" to x.x.x.x format.
  @param inaddr IPv4 address to be converted
  @param buf Buffer to hold the converted address
  @param buflen Length of buffer 
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int32_t tcp_inet_ntoa
(
	const qapi_DSS_Addr_t    inaddr, /* IPv4 address to be converted         */
	uint8_t                  *buf,    /* Buffer to hold the converted address */
	int32_t                  buflen  /* Length of buffer                     */
)
{
	uint8_t *paddr  = (uint8_t *)&inaddr.addr.v4;
	int32_t  rc = 0;

	if ((NULL == buf) || (0 >= buflen))
	{
		rc = -1;
	}
	else
	{
		if (-1 == snprintf((char*)buf, (unsigned int)buflen, "%d.%d.%d.%d",
							paddr[0],
							paddr[1],
							paddr[2],
							paddr[3]))
		{
			rc = -1;
		}
	}

	return rc;
}

/*-------------------------------------------------------------------------*/
/**
  @brief DNS Parse
  @param hostname Dns name
  @param ipaddr Socket IPv4/IPv6 internet address union 
  @param iface interface type
  @param resolved_ip Seperated IP
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
void dns_parser(const char *hostname, struct ip46addr *ipaddr, const char *iface, char* resolved_ip)
{
	int e = 0;
	char ip_str[48];

	LOG_INFO("[data_mgr]: hostname: %s is resolving in interface: %s", hostname, iface);

	e = qapi_Net_DNSc_Reshost_on_iface(hostname, ipaddr, iface);
    LOG_INFO("[data_mgr]: reshost ret: %d", e);
    
	if (e)
	{
		LOG_ERROR("[data_mgr]: Unable to resolve %s\n", hostname);
	}
	else
	{
		const char* ip_address = inet_ntop(ipaddr->type, &ipaddr->a, ip_str, sizeof(ip_str));
		LOG_INFO("[data_mgr]: %s --> %s", hostname, ip_address);
		strlcpy(resolved_ip, ip_address, 16);
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief Get IP address from the url
  @param dss_hndl 
  @param hostname URL Address
  @param resolved_ip_address IP address
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int get_ip_from_url(qapi_DSS_Hndl_t dss_hndl, const char *hostname, char* resolved_ip_address)
{
	int result, i, count = 0;

	unsigned int len = 0;
	qapi_Status_t status;
	char iface[INET_ADDRSTRLEN] = {0};
	char addr_v4[INET_ADDRSTRLEN] = {0};
	char submsk_v4[INET_ADDRSTRLEN] = {0};
	char gtwy_v4[INET_ADDRSTRLEN] = {0};

	char first_dns[INET_ADDRSTRLEN] = {0};
	char second_dns[INET_ADDRSTRLEN] = {0};
	qapi_DSS_Addr_Info_t info_ptr[DSS_ADDR_INFO_SIZE];

	struct ip46addr ipaddr; /* Resolved IP Addr */

	memset(&ipaddr, 0, sizeof(ipaddr));
	ipaddr.type = AF_INET;

	/* Get DNS server address */
	status = qapi_DSS_Get_IP_Addr_Count(dss_hndl, &len);
	if (QAPI_ERROR == status)
	{
		LOG_ERROR("[data_mgr]: Get IP address count error\n");
		return status;
	}
		
	status = qapi_DSS_Get_IP_Addr(dss_hndl, info_ptr, len);
	if (QAPI_ERROR == status)
	{
		LOG_ERROR("[data_mgr]: Get IP address error\n");
		return status;
	}
	
	if(info_ptr->iface_addr_s.valid_addr)
	{
		const char* local_ip_address = inet_ntop(AF_INET, &info_ptr->iface_addr_s.addr.v4, addr_v4, sizeof(addr_v4));
		strlcpy(coap_local_ip, local_ip_address, 16);
		
		LOG_INFO("[data_mgr]:IP address is %s", local_ip_address);
		LOG_INFO("[data_mgr]:subnet mask address %s", inet_ntop(AF_INET,&info_ptr->iface_mask, submsk_v4, sizeof(submsk_v4)));
		LOG_INFO("[data_mgr]:gateway address %s\n", inet_ntop(AF_INET,&info_ptr->gtwy_addr_s.addr.v4, gtwy_v4, sizeof(gtwy_v4)));
	}
	else
	{
		LOG_ERROR("data_mgr:IPv4 address is not valid");
	}
		
	count = GET_ADDR_INFO_MIN(len, DSS_ADDR_INFO_SIZE);
	for (i = 0; i < count; i++)
	{
		memset(first_dns, 0, sizeof(first_dns));
		tcp_inet_ntoa(info_ptr[i].dnsp_addr_s, (uint8_t*)first_dns, INET_ADDRSTRLEN);
		LOG_INFO("[data_mgr]: Primary DNS IP: %s", first_dns);

		memset(second_dns, 0, sizeof(second_dns));
		tcp_inet_ntoa(info_ptr[i].dnss_addr_s, (uint8_t*)second_dns, INET_ADDRSTRLEN);
		LOG_INFO("[data_mgr]: Second DNS IP: %s", second_dns);
	}

    if(!qapi_Net_DNSc_Is_Started())
    {
		LOG_INFO("[data_mgr]: starting DNS client.........");
		/* Start DNS service */
		result = qapi_Net_DNSc_Command(QAPI_NET_DNS_START_E);
		if(QAPI_DSS_SUCCESS != result)
		{
			LOG_ERROR("[data_mgr]: DAM_APP:qapi_Net_DNSc_Command(start) fail \n");
			return result;
		}
	}

	/* Get current active iface */
	memset(iface, 0, sizeof(iface));
	status = qapi_DSS_Get_Device_Name(dss_hndl, iface, 15);
	LOG_INFO("[data_mgr]: device_name: %s", iface);

	// /* Add dns server into corresponding interface */
	result = qapi_Net_DNSc_Add_Server_on_iface(first_dns, QAPI_NET_DNS_V4_PRIMARY_SERVER_ID, iface);
	result = qapi_Net_DNSc_Add_Server_on_iface(second_dns, QAPI_NET_DNS_V4_SECONDARY_SERVER_ID, iface);

	/* URL parser */
	dns_parser(hostname, &ipaddr, iface, resolved_ip_address);
	
	LOG_INFO("[data_mgr]: stopping DNS client.........");
	result = qapi_Net_DNSc_Command(QAPI_NET_DNS_STOP_E);

	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Checks given IP address is valid or not
  @param server_ip IPv4 address to be checked
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int is_valid_ip_address(char *server_ip)
{
	int ret;
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, server_ip, &(sa.sin_addr));

	ret = (result == 0)?1:0;

  	return ret;
}