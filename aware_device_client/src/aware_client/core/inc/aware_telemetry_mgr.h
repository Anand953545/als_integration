#ifndef AWARE__TELEMETRY_MGR_H
#define AWARE_TELEMETRY_MGR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* client initiated message*/

#define DEVICE_ACK_MSGID                0x0000
#define PERIODIC_CHECKIN_MSGID          0x0001
#define PERIODIC_CHECKIN_2_MSGID        0x0002
#define GEOFENCE_TRANSITION_MSGID       0x0010
#define STATE_CHANGE_MSGID              0x0020
#define REPORT_CONFIG_DATA_MSGID        0x0030

/* periodic APP mode */
#define APP_MODE_CHARGING               0x0001
#define APP_MODE_TETHERED               0x0002
#define APP_MODE_LOW_POWER              0x0004
#define APP_MODE_DOWNLOADING            0x0008
#define APP_MODE_HEALTHCHECK            0x0010
#define APP_MODE_FARADAYCAGE            0x0020

/* sensor data */
typedef enum{

    SENSOR_EVENT=0,
    SENSOR_ALS,
    SENSOR_PRESSURE,
    SENSOR_HUMIDIY,
    SENSOR_TEMPERATURE

} sensor_data_type_t;
/* TBD */
//#define REPORT_DATA_BLOB_MSGID
//#define REPORT_VITALS_MSGID
//#define UPGRADE_UPDATE_MSGID

typedef enum{
    LOC_GTP_1_0=0,
    LOC_GTP_2_0,
    LOC_WIFI,
    LOC_GNSS,
    LOC_CACHE

} location_tech_enum_t;

typedef enum{
    NETENV_FARADAY_CERTAIN =0,
    NETENV_FARADAY_POSSIBLE,
    NETENV_FLIGHT,
    NETENV_MARITIME,
    NETENV_CATM1,
    NETENV_NBIOT,
    NETENV_GPRS


} netenv_enum_t;

typedef enum{
    DEV_ACK_NACK_GENERIC=0,
    DEV_NACK_UNKNOWN_MSGID,
    DEV_NACK_CANNOT_SUPPORT,
    DEV_NACK_CANNOT_PARSE,
    DEV_NACK_OUT_OF_RAM,
    DEV_NACK_OUT_OF_FS,
    DEV_NACK_POWERING_DOWN

} dev_response_enum_t;



typedef struct{

    char device_id[32];
    uint16_t messageid;
    uint16_t seqnum;
    uint16_t retrynum;

} msg_premable_t;

typedef struct{

    location_tech_enum_t    location_tech_type;
    /*GNSS*/
    uint32_t lat;
    uint32_t lon;
    uint32_t alt;
    uint16_t timeto_fix_sec;
    /*GTP/WIFI*/
    uint16_t blobsize;
    void *data;


} location_fix_type_struct_t;



typedef struct{

    msg_premable_t sMsgPreamble;
    uint64_t Periodic_timestamp;
    uint16_t appmode;
    bool timeoffsetsize;
    bool locAssistanceDataNeeded;
    uint8_t battery_level;
    uint8_t nummsg;
    uint8_t msgnum;
    uint16_t modemask;
    uint16_t sensor_reading;
    uint16_t sec_start_period;
    sensor_data_type_t  sensor_data_type;
    uint16_t sensor_data_value;
    uint8_t timedriftnum;
    uint16_t start_period_sec;
    int32_t drift_amount;
    uint8_t battery_num;
    uint16_t battery_start_period_sec;
    uint8_t battery_level;

} period_check_in_struct_t;

typedef struct{

    msg_premable_t sMsgPreamble;
    period_check_in_struct_t    period_check_in_struct;
    uint8_t location;
    uint16_t location_start_period_sec;
    location_fix_type_struct_t  location_fix_type;
    uint8_t num_errors;
    uint16_t error_start_period_sec;
    uint32_t error_enum;
    uint8_t checkinfails;
    uint16_t check_start_period_sec;
    netenv_enum_t   netenv_at_failure;

} period_check_in2_struct_t;

typedef struct{
    uint16_t ack_msgid;
    uint16_t ack_seq_num;
    uint8_t ack_retry_num;
    dev_response_enum_t response_type;
    uint32_t next_report_period_sec;
    uint8_t staying_awake_sec;
} dev_ack_message_struct_t;





#endif