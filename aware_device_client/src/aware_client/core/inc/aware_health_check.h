#ifndef AWARE_HEALTH_CHECK_H_
#define AWARE_HEALTH_CHECK_H_

#include "app_context.h"

#define MAX_LEN 128
#define MAXSTR 128
#define MAXFILESIZE 1024

#define AWARE_HEALTH_CHECK_LOG_PATH "/datatx/hc.log"

int perform_health_check(app_context_t* app_ctx);

int hc_log_to_file(char *buf_ptr);

#endif