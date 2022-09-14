#include "aware_log.h"
#include "qapi_fs.h"

#define AWARE_HEALTH_CHECK_LOG_PATH  "/datatx/health_check.log"

int aware_log_to_file(char*);

#ifdef ENABLE_UART_DEBUG

void uart_init(QT_UART_CONF_PARA *uart_conf);
void uart_print(QT_UART_CONF_PARA *uart_conf, char *buff, uint16_t len);
void uart_recv(QT_UART_CONF_PARA *uart_conf);
void debug_uart_init(void);

QT_UART_CONF_PARA uart1_conf =
{
	NULL,
	QAPI_UART_PORT_001_E,
	tx_buff,
	sizeof(tx_buff),
	rx_buff,
	sizeof(rx_buff),
	115200
};

static void uart_rx_cb(uint32_t num_bytes, void *cb_data);
static void uart_tx_cb(uint32_t num_bytes, void *cb_data);

/**************************************************************************
*                                 FUNCTION
***************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief    Initialization of uart.
  @param    uart_conf   Structure variable for accessing the uart config parameters
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void uart_init(QT_UART_CONF_PARA *uart_conf)
{
	qapi_Status_t status;
	qapi_UART_Open_Config_t uart_cfg;

	uart_cfg.baud_Rate			= uart_conf->baudrate;
	uart_cfg.enable_Flow_Ctrl	= QAPI_FCTL_OFF_E;
	uart_cfg.bits_Per_Char		= QAPI_UART_8_BITS_PER_CHAR_E;
	uart_cfg.enable_Loopback 	= 0;
	uart_cfg.num_Stop_Bits		= QAPI_UART_1_0_STOP_BITS_E;
	uart_cfg.parity_Mode 		= QAPI_UART_NO_PARITY_E;
	uart_cfg.rx_CB_ISR			= (qapi_UART_Callback_Fn_t)&uart_rx_cb;
	uart_cfg.tx_CB_ISR			= (qapi_UART_Callback_Fn_t)&uart_tx_cb;;
	uart_cfg.user_mode_client   =1;

	status = qapi_UART_Open(&uart_conf->hdlr, uart_conf->port_id, &uart_cfg);

	status = qapi_UART_Power_On(uart_conf->hdlr);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Start a uart transmit action.
  @param    uart_conf   Structure variable for accessing the uart config parameters
  @param	buff		Buffer pointer to config parameter
  @param	len			Length of the parameter that has to transmit
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void uart_print(QT_UART_CONF_PARA *uart_conf, char *buff, uint16_t len)
{
	qapi_Status_t status;
	status = qapi_UART_Transmit(uart_conf->hdlr, buff, len, NULL);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Start a uart receive action.
  @param    uart_conf   Structure variable for accessing the uart config parameters
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void uart_recv(QT_UART_CONF_PARA *uart_conf)
{
	qapi_Status_t status;
	status = qapi_UART_Receive(uart_conf->hdlr, uart_conf->rx_buff, uart_conf->rx_len, (void*)uart_conf);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    uart rx callback handler.
  @param    cb_data   Structure variable for accessing the uart config parameters
  @param	num_bytes Number of bytes that has to be received
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void uart_rx_cb(uint32_t num_bytes, void *cb_data)
{
	QT_UART_CONF_PARA *uart_conf = (QT_UART_CONF_PARA*)cb_data;

	if(num_bytes == 0)
	{
		uart_recv(uart_conf);
		return;
	}
	else if(num_bytes >= uart_conf->rx_len)
	{
		num_bytes = uart_conf->rx_len;
	}

	uart_recv(uart_conf);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    uart tx callback handler.
  @param    cb_data   Structure variable for accessing the uart config parameters
  @param	num_bytes Number of bytes that has to be transmit
  @return   Void
 */
/*--------------------------------------------------------------------------*/
static void uart_tx_cb(uint32_t num_bytes, void *cb_data)
{
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Initialization debug uart.
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void debug_uart_init(void)
{
	/* uart 1 init */
	uart_init(&uart1_conf);
	/* start uart 1 receive */
	uart_recv(&uart1_conf);
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Output the debug log.
  @param    uart_hdlr   Structure variable for accessing the uart parameters
  @param	level		Message level
  @param	fmt			Pointer to the message string
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void uart_dbg(qapi_UART_Handle_t uart_hdlr, int level, const char* fmt, ...)
{
	char log_buf[256] = {0};

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(log_buf, sizeof(log_buf), fmt, ap);
	va_end( ap );

	if(level == MSG_LEGACY_HIGH)
		qapi_UART_Transmit(uart_hdlr, "[Info]", strlen("[Info]"), NULL);
	else if(level == MSG_LEGACY_MED)
		qapi_UART_Transmit(uart_hdlr, "[Debug]", strlen("[Debug]"), NULL);
	else if(level == MSG_LEGACY_ERROR)
		qapi_UART_Transmit(uart_hdlr, "[Error]", strlen("[Error]"), NULL);
	else if(level == MSG_LEGACY_FATAL)
		qapi_UART_Transmit(uart_hdlr, "[Fatal]", strlen("[Fatal]"), NULL);

    qapi_UART_Transmit(uart_hdlr, log_buf, strlen(log_buf), NULL);
    qapi_UART_Transmit(uart_hdlr, "\r\n", strlen("\r\n"), NULL);
    qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
}
#else


/*-------------------------------------------------------------------------*/
/**
  @brief    Output log.
  @param    buf_ptr   Buffer pointer to the message string
  @param	buf_size  Buffer size that has to be print
  @param	fmt		  Pointer to the message string
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void dam_format_log_msg
(
	char *buf_ptr,
	int buf_size,
	char *fmt,
	...
)
{
	va_list ap;
	/*-----------------------------------------------------------------------*/
	/* validate input param */
	if( NULL == buf_ptr || buf_size <= 0)
	{
		return;
	}
	/*-----------------------------------------------------------------------*/

	va_start( ap, fmt );
	vsnprintf( buf_ptr, (size_t)buf_size, fmt, ap );
	va_end( ap );

} /* dam_format_log_msg */

#endif


/*-------------------------------------------------------------------------*/
/**
  @brief    Aware log initialization
  @param    level   Log verbose level
  @return   Success or failure
 */
/*--------------------------------------------------------------------------*/
int aware_log_init(int level)
{
aware_log_set_verbose_level(level);

#ifdef ENABLE_UART_DEBUG
	debug_uart_init();
#endif

	return 0;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Sets log verbose level
  @param    level   Log level input
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void aware_log_set_verbose_level(int level)
{
	//TODO: Add support for logging level
}


/*-------------------------------------------------------------------------*/
/**
  @brief     Write log to an EFS file
  @param    buffer   The buffer to be logged into the file
  @return   0 on Success or -1 on failure
 */
/*--------------------------------------------------------------------------*/
int aware_log_to_file(char* buffer)
{
	int ret = QAPI_ERROR;
	int fd = -1;
	uint32_t bytes_written;
	int buf_size = strlen(buffer);

	ret = qapi_FS_Open_With_Mode(AWARE_HEALTH_CHECK_LOG_PATH, QAPI_FS_O_RDWR_E | QAPI_FS_O_CREAT_E | QAPI_FS_O_APPEND_E, QAPI_FS_S_IWUSR_E, &fd);
	if(ret != QAPI_OK)
	{
		return -1;
	}

	ret = qapi_FS_Write(fd, buffer, buf_size, &bytes_written);
	if(ret != QAPI_OK)
	{
		qapi_FS_Close(fd);
		return -1;
	}

	qapi_FS_Close(fd);

	return 0;
}