#include "log.h"

#ifdef ENABLE_UART_DEBUG

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
/*
@func
  uart_init
@brief
  [in] 	uart_handler
       	The handler which used to handle associated uart.
  [in] 	uart_port_id
       	The uart port id which customer want to use.
  [in] 	baud_rate
       	The uart port communication baud_rate.  
*/
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

/*
@func
  uart_print
@brief
  Start a uart transmit action.
*/
void uart_print(QT_UART_CONF_PARA *uart_conf, char *buff, uint16_t len)
{
	qapi_Status_t status;
	status = qapi_UART_Transmit(uart_conf->hdlr, buff, len, NULL);
}

/*
@func
  uart_recv
@brief
  Start a uart receive action.
*/
void uart_recv(QT_UART_CONF_PARA *uart_conf)
{
	qapi_Status_t status;
	status = qapi_UART_Receive(uart_conf->hdlr, uart_conf->rx_buff, uart_conf->rx_len, (void*)uart_conf);
}

/*
@func
  uart_rx_cb
@brief
  uart rx callback handler.
*/
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

/*
@func
  uart_tx_cb
@brief
  uart tx callback handler.
*/
static void uart_tx_cb(uint32_t num_bytes, void *cb_data)
{
}

void debug_uart_init(void)
{
#ifdef ENABLE_UART_DEBUG

	/* uart 1 init */
	uart_init(&uart1_conf);

	/* start uart 1 receive */
	uart_recv(&uart1_conf);

#endif
}

/*
@func
  qt_uart_dbg
@brief
  Output the debug log, for example.
*/
void qt_uart_dbg(qapi_UART_Handle_t uart_hdlr, const char* fmt, ...)
{
	char log_buf[256] = {0};

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(log_buf, sizeof(log_buf), fmt, ap);
	va_end( ap );

    qapi_UART_Transmit(uart_hdlr, log_buf, strlen(log_buf), NULL);
    qapi_UART_Transmit(uart_hdlr, "\r\n", strlen("\r\n"), NULL);
    qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
}
#else

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