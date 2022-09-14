#include "txm_module.h"
#include "qapi_timer.h"
#include "qapi_socket.h"
#include "aware_log.h"
#include "qapi_ns_utils.h"

/* server IPv4 address */
#define SERVER_ADDRESS "127.0.0.1"
/* tcp server port */
#define TCP_SERVER_PORT 5000
#define CLIENT_TO_SERVER_STR "FOTA DAM update available"

#define htons(s) ((((s) >> 8) & 0xff) | (((s) << 8) & 0xff00))

void send_fota_trigger();


/*-------------------------------------------------------------------------*/
/**
  @brief Send fota trigger
  @return Void
 */
/*--------------------------------------------------------------------------*/
void send_fota_trigger()
{
	UINT status = 0;
	int sock_ds = -1;
	int err_number = 0;
	int rc;
	char str_buf[128];
	struct sockaddr_in server_addr;
	unsigned short port = TCP_SERVER_PORT;
	fd_set rset;

	/*TCP Socket creation*/
	sock_ds = qapi_socket(AF_INET, SOCK_STREAM, 0);
	LOG_INFO("[FOTA Thread]: TCP client socket fd %d\n", sock_ds);

	if (sock_ds == -1)
	{
		LOG_ERROR("[FOTA Thread]: TCP client socket creation error. \n");
		return;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	qapi_setsockopt(sock_ds, SOL_SOCKET, SO_BIO, NULL, 0);

	LOG_INFO("[FOTA Thread]: TCP client calling connect\n");

	rc = qapi_connect(sock_ds, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if (rc == 0)
	{
		LOG_INFO("[FOTA Thread]: TCP client connect successfully.\n");
	}
	else
	{
		err_number = qapi_errno(sock_ds);
		LOG_ERROR("[FOTA Thread]: TCP client connect failed with error (%d)\n", err_number);
	}

	memset(str_buf, 0, sizeof(str_buf));
	strlcpy(str_buf, CLIENT_TO_SERVER_STR, sizeof(str_buf));

	while (1)
	{
		unsigned long set_signal = 0;
		if (qapi_send(sock_ds, str_buf, 128, 0) == -1)
		{
			err_number = qapi_errno(sock_ds);
			LOG_ERROR("[FOTA Thread]: TCP client qapi_send failure(%d)\n", err_number);
			qapi_socketclose(sock_ds);
			break;
		}
		else
		{
			LOG_INFO("[FOTA Thread]: TCP client send successfully: %s\n", str_buf);
			qapi_socketclose(sock_ds);
			break;
		}
	}
}