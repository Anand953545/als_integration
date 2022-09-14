/*====================================================================== 
 Copyright (c) 2022  Qualcomm Technologies, Inc. and/or its subsidiaries. 
 All rights reserved.
 Confidential - Qualcomm Technologies, Inc. - May Contain Trade Secrets
 *
 *        EDIT HISTORY FOR FILE
 *
 *	 This section contains comments describing changes made to the
 *	 module. Notice that changes are listed in reverse chronological
 *	 order.
 *
 ======================================================================*/

/**
  @file qapi_sensor_mgr.h

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none
*/

#ifndef _QAPI_SENSOR_MGR_H
#define _QAPI_SENSOR_MGR_H

#include "qapi_data_txm_base.h"
#include "qapi_status.h"
#include "qapi_driver_access.h"
#include "txm_module.h"

#define  TXM_QAPI_SENSOR_MGR_INIT                 TXM_QAPI_SENSOR_MGR_BASE + 1
#define  TXM_QAPI_SENSOR_MGR_SET_CB               TXM_QAPI_SENSOR_MGR_BASE + 2
#define  TXM_QAPI_SENSOR_MGR_SET_SENSOR_CONFIG    TXM_QAPI_SENSOR_MGR_BASE + 3
#define  TXM_QAPI_SENSOR_MGR_QUERY_SENSOR_IND     TXM_QAPI_SENSOR_MGR_BASE + 4
#define  TXM_QAPI_SENSOR_MGR_GET_SENSOR_DATA      TXM_QAPI_SENSOR_MGR_BASE + 5
#define  TXM_QAPI_SENSOR_MGR_GET_SENSOR_CONFIG    TXM_QAPI_SENSOR_MGR_BASE + 6
#define  TXM_QAPI_SENSOR_MGR_RELEASE              TXM_QAPI_SENSOR_MGR_BASE + 7

/** @addtogroup chapter_sensor_mgr
 *  @{ */

/** Maximum dimension of a sample of sensor data. */
#define MAX_SENSOR_SAMPLE_DIM_SZ       10

/** Maximum batch size of samples of sensor data. */
#define MAX_SENSOR_SAMPLE_BATCH_SZ     25

/** Maximum number of sensors. */
#define MAX_SENSOR_CNT                 10

/** Maximum number of sensor manager instances. */
#define SENSOR_MGR_MAX_INSTANCES 5

/** Sensor IDs. */
typedef enum
{
  QAPI_SENSOR_MGR_SENSOR_ID_MIN = -2147483647,  /**< Minimum value. Do not use */
  QAPI_SENSOR_MGR_SENSOR_ID_ALS = 1,            /**< Sensor ID for ALS sensor. */
  QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE = 2,       /**< Sensor ID for PRESSURE sensor. */
  QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY = 3,       /**< Sensor ID for HUMIDITY sensor. */
  QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE = 4,    /**< Sensor ID for TEMPERATURE sensor. */
  QAPI_SENSOR_MGR_SENSOR_ID_AMD = 5,            /**< Sensor ID for AMD sensor. */
  QAPI_SENSOR_MGR_SENSOR_ID_SMD = 6,            /**< Sensor ID for SMD sensor. @newpage */
  QAPI_SENSOR_MGR_SENSOR_ID_MAX = 2147483647,   /**< Maximum value. Do not use */
}qapi_Sensor_Mgr_Sensor_Id_t;

/** Sensor IDs masks. */
typedef uint16_t qapi_Sensor_Mgr_Sensor_Id_Mask_t;

#define QAPI_SENSOR_MGR_SENSOR_ID_ALS_MASK          (qapi_Sensor_Mgr_Sensor_Id_Mask_t)0x0001  /**< Sensor ID mask for ALS sensor. */
#define QAPI_SENSOR_MGR_SENSOR_ID_PRESSURE_MASK     (qapi_Sensor_Mgr_Sensor_Id_Mask_t)0x0002  /**< Sensor ID mask for PRESSURE sensor. */
#define QAPI_SENSOR_MGR_SENSOR_ID_HUMIDITY_MASK     (qapi_Sensor_Mgr_Sensor_Id_Mask_t)0x0004  /**< Sensor ID mask for HUMIDITY sensor. */
#define QAPI_SENSOR_MGR_SENSOR_ID_TEMPERATURE_MASK  (qapi_Sensor_Mgr_Sensor_Id_Mask_t)0x0008  /**< Sensor ID mask for TEMPERATURE sensor. */
#define QAPI_SENSOR_MGR_SENSOR_ID_AMD_MASK  (qapi_Sensor_Mgr_Sensor_Id_Mask_t)0x0010  /**< Sensor ID mask for AMD sensor. */
#define QAPI_SENSOR_MGR_SENSOR_ID_SMD_MASK  (qapi_Sensor_Mgr_Sensor_Id_Mask_t)0x0020  /**< Sensor ID mask for SMD sensor. */

/** Sensor manager QAPI message IDs. */
typedef enum {
 QAPI_SENSOR_MGR_SET_CONFIG_REQ_E = 1,     /**< Message ID for set sensor configuration request. */
 QAPI_SENSOR_MGR_SET_CONFIG_RESP_E,        /**< Message ID for set sensor configuration response. */
 QAPI_SENSOR_MGR_GET_CONFIG_REQ_E,         /**< Message ID for get sensor configuration request. */
 QAPI_SENSOR_MGR_GET_CONFIG_RESP_E,        /**< Message ID for get sensor configuration response. */
 QAPI_SENSOR_MGR_GET_SENSOR_DATA_REQ_E,    /**< Message ID for get sensor data request. */
 QAPI_SENSOR_MGR_GET_SENSOR_DATA_RESP_E,   /**< Message ID for get sensor data request. */
 QAPI_SENSOR_MGR_SENSOR_IND_E              /**< Message ID for sensor indication . */
} qapi_Sensor_Mgr_Info_ID_t;


/** Sensor Policy condition. */
typedef struct qapi_Sensor_Mgr_Sensor_Policy_Cond_s
{
  float alert_threshold_high;
  /**<   Any value greater than this is treated as Alert threashold breach.
    0XFFFFFFFF means this field is not considered for Alert threashold breach.*/

  float alert_threshold_low;
  /**<   Any value lower than this is treated as Alert threashold breach.
   0XFFFFFFFF means this field is not considered for Alert threashold breach.*/

  float pre_alert_threshold_high;
  /**<   Any value greater than this is treated as Pre Alert threashold breach.
    0XFFFFFFFF means this field is not considered for Pre Alert threashold breach.*/

  float pre_alert_threshold_low;
  /**<   Any value lower than this is treated as Pre Alert threashold breach.
   0XFFFFFFFF means this field is not considered for Pre Alert threashold breach.*/
}qapi_Sensor_Mgr_Sensor_Policy_Cond_t;

/** Sensor Configuration Policy. */
typedef struct qapi_Sensor_Mgr_Sensor_Config_Policy_s
{
  qapi_Sensor_Mgr_Sensor_Id_t sensor_id;
  /**< Sensor ID. */
  uint32_t sensor_policy_dim;
  /**< Sensor dimension.count */
  qapi_Sensor_Mgr_Sensor_Policy_Cond_t policy_cond[MAX_SENSOR_SAMPLE_DIM_SZ];
  /**< Sensor policy condition for each dimension. */
  uint8_t operating_mode;
  /**< Indicates whether a particular sensor is enabled or disabled. 0 - Disabled, 1 - Enabled. */
  uint8_t perf_mode;
  /**< High performance mode determines
      slope computation, dynamic measurement period
      0 - Default mode, 1 - High performance mode /Premium mode */
  uint8_t hysteresis_duration;
  /**< Duration of Alarm Breach before alarm interrupt generated.
       Min: 0 samples 
       Max: 15 samples */
  uint32_t high_perf_measurement_period;
  /**< Polling time of sensors during high performance mode
      Min: 60 secs
      Max: 900 secs 
      Resolution: 30 secs. */
  uint32_t measurement_period;
  /**< Measurement period(in secs). */
}qapi_Sensor_Mgr_Sensor_Config_Policy_t;

/** Set sensor configuration policy structure */
typedef struct qapi_Sensor_Mgr_Sensor_Set_Config_Policy_s
{
  qapi_Sensor_Mgr_Sensor_Id_t sensor_id;
  /**< Sensor ID. */
  uint32_t sensor_policy_dim;
  /**< Sensor dimension.count */
  qapi_Sensor_Mgr_Sensor_Policy_Cond_t policy_cond[MAX_SENSOR_SAMPLE_DIM_SZ];
  /**< Sensor policy condition for each dimension. */
  uint8_t operating_mode_valid;
  /**< Must be set to true if operating mode is being passed */
  uint8_t operating_mode;
  /**< Indicates whether a particular sensor is enabled or disabled. 0 - Disabled, 1 - Enabled. */
  uint8_t perf_mode_valid;
  /**< Must be set to true if performance mode is being passed */
  uint8_t perf_mode;
  /**< High performance mode determines
      slope computation, dynamic measurement period
      0 - Default mode, 1 - High performance mode /Premium mode */
  uint8_t hysteresis_duration_valid;
  /**< Must be set to true if hysteresis duration is being passed */
  uint8_t hysteresis_duration;
  /**< Duration of Alarm Breach before alarm interrupt generated.
       Min: 0 samples 
       Max: 15 samples */
  uint8_t high_perf_measurement_period_valid;
  /**< Must be set to true if high performance measurement period is being passed */
  uint32_t high_perf_measurement_period;
  /**< Polling time of sensors during high performance mode
      Min: 60 secs
      Max: 900 secs 
      Resolution: 30 secs. */
  uint8_t measurement_period_valid;
  /**< Must be set to true if measurement period is being passed */
  uint32_t measurement_period;
  /**< Measurement period(in secs). */
}qapi_Sensor_Mgr_Sensor_Set_Config_Policy_t;

/** Set sensor configuration error .*/
typedef enum {
  QAPI_SENSOR_MGR_SENSOR_ERROR_CONFIG_MIN_E = -2147483647,
  /**< Minimum value. Do not use */
  QAPI_SENSOR_MGR_ALARM_THRESHOLD_E = 1
  , /**<  Sensor config error Alarm Threshold \n   */
  QAPI_SENSOR_MGR_PRE_ALERT_THRESHOLD_E = 2,
  /**<  Sensor config error Pre Alert Threshold \n   */
  QAPI_SENSOR_MGR_MEASUREMENT_PERIOD_E = 3,
  /**<  Sensor config error Measurement Period\n   */
  QAPI_SENSOR_MGR_ALARM_HYSTERESIS_DURATION_E = 4,
  /**<  Sensor config error Alarm Hysteresis Duration \n   */
  QAPI_SENSOR_MGR_HIGH_PERF_MEASUREMENT_PERIOD_E = 5,
  /**<  Sensor config error High Performance Measurement Period \n   */
  QAPI_SENSOR_MGR_SENSOR_ERROR_CONFIG_MAX_E = 2147483647
  /**< Maximum value. Do not use */
}qapi_Sensor_Mgr_Sensor_Error_Evt_e;

/** Sensor configuration response structure .*/
typedef struct qapi_Sensor_Mgr_Sensor_Config_Rsp_s
{
  qapi_Sensor_Mgr_Sensor_Id_t sensor_id;
  /**< Sensor ID. */
  uint8_t result;
  /**< 0 - Success, 1 - Failure. */
  qapi_Sensor_Mgr_Sensor_Error_Evt_e error_cfg;
  /**< Error configuration event. */
  uint8_t error_type;
  /**< Error value. */
}qapi_Sensor_Mgr_Sensor_Config_Rsp_t;

/** Set sensor configuration response structure .*/
typedef struct qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_s
{
  uint32_t sensor_resp_len;
  /**< Must be set to # of elements in sensor_resp */
  qapi_Sensor_Mgr_Sensor_Config_Rsp_t sensor_resp[MAX_SENSOR_CNT];
  /** Sensor configuration response structure .*/
}qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t;

/** Get sensor configuration response structure .*/
typedef struct qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_s
{
  uint32_t sensor_config_list_len;
  /**< Must be set to # of elements in sensor_config_list */
  qapi_Sensor_Mgr_Sensor_Config_Policy_t sensor_config_list[MAX_SENSOR_CNT];
  /** Sensor Configuration Policy. */
}qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t;

/** Get sensor configuration request structure .*/
typedef struct qapi_Sensor_Mgr_Sensor_Config_Get_Req_s
{
  qapi_Sensor_Mgr_Sensor_Id_Mask_t sensor_id;
  /**< Sensor ID mask. */
}qapi_Sensor_Mgr_Sensor_Config_Get_Req_t;

/** Sensor Set Config Request message. */
typedef struct qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_s
{
  qapi_Sensor_Mgr_Info_ID_t type;
  /** Sensor manager QAPI message IDs. */
  union
  {
    qapi_Sensor_Mgr_Sensor_Set_Config_Policy_t *set_cfg;
    /** Set sensor configuration policy structure */
  }req;  /** Request */
}qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t;

/** Sensor Set Config Response message. */
typedef struct qapi_Sensor_Mgr_Sensor_Config_Rsp_Msg_s
{
  qapi_Sensor_Mgr_Info_ID_t type;
  /** Sensor manager QAPI message IDs. */
  union
  {
    qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_t *set_cfg_resp;
    /** Set sensor configuration response structure .*/
  }resp;  /** Response */
}qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t;


/** Sensor Get Config Response message. */
typedef struct qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_s
{
  qapi_Sensor_Mgr_Info_ID_t type;
  /** Sensor manager QAPI message IDs. */
  union
  {
    qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_t *get_cfg_resp;
    /** Get sensor configuration response structure .*/
  }resp;  /** Response */
}qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t;

/** Sensor Get Config Request message. */
typedef struct qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_s
{
  qapi_Sensor_Mgr_Info_ID_t type;
  /** Sensor manager QAPI message IDs. */
  union
  {
    qapi_Sensor_Mgr_Sensor_Config_Get_Req_t *get_cfg;
    /** Get sensor configuration request structure .*/
  }req;  /** Request */
}qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t;

/** Sensor sample data. */
typedef struct qapi_Sensor_Mgr_Sensor_Sample_Data_s
{
  uint32_t sample_dim;
  /**< Number of sensor dimension. */
  double sample[MAX_SENSOR_SAMPLE_DIM_SZ];
  /**< Sensor sample for each dimension. */
  uint64_t timestamp;
  /**< Timestamp when sample collected. */
}qapi_Sensor_Mgr_Sensor_Sample_Data_t;

/** Sensor sample data batch. */
typedef struct qapi_Sensor_Mgr_Sensor_Sample_Data_Batch_s
{
  qapi_Sensor_Mgr_Sensor_Id_t sensor_id;
  /**< Sensor ID. */ 
  uint32_t sample_batch_len;
  /**< Sensor sample batch length. */
  qapi_Sensor_Mgr_Sensor_Sample_Data_t sample_batch[MAX_SENSOR_SAMPLE_BATCH_SZ];
  /**< Sensor sample data batch. */
}qapi_Sensor_Mgr_Sensor_Sample_Data_Batch_t;

/** Sensor state enum. */
typedef enum {
  QAPI_SENSOR_MGR_SENSOR_STATE_MIN_E = -2147483647,
  /**< Minimum value. Do not use */
  QAPI_SENSOR_MGR_MEASUREMENT_E = 0,
  /**< Measurement state. */
  QAPI_SENSOR_MGR_PRE_ALERT_LOW_E = 1,
  /**< Pre alert low state. */
  QAPI_SENSOR_MGR_PRE_ALERT_HIGH_E = 2,
  /**< Pre alert high state. */
  QAPI_SENSOR_MGR_OUT_OF_RANGE_LOW_E = 3,
  /**< Out of range low state. */
  QAPI_SENSOR_MGR_OUT_OF_RANGE_HIGH_E = 4,
  /**< Out of range high state. */
  QAPI_SENSOR_MGR_OPERATION_RANGE_E = 5,
  /**< Operational state. */
  QAPI_SENSOR_MGR_SENSOR_STATE_MAX_E = 2147483647
  /**< Maximum value. Do not use */
}qapi_Sensor_Mgr_Sensor_State_e;

/** Sensor algorithm data. */
typedef struct qapi_Sensor_Mgr_Sensor_Sample_Data_Algo_s
{
  qapi_Sensor_Mgr_Sensor_Id_t sensor_id;
  /**< Sensor ID. */ 
  qapi_Sensor_Mgr_Sensor_State_e event_type;
  /**< Sensor event. */
  qapi_Sensor_Mgr_Sensor_State_e sensor_state;
  /**< Sensor state . */
  float ttc;
  /**< High Performance -Indicates time remaining (in mins) until sensor measurements breach threshold. 
       Restore measurement period to default. */
  float slope;
  /**< High Performance: Indicates rate of change of sensor measurements */
  uint32_t high_perf_measurement_period;
  /**< High Performance mesaurement perodicity. */
}qapi_Sensor_Mgr_Sensor_Sample_Data_Algo_t;

/** Sensor data response structure. */
typedef struct qapi_Sensor_Mgr_Sensor_Data_Resp_s
{
  uint8_t sensor_info_valid;
  /**< Sensor info valid. */
  uint32_t sensor_info_len;
  /**< Number of sensors whose data is contained in response. */
  qapi_Sensor_Mgr_Sensor_Sample_Data_Batch_t sensor_data[MAX_SENSOR_CNT];
  /**< Sensor data for each of sensors contained in response. */

  uint8_t algo_info_valid;
  /**< Algorithm info valid. */
  uint32_t algo_info_len;
  /**< Number of sensors whose algorithm info is contained in response. */
  qapi_Sensor_Mgr_Sensor_Sample_Data_Algo_t algo_data[MAX_SENSOR_CNT]; 
  /**< Algorithm data for each of sensors contained in response. */
}qapi_Sensor_Mgr_Sensor_Data_Resp_t;

/** Sensor data request structure. */
typedef struct qapi_Sensor_Mgr_Sensor_Data_Req_s
{
  qapi_Sensor_Mgr_Sensor_Id_Mask_t sensor_id_mask;
  /**< Sensor ID mask. */
  uint8_t is_logging_mode_valid;
  /**< Must be set to true if is_logging_mode is being passed */
  uint8_t is_logging_mode;
  /**< Logging mode. */
}qapi_Sensor_Mgr_Sensor_Data_Req_t;

/** Sensor data request message. */
typedef struct qapi_Sensor_Mgr_Sensor_Data_Req_Msg_s
{
  qapi_Sensor_Mgr_Info_ID_t type;
  /** Sensor manager QAPI message IDs. */
  union
  {
    qapi_Sensor_Mgr_Sensor_Data_Req_t *get_sensor_data;
    /** Sensor data request structure. */
  }req;    /**< Request */
}qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t;

/** Sensor data response message. */
typedef struct qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_s
{
  qapi_Sensor_Mgr_Info_ID_t type;
  /** Sensor manager QAPI message IDs. */
  union
  {
    qapi_Sensor_Mgr_Sensor_Data_Resp_t *get_sensor_data_rsp;
    /** Sensor data response structure. */
  }resp;  /**< Response */
}qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t;


/** Sensor data indication message. */
typedef struct qapi_Sensor_Mgr_Sensor_Ind_Msg_s
{
  uint8_t policy_met_sensor_mask_valid;
  /**< Policy met mask valid. */
  qapi_Sensor_Mgr_Sensor_Id_Mask_t policy_met_sensor_mask;
  /**< Policy met Sensor ID mask. */
  uint8_t batch_full_sensor_mask_valid;
  /**< Batch full mask valid. */
  qapi_Sensor_Mgr_Sensor_Id_Mask_t batch_full_sensor_mask;
  /**< Batch full Sensor ID mask. */
  uint8_t sensor_fail_cond_met_mask_valid;
  /**< Bad state Sensor mask valid. */
  qapi_Sensor_Mgr_Sensor_Id_Mask_t sensor_fail_cond_met_mask;
  /**< Bad state Sensor ID mask. */
}qapi_Sensor_Mgr_Sensor_Ind_Msg_t;


/** Sensor data indication message. */
typedef struct qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_s
{
  qapi_Sensor_Mgr_Info_ID_t type;
  /** Sensor manager QAPI message IDs. */
  union
  {
    qapi_Sensor_Mgr_Sensor_Ind_Msg_t *ind_data;
    /** Sensor data indication message. */
  }ind;  /**< Indication */
}qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t;

/** Sensor manager client handle. */
typedef uint32_t qapi_Sensor_Mgr_Hndl_t;

/** Sensor manager indication callback to be set by client. */
typedef void (*qapi_Sensor_Mgr_Sensor_Ind_Callback_t)(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl,
                                                          const qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *ind_msg);


static __inline int sensor_mgr_set_byte_pool(qapi_Sensor_Mgr_Hndl_t hndl, void *sensor_mgr_byte_pool_ptr);
static __inline int sensor_mgr_release_indirection(qapi_Sensor_Mgr_Hndl_t hndl);


static __inline void qapi_sensor_mgr_cb_uspace_dispatcher(UINT cb_id,
                                                             void *app_cb,
                                                             UINT cb_param1,
                                                             UINT cb_param2,
                                                             UINT cb_param3,
                                                             UINT cb_param4,
                                                             UINT cb_param5,
                                                             UINT cb_param6,
                                                             UINT cb_param7,
                                                             UINT cb_param8)
{
  void (*pfn_app_cb1) (qapi_Sensor_Mgr_Hndl_t , qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *);
  qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *ind_msg = NULL;
  if(cb_id == DATA_CUSTOM_CB_SENSOR_MGR_IND)
  {
    pfn_app_cb1 = (void (*)(qapi_Sensor_Mgr_Hndl_t, qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *))app_cb;
    ind_msg = (qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *)cb_param2;
    (pfn_app_cb1)((qapi_Sensor_Mgr_Hndl_t)cb_param1, 
                  (qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *)cb_param2);
    if(ind_msg)
    {
      if(ind_msg->ind.ind_data)
      {
        tx_byte_release(ind_msg->ind.ind_data);
      }
      tx_byte_release(ind_msg);
    }
  }
}

/** @} */

#ifdef  QAPI_TXM_MODULE

#define qapi_Sensor_Mgr_Init(a)                         ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_MGR_INIT,    (ULONG) a, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Mgr_Set_Callback(a, b, c)              ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_MGR_SET_CB,  (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) qapi_sensor_mgr_cb_uspace_dispatcher, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Mgr_Set_Sensor_Config(a, b, c, d)   ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_MGR_SET_SENSOR_CONFIG,     (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Mgr_Get_Sensor_Config(a, b, c)      ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_MGR_GET_SENSOR_CONFIG,     (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Mgr_Query_Sensor_Ind(a, b)          ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_MGR_QUERY_SENSOR_IND,     (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Mgr_Get_Sensor_Data(a, b, c)        ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_MGR_GET_SENSOR_DATA,     (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Mgr_Release(a)                       sensor_mgr_release_indirection(a)

/**
 * @brief Macro that Passes Byte Pool Pointer for Application using sensor mgr QAPI's.
 * @param[in] 'a' : Handle
 * @param[in] 'b' : Pointer to Byte Pool 
 * @return QAPI_OK is returned on success. On error, <0 is returned.
 * (This Macro is only used in DAM Space)
 */
#define qapi_Sensor_Mgr_Pass_Pool_Ptr(a,b)             sensor_mgr_set_byte_pool(a,b)

static __inline int sensor_mgr_set_byte_pool(qapi_Sensor_Mgr_Hndl_t hndl, void *sensor_mgr_byte_pool_ptr)
{
  qapi_cb_params_uspace_ptr_t *uspace_memory_sensor_mgr = NULL;
  int ret = QAPI_ERROR;

  /* input parameter validation*/
  if((hndl == NULL) || sensor_mgr_byte_pool_ptr == NULL)
  {
    return QAPI_ERR_INVALID_PARAM;
  }

  /* Retrieve the user space information stored internally */
  qapi_data_map_handle_to_u_addr(hndl, QAPI_APP_SENSOR_MGR, (void**)&uspace_memory_sensor_mgr);
  /* Handle to user space information already exists */
  if (uspace_memory_sensor_mgr)
  {
    return QAPI_OK;
  }

  tx_byte_allocate(sensor_mgr_byte_pool_ptr, (VOID **) &uspace_memory_sensor_mgr,(sizeof(qapi_cb_params_uspace_ptr_t)), TX_NO_WAIT);
  if (NULL == uspace_memory_sensor_mgr)
  {
    return QAPI_ERR_NO_MEMORY;
  }

  memset (uspace_memory_sensor_mgr,0, sizeof(qapi_cb_params_uspace_ptr_t));

  /* Initialize user space information */
  uspace_memory_sensor_mgr->usr_pool_ptr = sensor_mgr_byte_pool_ptr;  
  
  /* Map the user space information to the handle internally and store */ 
  ret = qapi_data_map_u_addr_to_handle((void *)(hndl), QAPI_APP_SENSOR_MGR, uspace_memory_sensor_mgr, 0x00);
  if (ret != QAPI_OK)
  {
    /* Release the allocated resources */
    if (uspace_memory_sensor_mgr)
      tx_byte_release(uspace_memory_sensor_mgr);
  }
  
  return ret;
  
}

/*
 * Releases the user space allocated resources and dispatches the call to qapi_Sensor_Mgr_Release 
 * Parameter 'a' : Handle
 * On success, #QAPI_OK is returned. On error, appropriate QAPI_ERR_ code(qapi_status.h) is returned.
 * (This is only used in DAM Space)
 */
static __inline int sensor_mgr_release_indirection(qapi_Sensor_Mgr_Hndl_t hndl)
{
  int ret = QAPI_ERROR;
  qapi_cb_params_uspace_ptr_t *uspace_memory_sensor_mgr = NULL;
  
  if(hndl == NULL)
  {
    return QAPI_ERR_INVALID_PARAM;
  }
  
  /* Retrieve the user space information stored internally */
  qapi_data_map_handle_to_u_addr(hndl, QAPI_APP_SENSOR_MGR, (void**)&uspace_memory_sensor_mgr);
  
  /* Release the allocated resources */
  if (uspace_memory_sensor_mgr)
    tx_byte_release(uspace_memory_sensor_mgr);
  
  /* Dispatch the call to the module manager */
  ret = ((qapi_Status_t) (_txm_module_system_call12)(TXM_QAPI_SENSOR_MGR_RELEASE, (ULONG) hndl, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0));
  return ret;
}

#else
	
qapi_Status_t qapi_Sensor_Mgr_Handler(UINT id, UINT a, UINT b, UINT c, UINT d, UINT e, UINT f, UINT g, UINT h, UINT i, UINT j, UINT k, UINT l);

/** @addtogroup chapter_sensor_mgr
@{ */

/**
*    @versiontable{2.0,2.45,
*    Data\_Services 1.26.0  &  Introduced. @tblendline
*    }
*    @ingroup qapi_Sensor_Mgr_Init
*
*    Creates a new sensor manager context.
*
*    @param[out] sensor_mgr_hndl  Newly created sensor manager context.
*
*    @return See Section @xref{hdr:QAPIStatust}. \n
*            On success, #QAPI_OK (0) is returned. Other value on error.
*/
qapi_Status_t qapi_Sensor_Mgr_Init(qapi_Sensor_Mgr_Hndl_t *sensor_mgr_hndl);

/**
*    @versiontable{2.0,2.45,
*    Data\_Services 1.26.0 &  Introduced. @tblendline
*    }
*    @ingroup qapi_Sensor_Mgr_Set_Callback
*
*    Sets a sensor manager indication callback.
*
*    @param[in] sensor_mgr_hndl  Sensor manager handle.
*    @param[in] type        Callback type.
*                           #QAPI_SENSOR_MGR_SENSOR_IND_E id needs to passed for message id for now.
*    @param[in] cbk         Callback to be registered.
*
*    @return See Section @xref{hdr:QAPIStatust}. \n
*            On success, #QAPI_OK (0) is returned. Other value on error.
*
*    @dependencies
*    qapi_Sensor_Mgr_Init() @newpage
*/
qapi_Status_t qapi_Sensor_Mgr_Set_Callback(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl,
                                                       qapi_Sensor_Mgr_Info_ID_t type,
                                                       qapi_Sensor_Mgr_Sensor_Ind_Callback_t cbk);

/**
*    @versiontable{2.0,2.45,
*    Data\_Services 1.26.0  &  Introduced. @tblendline
*    }
*    @ingroup qapi_Sensor_Mgr_Set_Sensor_Config
*
*    Sets sensor configuration. API can be used to set for single/multiple sensors.
*
*    @param[in] sensor_mgr_hndl  Sensor manager handle.
*    @param[in] request          Pointer to an array of Sensor Configuration request messages to be set. 
*                                #QAPI_SENSOR_MGR_SET_CONFIG_REQ_E id needs to passed for message id.
*    @param[in] cfg_len          Number of elements contained in the array of Sensor Configuration request messages.
*....@param[in,out] response     Pointer to an array of Sensor Configuration response messages to be set. 
*                                #QAPI_SENSOR_MGR_SET_CONFIG_RESP_E id needs to passed for message id.
*                                Response structure would be updated based on the values obtained from sensor manager.

*    @return See Section @xref{hdr:QAPIStatust}. \n
*            On success, #QAPI_OK (0) is returned. Other value on error.
*
*    @dependencies qapi_Sensor_Mgr_Init().
*/
qapi_Status_t qapi_Sensor_Mgr_Set_Sensor_Config(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl,
                                                               qapi_Sensor_Mgr_Sensor_Config_Set_Req_Msg_t *request,
                                                               uint32_t cfg_len,
                                                               qapi_Sensor_Mgr_Sensor_Config_Set_Rsp_Msg_t *response);

/**
*    @versiontable{2.0,2.45,
*    Data\_Services 1.26.0  &  Introduced. @tblendline
*    }
*    @ingroup qapi_Sensor_Mgr_Get_Sensor_Config
*
*    Gets sensor configuration. API can be used to get for single/multiple sensors.
*
*    @param[in] sensor_mgr_hndl  Sensor manager handle.
*    @param[in] request          Pointer to an array of Sensor Configuration request messages to be get. 
*                                #QAPI_SENSOR_MGR_GET_CONFIG_REQ_E id needs to passed for message id.
*....@param[in,out] response     Pointer to an array of Sensor Configuration response messages to be set. 
*                                #QAPI_SENSOR_MGR_GET_CONFIG_RESP_E id needs to passed for message id.
*                                Response structure would be updated based on the values obtained from sensor manager.

*    @return See Section @xref{hdr:QAPIStatust}. \n
*            On success, #QAPI_OK (0) is returned. Other value on error.
*
*    @dependencies qapi_Sensor_Mgr_Init().
*/
qapi_Status_t qapi_Sensor_Mgr_Get_Sensor_Config(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl,
                                                               qapi_Sensor_Mgr_Sensor_Config_Get_Req_Msg_t *request,
                                                               qapi_Sensor_Mgr_Sensor_Config_Get_Rsp_Msg_t *response);

/**
*    @versiontable{2.0,2.45,
*    Data\_Services 1.26.0  &  Introduced. @tblendline
*    }
*    @ingroup qapi_Sensor_Mgr_Query_Sensor_Ind
*
*    Queries sensor for any pending or missed indication.
*
*    @param[in] sensor_mgr_hndl  Sensor Mgr handle.
*    @param[in,out] ind_msg         Sensor indication response message received.
*                                #QAPI_SENSOR_MGR_SENSOR_IND_E id needs to passed for message id.
*
*    @return See Section @xref{hdr:QAPIStatust}. \n
*            On success, #QAPI_OK (0) is returned. Other value on error.
*
*    @dependencies qapi_Sensor_Mgr_Init().
*/
qapi_Status_t qapi_Sensor_Mgr_Query_Sensor_Ind(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl,
                                                             qapi_Sensor_Mgr_Sensor_Data_Ind_Msg_t *ind_msg);

/**
*    @versiontable{2.0,2.45,
*    Data\_Services 1.26.0  &  Introduced. @tblendline
*    }
*    @ingroup qapi_Sensor_Mgr_Get_Sensor_Data
*
*    Gets sensor data.API can be used to get for single/multiple sensors.
*
*    @param[in] sensor_mgr_hndl  Sensor manager handle.
*    @param[in] req              Pointer to an array of Sensor Configuration request message to get sensor data. 
*                                #QAPI_SENSOR_MGR_GET_SENSOR_DATA_REQ_E id needs to passed for message id.
*    @param[in,out] resp         Pointer to an array of Sensor Configuration response messages to sensor data. 
*                                #QAPI_SENSOR_MGR_GET_SENSOR_DATA_RESP_E id needs to passed for message id.
*
*    @return See Section @xref{hdr:QAPIStatust}. \n
*            On success, #QAPI_OK (0) is returned. Other value on error.
*
*    @dependencies qapi_Sensor_Mgr_Init().
*/
qapi_Status_t qapi_Sensor_Mgr_Get_Sensor_Data(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl,
                                                            qapi_Sensor_Mgr_Sensor_Data_Req_Msg_t *req,
                                                            qapi_Sensor_Mgr_Sensor_Data_Rsp_Msg_t *resp);

/**
*    @versiontable{2.0,2.45,
*    Data\_Services 1.26.0  &  Introduced. @tblendline
*    }
*    @ingroup qapi_Sensor_Mgr_Release
*
*    Releases sensor manager context.
*
*    @param[in] sensor_mgr_hndl  Sensor manager handle.
*
*    @return See Section @xref{hdr:QAPIStatust}. \n
*            On success, #QAPI_OK (0) is returned. Other value on error.
*
*    @dependencies qapi_Sensor_Mgr_Init().
*/
qapi_Status_t qapi_Sensor_Mgr_Release(qapi_Sensor_Mgr_Hndl_t sensor_mgr_hndl);

/** @} */

#endif

#endif
