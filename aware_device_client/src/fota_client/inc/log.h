#ifndef _LOG_H_
#define _LOG_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "qapi_diag.h"
#include "qapi_types.h"
#include "qapi_timer.h"

#ifdef ENABLE_UART_DEBUG
#include "qapi_uart.h"

typedef struct{
	qapi_UART_Handle_t  hdlr;
	qapi_UART_Port_Id_e port_id;
	char *tx_buff;
	uint32_t tx_len;
	char *rx_buff;
	uint32_t rx_len;
	uint32_t baudrate;
} QT_UART_CONF_PARA;

void uart_init(QT_UART_CONF_PARA *uart_conf);
void uart_print(QT_UART_CONF_PARA *uart_conf, char *buff, uint16_t len);
void uart_recv(QT_UART_CONF_PARA *uart_conf);
void qt_uart_dbg(qapi_UART_Handle_t uart_hdlr, const char* fmt, ...);

void debug_uart_init(void);

/* uart rx tx buffer */
static char rx_buff[1024];
static char tx_buff[1024];

extern QT_UART_CONF_PARA uart1_conf;

#define LOG_INFO(...) qt_uart_dbg(uart1_conf.hdlr, __VA_ARGS__)
#define LOG_DEBUG(...) qt_uart_dbg(uart1_conf.hdlr, __VA_ARGS__)
#define LOG_ERROR(...) qt_uart_dbg(uart1_conf.hdlr, __VA_ARGS__)
#define LOG_FATAL(...) qt_uart_dbg(uart1_conf.hdlr, __VA_ARGS__)

#else
#include "qflog_utils.h"

void dam_format_log_msg
(
char *buf_ptr,
int buf_size,
char *fmt,
...
);

/* Maximum length of log message */
#define MAX_DIAG_LOG_MSG_SIZE       512
 
/* Log message to Diag */
#define DAM_LOG_MSG_DIAG( lvl, ... )  \
	{ \
		char log_diag_buf[ MAX_DIAG_LOG_MSG_SIZE ]; \
		\
		/* Format message for logging */  \
		dam_format_log_msg( log_diag_buf, MAX_DIAG_LOG_MSG_SIZE, __VA_ARGS__ );\
		\
		/* Log message to Diag */  \
		QAPI_MSG_SPRINTF( MSG_SSID_LINUX_DATA, lvl, "%s", log_diag_buf );  \
		\
		/* Log message to Terminal */  \
		QFLOG_MSG(MSG_SSID_DFLT, lvl, "%s", log_diag_buf); \
	}

#define LOG_INFO(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_HIGH, __VA_ARGS__)
#define LOG_DEBUG(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_MED, __VA_ARGS__)
#define LOG_ERROR(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_FATAL, __VA_ARGS__)

#endif

#endif