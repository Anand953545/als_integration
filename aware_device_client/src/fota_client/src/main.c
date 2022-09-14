#include "txm_module.h"
#include "qapi_timer.h"
#include "qapi_socket.h"
#include "qapi_ns_utils.h"
#include "log.h"
#include "run_time_config.h"
#include "restore_backup_dam.h"
#include "aware_utils.h"

#define EVENT_THREAD_PRIORITY 180
#define EVENT_THREAD_STACK_SIZE (1024 * 16)

/* server IPv4 address */
#define SERVER_ADDRESS "127.0.0.1"
/* tcp server port */
#define TCP_SERVER_PORT 5000

#define htons(s) ((((s) >> 8) & 0xff) | (((s) << 8) & 0xff00))

//the unit is millisecond
#define CLIENT_WAIT_TIME 1000

TX_BYTE_POOL *byte_pool_task;
#define TASK_BYTE_POOL_SIZE  (32*1024)
char free_memory_task[TASK_BYTE_POOL_SIZE];

/* thread handle */
TX_THREAD *event_thread_handle;
char *event_thread_stack = NULL;

/* TX EVENT_FLAGS handle */
TX_EVENT_FLAGS_GROUP *tx_event_flags_handle = NULL;
qapi_TIMER_handle_t log_timer;
run_time_config_t	run_time_cfg;

#define TIMER_INTERVAL 20
#define TIMER_EXPIRY (0x0001 << 0) //bit 0

#define SERVER_RECV_ERROR		-1
#define SERVER_RECV_OK			 0
#define FOTA_UPGRADE			 6

void fota_client_app_thread(ULONG para);
int stop_fota_client_thread(void);
int fota_client_exit();

int aware_dam_app_start(void)
{
	int ret = -1;
	UINT status = 0;
	ULONG flags_to_get = 0;
	UINT count = 0;

	qapi_Timer_Sleep(TIMER_INTERVAL, QAPI_TIMER_UNIT_SEC, true);

#ifdef ENABLE_UART_DEBUG
	debug_uart_init();
#endif
	
	ret = run_time_config_create(&run_time_cfg);
    if(ret != 0) {
        LOG_ERROR("[fota_client]: app_config_create : failed\n");
        goto exit;
    }
	
	if(run_time_cfg.last_device_state!=FOTA_UPGRADE)
	{
		LOG_INFO("[fota_client], last device state is not fota upgrade,Exit ");	
		goto exit;
	}

	LOG_INFO("[fota_client], last device state :fota upgrade");	
	ret = txm_module_object_allocate((void **)&byte_pool_task, sizeof(TX_BYTE_POOL));
	if (ret != TX_SUCCESS)
	{
		LOG_DEBUG("[fota_client]: txm_module_object_allocate failed, %d", ret);
		goto exit;
	}

	ret = tx_byte_pool_create(byte_pool_task, "task application pool", free_memory_task, TASK_BYTE_POOL_SIZE);
	if (ret != TX_SUCCESS)
	{
		LOG_DEBUG("[fota_client]: tx_byte_pool_create failed, %d", ret);
		fota_client_exit();
		goto exit;
	}

	/* create a new event_flag : event_flags */
	ret = txm_module_object_allocate((void **)&tx_event_flags_handle, sizeof(TX_EVENT_FLAGS_GROUP));
	if (ret != TX_SUCCESS)
	{
		LOG_DEBUG("[fota_client]: txm_module_object_allocate failed, %d", ret);
		fota_client_exit();
		goto exit;
	}
	status = tx_event_flags_create(tx_event_flags_handle, "event_flags");
	if (TX_SUCCESS != status)
	{
		LOG_DEBUG("[fota_client]: tx_event_flags_create failed with status %d", status);
		fota_client_exit();
		goto exit;
	}
	
	/* create a new event task */
	if (TX_SUCCESS != txm_module_object_allocate((void **)&event_thread_handle, sizeof(TX_THREAD)))
	{
		LOG_INFO("[fota_client]: txm_module_object_allocate sub1_thread_handle failed");
		fota_client_exit();
		goto exit;
	}

	ret = tx_byte_allocate(byte_pool_task, (VOID **)&event_thread_stack, EVENT_THREAD_STACK_SIZE, TX_NO_WAIT);
	if (ret != TX_SUCCESS)
	{
		LOG_INFO("[fota_client]: txm_module_object_allocate sub1_thread_stack failed");
		fota_client_exit();
		goto exit;
	}

	LOG_INFO("[fota_client]: Thread created");
	/* create a new task : event */
	ret = tx_thread_create(event_thread_handle,
						   "Event Task Thread",
						   fota_client_app_thread,
						   0,
						   event_thread_stack,
						   EVENT_THREAD_STACK_SIZE,
						   EVENT_THREAD_PRIORITY,
						   EVENT_THREAD_PRIORITY,
						   TX_NO_TIME_SLICE,
						   TX_AUTO_START);

	if (ret != TX_SUCCESS)
	{
		LOG_INFO("[fota_client]: Thread creation failed");
		return ret;
	}

exit:
    return 0;
}

int fota_client_exit()
{
	LOG_INFO("[fota_client]:fota_client_exit");

	//deallocate the byte pool
    txm_module_object_deallocate(byte_pool_task);
    //deallocate the signal handle
	txm_module_object_deallocate(tx_event_flags_handle);

    LOG_INFO("[fota_client]: fota client thread exit\n");

	return 0;
}

void timer_expiry_cb(uint32 userData)
{
	LOG_INFO("[fota_client]: timer expiry");
	restore_backup_dam();
	stop_app_timer(log_timer);
	stop_fota_client_thread();

	//tx_event_flags_set(tx_event_flags_handle, TIMER_EXPIRY, TX_OR);
	return;
}

void fota_client_app_thread(ULONG para)
{
	UINT status = 0;
	ULONG flags_to_get = 0;
	int sock_ds = -1;
	int acc_sock_ds = -1;

	int errno = 0;
	int rc;
	int len;
	char str_buf[128];
	struct sockaddr_in server_addr, client_addr;
	unsigned short port = TCP_SERVER_PORT;
	fd_set rset;
	int res = -1;
	uint8_t client_recv_status_flag=-1;

	/*TCP Socket creation*/
	sock_ds = qapi_socket(AF_INET, SOCK_STREAM, 0);
	LOG_INFO("[fota_client]: TCP client socket fd %d", sock_ds);
	if (sock_ds == -1)
	{
		LOG_ERROR("[fota_client]: TCP client socket creation error.");
		return;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

	if (qapi_bind(sock_ds, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
	{
		LOG_ERROR("[fota_client]: Address binding error");
		return;
	}

	/*Server with passive socket*/

	if (qapi_listen(sock_ds, 5) == -1)
	{
		errno = qapi_errno(sock_ds);
		LOG_ERROR("[fota_client]: Listen call error: %d", errno);
		return;
	}

	memset(&client_addr, 0, sizeof(client_addr));
	LOG_INFO("[fota_client]: TCP server accept is called.");
	tx_event_flags_set(tx_event_flags_handle, 0x0, TX_AND);

	log_timer = start_app_timer(120, &timer_expiry_cb);

	
	acc_sock_ds = qapi_accept(sock_ds, (struct sockaddr *)&client_addr, &len);
	if (acc_sock_ds == -1)
	{
		LOG_INFO("[fota_client]: Accept failed.");
		qapi_socketclose(sock_ds);
		return;
	}
	else
	{
		LOG_INFO("[fota_client]: Accept Successfull, accept socket is %d", acc_sock_ds);
	}
	
	while (1)
	{

		qapi_fd_zero(&rset);
		qapi_fd_set(acc_sock_ds, &rset);
		res = qapi_select(&rset, NULL, NULL, CLIENT_WAIT_TIME);
		if (res > 0)
		{
			if (1 == qapi_fd_isset(acc_sock_ds, &rset))
			{
				memset(str_buf, 0, sizeof(str_buf));
				rc = qapi_recv(acc_sock_ds, str_buf, 128, 0);
				if(rc>0)
				{
					LOG_INFO("[fota_client]: TCP client recv successfully:.%s", str_buf);
					if (!qapi_fd_clr(acc_sock_ds, &rset))
					{
						LOG_INFO("[fota_client]: TCP client qapi_fd_clr successfully.");
					}
					LOG_INFO("[fota_client]: socket closed");
					qapi_socketclose(sock_ds);
					 
					stop_fota_client_thread();
					break;	
				}
			}
		}
	}

	qapi_socketclose(sock_ds);
}


int stop_fota_client_thread(void)
{
    LOG_INFO("[fota_client]: stop fota client thread");
	stop_app_timer(log_timer);

	tx_thread_terminate(event_thread_handle);
    tx_thread_delete(event_thread_handle);

	txm_module_object_deallocate(byte_pool_task);
    //deallocate the signal handle
	txm_module_object_deallocate(tx_event_flags_handle);

    LOG_INFO("[fota_client]: fota client thread exit");

    return 0;
}
