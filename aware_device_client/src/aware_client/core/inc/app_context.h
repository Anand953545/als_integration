#ifndef RUNTIME_DATA_H_
#define RUNTIME_DATA_H_

#include <stdint.h>
#include "qapi_types.h"

#include "app_config.h"
#include "device_unique_config.h"
#include "device_general_config.h"
#include "device_common_config.h"
#include "runtime_config.h"
#include "sensor_config.h"

#define ESUCCESS 0
#define EFAILURE -1

#define LOW_BATTERY_LEVEL 20

/**
 * @brief App Mode Bitmask
 * 
**/
typedef enum {
    APP_MODE_CHARGING = 0x0001,
    APP_MODE_TETHERED = 0x0002,
    APP_MODE_LOWPOWER = 0x0004,
    APP_MODE_DOWNLOADING = 0x0008,
    APP_MODE_HEALTHCHECK = 0x0010,
    APP_MODE_FARADAYCAGE = 0x0020
} app_mode_t;

/**
 * @brief Health check error bitmask
 * 
**/
typedef enum {
    INTI_ERROR_NONE = 0,
    INTI_ERROR_NO_AWARE_INFO,
    INTI_ERROR_NO_SECURITY,
    INTI_ERROR_INVALID_CONF_FILES,
    INTI_ERROR_NO_SIM,
    INTI_ERROR_HCFAIL
} init_error_t;

/**
 * @brief Device states
 * 
**/
typedef enum {
    DEVICE_STATE_ERROR = 0,
    DEVICE_STATE_POWERING_UP,
    DEVICE_STATE_WAKING_UP,
    DEVICE_STATE_APP_READY,
    DEVICE_STATE_GOING_TO_SLEEP,
    DEVICE_STATE_POWERING_DOWN,
    DEVICE_STATE_UPGRADE
} device_state_t;

/**
 * @brief Application States
 * 
**/
typedef enum {
    APP_STATE_INACTIVE = 0,
    APP_STATE_UNPROVISIONED,
    APP_STATE_PROVISIONED_BUT_NOT_IN_SERVICE,
    APP_STATE_ACTIVE,
    APP_STATE_ACTIVE_JOURNEY
} app_state_t;

/**
 * @brief Application sub states
 * 
**/
typedef enum {
    ACTIVE_SUB_STATE_DORMANT = 0,
    ACTIVE_SUB_STATE_CHECKING_IN,
    ACTIVE_SUB_STATE_WAITING,
    ACTIVE_SUB_STATE_PROCESSING_ACTIONS,
    ACTIVE_SUB_STATE_FLUSHING,
    ACTIVE_SUB_STATE_GOING_TO_SLEEP
} active_substate_t;

typedef enum{

    FOTA_FILE_DOWNLOAD = 0,
    FOTA_READY_TO_APPLY,
    FOTA_APPLYING,
    FOTA_NORAMAL_OPERATION,
    FOTA_UPGRADE
} fota_state_t;

/**
 * @brief Application sub state bitmask
 * 
**/
typedef enum {
    INPROG_TIMEUPDATE = 0x01,
    INPROG_LOC = 0x02,
} active_substateBM_t;

/**
 * @brief Led States
 * 
**/
typedef enum {
    LED_STATE_ERROR_GENERAL=0,
    LED_STATE_NO_SIM,
    LED_STATE_MISSING_PROV,
    LED_STATE_MISSING_PSK,
    LED_STATE_FS_ERROR,
    LED_STATE_READY_TO_ACTIVATE,
    LED_STATE_HEALTHCHECK,
    LED_STATE_ACTIVATED
} led_state_t;

typedef enum
{
	SENSOR_MGR_SET_CONFIG = 0,
	SENSOR_MGR_GET_CONFIG,
	SENSOR_MGR_GET_SENSOR_DATA,
	SENSOR_MGR_WAIT_FOR_CALLBACK,
	SENSOR_MGR_ENTER_INTO_PSM
} sensor_mgr_state_t;

/**
 * @brief Persisted runtime data
 * 
**/
typedef struct {
    device_state_t last_dev_state;
    app_state_t last_app_state;
    init_error_t init_error;
    uint16_t stayawake_timeout;
    uint8_t current_segment;
    uint64_t last_fixtime_ms;
    uint64_t last_checkin_time_ms;
    uint64_t start_time_ms;
    uint32_t stop_time;
    uint8_t num_checkin;
    uint8_t fence_complete;
    uint8_t loc_already_checked;
    uint8_t trans_already_checked;
    uint8_t sensor_already_checked; 
} persisted_runtime_data_t;

/**
 * @brief Application Runtime data
 * 
**/
typedef struct {
	led_state_t led_state;
    device_state_t dev_state;
    app_state_t app_state;
    active_substate_t app_substate;
    active_substateBM_t app_substate_bm; 
	int location_tech;
    uint16_t device_mode;
    uint16_t current_ramkb;
    uint16_t current_storagekb;
    uint16_t current_periodic_report_backlogsizekb;
    uint16_t current_diagdatasizekb;
    uint16_t current_partialfilesizekb;
    uint32_t uptime_sleep_sec;
    uint32_t uptime_fulldevicepowerup;
    uint16_t ondemand_sensor_reading;
    uint16_t ondemand_battery;
} runtime_data_t;

typedef struct
{
   float als;
   float temperature;
   float humidity;
   float pressure;	
} sensor_data_t;

typedef struct
{
    // Release the buffer if not NULL
    void* dataBufferPtr;
    size_t dataBufferSize;
} gtp_data_t;

/**
 * @brief Application Context
 * 
**/
typedef struct {
    init_error_t init_error;
	device_unique_config_t dhu_cfg;
    device_general_config_t dhg_cfg;
    device_common_config_t dhc_cfg;
    app_config_t app_cfg;
    runtime_config_t runtime_cfg;

    sensor_config_t als_sensor_cfg;
    sensor_config_t temperature_sensor_cfg;
    sensor_config_t pressure_sensor_cfg;
    sensor_config_t humidity_sensor_cfg;
    
    app_mode_t app_mode;

	int app_priority;
    int battery_level;

    device_state_t device_state;
    app_state_t app_state;
    active_substate_t app_substate;
    fota_state_t fota_state;
	sensor_mgr_state_t sensor_mgr_state;
} app_context_t;

int store_aware_app_context_to_persistence(void);
int restore_aware_app_context_from_persistence(void);

#endif