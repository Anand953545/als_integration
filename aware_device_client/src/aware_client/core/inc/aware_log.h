#ifndef _AWARE_LOG_H_
#define _AWARE_LOG_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "qapi_diag.h"
#include "qapi_types.h"
#include "qapi_timer.h"

typedef enum 
{
    AWARE_LOG_OFF = 0,
	AWARE_LOG_FATAL,
	AWARE_LOG_ERROR,
    AWARE_LOG_INFO,
    AWARE_LOG_DEBUG
} aware_log_level;

static int g_log_level = AWARE_LOG_DEBUG;

int aware_log_init(int level);
void aware_log_set_verbose_level(int level);

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

void uart_dbg(qapi_UART_Handle_t uart_hdlr, int level, const char* fmt, ...);

/* uart rx tx buffer */
static char rx_buff[1024];
static char tx_buff[1024];

extern QT_UART_CONF_PARA uart1_conf;

#ifdef ENABLE_ALL_LOGS
	#define LOG_FATAL(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_FATAL, __VA_ARGS__)
	#define LOG_ERROR(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_ERROR, __VA_ARGS__)
	#define LOG_DEBUG(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_MED, __VA_ARGS__)
	#define LOG_INFO(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_HIGH, __VA_ARGS__)
	#define LOG_HEALTH_CHECK(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_LOW, __VA_ARGS__)
#else
	#ifdef ENABLE_LOG_INFO
	#define LOG_INFO(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_HIGH, __VA_ARGS__)
	#else
	#define LOG_INFO(...)
	#endif

	#ifdef ENABLE_LOG_DEBUG
	#define LOG_DEBUG(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_MED, __VA_ARGS__)
	#else
	#define LOG_DEBUG(...)
	#endif

	#ifdef ENABLE_LOG_ERROR
	#define LOG_ERROR(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_ERROR, __VA_ARGS__)
	#else
	#define LOG_ERROR(...)
	#endif

	#ifdef ENABLE_LOG_FATAL
	#define LOG_FATAL(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_FATAL, __VA_ARGS__)
	#else
	#define LOG_FATAL(...)
	#endif

	#ifdef ENABLE_LOG_HEALTH_CHECK
	#define LOG_HEALTH_CHECK(...) uart_dbg(uart1_conf.hdlr, MSG_LEGACY_LOW, __VA_ARGS__)
	#else
	#define LOG_HEALTH_CHECK(...)
	#endif
#endif

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

#ifdef ENABLE_ALL_LOGS
	#define LOG_FATAL(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_FATAL, __VA_ARGS__)
	#define LOG_ERROR(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_ERROR, __VA_ARGS__)
	#define LOG_DEBUG(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_MED, __VA_ARGS__)
	#define LOG_INFO(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_HIGH, __VA_ARGS__)
	#define LOG_HEALTH_CHECK(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_LOW, __VA_ARGS__)
#else
	#ifdef ENABLE_LOG_INFO
	#define LOG_INFO(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_HIGH, __VA_ARGS__)
	#else
	#define LOG_INFO(...)
	#endif

	#ifdef ENABLE_LOG_DEBUG
	#define LOG_DEBUG(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_MED, __VA_ARGS__)
	#else
	#define LOG_DEBUG(...)
	#endif

	#ifdef ENABLE_LOG_ERROR
	#define LOG_ERROR(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_ERROR, __VA_ARGS__)
	#else
	#define LOG_ERROR(...)
	#endif

	#ifdef ENABLE_LOG_FATAL
	#define LOG_FATAL(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_FATAL, __VA_ARGS__)
	#else
	#define LOG_FATAL(...)
	#endif

	#ifdef ENABLE_LOG_HEALTH_CHECK
	#define LOG_HEALTH_CHECK(...) DAM_LOG_MSG_DIAG(MSG_LEGACY_LOW, __VA_ARGS__)
	#else
	#define LOG_HEALTH_CHECK(...)
	#endif
#endif

#endif

#endif