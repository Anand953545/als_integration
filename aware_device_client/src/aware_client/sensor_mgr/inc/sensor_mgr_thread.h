#ifndef __SENSOR_MGR_THREAD_H__
#define __SENSOR_MGR_THREAD_H__

#include <stdint.h>

void sensor_mgr_thread_task_entry(unsigned long args);
char* sensor_mgr_thread_get_task_name(void);
int sensor_mgr_thread_get_task_id(void);

#endif /* __SENSOR_MGR_THREAD_H__ */
