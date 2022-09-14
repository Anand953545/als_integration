#ifndef AWARE_UTILS_H_
#define AWARE_UTILS_H_

#include "qapi_timer.h"
#include "qapi_fs.h"

qapi_TIMER_handle_t start_app_timer(uint32_t initial_delay, void* sigs_func_ptr);
void stop_app_timer(qapi_TIMER_handle_t timer_handle);
int restart_app_timer(qapi_TIMER_handle_t timer_handle, uint32_t timeout);
int aware_atoi(const char *str);
size_t aware_int_to_str(int64_t data,	uint8_t* string, size_t length);
size_t aware_float_to_str(double data, uint8_t* string, size_t length);
char * strtok2(char *str, const char *delim);

uint64_t get_current_time_in_milliseconds(void);
int get_battery_level(int* battery_level);
int get_free_space_in_EFS(struct qapi_FS_Statvfs_Type_s* file_statvfs);

#endif