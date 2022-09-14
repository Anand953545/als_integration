/*
 Copyright (c) 2017-2021  Qualcomm Technologies, Inc. and/or its subsidiaries.
 All rights reserved.
 Confidential - Qualcomm Technologies, Inc. - May Contain Trade Secrets
 ======================================================================*/
 /*======================================================================
 *
 *        EDIT HISTORY FOR FILE
 *
 *	 This section contains comments describing changes made to the
 *	 module. Notice that changes are listed in reverse chronological
 *	 order.
 *
 *
 *
 *
 * when 		who 	what, where, why
 * ---------- 	---   ------------------------------------------------
 * 2018-01-11   leo   (Tech Comm) Edited/added Doxygen comments and markup.
 * 2017-09-02   leo   (Tech Comm) Edited/added Doxygen comments and markup.
 * 2017-04-25   leo   (Tech Comm) Edited/added Doxygen comments and markup.
 ======================================================================*/

/*
!! IMPORTANT NOTE: "DATA SERVICES" VERSION CATEGORY.
*/


/**
  @file qapi_device_info.h

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none
*/
 
#ifndef _QAPI_DEVICE_INFO_H
#define _QAPI_DEVICE_INFO_H

#include "stdbool.h"
#include "qapi_data_txm_base.h"
#include "qapi_status.h"
#include "qapi_driver_access.h"

#ifndef MOB_TX
#include "txm_module.h"
#endif

/** @addtogroup qapi_device_info 
@{ */

#define  TXM_QAPI_DEVICE_INFO_INIT               TXM_QAPI_DEVICE_INFO_BASE + 1
#define  TXM_QAPI_DEVICE_INFO_GET                TXM_QAPI_DEVICE_INFO_BASE + 2
#define  TXM_QAPI_DEVICE_INFO_RELEASE            TXM_QAPI_DEVICE_INFO_BASE + 3
#define  TXM_QAPI_DEVICE_INFO_SET_CB             TXM_QAPI_DEVICE_INFO_BASE + 4
#define  TXM_QAPI_DEVICE_INFO_RESET              TXM_QAPI_DEVICE_INFO_BASE + 5

#define  TXM_QAPI_DEVICE_INFO_INIT2              TXM_QAPI_DEVICE_INFO_BASE_V2 + 1
#define  TXM_QAPI_DEVICE_INFO_GET2               TXM_QAPI_DEVICE_INFO_BASE_V2 + 2
#define  TXM_QAPI_DEVICE_INFO_RELEASE2           TXM_QAPI_DEVICE_INFO_BASE_V2 + 3
#define  TXM_QAPI_DEVICE_INFO_SET_CB2            TXM_QAPI_DEVICE_INFO_BASE_V2 + 4
#define  TXM_QAPI_DEVICE_INFO_RESET2             TXM_QAPI_DEVICE_INFO_BASE_V2 + 5
#define  TXM_QAPI_DEVICE_INFO_SET                TXM_QAPI_DEVICE_INFO_BASE_V2 + 6
#define  TXM_QAPI_DEVICE_INFO_REQUEST            TXM_QAPI_DEVICE_INFO_BASE_V2 + 7
#define  TXM_QAPI_DEVICE_INFO_CLEAR_CB2          TXM_QAPI_DEVICE_INFO_BASE_V2 + 8


/** Macros for mode preference (QAPI_DEVICE_INFO_MODE_PREF_E). */
#define QAPI_DEVICE_INFO_MODE_PREF_CDMA2000_1X     0x01   /**< Mode preference: CDMA 2000 1X. */
#define QAPI_DEVICE_INFO_MODE_PREF_CDMA2000_HRPD   0x02   /**< Mode preference: CDMA 2000 HRPD. */
#define QAPI_DEVICE_INFO_MODE_PREF_GSM             0x04   /**< Mode preference: GSM. */
#define QAPI_DEVICE_INFO_MODE_PREF_UMTS            0x08   /**< Mode preference: UMTS. */
#define QAPI_DEVICE_INFO_MODE_PREF_LTE             0x10   /**< Mode preference: LTE. */
#define QAPI_DEVICE_INFO_MODE_PREF_TDSCDMA         0x20   /**< Mode preference: TDSCDMA. */
#define QAPI_DEVICE_INFO_MODE_PREF_NR5G            0x40   /**< Mode preference: NR5G. */

/** Macros for ciot_mode_preference(QAPI_DEVICE_INFO_CIOT_LTE_OP_MODE_PREF_E). */
#define QAPI_DEVICE_INFO_LTE_OP_MODE_PREF_WB    ((uint64_t)0x01)   /**< LTE wideband. */
#define QAPI_DEVICE_INFO_LTE_OP_MODE_PREF_M1    ((uint64_t)0x02)   /**< LTE M1. */
#define QAPI_DEVICE_INFO_LTE_OP_MODE_PREF_NB1   ((uint64_t)0x04)   /**< LTE NB1. */

#define QAPI_CELL_SEARCH_MAX_NUM         8  /**< Maximum number of PCIs per RAT. */
#define QAPI_PCI_SCAN_MAX_NUM_PLMN       6  /**< Maximum number of MNC/MCC for a single cell. */

#define QAPI_DEVICE_INFO_THRESHOLD_LIST_MAX 32  /**< Maximum number element in threshold list.  */


/** Valid values for Ciot_Acquisition_order (#QAPI_DEVICE_INFO_RAT_ACQ_PREF_E). */
typedef enum {
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_MIN_ENUM_VAL = -2147483647,
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_NO_SVC = 0x00,   /**< No Service. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_CDMA_1X = 0x01,   /**< CDMA 1X. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_CDMA_1XEVDO = 0x02,   /**< CDMA HRPD. */ 
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_AMPS  = 0x03,   /**< AMPS. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_GSM = 0x04,   /**< GSM. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_UMTS = 0x05,   /**< UMTS. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_WLAN = 0x06,   /**< WLAN. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_GPS = 0x07,   /**< GPS. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_LTE  = 0x08,   /**< LTE. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_TDSCDMA = 0x09,   /**< TD-SCDMA. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_LTE_M1 = 0x0a,   /**< LTE M1. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_LTE_NB1 = 0x0b,   /**< LTE NB1. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_NR5G  = 0x0c,   /**< NR5G. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_NO_CHANGE = -1,   /**< No change. */
  QAPI_DEVICE_INFO_ACQ_ORDER_PREF_MAX_ENUM_VAL = 2147483647 
}acq_order_pref_enum;

/** Macros for band preference. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_1   ((uint64_t)0x000000000000001ull)  /**< E-UTRA Operating Band 1. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_2   ((uint64_t)0x000000000000002ull)  /**< E-UTRA Operating Band 2. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_3   ((uint64_t)0x000000000000004ull)  /**< E-UTRA Operating Band 3. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_5   ((uint64_t)0x000000000000010ull)  /**< E-UTRA Operating Band 5. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_6   ((uint64_t)0x000000000000020ull)  /**< E-UTRA Operating Band 6. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_7   ((uint64_t)0x000000000000040ull)  /**< E-UTRA Operating Band 7. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_8   ((uint64_t)0x000000000000080ull)  /**< E-UTRA Operating Band 8. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_9   ((uint64_t)0x000000000000100ull)  /**< E-UTRA Operating Band 9. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_10  ((uint64_t)0x000000000000200ull)  /**< E-UTRA Operating Band 10. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_11  ((uint64_t)0x000000000000400ull)  /**< E-UTRA Operating Band 11. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_12  ((uint64_t)0x000000000000800ull)  /**< E-UTRA Operating Band 12. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_13  ((uint64_t)0x000000000001000ull)  /**< E-UTRA Operating Band 13. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_14  ((uint64_t)0x000000000002000ull)  /**< E-UTRA Operating Band 14. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_17  ((uint64_t)0x000000000010000ull)  /**< E-UTRA Operating Band 17. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_18  ((uint64_t)0x000000000020000ull)  /**< E-UTRA Operating Band 18. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_19  ((uint64_t)0x000000000040000ull)  /**< E-UTRA Operating Band 19. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_20  ((uint64_t)0x000000000080000ull)  /**< E-UTRA Operating Band 20 */
#define QAPI_DEVICE_INFO_OPERATING_BAND_21  ((uint64_t)0x000000000100000ull)  /**< E-UTRA Operating Band 21. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_23  ((uint64_t)0x000000000400000ull)  /**< E-UTRA Operating Band 23. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_24  ((uint64_t)0x000000000800000ull)  /**< E-UTRA Operating Band 24. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_25  ((uint64_t)0x000000001000000ull)  /**< E-UTRA Operating Band 25. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_26  ((uint64_t)0x000000002000000ull)  /**< E-UTRA Operating Band 26. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_28  ((uint64_t)0x000000008000000ull)  /**< E-UTRA Operating Band 28. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_29  ((uint64_t)0x000000010000000ull)  /**< E-UTRA Operating Band 29. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_32  ((uint64_t)0x000000020000000ull)  /**< E-UTRA Operating Band 32. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_30  ((uint64_t)0x000000080000000ull)  /**< E-UTRA Operating Band 30. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_33  ((uint64_t)0x000000100000000ull)  /**< E-UTRA Operating Band 33. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_34  ((uint64_t)0x000000200000000ull)  /**< E-UTRA Operating Band 34. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_35  ((uint64_t)0x000000400000000ull)  /**< E-UTRA Operating Band 35. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_36  ((uint64_t)0x000000800000000ull)  /**< E-UTRA Operating Band 36. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_37  ((uint64_t)0x000001000000000ull)  /**< E-UTRA Operating Band 37. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_38  ((uint64_t)0x000002000000000ull)  /**< E-UTRA Operating Band 38. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_39  ((uint64_t)0x000004000000000ull)  /**< E-UTRA Operating Band 39. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_40  ((uint64_t)0x000008000000000ull)  /**< E-UTRA Operating Band 40. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_41  ((uint64_t)0x000010000000000ull)  /**< E-UTRA Operating Band 41. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_42  ((uint64_t)0x000020000000000ull)  /**< E-UTRA Operating Band 42. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_43  ((uint64_t)0x000040000000000ull)  /**< E-UTRA Operating Band 43. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_46  ((uint64_t)0x000200000000000ull)  /**< E-UTRA Operating Band 46. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_47  ((uint64_t)0x000400000000000ull)  /**< E-UTRA Operating Band 47. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_48  ((uint64_t)0x000800000000000ull)  /**< E-UTRA Operating Band 48. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_125 ((uint64_t)0x1000000000000000ull)  /**< E-UTRA Operating Band 125. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_126 ((uint64_t)0x2000000000000000ull)  /**< E-UTRA Operating Band 126. */
#define QAPI_DEVICE_INFO_OPERATING_BAND_127 ((uint64_t)0x4000000000000000ull)  /**< E-UTRA Operating Band 127. */

/** Valid values for battery status (#QAPI_DEVICE_INFO_BATTERY_STATUS_E). */
typedef enum
{
  QAPI_DEVICE_INFO_BAT_OV = 0x00,           /**< Over battery voltage. */
  QAPI_DEVICE_INFO_BAT_UV = 0x01,           /**< Low battery voltage. */
  QAPI_DEVICE_INFO_BAT_MISSING = 0x02,      /**< Battery is missing. */
  QAPI_DEVICE_INFO_BAT_GOOD_HEALTH = 0x03   /**< Battery voltage is good. */
} battery_status;


/** Valid values for network service status (QAPI_DEVICE_INFO_SERVICE_STATE_E). */
typedef enum
{
  QAPI_DEVICE_INFO_SRV_STATE_NO_SRV = 0, /**< No service. */
  QAPI_DEVICE_INFO_SRV_STATE_SRV = 1     /**< Service is available. */
}srv_status;


/** Valid values for network indication (#QAPI_DEVICE_INFO_NETWORK_IND_E). */
typedef enum
{
  QAPI_DEVICE_INFO_NW_IND_NO_SRV = 0, /**< No service. */
  QAPI_DEVICE_INFO_NW_IND_SRV = 1     /**< Service is available. @newpage */
}nw_indication;


/** Valid values for network RRC state. */
typedef enum
{
  QAPI_DEVICE_INFO_RRC_IDLE = 0,       /**< Status: Idle. */
  QAPI_DEVICE_INFO_RRC_CONNECTED = 1   /**< Status: connected. */
}rrc_state;


/** Valid values for network Extended Mobility Management (EMM) state. */
typedef enum
{
  QAPI_DEVICE_INFO_EMM_NULL = 0,                              /**< Null. */
  QAPI_DEVICE_INFO_EMM_DEREGISTERED = 1,                      /**< Deregistered. */
  QAPI_DEVICE_INFO_EMM_REGISTERED_INITIATED = 2,              /**< Registered, initiated. */
  QAPI_DEVICE_INFO_EMM_REGISTERED = 3,                        /**< Registered. */
  QAPI_DEVICE_INFO_EMM_TRACKING_AREA_UPDATING_INITIATED = 4,  /**< Tracking area update initiated. */
  QAPI_DEVICE_INFO_EMM_SERVICE_REQUEST_INITIATED = 5,         /**< Service request initiated. */
  QAPI_DEVICE_INFO_EMM_DEREGISTERED_INITIATED = 6             /**< Deregistered, initiated. */
}emm_state;


/** Valid values for network roaming status (#QAPI_DEVICE_INFO_ROAMING_E). */
typedef enum
{
  QAPI_DEVICE_INFO_ROAMING_STATUS_OFF = 0,  /**< Roaming status: OFF. */
  QAPI_DEVICE_INFO_ROAMING_STATUS_ON = 1    /**< Roaming status: ON. */
}roaming_info;


/** Valid value for sim state (#QAPI_DEVICE_INFO_SIM_STATE_E). */
typedef enum
{
  QAPI_DEVICE_INFO_SIM_STATE_UNKNOWN = 0x00,            /**< Unknown. */
  QAPI_DEVICE_INFO_SIM_STATE_DETECTED = 0x01,           /**< Detected. */
  QAPI_DEVICE_INFO_SIM_STATE_PIN1_OR_UPIN_REQ = 0x02,   /**< PIN1 or UPIN is required. */
  QAPI_DEVICE_INFO_SIM_STATE_PUK1_OR_PUK_REQ = 0x03,    /**< PUK1 or PUK for UPIN is required. */
  QAPI_DEVICE_INFO_SIM_STATE_PERSON_CHECK_REQ = 0x04,   /**< Personalization state must be checked. */
  QAPI_DEVICE_INFO_SIM_STATE_PIN1_PERM_BLOCKED  = 0x05, /**< PIN1 is blocked. */ 
  QAPI_DEVICE_INFO_SIM_STATE_ILLEGAL = 0x06,            /**< Illegal. */
  QAPI_DEVICE_INFO_SIM_STATE_READY = 0x07               /**< Ready. @newpage*/
}sim_state;

typedef enum
{
  QAPI_DEVICE_INFO_NO_SRV_V01  = 0x00, /**<  No service. */
  QAPI_DEVICE_INFO_LTE_WB_V01  = 0x01, /**<  Camped on LTE wideband. */
  QAPI_DEVICE_INFO_LTE_M1_V01  = 0x02, /**<  Camped on LTE M1. */
  QAPI_DEVICE_INFO_LTE_NB1_V01 = 0x03  /**<  Camped on LTE NB1. */  
}ciot_lte_op_mode;


typedef enum { 
  QAPI_DEVICE_INFO_GNSS_PRIORITY   = 0, /**<  Positioning procedure priority is higher than usual communication and page reception. */
  QAPI_DEVICE_INFO_WWAN_PRIORITY   = 1, /**<  Communication priority (including page reception) is higher than positioning procedures. */ 
}client_group;


typedef enum {
  QAPI_DEVICE_INFO_NONE           = 0,  /**< None loaded/pending. */
  QAPI_DEVICE_INFO_WWAN_PENDING   = 1,  /**< WWAN pending state. */
  QAPI_DEVICE_INFO_GPS_PENDING    = 2,  /**< GPS pending state. */
  QAPI_DEVICE_INFO_WWAN_LOADED    = 3,  /**< WWAN loaded state. */
  QAPI_DEVICE_INFO_GPS_LOADED     = 4   /**< GPS loaded state. */
}loaded_app_info;

typedef enum {
  QAPI_DEVICE_INFO_JAMMING_NOTFOUND = 0,   /**< Jamming status not found. */
  QAPI_DEVICE_INFO_JAMMING_FOUND,          /**< Jamming status found. */
  QAPI_DEVICE_INFO_JAMMING_UNKNOWN,        /**< Jamming status unknown. */
}jammer_status;

typedef enum {
     QAPI_DEVICE_INFO_VOICE_DIAL_CALL_REQ = 0,   /**< Dial VoLTE call request. */
     QAPI_DEVICE_INFO_VOICE_END_CALL_REQ,        /**< End VoLTE call request. */
     QAPI_DEVICE_INFO_VOICE_ANSWER_CALL_REQ,     /**< Answer or reject VoLTE call request. @newpage */
}voice_call_req_type;


/** Macro for Network Bearer values (QAPI_DEVICE_INFO_NETWORK_BEARER_E). */
#define QAPI_DEVICE_INFO_SRV_TYPE_LTE     1   /**< Nw bearer srvc type :LTE. */
#define QAPI_DEVICE_INFO_SRV_TYPE_GSM     2   /**< Nw bearer srvc type :GSM. */
#define QAPI_DEVICE_INFO_SRV_TYPE_WCDMA   3   /**< Nw bearer srvc type :WCDMA. */
#define QAPI_DEVICE_INFO_LTE_TDD          5   /**< Nw bearer srvc type :LTE  Mode: TDD. */
#define QAPI_DEVICE_INFO_LTE_FDD          6   /**< Nw bearer srvc type :LTE  Mode: FDD. */
#define QAPI_DEVICE_INFO_LTE_NB_IOT       7   /**< Nw bearer srvc type :LTE  Mode: NB-IOT.*/




/** Device information types. \n  
 *   <b>Response type:</b> Return value of the Device ID as specified in the enum qapi_Device_Info_Type_t. \n
 *   <b>Operation:</b>
 *   \li Query - Queries the device information using the API qapi_Device_Info_Get_v2().
 *   \li Set - Sets the device information using the API qapi_Device_Info_Set(). \n
     \li Indication - Sets the device information callback, registering for various indications using the API qapi_Device_Info_Set_Callback_v2().
 */
typedef enum {

 QAPI_DEVICE_INFO_BUILD_ID_E,
 /**< Device BUILD_ID. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_IMEI_E,
 /**< Device IMEI. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_IMSI_E,
 /**< UIM IMSI. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_OS_VERSION_E,
 /**< Device OS version. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MANUFACTURER_E,
 /**< Device manufacturer. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MODEL_ID_E,
 /**< Device model ID. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_BATTERY_STATUS_E,
 /**< Device battery status. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_BATTERY_PERCENTAGE_E,
 /**< Device battery percentage. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEIVCE_INFO_TIME_ZONE_E,
 /**< @deprecated Use enum #QAPI_DEVICE_INFO_TIME_ZONE_E instead. \n Device time zone. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_TIME_ZONE_E = QAPI_DEIVCE_INFO_TIME_ZONE_E,
 /**< Device time zone. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEIVCE_INFO_ICCID_E,
 /**< @deprecated Use enum #QAPI_DEVICE_INFO_ICCID_E instead. \n Device ICCID. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_ICCID_E = QAPI_DEIVCE_INFO_ICCID_E,
 /**< Device ICCID. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_4G_SIG_STRENGTH_E,
 /**< Network signal strength. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_BASE_STATION_ID_E,
 /**< Network base station ID. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MCC_E,
 /**< Network MCC. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_MNC_E,
 /**< Network MNC. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_SERVICE_STATE_E,
 /**< Network service status. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_MDN_E,
 /**< Device MDN. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_TAC_E,
 /**< Network tracking area code. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_CELL_ID_E,
 /**< Network cell ID. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_RCCS_E,
 /**< Network RRC state. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_EMMS_E,
 /**< Network EMM state. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 DEPRACATED1,
 /* Information to keep enum numbering consistent. */

 QAPI_DEVICE_INFO_SERVING_PCI_E,
 /**< Network serving cell PCI. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_SERVING_RSRQ_E,
 /**< Serving cell RSRQ. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_SERVING_EARFCN_E,
 /**< Serving cell EARFCN. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 DEPRACATED2,
 /* Information to keep enum numbering consistent. */
 DEPRACATED3,
 /* Information to keep enum numbering consistent. */
 DEPRACATED4,
 /* Information to keep enum numbering consistent. */
 DEPRACATED5,
 /* Information to keep enum numbering consistent. */
 DEPRACATED6,
 /* Information to keep enum numbering consistent. */

 QAPI_DEVICE_INFO_NETWORK_IND_E,
 /**< Network indication. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_ROAMING_E,
 /**< Roaming status. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_LAST_POWER_ON_E,
 /**< Last power on time. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_CHIPID_STRING_E,
 /**< Chipset name. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

  DEPRACATED7,
 /* Information to keep enum numbering consistent. */

 QAPI_DEVICE_INFO_SIM_STATE_E,
 /**< SIM state. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_NETWORK_BEARER_E,
 /**< Network bearer. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_LINK_QUALITY_E,
 /**< Network link quality. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query and Indication. */

 QAPI_DEVICE_INFO_TX_BYTES_E,
 /**< Device Tx bytes. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Indication. */

 QAPI_DEVICE_INFO_RX_BYTES_E,
 /**< Device Rx bytes. \n Response type: #QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Indication. */

 QAPI_DEVICE_INFO_ANY,
 /**< Any device information. */

 QAPI_DEVICE_INFO_CIOT_LTE_OP_MODE_PREF_E,
 /**< Device LTE operational mode preference. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_LTE_M1_BAND_PREF_E,
 /**< Device LTE M1 band preference. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_LTE_NB1_BAND_PREF_E,
 /**< Device LTE NB1 band preference. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_MODE_PREF_E,
 /**< Device mode preference. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query.. */

 QAPI_DEVICE_INFO_RAT_ACQ_PREF_E,
 /**< RAT Acquisition order preference. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_PS_DETACH_E,
 /**< Device PS Detach. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set. */

 QAPI_DEVICE_INFO_PS_ATTACH_E,
 /**< Device PS Attach. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set. */

 QAPI_DEVICE_INFO_PSM_TIMER_E,
 /**< Device PSM timer. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_ACTIVE_TIMER_E,
 /**< Device power save mode active timer value. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_LTE_OP_MODE_E,
/**< Device LTE operational mode. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_LAC_E,
 /**< Location Area Code. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_RAT_TYPE_GSM_CELL_E,
 /**< Device Cell info RAT type - GSM. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_RAT_TYPE_LTE_M1_CELL_E,
 /**< Device Cell info RAT type - LTE M1. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_RAT_TYPE_LTE_NB1_CELL_E,
 /**< Device Cell info RAT type -LTE NB1 Cell info. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_BAND_GSM_CELL_E,
 /**< Device Cell info Band for RAT type GSM. \n Request type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set. */

 QAPI_DEVICE_INFO_BAND_LTE_M1_CELL_E,
 /**< Device Cell info Band for RAT type LTE M1. \n Request type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set. */

 QAPI_DEVICE_INFO_BAND_LTE_NB1_CELL_E,
 /**< Device Cell info Band for RAT type LTE NB1. \n Request type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set. */

 QAPI_DEVICE_INFO_SET_APP_PRIORITY_E,
 /**< Device App priority. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set. */

 QAPI_DEVICE_INFO_GET_APP_PRIORITY_E,
 /**< Device App priority. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_GET_LOADED_TECH_E,
 /**< Device currently loaded technology. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */
 
 QAPI_DEVICE_INFO_OPERATING_MODE_E,
 /**< Device operating mode. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E, 0 = Online mode, 1 = Low Power mode. Operation: Set and Query. */

 QAPI_DEVICE_INFO_EDRX_E,
 /**< eDRX information. \n Response type: #QAPI_DEVICE_INFO_TYPE_EDRX_INFO_E. Operation: Indication. */

 QAPI_DEVICE_INFO_PLMN_LIST_E,
 /**< Scan PLMN list. \n Response type: #QAPI_DEVICE_INFO_TYPE_PLMN_LIST_E. Operation: Query. */
 
 QAPI_DEVICE_INFO_PLMN_SELECT_E,
 /**< Select PLMN. \n Response type: #QAPI_DEVICE_INFO_TYPE_ARRAY_E, Val[0]: Net Select Prefer, 0 = Auto, 1 = Manual; Val[1]: MCC; Val[2]: MNC. Operation: Set. */
 
 QAPI_DEVICE_INFO_CEMODE_E,
 /**< CEMODE info. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E, 0 = PS mode2, 1 = CS/PS mode1, 2 = CS/PS mode2. Operation: Set and Query. */

 QAPI_DEVICE_INFO_SHUTDOWN_E,
 /**< Shutdown UE. \n Response type: Not applicable. Operation: Set. */

 QAPI_DEVICE_INFO_WMS_EVENT_REPORT_E,
 /**< WMS. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Indication. */

 QAPI_DEVICE_INFO_JAMMER_IND_E,
 /**< Jamming status. \n Response type: qapi_Device_Info_Jamming_Status_Get_Rsp_t. Operation: Indication. */

 QAPI_DEVICE_INFO_IMS_IOT_REGISTRATION_STATUS_IND_E,
 /**< Registration status. \n Response type: qapi_Device_Info_IMS_Registration_Status_Rsp_t. Operation: Indication. */

 QAPI_DEVICE_INFO_VOICE_ALL_CALL_STATUS_IND_E,
 /**< All call status. \n Response type: qapi_Device_Info_Call_Status_Rsp_t. Operation: Indication. */

 QAPI_DEVICE_INFO_SUBS_ENABLE_E,
 /**< Enable/disable subscription. \n Response type: Not applicable. Operation: Set. */

 QAPI_DEVICE_INFO_MODEM_SERVICE_STATE_E,
 /**< Modem service state: Loaded or not loaded. \n Response type: #QAPI_DEVICE_INFO_TYPE_BOOLEAN_E Operation: Indication and Query. */

 QAPI_DEVICE_INFO_WWAN_SLEEP_INFO_E,
 /**< WWAN sleep duration information. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E Operation: Indication. */

 QAPI_DEVICE_INFO_WWAN_SLEEP_THRESHOLD_E,
 /**< WWAN sleep threshold. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E Operation: Set and Query. */
 QAPI_DEVICE_INFO_LTE_M1_BAND_PREF_EXT_E,
 /**< Device LTE M1 band preference extended. \n Response type: QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_LTE_NB1_BAND_PREF_EXT_E,
 /**< Device LTE NB1 band preference extended. \n Response type: QAPI_DEVICE_INFO_TYPE_ARRAY_E. Operation: Set and Query. */

 QAPI_DEVICE_INFO_FIRMWARE_VERSION_E,
 /**< Device Firmware version. \n Response type: QAPI_DEVICE_INFO_TYPE_BUFFER_E. Operation: Query. */

 QAPI_DEVICE_INFO_SINR_E,
 /**Device SINR. \n Reponse type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */
 
 QAPI_DEVICE_INFO_CE_E,
 /**Device CE level. \n Reponse type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */ 

 QAPI_DEVICE_INFO_SET_NZ_BSR_E,
 /**< Set device NZ BSR. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Set. */

 QAPI_DEVICE_INFO_GET_NZ_BSR_E,
 /**< Get device NZ BSR. \n Response type: #QAPI_DEVICE_INFO_TYPE_INTEGER_E. Operation: Query. */

 QAPI_DEVICE_INFO_INCOMING_SMS_E,
 /**< Incoming MT SMS indication. \n Response type: qapi_Device_Info_Incoming_SMS_Ind_t. Operation: Indication. */

 QAPI_DEVICE_INFO_ROUTE_INCOMING_SMS_E,
 /**< Incoming route MT delievery SMS. \n Response type: qapi_Device_Info_Route_Incoming_SMS_Ind_t. Operation: Indication. */

 QAPI_DEVICE_INFO_SMS_STATUS_REPORT_E,
 /**< SMS status report. \n Response type: qapi_Device_Info_SMS_Staus_Report_t. Operation: Indication. */

 QAPI_DEVICE_INFO_SEND_SMS_RSP_E,
  /**< Send SMS response. \n Response type: qapi_Device_Info_Send_SMS_Rsp_t. Operation: Indication. */
} qapi_Device_Info_ID_t;


/** Device information request types. */
typedef enum {
  /*UIM Request*/
  QAPI_DEVICE_INFO_REQ_VERIFY_PIN_E,  /**< Verify PIN. */
  QAPI_DEVICE_INFO_REQ_CHANGE_PIN_E,  /**< Change PIN. */
  QAPI_DEVICE_INFO_REQ_UNBLOCK_PIN_E, /**< Unblock PIN. */
  QAPI_DEVICE_INFO_REQ_PROTECT_PIN_E, /**< Enable/disable PIN. */

  QAPI_DEVICE_INFO_REQ_SET_FDN_STATUS_E,       /**< Enable/disable FDN Service. */
  QAPI_DEVICE_INFO_REQ_GET_FDN_STATUS_E,       /**< Enable/disable FDN Service. */
  
  QAPI_DEVICE_INFO_REQ_SEND_APDU_E,            /**< Send APDU. */
  
  QAPI_DEVICE_INFO_REQ_READ_PREFERRED_PLMN_E,  /**< Read preferred PLMN list. */
  QAPI_DEVICE_INFO_REQ_WRITE_PREFERRED_PLMN_E, /**< Write preferred PLMN item. */

  QAPI_DEVICE_INFO_REQ_READ_UICC_APPLICATIONS_E, /**< Read UICC Application list. */
  /*NAS Request*/
  QAPI_DEVICE_INFO_REQ_EDRX_SET_E,                 /**< Set edrx parameters. */
  QAPI_DEVICE_INFO_REQ_EDRX_GET_E,                 /**< Get edrx parameters. */
  QAPI_DEVICE_INFO_REQ_GET_JAMMER_STATUS_E,        /**< Get jamming status. */
  /*IMS_IOT Request*/
  QAPI_DEVICE_INFO_REQ_GET_IMS_REGISTRATION_E,     /**< Get IMS registration status. */
  /*Voice Request*/
  QAPI_DEVICE_INFO_REQ_VOLTE_CALL_E,               /**< Dial, answer, or reject VoLTE call. */

  /*Data Profile Request*/
  QAPI_DEVICE_INFO_REQ_SET_QCPDPP_E,               /**< Set profile authentication parameters. */
  QAPI_DEVICE_INFO_REQ_SET_CGDCONT_E,              /**< Set basic PDP context parameters. */
  QAPI_DEVICE_INFO_REQ_SET_QCPDPIMSCFG_E,          /**< Set IMS related PDP profile parameters. */
  QAPI_DEVICE_INFO_REQ_SET_QCPDPCFG_E,             /**< Set APN related PDP profile parameters. */
  QAPI_DEVICE_INFO_REQ_GET_QCPDPP_E,               /**< Fetch profile authentication parameters. */
  QAPI_DEVICE_INFO_REQ_GET_CGDCONT_E,              /**< Fetch basic PDP context parameters. */
  QAPI_DEVICE_INFO_REQ_GET_QCPDPIMSCFG_E,          /**< Fetch IMS related PDP profile parameters. */
  QAPI_DEVICE_INFO_REQ_GET_QCPDPCFG_E,             /**< Fetch APN related PDP profile parameters. */
  QAPI_DEVICE_INFO_SCAN_CONFIG_E,				   /**< Get Scan Counter Configuration parameters. >**/
  /*NAS FSK Request*/
  QAPI_DEVICE_INFO_REQ_FSK_START_E,                /**< FSK start request. */
  QAPI_DEVICE_INFO_REQ_FSK_STOP_E,                 /**< FSK stop request. */
  QAPI_DEVICE_INFO_REQ_FSK_DATA_E,                 /**< FSK data request. */
  QAPI_DEVICE_INFO_REQ_GET_FSK_DEBUG_E,            /**< Get FSK debug parameters request. */
  QAPI_DEVICE_INFO_REQ_SET_FSK_DEBUG_E,            /**< Set FSK debug parameters request. */
  QAPI_DEVICE_INFO_REQ_GET_FSK_HOP_TABLE_E,        /**< Get FSK hop table parameters request. */
  QAPI_DEVICE_INFO_REQ_SET_FSK_HOP_TABLE_E,        /**< Set FSK hop table parameters request. */
  QAPI_DEVICE_INFO_REQ_GET_FSK_PCL_E,              /**< Get FSK pcl parameters request. */
  QAPI_DEVICE_INFO_REQ_SET_FSK_PCL_E,              /**< Set FSK pcl parameters request. @newpage*/
  QAPI_DEVICE_INFO_REQ_GET_QRXLEVMIN_E,            /**< Get qRxlevmin request. */
  /*IMS_IOT Indication Register Request*/
  QAPI_DEVICE_INFO_REQ_SET_IMS_IOT_IND_REGISTER_E, /**< Set IMS IOT indication register. */
  QAPI_DEVICE_INFO_REQ_CONFIG_SIGNAL_STRENGTH_E,   /**< Set singnal strength threshold. */
  QAPI_DEVICE_INFO_REQ_GET_SRXLEV_E,                /**< Get srxlev request. */
 /* WMS Request*/
  QAPI_DEVICE_INFO_REQ_SEND_SMS_E,                 /**< Send SMS parameters request. */
  QAPI_DEVICE_INFO_REQ_WRITE_SMS_E,                /**< Write SMS parameters request. */
  QAPI_DEVICE_INFO_REQ_READ_SMS_E,                 /**< Read SMS parameters request. */
  QAPI_DEVICE_INFO_REQ_DELETE_SMS_E,               /**< Delete SMS parameters request. */
  QAPI_DEVICE_INFO_REQ_LIST_SMS_E,                 /**< List SMS parameters request. */
  QAPI_DEVICE_INFO_REQ_SET_SMS_ROUTES_E,           /**< Set SMS routes parameters request. */
  QAPI_DEVICE_INFO_REQ_GET_SMS_ROUTES_E,           /**< Get SMS routes parameters request. */
  QAPI_DEVICE_INFO_REQ_SEND_SMS_ACK_E,             /**< Send SMS acknowledgement parameters request. */
  QAPI_DEVICE_INFO_REQ_GET_SMSC_E,                 /**< Get SMSC parameters request. */
  QAPI_DEVICE_INFO_REQ_SET_SMSC_E,                 /**< Set SMSC parameters request. */
}qapi_Device_Info_Req_ID_t;


/** Device information response types. */
typedef enum {
 QAPI_DEVICE_INFO_TYPE_BOOLEAN_E,
 /**< Response type is Boolean. */
 QAPI_DEVICE_INFO_TYPE_INTEGER_E,
 /**< Response type is integer. */
 QAPI_DEVICE_INFO_TYPE_BUFFER_E,
 /**< Response type is buffer. */
 QAPI_DEVICE_INFO_TYPE_ARRAY_E,
 /**< Response type is array. */
 QAPI_DEVICE_INFO_TYPE_CELL_INFO_E,
 /**< Response type is qapi_cell_info type. */
 QAPI_DEVICE_INFO_TYPE_EDRX_INFO_E,
 /**< Response type is qapi_Device_Info_Rsp_EDRX_Get_t. */
 QAPI_DEVICE_INFO_TYPE_PLMN_LIST_E,
 /**< Response type is qapi_Device_Info_PLMN_Info_List_t. */
 QAPI_DEVICE_INFO_TYPE_JAMMING_STATUS_E,
 /**< Response type is qapi_Device_Info_Jamming_Status_Get_Rsp_t. */
 QAPI_DEVICE_INFO_TYPE_IMS_REGISTRATION_STATUS_E,
 /**< Response type is qapi_Device_Info_IMS_Registration_Status_Rsp_t. */
 QAPI_DEVICE_INFO_TYPE_VOLTE_CALL_STATUS_E,
 /**< Response type is qapi_Device_Info_Call_Status_Rsp_t. */
 QAPI_DEVICE_INFO_TYPE_INCOMING_SMS_E,
 /**< Response type is qapi_Device_Info_Incoming_SMS_Ind_t. */
 QAPI_DEVICE_INFO_TYPE_ROUTE_INCOMING_SMS_E,
 /**< Response type is qapi_Device_Info_Route_Incoming_SMS_Ind_t. */
 QAPI_DEVICE_INFO_TYPE_SMS_STATUS_REPORT_E,
 /**< Response type is qapi_Device_Info_Route_SMS_Status_Report_t. */
 QAPI_DEVICE_INFO_TYPE_SEND_SMS_RSP_E,
 /**< Response type is qapi_Device_Info_Send_SMS_Ack_Rsp_t. */
} qapi_Device_Info_Type_t;

/*========================================================
 * Device Info structure                               *
 ========================================================*/

/** Maximum size of #qapi_Device_Info_t valuebuf. */
#define QAPI_DEVICE_INFO_BUF_SIZE 128

/**< Julian year. */
#define JULIAN_YEAR 1980   

/**< Maximum number of device info instances. */
#define DEVICE_INFO_MAX_INSTANCES 5  

/**< Maximum length of the integer array. */
#define MAX_LEN_VAL 8    

/**
 QAPI device information structure.
*/
typedef struct {
  qapi_Device_Info_ID_t id; /**< Required information ID. */
  qapi_Device_Info_Type_t info_type; /**< Response type. */
/** Union of values. */
  union {
/** Union of buffer values. */
    struct {
      char buf[QAPI_DEVICE_INFO_BUF_SIZE]; /**< Response buffer. */
      uint32_t len; /**< Length of the response string. */
    }valuebuf;    
    int64_t valueint; /**< Response integer value. */
    bool valuebool; /**< Response Boolean value.*/
/** Union of integer array values. */
    struct 
    {
      int buf[MAX_LEN_VAL]; /**< Response type: integer array. */
      uint32_t len; /**< Length of the array. @newpagetable */
    }valint; 
    void* device_info_extn; /**< Extended parameter. */
  }u; /**< Union of Device Information value based on response type. */
}qapi_Device_Info_t;

/** Device information handle. */
typedef void * qapi_Device_Info_Hndl_t;

/**
 * QAPI device information callback typedef.
 * @deprecated in DAM space. Use callback qapi_Device_Info_Callback_v2() instead.
 */
typedef void (*qapi_Device_Info_Callback_t)(const qapi_Device_Info_t *info);

/** QAPI device information callback_v2 typedef. @newpage */
typedef void (*qapi_Device_Info_Callback_t_v2)(qapi_Device_Info_Hndl_t device_info_hndl, const qapi_Device_Info_t *info);

/**
 QAPI cell scan information for GSM structure.
*/
typedef struct 
{ 
  uint16_t arfcn; /**< Absolute RF channel number.*/
  uint8_t bsic;   /**< Base station identity code. */
  uint16_t mcc;  /**<  A 16-bit integer representation of MCC. Range: 0 to 999. */
  uint16_t mnc;  /**<  A 16-bit integer representation of MNC. Range: 0 to 999. */
  uint16_t lac;   /**< Location area code. */
  uint16_t cell_identity;  /**< Cell identity. */
  boolean cell_barred;     /**< Cell barred. */
  int16_t rssi;            /**< Cell Rx measurement. Values range between 0 and 63. */
  int gprs_support;        /**< Informs if PS is supported. */
}qapi_Gsm_Cell_Info;

/**
 QAPI cell scan information for LTE structure.
*/
typedef struct 
{
  uint32_t earfcn; /**<   Extended absolute RF channel number. */
  uint16_t phy_cell_id;  /**<   Physical cell ID. */
  uint32_t plmn_len;  /**< Must be set to number of elements in MCC and MNC. */
  uint16_t mcc[QAPI_PCI_SCAN_MAX_NUM_PLMN];  /**< List of associated MCC. */
  uint16_t mnc[QAPI_PCI_SCAN_MAX_NUM_PLMN];  /**< List of associated MNC. */
  uint16_t tac;  /**< Tracking area code (only applicable for LTE).*/
  uint32_t cell_id;   /**< Global cell ID. */
  boolean cell_barred;  /**< Cell barred. */
  int16_t rsrp;  /**< Combined rsrp. */
  int16_t rsrq;  /**< Combined rsrq. */
  int bandwidth;  /**< Band indicator for the ARFCN provided. */
  int16_t rssi;  /**< Combined rssi. @newpagetable */
}qapi_Lte_Cell_Info;

/**
 QAPI cell scan information structure.
*/
typedef struct
{
  size_t size;    /**< Size of the structure. */
  uint32_t cinfo_len; /**< Set to number of elements in cell search information. */
  qapi_Gsm_Cell_Info g_cinfo[QAPI_CELL_SEARCH_MAX_NUM];  /**< GERAN cell search information. */
  qapi_Lte_Cell_Info lte_cinfo[QAPI_CELL_SEARCH_MAX_NUM]; /**< LTE cell search information. */
}qapi_Cell_Info;


/** Device information PIN ID types. */
typedef enum {
  QAPI_DEVICE_INFO_PIN_ID_PIN1 = 1,   /**< Pin1. */
  QAPI_DEVICE_INFO_PIN_ID_PIN2 ,      /**< Pin2. */
  QAPI_DEVICE_INFO_PIN_ID_UNIVERSER_PIN , /**< Universer pin. */
}qapi_Device_Info_Pin_Id_t;

#define MAX_PIN_VALUE_LENGTH 8

/**
 QAPI verify PIN request structure.
*/
typedef struct {
  qapi_Device_Info_Pin_Id_t pin_id;      /**< Pin ID to verify. */  
  char                      pin_value[MAX_PIN_VALUE_LENGTH + 1];  /**< Pin value to verify. */  
}qapi_Device_Info_Req_Verify_Pin_t;

/** Device information UIM error types. */
typedef enum {
  QAPI_DEVICE_INFO_UIM_NO_ERROR,	       /**< No UIM error return. */  
  QAPI_DEVICE_INFO_UIM_INCORRECT_PIN,      /**< Incorrent PIN input. */  
  QAPI_DEVICE_INFO_UIM_PIN_BLOCKED,        /**< PIN is blocked. */  
  QAPI_DEVICE_INFO_UIM_PIN_PERM_BLOCKED,   /**< PIN is blocked permanently. */  
  QAPI_DEVICE_INFO_UIM_ACCESS_DENIED,      /**< Access denied from UIM. */  
  QAPI_DEVICE_INFO_UIM_INTERNAL_ERROR,     /**< Internal error. @newpage */  
}qapi_Device_Info_UIM_Response_code;

/**
 QAPI change PIN request structure.
*/
typedef struct {
  qapi_Device_Info_Pin_Id_t pin_id;       /**< Pin ID to change. */  
  char                      old_pin_value[MAX_PIN_VALUE_LENGTH + 1];   /**< Old pin value. */  
  char                      new_pin_value[MAX_PIN_VALUE_LENGTH + 1];   /**< New pin value. */  
}qapi_Device_Info_Req_Change_Pin_t;

/**
 QAPI unblock PIN request structure.
*/
typedef struct {
  qapi_Device_Info_Pin_Id_t pin_id;     /**< Pin ID to unblock. */ 
  char                      puk_value    [MAX_PIN_VALUE_LENGTH + 1];  /**< PUK pin value to unblock PIN. */  
  char                      new_pin_value[MAX_PIN_VALUE_LENGTH + 1];  /**< New pin value. @newpagetable */
}qapi_Device_Info_Req_Unblock_Pin_t;

/**
 QAPI protect PIN request structure.
*/
typedef struct {
  qapi_Device_Info_Pin_Id_t pin_id;  /**< Pin ID. */ 
  char                      pin_value[MAX_PIN_VALUE_LENGTH + 1];	/**< Pin value. */
  uint8_t                   isEnablePin;  /**< Enable/disable PIN operation. TRUE means Enable PIN. */
}qapi_Device_Info_Req_Protect_Pin_t;

/**
 QAPI set FDN status request structure.
*/
typedef struct {
  uint8_t                   isEnableFDN;  /**< Enable/disable FDN. */ 
}qapi_Device_Info_Req_Set_FDN_t;

/** Device information FDN status types. */
typedef enum 
{
  QAPI_DEVICE_INFO_FDN_NOT_AVAILABLE,      /**< FDN is not availlable. */ 
  QAPI_DEVICE_INFO_FDN_AVAILABLE_DISABLED, /**< FDN is available but disabled. */ 
  QAPI_DEVICE_INFO_FDN_AVAILABLE_ENABLED,  /**< FDN is available and enabled. */ 
}qapi_Device_Info_FDN_Status_t;

/**
 QAPI device information APDU command structure.
*/
typedef struct {
  uint8_t  apdu_cmd_length;                 /**< APDU command data length. */
  uint8_t* apdu_cmd_data;                   /**< APDU command data. */
}qapi_Device_Info_APDU_Command_t;

/**
 QAPI device information UIM response structure.
*/
typedef struct {
  qapi_Device_Info_UIM_Response_code error;  /**< UIM response code. */
  uint8_t card_result_sw1;                   /**< SW1 status word1 received from the card. */
  uint8_t card_result_sw2;                   /**< SW2 status word2 received from the card. @newpagetable */
}qapi_Device_Info_UIM_Response_t;

/**
 QAPI device information UIM PIN response structure.
*/
typedef struct {
  qapi_Device_Info_UIM_Response_t  uim_response;  /**< UIM response data. */
  uint8_t retries_left_valid;                     /**< Indicates if verify_left/unblock_left are valid. */
  uint8_t verify_left;                            /**< Number of remaining attempts to verify the PIN.*/
  uint8_t unblock_left;                           /**< Number of remaining attempts to unblock the PIN.*/
}qapi_Device_Info_Pin_Response_t;

/**
 QAPI device information FDN get response structure.
*/
typedef struct {
  qapi_Device_Info_FDN_Status_t    fdn_status;   /**< FDN status value. */
}qapi_Device_Info_Get_FDN_Rsponse_t;

/**
 QAPI device information APDU command response structure.
*/
typedef struct {
  qapi_Device_Info_UIM_Response_t uim_response;  /**< UIM response data. */
  uint8_t  apdu_resp_length;                     /**< APDU command response data length. */
  uint8_t* apdu_resp_data;                       /**< APDU command response data. @newpagetable */
}qapi_Device_Info_APDU_Response_t;

/**
 QAPI device information Preferred PLMN data structure.
*/
typedef struct qapi_Device_Info_Preferred_PLMN_s{
  struct qapi_Device_Info_Preferred_PLMN_s *next; /**< Pointer to the next preferred PLMN item information. */
  uint8_t index;                                  /**< Index ID of PLMN item. */
  uint8_t plmn[3];                                /**< PLMN code. */
  uint8_t gsm_act        :1;                      /**< GSM access technology selected. */
  uint8_t gsm_compact_act:1;                      /**< GSM COMPACT access technology selected. */
  uint8_t utran_act      :1;                      /**< UTRAN access technology selected. */
  uint8_t eutran_act     :1;                      /**< EUTRAN access technology selected. */
}qapi_Device_Info_Preferred_PLMN_t;

/** Device information preferred PLMN List type types. */
typedef enum {
  QAPI_DEVICE_INFO_PREFERRED_PLMN_LIST_PLMNWACT,    /** PLMN list from EFplmnwact (User controlled PLMN selector with Access Technology). */
  QAPI_DEVICE_INFO_PREFERRED_PLMN_LIST_OPLMNWACT,   /** PLMN list from EFoplmnwact (Operator controlled PLMN selector with Access Technology). */
  QAPI_DEVICE_INFO_PREFERRED_PLMN_LIST_HPLMNWACT,   /** PLMN list from EFhplmnwact (HPLMN selector with Access Technology). */
}qapi_Device_Info_Preferred_PLMN_List_ID_t;

/**
 QAPI device information Preferred PLMN set request structure.
*/
typedef struct {
  qapi_Device_Info_Preferred_PLMN_List_ID_t list_id;    /**< PLMN List type. */
  qapi_Device_Info_Preferred_PLMN_t         plmn_data;  /**< Preferred PLMN data item. @newpagetable */
}qapi_Device_Info_Preferred_PLMN_Set_Req_t;

/**
 QAPI device information Preferred PLMN read request structure.
*/
typedef struct {
  qapi_Device_Info_Preferred_PLMN_List_ID_t list_id;   /**< PLMN List type. */
}qapi_Device_Info_Preferred_PLMN_Read_Req_t;

/**
 QAPI device information Preferred PLMN read response structure.
*/
typedef struct {
  qapi_Device_Info_UIM_Response_t uim_response;	      /**< UIM response data. */
  uint8_t total_plmn_number;                          /**< Total PLMN item number in PLMN List . */
  uint8_t valid_plmn_number;                          /**< Valid PLMN item number in PLMN List . */
  qapi_Device_Info_Preferred_PLMN_t* plmn_list_head;  /**< Valid PLMN item data list. */
}qapi_Device_Info_Preferred_PLMN_Read_Rsp_t;

#define QAPI_DEVICE_INFO_MAX_UICC_APPLICATIONS_NUMBER 5
#define QAPI_DEVICE_INFO_UICC_APPLICATIONS_MAX_LENGTH 64

/**
 QAPI device information UICC Application read response structure.
*/
typedef struct {
   qapi_Device_Info_UIM_Response_t uim_response;	 /**< UIM response data. */
   uint8_t application_recs[QAPI_DEVICE_INFO_MAX_UICC_APPLICATIONS_NUMBER][QAPI_DEVICE_INFO_UICC_APPLICATIONS_MAX_LENGTH]; /**< Records data from EFDIR file. */
   uint8_t application_rec_number;  /**< Number of records in EFDIR file. */
   uint8_t application_rec_length;  /**< Length of each record in EFDIR file. */
}qapi_Device_Info_UICC_Application_Rsp_t;

/**
 QAPI eDRX AcT type define.
*/
typedef enum {
  QAPI_DEVICE_INFO_EDRX_ACT_NOT_USED = 0,
  /**< AcT is not specified. */
  QAPI_DEVICE_INFO_EDRX_ACT_LTE_M1 = 4,
  /**< LTE Cat M1. */
  QAPI_DEVICE_INFO_EDRX_ACT_LTE_NB1 = 5,
  /**< LTE Cat NB1. */
}qapi_Device_Info_EDRX_Act_Type_t;

#define QAPI_DEVICE_INFO_EDRX_VALUE_NOT_USED 0xFF
  /**< edrx_value or paging_time_window not used. */

/**
 EDRX set request parameter, map to #QAPI_DEVICE_INFO_REQ_EDRX_SET_E ID.
*/
typedef struct {
  boolean edrx_mode; 
  /**< 0 - disable, 1 - enable. */
  qapi_Device_Info_EDRX_Act_Type_t act_type;
  uint8_t edrx_value; 
  /**< Bit-4 to 1 of octet3 of the extended DRX parameter information element. See subclause 10.5.5.32 of 24.008, scope 0 ~ 0x0F. */
}qapi_Device_Info_Req_EDRX_Set_t;

/**
 EDRX gets request parameter and maps to #QAPI_DEVICE_INFO_REQ_EDRX_GET_E ID.
*/
typedef struct {
  qapi_Device_Info_EDRX_Act_Type_t act_type; /**< EDRX LTE Access technology type. */
}qapi_Device_Info_Req_EDRX_Get_t;

/**
 EDRX gets reponse parameter and maps to #QAPI_DEVICE_INFO_REQ_EDRX_GET_E ID.
*/
typedef struct {
  boolean edrx_mode; 
  /**< 0 - disable, 1 - enable. */
  qapi_Device_Info_EDRX_Act_Type_t act_type;
  uint8_t edrx_value; 
  /**< Bit-4 to 1 of octet3 of the extended DRX parameter information element. See subclause 10.5.5.32 of 24.008, scope 0 ~ 0x0F. */
  uint8_t paging_time_window; 
  /**< Bit-8 to 5 of octet3 of the extended DRX parameter information element. See subclause 10.5.5.32 of 24.008, scope 0 ~ 0x0F. @newpagetable */
}qapi_Device_Info_Rsp_EDRX_Get_t;


/** Device information RAT types. */
typedef enum {
  QAPI_DEVICE_INFO_LTE_NB1 = 1,  /**< LTE NB1. */
  QAPI_DEVICE_INFO_LTE_M1,       /**< LTE M1. */
  QAPI_DEVICE_INFO_GSM,          /**< GSM. */
  QAPI_DEVICE_INFO_INTRA_FREQ,	 /**< LTE intrafrequency. */
  QAPI_DEVICE_INFO_INTER_FREQ,	 /**< LTE interfrequency. */
  QAPI_DEVICE_INFO_LTE_GSM,		 /**< LTE - GSM. */
  QAPI_DEVICE_INFO_LTE_WCDMA,	 /**< LTE - WCDMA. */
  QAPI_DEVICE_INFO_WCDMA_LTE	 /**< WCDMA - LTE. @newpagetable */
}qapi_Device_Info_RAT_Type_t;


/**
 QAPI device information jamming status gets response structure.
*/
typedef struct {
  qapi_Device_Info_RAT_Type_t rat_type; /**< RAT type. */
  uint8_t jammer_status_valid;      	/**< Must be set to true if jammer_status is being passed. */
  uint16_t jammer_status;          	    /**< Jammer_status. */
  uint8_t soft_jammer_flag_valid;   	/**< Must be set to true if soft_jammer_flag is being passed. */
  uint8_t soft_jammer_flag;         	/**< Soft jammer flag. */
}qapi_Device_Info_Jamming_Status_Get_Rsp_t;


/**
 QAPI device information jamming status gets request structure.
*/
typedef struct {
  qapi_Device_Info_RAT_Type_t rat_type; /**< RAT type. */
}qapi_Device_Info_Jamming_Status_Get_Req_t;

/**
 ims_iot indication register request structure.
*/
typedef struct {
  bool   srv_status_change_ind;
  /**< Flag to register for service status change indications:\n
       - Set FALSE to deregister service change indications.
       - Set TRUE to register for service change indications.
  */
}qapi_Device_Info_IMS_IOT_Ind_Register_Req_t;


/**
 ims_iot registration status.
*/
typedef enum {
  QAPI_DEVICE_INFO_IMS_IOT_STATUS_MIN_V01 = 0,                /**< Invalid registration status for IMS. */
  QAPI_DEVICE_INFO_IMS_IOT_STATUS_NOT_REGISTERED_V01 = 1,     /**< Not registered or registration failed state for IMS. */
  QAPI_DEVICE_INFO_IMS_IOT_STATUS_REGISTERING_V01 = 2,        /**< Registering for IMS. */
  QAPI_DEVICE_INFO_IMS_IOT_STATUS_REGISTERED_V01 = 3,         /**< Registered for IMS. */
  QAPI_DEVICE_INFO_IMS_IOT_STATUS_LIMITED_REGISTERED_V01 = 4, /**< Limited registration for IMS. */
  QAPI_DEVICE_INFO_IMS_IOT_STATUS_MAX_V01 = 5,                /**< Invalid registration status for IMS. */
}qapi_Device_Info_IMS_Iot_Reg_Status_t;

/**
 QAPI device information IMS registration response structure.
*/
typedef struct {
  qapi_Device_Info_IMS_Iot_Reg_Status_t registration_status; /**< IMS registration status. @newpagetable */
}qapi_Device_Info_IMS_Registration_Status_Rsp_t;

/**
 Voice call reject cause.
*/
typedef enum {
  QAPI_DEVICE_INFO_Call_REJECT_CAUSE_INVALID             = 0x00,
  /**< Invalid value. */
  QAPI_DEVICE_INFO_Call_REJECT_CAUSE_USER_BUSY           = 0x01,
  /**< User is busy. */
  QAPI_DEVICE_INFO_Call_REJECT_CAUSE_USER_REJECT         = 0x02,
  /**< User rejected the call. */
  QAPI_DEVICE_INFO_Call_REJECT_CAUSE_LOW_BATTERY         = 0x03,
  /**< Call was rejected because of a low battery. */
  QAPI_DEVICE_INFO_Call_REJECT_CAUSE_BLACKLISTED_CALL_ID = 0x04,
  /**< Call was rejected because the number was blacklisted. */
  QAPI_DEVICE_INFO_Call_REJECT_CAUSE_DEAD_BATTERY        = 0x05,
  /**< Call was rejected because of a dead battery. */
  QAPI_DEVICE_INFO_Call_REJECT_CAUSE_UNWANTED_CALL       = 0x06,
  /**< Call was rejected because the received call was an unwanted robocall. */
}qapi_Device_Info_Call_Reject_Cause_t;

/**
 Voice call state.
*/
typedef enum {
  QAPI_DEVICE_INFO_CALL_STATE_ORIGINATION = 0x01,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_INCOMING = 0x02,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_CONVERSATION = 0x03,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_CC_IN_PROGRESS = 0x04,
  /**< Call is originating but waiting for call control to complete. */
  QAPI_DEVICE_INFO_CALL_STATE_ALERTING = 0x05,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_HOLD = 0x06,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_WAITING = 0x07,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_DISCONNECTING = 0x08,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_END = 0x09,
  /**< */
  QAPI_DEVICE_INFO_CALL_STATE_SETUP = 0x0A,
  /**< MT call is in Setup state in 3GPP. @newpage */
}qapi_Device_info_Call_State_t;

/**
 Voice call redirection.
*/
typedef enum {
  QAPI_DEVICE_INFO_CALL_DIRECTION_MO = 0x01, /**< Mobile originating call (originating a call). */
  QAPI_DEVICE_INFO_CALL_DIRECTION_MT = 0x02, /**< Mobile terminating call (receiving a call). */
}qapi_Device_info_Call_Direction_t;

/**
 QAPI device information voice call response structure.
*/
typedef struct {
  uint8_t call_id;                              /**< Unique call identifier for the call. */
  uint8_t call_state_valid;                     /**< Must be set to TRUE if call_state is being passed. */
  qapi_Device_info_Call_State_t call_state;     /**< Call state. */
  uint8_t direction_valid;                      /**< Must be set to TRUE if direction is being passed. */
  qapi_Device_info_Call_Direction_t direction;  /**< Call redirection, MO or MT. @newpagetable */
}qapi_Device_Info_Call_Status_Rsp_t;

/**
* QAPI Device Info Scan Config Power Inc Type.
**/
typedef enum {
  QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_TYPE_ENUM_MIN_ENUM_VAL = -2147483647,  /**< Forces a 32-bit signed enum. Do not change or use. */
  QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_STATIC = 0x00,                         /**< Power save duration remains static. */
  QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_LINEAR = 0x01,                         /**< Power save duration is incremented linearly. */
  QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_EXP = 0x02,                            /**< Power save duration is incremented exponentially  */
  QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_TYPE_ENUM_MAX_ENUM_VAL = 2147483647    /**< Forces a 32-bit signed enum. Do not change or use. */
}qapi_Device_Info_Power_Save_Duration_Inc_Type_t;

/**
* QAPI device info scan configuration response structure.
**/
typedef struct {
  uint8_t scan_counter_valid;  /**< Must be set to TRUE if scan_counter is being passed. */
  uint8_t scan_counter;        /**< The number of times a device shall scan all rats in
                                    the rat priority list before entering power save. */
  uint8_t power_save_duration_min_valid;  /**< Must be set to TRUE if power_save_duration_min is being passed. */
  uint32_t power_save_duration_min;       /**< Value in seconds. The initial duration for which the device 
                                               will stop scanning if no service is found after scanning for a 
											   number of attempts defined by scan_counter. */

  uint8_t power_save_duration_inc_type_valid;  /**< Must be set to TRUE if power_save_duration_inc_type is being passed. */
  qapi_Device_Info_Power_Save_Duration_Inc_Type_t power_save_duration_inc_type;  /**< Information on how the power duration value increases values: \n
      - QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_STATIC (0x00) --  Power save duration remains static. \n 
      - QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_LINEAR (0x01) --  Power save duration gets incremented linearly. \n 
      - QAPI_DEVICE_INFO_POWER_SAVE_DURATION_INC_EXP (0x02) --  Power save duration gets incremented exponentially. 
 */
  uint8_t power_save_duration_max_valid;  /**< Must be set to TRUE if power_save_duration_max is being passed. */
  uint32_t power_save_duration_max;       /**< Value in seconds. The maximum duration for which the device 
                                               will stop scanning if no service is found after scanning for a 
											   number of attempts defined by scan_counter. */
  uint8_t hplmn_scan_interval_valid;      /**< Must be set to TRUE if hplmn_scan_interval is being passed. */
  uint32_t hplmn_scan_interval;           /**< Value in minutes. The duration after which device will 
                                               start to look for HPLMN when camped on a roaming network. */
  uint8_t ciot_pref_rat_scan_interval_valid;  /**< Must be set to TRUE if ciot_pref_rat_scan_interval is being passed. */
  uint32_t ciot_pref_rat_scan_interval;       /**< Value in minutes. The duration after which device will start to 
                                                   look for a more preferred CIOT RAT when camped on a less preferred RAT in home. @newpagetable */

}qapi_Device_Info_Scan_Config_Rsp_t;

#define MAX_CALL_NUM_LENGTH 81
#define CALL_TYPE_VOICE_IP  0x02

/**
 QAPI device information voice call request structure.
*/
typedef struct {
  voice_call_req_type call_req_type;/**< VoLTE call request. */
  union {
    /*When call_req_type is QAPI_DEVICE_INFO_VOICE_DIAL_CALL_REQ */
    char      calling_number[MAX_CALL_NUM_LENGTH + 1];             /**< VoLTE calling number. */
    /*when call_req_type is QAPI_DEVICE_INFO_VOICE_END_CALL_REQ*/
    struct
    {
      uint8_t             call_id;                        /**< VoLTE call ID. */
      qapi_Device_Info_Call_Reject_Cause_t reject_cause;  /**< Call end cause. @newpagetable */
    }endCallReq; /**< End call request structure. */
    /*when call_req_type is QAPI_DEVICE_INFO_VOICE_ANSWER_CALL_REQ*/
    struct
    {
      uint8_t             call_id;                        /**< VoLTE call ID. */
      bool                reject_call;                    /**< Set to FALSE to anwser call, TRUE to reject call. */
      qapi_Device_Info_Call_Reject_Cause_t reject_cause;  /**< Call end cause. */
    }answerCallReq; /**< Answer call request structure. */
  }callReq; /**< Union of call request. */
}qapi_Device_Info_Call_Req_t;

#define QAPI_DEVICE_INFO_APN_NAME_MAX 150  /**< Maximum APN name length. */
#define QAPI_DEVICE_INFO_USERNAME     127  /**< Maximum user name length. */
#define QAPI_DEVICE_INFO_PASSWORD     127  /**< Maximum password length. */

#define QAPI_MAX_ERR_CODE   __QAPI_ERROR(QAPI_MOD_BASE, (-16))
/**< General error code maximum value. */

/**
 QAPI Device information data profile error types.
*/
typedef enum {
  QAPI_DEVICE_INFO_PDP_NO_ERROR,
  /**< No PDP error return. */  
  QAPI_DEVICE_INFO_PDP_FAIL = (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE)),
  /**< Internal failure. */  
  QAPI_DEVICE_INFO_PDP_ERR_INVAL_IDENT= (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 1)),
  /**< Invalid identifier. */  
  QAPI_DEVICE_INFO_PDP_ERR_INVAL= (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 2)),
  /**< Invalid arguments. */  
  QAPI_DEVICE_INFO_PDP_ERR_OUT_OF_PROFILES= (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 3)),
  /**< No profile available while create. */  
  QAPI_DEVICE_INFO_PDP_ERR_OUT_OF_MEMORY= (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 4)),
  /**< Out of memory. */  
  QAPI_DEVICE_INFO_PDP_ERR_FILE_ACCESS= (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 5)),
  /**< Error accessing while EFS. */  
  QAPI_DEVICE_INFO_PDP_ERR_EOF= (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 6)),
  /**< Error end of file. */  
  QAPI_DEVICE_INFO_PDP_PROFILE_ALREADY_PRESENT= (__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 7))
  /**< Profile with same IP + APN name is present. @newpage */  
}qapi_Device_Info_PDP_Response_code;

/**
 QAPI Device information system setting error types.
*/
#define QAPI_DEVICE_INFO_ERR_BAND_NOT_SUPPORTED   ((qapi_Status_t)(__QAPI_ERROR(QAPI_MOD_BASE, QAPI_MAX_ERR_CODE + 8))) /**< Band is not supported. */

/**
 QAPI Device information PDP type.
*/
typedef enum {
  QAPI_DEVICE_INFO_PDP_MIN_VAL    = -2147483647,
  /**< Minimum value. */
  QAPI_DEVICE_INFO_PDP_IPV4_VAL   = 0x00,
  /**< IPv4 family type. */
  QAPI_DEVICE_INFO_PDP_PPP_VAL    = 0x01,
  /**< PPP family type. */
  QAPI_DEVICE_INFO_PDP_IPV6_VAL   = 0x02,
  /**< IPv6 family type. */
  QAPI_DEVICE_INFO_PDP_IPV4V6_VAL = 0x03,
  /**< IPv4v6 family type. */
  QAPI_DEVICE_INFO_PDP_NON_IP_VAL = 0x04,
  /**< Non_IP family type. */
  QAPI_DEVICE_INFO_PDP_MAX_VAL    = 2147483647,
  /**< Maximum value. */
}qapi_Device_Info_PDP_Type_e;

/**
 QAPI Device information authentication preference type.
*/
typedef enum {
  QAPI_DEVICE_INFO_PAP_CHAP_NOT_ALLOWED_E,
  /**< Neither of the authentication protocols (PAP, CHAP) are allowed. */  
  QAPI_DEVICE_INFO_PAP_ONLY_ALLOWED_E,
  /**< Only PAP authentication protocol is allowed. */
  QAPI_DEVICE_INFO_CHAP_ONLY_ALLOWED_E,
  /**< Only CHAP authentication protocol is allowed. */
  QAPI_DEVICE_INFO_PAP_CHAP_BOTH_ALLOWED_E
  /**< Both PAP and CHAP authentication protocols are allowed. @newpage */
} qapi_Device_Info_Auth_Type_e;

/**
 QAPI Device information signal strength option based on network type.
*/

typedef enum {  
  QAPI_DEVICE_INFO_GSM_RSSI = 1,     /**< */      
  QAPI_DEVICE_INFO_WCDMA_RSSI,       /**< */
  QAPI_DEVICE_INFO_LTE_RSRP,         /**< */
  QAPI_DEVICE_INFO_LTE_RSRQ,         /**< */
}qapi_Device_Info_Signal_Strength_Option_t;


/**
 QAPI Device information signal strength value type.
*/

typedef enum {  
  QAPI_DEVICE_INFO_DELTA_VALUE = 1,        /**< */      
  QAPI_DEVICE_INFO_THRESHOLD_LIST,         /**< */
}qapi_Device_Info_Sig_Strength_Value_Type_t;


/**
 QAPI device info QCPDPP set request structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Type_e pdp_type;
  /**< PDP type (mandatory). Range (0-4). */
  bool apn_name_valid;
  /**< APN name field is set or not. */
  char apn_name[QAPI_DEVICE_INFO_APN_NAME_MAX + 1];
  /**< APN name (mandatory). */
  bool auth_type_valid;
  /**< Authentication preference field is set or not. */
  qapi_Device_Info_Auth_Type_e auth_type;
  /**< Authentication preference. Range (0-3). */
  bool username_valid;
  /**< Username field is set or not. */
  char username[QAPI_DEVICE_INFO_USERNAME + 1];
  /**< Username. */
  bool password_valid;
  /**< Password field is set or not. */
  char password[QAPI_DEVICE_INFO_PASSWORD + 1];
  /**< Password. */
} qapi_Device_Info_Set_QCPDPP_Req_t;

/**
 QAPI device information QCPDPP set response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. @newpagetable */
} qapi_Device_Info_Set_QCPDPP_Rsp_t;

/**
 QAPI device information QCPDPP get request structure.
*/
typedef struct {
  char __placeholder;  /**< This element is a placeholder to prevent the declaration of an empty struct.*/
} qapi_Device_Info_Get_QCPDPP_Req_t;

/**
 QAPI device information QCPDPP get structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Type_e pdp_type;
  /**< PDP type (mandatory). */
  bool apn_name_valid;
  /**< APN name field is set or not. */
  char apn_name[QAPI_DEVICE_INFO_APN_NAME_MAX + 1];
  /**< APN name (mandatory). */
  bool auth_type_valid;
  /**< Authentication preference field is set or not. */
  qapi_Device_Info_Auth_Type_e auth_type;
  /**< Authentication preference. */
  bool username_valid;
  /**< Username field is set or not. */
  char username[QAPI_DEVICE_INFO_USERNAME + 1];
  /**< Username. */
} qapi_Device_Info_Get_QCPDPP_t;

/**
 QAPI device information QCPDPP get response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. */
  uint8_t  count;                               /**< Number of profiles returned. */
  qapi_Device_Info_Get_QCPDPP_t *list;          /**< List of profile settings. @newpagetable */
} qapi_Device_Info_Get_QCPDPP_Rsp_t;

/**
 QAPI device information PDP header compression type.
*/
typedef enum {
  QAPI_DEVICE_INFO_PDP_HDR_COMPR_TYPE_OFF_E,
  /**< PDP header compression is off. */
  QAPI_DEVICE_INFO_PDP_HDR_COMPR_TYPE_MANUFACTURER_E,
  /**< Manufacturer preferred compression. */
  QAPI_DEVICE_INFO_PDP_HDR_COMPR_TYPE_RFC_1144_E,
  /**< PDP header compression based on RFC2507. */
  QAPI_DEVICE_INFO_PDP_HDR_COMPR_TYPE_RFC_2507_E,
  /**< PDP header compression based on RFC1144. */
  QAPI_DEVICE_INFO_PDP_HDR_COMPR_TYPE_RFC_3095_E,
  /**< PDP header compression based on RFC3095. */
} qapi_Device_Info_PDP_Hdr_Compr_Type_e;

/**
 QAPI device information PDP data compression type.
*/
typedef enum {
  QAPI_DEVICE_INFO_PDP_DATA_COMPR_TYPE_OFF_E,
  /**< PDP data compression is off. */
  QAPI_DEVICE_INFO_PDP_DATA_COMPR_TYPE_MANUFACTURER_E,
  /**< Manufacturer preferred compression. */
  QAPI_DEVICE_INFO_PDP_DATA_COMPR_TYPE_V42_E,
  /**< V.42BIS data compression. */
  QAPI_DEVICE_INFO_PDP_DATA_COMPR_TYPE_V44_E,
  /**< V.44 data compression */
} qapi_Device_Info_PDP_Data_Compr_Type_e;

/**
 QAPI device information IP address structure.
*/
typedef struct qapi_Device_Info_Addr_s
{
  char valid_addr; /**< Indicates whether a valid address is available. */   
  union qapi_device_info_ip_address_u /** Union of IP addresses. */
  {
    uint32_t v4;            /**< Accesses IPv4 address. */
    uint64_t v6_addr64[2];  /**< Accesses IPv6 address. */
    uint32_t v6_addr32[4];  /**< Accesses IPv6 address as four 32-bit integers. */
    uint16_t v6_addr16[8];  /**< Accesses octets of the IPv6 address. */
    uint8_t  v6_addr8[16];  /**< Accesses octets of the IPv6 address as 16 8-bit integers. */
  } addr;                   /**< Union of IP addresses. */
} qapi_Device_Info_Addr_t;

/**
 QAPI device information PDP address information.
*/
typedef struct {
  qapi_Device_Info_PDP_Type_e pdp_type;              /**< PDP type (mandatory). */
  bool apn_name_valid;                               /**< APN name field is set or not. */
  char apn_name[QAPI_DEVICE_INFO_APN_NAME_MAX + 1];  /**< APN name (mandatory). */
  qapi_Device_Info_Addr_t ipv4_addr;                 /**< IPv4 address information. */
  qapi_Device_Info_Addr_t ipv6_addr;                 /**< IPv6 address information. @newpagetable */
} qapi_Device_Info_PDP_Address_t;

/**
 QAPI device information CGDCONT set request structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Address_t addr_info;
  /**< PDP type, APN name, and IP address information. PDP IP address information
       not currently supported. */
  bool d_comp_valid;
  /**< Data compression field is set or not. Range (0-3). */
  qapi_Device_Info_PDP_Data_Compr_Type_e d_comp;
  /**< Data compression type. */
  bool h_comp_valid;
  /**< Header compression field is set or not. */
  qapi_Device_Info_PDP_Hdr_Compr_Type_e h_comp;
  /**< Header compression type. Range (0-4). */
  bool ipv4AddrAlloc_valid;
  /**< IPv4AddrAlloc field is set or not. */
  uint8_t ipv4AddrAlloc;
  /**< Type of IPv4 address allocation (not supported). */
  bool delete_profile_valid;
  /**< Delete profile field is set or not. */
  bool delete_profile;
  /**< Delete profile corresponding to PDP type and APN name passed. */
} qapi_Device_Info_Set_CGDCONT_Req_t;

/**
 QAPI device information CGDCONT set response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. */
} qapi_Device_Info_Set_CGDCONT_Rsp_t;

/**
 QAPI device information CGDCONT get request structure.
*/
typedef struct {
  char __placeholder;  /**< This element is a placeholder to prevent the declaration of an empty structure. @newpagetable */
} qapi_Device_Info_Get_CGDCONT_Req_t;

/**
 QAPI device information CGDCONT get structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Address_t addr_info;
  /**< PDP type, APN name, and IP address information. PDP IP address information
       not currently supported. */
  bool d_comp_valid;
  /**< Data compression field is set or not. */
  qapi_Device_Info_PDP_Data_Compr_Type_e d_comp;
  /**< Data compression type. */
  bool h_comp_valid;
  /**< Header compression field is set or not. */
  qapi_Device_Info_PDP_Hdr_Compr_Type_e h_comp;
  /**< Header compression type. */
  bool ipv4AddrAlloc_valid;
  /**< IPv4AddrAlloc field is set or not. */
  uint8_t ipv4AddrAlloc;
  /**< Type of IPv4 address allocation. Always returns 0. */
} qapi_Device_Info_Get_CGDCONT_t;

/**
 QAPI device information CGDCONT get response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. */
  uint8_t  count;                               /**< Number of profiles returned. */
  qapi_Device_Info_Get_CGDCONT_t *list;         /**< List of profile settings. @newpagetable */
} qapi_Device_Info_Get_CGDCONT_Rsp_t;

/**
 QAPI device information QCPDPIMSCFGE set request structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Type_e pdp_type;              /**< PDP type (mandatory). */
  bool apn_name_valid;                               /**< APN name field is set or not. */
  char apn_name[QAPI_DEVICE_INFO_APN_NAME_MAX + 1];  /**< APN name (mandatory). */
  bool address_flag_valid;                           /**< Address flag field is set or not. */
  bool address_flag;                                 /**< Address flag field (not supported). */
  bool dhcp_flag_valid;                              /**< DHCP flag field is set or not. */
  bool dhcp_flag;                                    /**< PCSCF address using PCO Flag. */
  bool cn_flag_valid;                                /**< CN flag field is set or not. */
  bool cn_flag;                                      /**< IM CN flag. */
} qapi_Device_Info_Set_QCPDPIMSCFGE_Req_t;

/**
 QAPI device information QCPDPIMSCFGE set response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. */
} qapi_Device_Info_Set_QCPDPIMSCFGE_Rsp_t;

/**
 QAPI device information QCPDPIMSCFGE get request structure.
*/
typedef struct {
  char __placeholder; 
  /**< This element is a placeholder to prevent the declaration of an empty structure. @newpagetable */
} qapi_Device_Info_Get_QCPDPIMSCFGE_Req_t;

/**
 QAPI device info QCPDPIMSCFGE get structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Type_e pdp_type;              /**< PDP type (mandatory). */
  bool apn_name_valid;                               /**< APN name field is set or not. */
  char apn_name[QAPI_DEVICE_INFO_APN_NAME_MAX + 1];  /**< APN name (mandatory). */
  bool address_flag_valid;                           /**< Address flag field is set or not. */
  bool address_flag;                                 /**< Address flag field (not supported). */
  bool dhcp_flag_valid;                              /**< DHCP flag field is set or not. */
  bool dhcp_flag;                                    /**< PCSCF address using PCO Flag. */
  bool cn_flag_valid;                                /**< CN flag field is set or not. */
  bool cn_flag;                                      /**< IM CN flag. */
} qapi_Device_Info_Get_QCPDPIMSCFGE_t;

/**
 QAPI device information QCPDPIMSCFGE get response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. */
  uint8_t  count;                               /**< Number of profiles returned. */
  qapi_Device_Info_Get_QCPDPIMSCFGE_t *list;    /**< List of profile settings. */
} qapi_Device_Info_Get_QCPDPIMSCFGE_Rsp_t;


#define QAPI_DEVICE_INFO_APN_BEARER_G 0x01
#define QAPI_DEVICE_INFO_APN_BEARER_W  0x02
#define QAPI_DEVICE_INFO_APN_BEARER_L  0x04
#define QAPI_DEVICE_INFO_APN_BEARER_ANY 0x8000000000000000

/**
 QAPI device information MNC structure.
*/
typedef struct {
  uint16_t mnc;                    /**< A 16-bit integer representation of MNC. */
  uint8_t mnc_includes_pcs_digit;  /**< MNC PCS digit include status. @newpagetable */
} qapi_Device_Info_MNC_t;

/**
 QAPI device information QCPDPCFGE set request structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Type_e pdp_type;
  /**< PDP type (mandatory). */
  bool apn_name_valid;
  /**< APN name field is set or not. */
  char apn_name[QAPI_DEVICE_INFO_APN_NAME_MAX + 1];
  /**< APN name (mandatory). */
  bool apn_disable_flag_valid;
  /**< APN disable flag field is set or not. */
  bool apn_disable_flag;
  /**< APN disabled flag. */
  bool timer_value_valid;
  /**< Timer value field is set or not. */
  uint32_t timer_value;
  /**< PDN inactivity timeout (in seconds). Range (0-122820). */
  bool apn_class_valid;
  /**< APN class field is set or not. */
  uint8_t apn_class;
  /**< APN class. Range (0-16). */
  bool apn_bearer_valid;
  /**< APN bearer field is set or not. */
  uint64_t apn_bearer;
  /**< APN bearer. */
  bool max_pdn_conn_per_block_valid;
  /**< Maximum PDN connections per block field is set or not. */
  uint16_t max_pdn_conn_per_block;
  /**< Maximum PDN connections per time block. Range (0-1023). */
  bool max_pdn_conn_timer_valid;
  /**< Maximum PDN connections timer field is set or not. */
  uint16_t max_pdn_conn_timer;
  /**< Maximum PDN connections timer (in secs). Range (0-3600). */
  bool pdn_req_wait_timer_valid;
  /**< PDN request wait timer field is set or not. */
  uint16_t pdn_req_wait_timer;
  /**< PDN request wait timer (in seconds). Range (0-1023). */
  bool emergency_calls_supported_valid;
  /**< Emergency call supported field is set or not. */
  bool emergency_calls_supported;
  /**< Emergency call (not supported). */
  bool operator_reserved_pco_valid;
  /**< Operator reserverd PCO field is set or not. */
  uint16_t operator_reserved_pco;
  /**<  Operator reserved PCO ID. Range (65280-65535). */
  bool mcc_valid;
  /**< MCC field is set or not. */
  uint16_t mcc;
  /**< Mobile Country Code. Range (0-999). */
  bool mnc_valid;
  /**< MNC field is set or not. */
  qapi_Device_Info_MNC_t mnc;
  /**< Mobile Network Code. Range (0-999). */
} qapi_Device_Info_Set_QCPDPCFGE_Req_t;

/**
 QAPI device information QCPDPCFGE get request structure.
*/
typedef struct {
  char __placeholder; 
  /**< This element is a placeholder to prevent the declaration of an empty structure. */
} qapi_Device_Info_Get_QCPDPCFGE_Req_t;

/**
 QAPI device information QCPDPCFGE set response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. @newpagetable */
} qapi_Device_Info_Set_QCPDPCFGE_Rsp_t;

/**
 QAPI device information QCPDPCFGE get structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Type_e pdp_type;
  /**< PDP type (mandatory). */
  bool apn_name_valid;
  /**< APN name field is set or not. */
  char apn_name[QAPI_DEVICE_INFO_APN_NAME_MAX + 1];
  /**< APN name (mandatory). */
  bool apn_disable_flag_valid;
  /**< APN disable flag field is set or not. */
  bool apn_disable_flag;
  /**< APN disabled flag. */
  bool timer_value_valid;
  /**< Timer value field is set or not. */
  uint32_t timer_value;
  /**< PDN inactivity temout. */
  bool apn_class_valid;
  /**< APN class field is set or not. */
  uint8_t apn_class;
  /**< APN class. */
  bool apn_bearer_valid;
  /**< APN bearer field is set or not. */
  uint64_t apn_bearer;
  /**< APN bearer. */
  bool max_pdn_conn_per_block_valid;
  /**< Maximum PDN connections per block field is set or not. */
  uint16_t max_pdn_conn_per_block;
  /**< Maximum PDN connections per time block. */
  bool max_pdn_conn_timer_valid;
  /**< Maximum PDN connections timer field is set or not. */
  uint16_t max_pdn_conn_timer;
  /**< Maximum PDN connections timer. */
  bool pdn_req_wait_timer_valid;
  /**< PDN request wait timer field is set or not. */
  uint16_t pdn_req_wait_timer;
  /**< PDN request wait timer. */
  bool emergency_calls_supported_valid;
  /**< Emergency call supported field is set or not. */
  bool emergency_calls_supported;
  /**< Emergency call (not supported). */
  bool operator_reserved_pco_valid;
  /**< Operator reserverd PCO field is set or not. */
  uint16_t operator_reserved_pco;
  /**<  Operator reserved PCO ID. */
  bool mcc_valid;
  /**< MCC field is set or not. */
  uint16_t mcc;
  /**< Mobile Country Code. */
  bool mnc_valid;
  /**< MNC field is set or not. */
  qapi_Device_Info_MNC_t mnc;
  /**< Mobile Network Code. @newpagetable */
} qapi_Device_Info_Get_QCPDPCFGE_t;

/**
 QAPI device information QCPDPCFGE get response structure.
*/
typedef struct {
  qapi_Device_Info_PDP_Response_code response;  /**< Response value. */
  uint8_t  count;                               /**< Number of profiles returned. */
  qapi_Device_Info_Get_QCPDPCFGE_t *list;       /**< List of profile settings. */
} qapi_Device_Info_Get_QCPDPCFGE_Rsp_t; 

/**
 QAPI device information FSK start request structure.
*/
typedef struct {
  /*  BAUDRATE */
  uint8_t baudrate_valid;  /**< Must be set to TRUE if baudrate is being passed. */
  uint8_t baudrate;
 /**<  Baudrate at which FSK transmission will start. \n
    Possible values: \n
    - 0 -- 3466bps \n 
    - 1 -- 1733bps \n
    - 2 -- 866bps \n
    If TLV is not present, the default value available at GL1 will be used. \n*/

  /*  GAPTIME */
  uint8_t gaptime_valid;  /**< Must be set to TRUE if gaptime is being passed. */
  uint16_t gaptime;
  /**< Possibile values depend on baudrate value, with respect to the number of bits per TDMA frame. \n
     Maximum allowed bits: 1024 \n
     If TLV is not present, the default value available at GL1 will be used. \n*/

  /*  RESTART_MODE */
  uint8_t restart_mode_valid;  /**< Must be set to TRUE if restart_mode is being passed. */
  uint8_t restart_mode;
  /**<  - 0 -- Sends data using all channels/frequencies defined with QMI_NAS_SET_FSK_HOP_TABLE_REQ_MSG (default) \n
        - 1 -- Shall work as 0 but indicates storing the next frequency index of the hopping table when FSKSTOP command is sent. @newpagetable */
} qapi_Device_Info_FSK_Start_Req_t;

/**< Maximum FSK data to be sent. */
#define QAPI_DEVICE_INFO_FSK_MAX_BYTES_TO_SEND 200

/**
 QAPI device information FSK data request structure.
*/
typedef struct {
/* Mandatory */
/* DATA */
uint32_t data_len;                                     /**< Must be set to number of elements in data. */
uint8_t data[QAPI_DEVICE_INFO_FSK_MAX_BYTES_TO_SEND];  /**< FSK Data to be sent to GERAN. */
} qapi_Device_Info_FSK_Data_Req_t;

/**
 QAPI device information get FSK debug parameters response structure.
*/
typedef struct {
  /*  DELTA */
  uint8_t delta_valid;  /**< Must be set to TRUE if delta is being passed. */
  uint8_t delta;        /**< Delta of frequency (in kHz). Possible values: 1, 2, 3, 5.
  							 If TLV is not present, the default available at GL1 will be used. */

  /*  FREQ */
  uint8_t freq_valid;  /**< Must be set to TRUE if frequency is being passed. */
  uint32_t freq;
  /**<  Frequency for debugging. Possible values: 902000-928000, 966037-967957. */

  /*  BAUDRATE */
  uint8_t baudrate_valid;  /**< Must be set to TRUE if baudrate is being passed. */
  uint8_t baudrate;
  /**<  Baudrate at which FSK transmission will start. \n
    Possible values: \n
    - 0 -- 3466bps \n 
    - 1 -- 1733bps\n
    - 2 -- 866bps \n
    If TLV is not present, the default value available at GL1 will be used. @newpagetable \n*/
} qapi_Device_Info_Get_FSK_Debug_Rsp_t;

/**
 QAPI device information set FSK debug parameters request structure.
*/
typedef struct {
  /* Mandatory */
  /*  FREQ */
  uint32_t freq;
  /**<  Frequency for debugging. Possible values: 902000-928000, 966037-967957. */

  /*  DELTA */
  uint8_t delta_valid;  /**< Must be set to TRUE if delta is being passed. */
  uint8_t delta;
  /**<  Delta of frequency (in kHz). Possible values: 1, 2, 3, 5.
        If TLV is not present, the default value available at GL1 will be used. */

  /*  BAUDRATE */
  uint8_t baudrate_valid;  /**< Must be set to TRUE if baudrate is being passed. */
  uint8_t baudrate;
  /**<  Baudrate at which FSK transmission will start. \n
    Possible values: \n
    - 0 -- 3466bps \n 
    - 1 -- 1733bps \n
    - 2 -- 866bps \n
    If TLV is not present, the default value available at GL1 will be used. */
} qapi_Device_Info_Set_FSK_Debug_Req_t;

#define QAPI_DEVICE_INFO_FSK_MAX_FREQS 50 

/**
 QAPI device information FSK hop table entry structure.
*/
typedef struct {

  uint8_t delta;
  /**< Delta of frequency (in kHz). Possible values: 1, 2, 3, 5. */

  uint32_t freq_len;  /**< Must be set to number of elements in frequency. */
  uint32_t freq[QAPI_DEVICE_INFO_FSK_MAX_FREQS]; 
  /**< Possible values: 902000-928000, 966037-967957. */
  /**< Frequencies through which the module can automatically hop (in KHz). */
}qapi_Device_Info_FSK_Hoptable_Entry_Type;

/**
 QAPI device information set hop table contents per index request structure.
*/
typedef struct {

  /* Mandatory */
  /*  INDEX */
  uint8_t index;
  /**< Index of record in hop table that must be set. Possible values: 0, 1, 2.\n*/

  /* Mandatory */
  /*  FSK_HOP_TABLE */
  qapi_Device_Info_FSK_Hoptable_Entry_Type fsk_hop_table;  /**< @newpagetable */
} qapi_Device_Info_Set_FSK_Hoptable_Req_t;

/**
 QAPI device information get hop table contents response structure.
*/
typedef struct {

  /* FSK_HOP_TABLE */
  uint8_t fsk_hop_table_valid;  /**< Must be set to TRUE if fsk_hop_table is being passed. */
  uint32_t fsk_hop_table_len;   /**< Must be set to number of elements in fsk_hop_table. */
  qapi_Device_Info_FSK_Hoptable_Entry_Type fsk_hop_table[QAPI_DEVICE_INFO_FSK_MAX_FREQS];
    /**< fsk_hop_table list. */ 
} qapi_Device_Info_Get_FSK_Hoptable_Rsp_t;

/**
 QAPI device information get PCL response structure.
*/
typedef struct {

  /*  PCL_GSM */
  uint8_t pcl_gsm_valid;  /**< Must be set to TRUE if pcl_gsm is being passed. */
  uint8_t pcl_gsm;
  /**<  PCL for GSM band. \n
        Range: (5-19, 99) \n
		Default: 5 \n
        If TLV is not present, the default value available at GL1 will be used. */

  /*  PCL_DCS */
  uint8_t pcl_dcs_valid;  /**< Must be set to TRUE if pcl_dcs is being passed. */
  uint8_t pcl_dcs;
  /**< PCL for DCS band. \n
       Range: (0-15, 99) \n
	   Default: 99. \n
       If TLV is not present, the default value available at GL1 will be used. */

  /*  PCL_PCS */
  uint8_t pcl_pcs_valid;  /**< Must be set to TRUE if pcl_pcs is being passed. */
  uint8_t pcl_pcs;
  /**< PCL for PCS band. \n
       Range: (0-15, 99) \n
	   Default: 99. \n
       If TLV is not present, the default value available at GL1 will be used. @newpagetable */
}qapi_Device_Info_Get_FSK_PCL_Rsp_t;

/**
 QAPI device info set PCL request structure.
*/
typedef struct {

  /*  PCL_GSM */
  uint8_t pcl_gsm_valid;  /**< Must be set to TRUE if pcl_gsm is being passed */
  uint8_t pcl_gsm;
  /**<  PCL for GSM band. \n
        Range: (5-19, 99) \n
		Default: 5. \n
        If TLV is not present, the default value available at GL1 will be used. */

  /*  PCL_DCL */
  uint8_t pcl_dcs_valid;  /**< Must be set to TRUE if pcl_dcs is being passed */
  uint8_t pcl_dcs;
  /**<  PCL for DCS band. \n
        Range: (0-15, 99) \n
		Default: 99 \n
        If TLV is not present, the default value available at GL1 will be used. */

  /*  PCL_PCS */
  uint8_t pcl_pcs_valid;  /**< Must be set to true if pcl_pcs is being passed. */
  uint8_t pcl_pcs;
  /**<  PCL for PCS band. \n
        Range: (0-15, 99) \n
		Default: 99 \n
        If TLV is not present, the default value available at GL1 will be used. */
}qapi_Device_Info_Set_FSK_PCL_Req_t;

/**
 QAPI device info qRxlevmin get request structure.
*/
typedef struct {
  char __placeholder;  /**< This element is a placeholder to prevent the declaration of
                            an empty structure. */
} qapi_Device_Info_Get_QRXLEVMIN_Req_t;

/**
 QAPI device info qRxlevmin get response structure.
*/
typedef struct {

  qapi_Device_Info_RAT_Type_t rat_type; /**< Radio access technology (RAT) type. */
  /*  q_rxlevmin */
  uint8_t q_rxlevmin_valid;  /**< Must be set to TRUE if q_rxlevmin is being passed. */
  int32_t q_rxlevmin;        /**< Specifies the minimum required RX level in the cell in dBm. Range: (-140,-44)db. */

  /*  q_rxlevmin_ce */
  uint8_t qrxlevmin_ce_valid;  /**< Must be set to TRUE if qrxlevmin_ce is being passed. */
  int32_t qrxlevmin_ce;        /**< Specifies the specific minimum required coverage RX level in the cell in dBm. Range: (-140, -44)db. */

  /*  q_rxlevmin_ce1 */
  uint8_t qrxlevmin_ce1_valid;  /**< Must be set to TRUE if qrxlevmin_ce1 is being passed. */
  int32_t qrxlevmin_ce1;        /**< Specifies the specific minimum required coverage RX level in the cell in dBm. Range: (-140, -44)db. */

  /*  rxlev_access_min */
  uint8_t rxlev_access_min_valid;  /**< Must be set to TRUE if rxlev_access_min is being passed. */
  uint8_t rxlev_access_min;        /**< The minimum received power at which UE is allowed to try attaching. Range: (0, 63). */

} qapi_Device_Info_Get_QRXLEVMIN_Rsp_t;

/**
 QAPI device info configuration signal strenth option type and value structure. \n

 Threshold list: Array (maximum of 32 values) of thresholds (in units of 0.1 dBm). \n 
                 For example, to set thresholds at -20 dBm and -15 dBm, 
                 the threshold list values are {-200, -150}. \n

                 - GSM RSSI   : Range for RSSI values: -111 to -48  (in dBm). \n
                 - WCDMA RSSI : Range for RSSI values: -121 to  0   (in dBm). \n
                 - LTE RSRQ   : Range for RSRQ values: -20  to -3   (in dBm). \n
                 - LTE RSRP   : Range for RSRP values: -140 to -44  (in dBm). \n

 Delta value: Delta (in units of 0.1 dBm) \n. 
              For example, to set a delta of 10 dBm, the delta value must be set to 100. \n
              A value of 0 is rejected with #QAPI_ERROR.

*/

typedef struct {
   qapi_Device_Info_Signal_Strength_Option_t sig_strength_opt; /**< Signal strength option based on network type. */
   qapi_Device_Info_Sig_Strength_Value_Type_t value_type;      /**< Signal strength value type. */
   union{
   /** Union of threshold list and delta values. */
      struct {
        uint32_t list_len;    /**< Must be set to number of elements in threshold_list. */
        int16_t  list[QAPI_DEVICE_INFO_THRESHOLD_LIST_MAX]; /**< Array of RSSI thresholds (in units of 0.1 dBm) with a maximum of 32 values. */
      } threshold;
      struct {
        uint16_t value;  /**< Delta value (in units of 0.1 dBm). */
      } delta;
   }u; /**< Union of signal strength values. */
}qapi_Device_Info_Signal_Strength_Option_Value_t; 


/**
 QAPI device info configuration signal strenth threshold request structure.
*/
typedef struct {
  uint32_t option_value_len;
  /**< Must be set to number of signal strength option need to configure based on network type. */
  qapi_Device_Info_Signal_Strength_Option_Value_t *opt_value;
  /**< List of signal strength option types and values. */
} qapi_Device_Info_Config_Signal_strength_Req_t;

/** Maximum number of neighboring frequencies for a given RAT type. */
#define QAPI_DEVICE_INFO_MAX_FREQ_COUNT 3

/** Maximum number of neighboring cells for a given frequency. */
#define QAPI_DEVICE_INFO_MAX_CELL_COUNT 8

/**
  Structure containing cell details and their corresponding srxlev values.
*/

typedef struct
{
  uint16_t cell_count;								/**< Number of cells present for the given arfcn. */
  uint16_t arfcn;									/**< Absolute radio frequency channel number. */
  uint16_t pci[QAPI_DEVICE_INFO_MAX_CELL_COUNT];	/**< Identifier for cells: \n
  														Base station ID in case of neighbouring gsm cell. \n
  														Primary scrambling code in case of neighbouring wcdma cell. \n
  														Physical Cell ID in case of LTE cell. */	
  int16_t  srxlev[QAPI_DEVICE_INFO_MAX_CELL_COUNT];	/**< Corresponding srxlev value for the cells in the above cell array. */
}qapi_Device_Info_freq_cells_info_t;

/**
 QAPI device info srxlev get request structure.
*/

typedef struct {
  qapi_Device_Info_RAT_Type_t rat_type;				/**< RAT type currently serving the UE. */
} qapi_Device_Info_Get_Srxlev_Req_t;

/**
 QAPI device info srxlev get response structure.
*/

typedef struct
{
  uint16_t freq_count;															/**< Number of frequency values received for a give RAT type. */
  qapi_Device_Info_freq_cells_info_t freqs[QAPI_DEVICE_INFO_MAX_FREQ_COUNT];	/**< Cell details for the each frequency value. */		
}qapi_Device_Info_Get_Srxlev_Rsp_t;

/**
 QAPI Device information wireless messaging service (WMS) format.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_FORMAT_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_FORMAT_CDMA = 0x00,  /**< CDMA message format. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_FORMAT_GW_PP = 0x06, /**< GW-PP message format. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_FORMAT_GW_BC = 0x07, /**< GW-BC message format. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_FORMAT_MWI = 0x08,   /**< MWI message format. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_FORMAT_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
} qapi_Device_Info_WMS_Message_Format_e;

/**
 QAPI Device information WMS storage type.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_STORAGE_TYPE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_STORAGE_TYPE_UIM = 0x00,  /**< UIM storage type. */
  QAPI_DEVICE_INFO_WMS_STORAGE_TYPE_NV = 0x01,   /**< Non-volatile storage type. */
  QAPI_DEVICE_INFO_WMS_STORAGE_TYPE_NONE = -1,   /**< No storage type. */
  QAPI_DEVICE_INFO_WMS_STORAGE_TYPE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
} qapi_Device_Info_WMS_Storage_Type_e;

/**
 QAPI Device information WMS tag type.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_TAG_TYPE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_TAG_TYPE_MT_READ = 0x00,     /**< MT read message tag.*/   
  QAPI_DEVICE_INFO_WMS_TAG_TYPE_MT_NOT_READ = 0x01, /**< MT unread message tag.*/
  QAPI_DEVICE_INFO_WMS_TAG_TYPE_MO_SENT = 0x02,     /**< MO sent message tag.*/   
  QAPI_DEVICE_INFO_WMS_TAG_TYPE_MO_NOT_SENT = 0x03, /**< MO not sent message tag.*/
  QAPI_DEVICE_INFO_WMS_MESSAGE_TAG_TYPE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
} qapi_Device_Info_WMS_Message_Tag_Type_e;

/**
 QAPI Device information WMS message mode.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_MODE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_MODE_CDMA = 0x00, /**< CDMA message mode. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_MODE_GW = 0x01,   /**< GW message mode. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_MODE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
} qapi_Device_Info_WMS_Message_Mode_e;

/**
 QAPI Device information WMS message type.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_TYPE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_TYPE_POINT_TO_POINT = 0x00,           /**< Point-to-point message type. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_TYPE_ENUM_MAX_ENUM_VAL = 2147483647   /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Message_Type_e;

/**
 QAPI Device information WMS message class.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_0 = 0x00,  /**< Class 0 message.*/
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_1 = 0x01,  /**< Class 1 message. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_2 = 0x02,  /**< Class 2 message. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_3 = 0x03,  /**< Class 3 message. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_NONE = 0x04,  /**< No class message. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_CDMA = 0x05,  /**< CDMA class message. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_CLASS_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Message_Class_e;

/**
 QAPI Device information WMS receipt action.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_RECEIPT_ACTION_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_DISCARD = 0x00,          /**< Discard received message. */
  QAPI_DEVICE_INFO_WMS_STORE_AND_NOTIFY = 0x01, /**< Store and notify received message. */
  QAPI_DEVICE_INFO_WMS_TRANSFER_ONLY = 0x02,    /**< Only transfer received message. */
  QAPI_DEVICE_INFO_WMS_TRANSFER_AND_ACK = 0x03, /**< Transfer and acknowledgement of received message. */
  QAPI_DEVICE_INFO_WMS_UNKNOWN = -1,            /**< Unknown receipt action. */
  QAPI_DEVICE_INFO_WMS_RECEIPT_ACTION_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Receipt_Action_e;

/**
 QAPI Device information WMS transfer indication.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_TRANSFER_IND_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_TRANSFER_IND_SIM = 0x00,     /**< Transfer message to SIM. */
  QAPI_DEVICE_INFO_WMS_TRANSFER_IND_CLIENT = 0x01,  /**< Transfer message to client. */
  QAPI_DEVICE_INFO_WMS_TRANSFER_IND_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Transfer_Ind_e;

/**
 QAPI Device information WMS message protocol.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_PROTOCOL_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_PROTOCOL_CDMA = 0x00,  /**< CDMA message protocol. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_PROTOCOL_WCDMA = 0x01, /**< WCDMA message protocol. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_PROTOCOL_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Message_Protocol_e;

/**
 QAPI Device information WMS Relay Layer Protocol (RP) failure cause.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_UNASSIGNED_NUMBER = 0x01, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_OPERATOR_DETERMINED_BARRING = 0x08, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_CALL_BARRED = 0x0A, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_RESERVED = 0x0B, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_SMS_TRANSFER_REJECTED = 0x15, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_MEMORY_CAP_EXCEEDED = 0x16, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_DESTINATION_OUT_OF_ORDER = 0x1B, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_UNIDENTIFIED_SUBSCRIBER = 0x1C, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_FACILITY_REJECTED = 0x1D, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_UNKNOWN_SUBSCRIBER = 0x1E, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_NETWORK_OUT_OF_ORDER = 0x26, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_TEMPORARY_FAILURE = 0x29, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_CONGESTION = 0x2A, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_RESOURCES_UNAVAILABLE = 0x2F, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_REQUESTED_FACILITY_NOT_SUBSCRIBED = 0x32, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_REQUESTED_FACILITY_NOT_IMPLEMENTED = 0x45, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_INVALID_SMS_TRANSFER_REFERENCE_VALUE = 0x51, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE = 0x5F, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_INVALID_MANDATORY_INFO = 0x60, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_MESSAGE_TYPE_NOT_IMPLEMENTED = 0x61, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_SMS = 0x62, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_INFO_ELEMENT_NOT_IMPLEMENTED = 0x63, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_PROTOCOL_ERROR = 0x6F, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_INTERWORKING = 0x7F, 
  QAPI_DEVICE_INFO_WMS_RP_CAUSE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_RP_Cause_e;

/**
 QAPI Device information WMS Transfer Layer Protocol (TP) failure cause.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_TELE_INTERWORKING_NOT_SUPPORTED = -128, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SHORT_MESSAGE_TYPE_0_NOT_SUPPORTED = -127, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SHORT_MESSAGE_CANNOT_BE_REPLACED = -126, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_UNSPECIFIED_PID_ERROR = -113, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_DCS_NOT_SUPPORTED = -112, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_MESSAGE_CLASS_NOT_SUPPORTED = -111, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_UNSPECIFIED_DCS_ERROR = -97, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_COMMAND_CANNOT_BE_ACTIONED = -96, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_COMMAND_UNSUPPORTED = -95, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_UNSPECIFIED_COMMAND_ERROR = -81, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_TPDU_NOT_SUPPORTED = -80, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SC_BUSY = -64, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_NO_SC_SUBSCRIPTION = -63, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SC_SYS_FAILURE = -62, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_INVALID_SME_ADDRESS = -61, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_DESTINATION_SME_BARRED = -60, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SM_REJECTED_OR_DUPLICATE = -59, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_TP_VPF_NOT_SUPPORTED = -58, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_TP_VP_NOT_SUPPORTED = -57, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SIM_SMS_STORAGE_FULL = -48, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_NO_SMS_STORAGE_CAP_IN_SIM = -47, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_MS_ERROR = -46, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_MEMORY_CAP_EXCEEDED = -45, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SIM_APP_TOOLKIT_BUSY = -44, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_SIM_DATA_DOWNLOAD_ERROR = -43, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_UNSPECIFIED_ERROR = -1, 
  QAPI_DEVICE_INFO_WMS_TP_CAUSE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_TP_Cause_e;

/**
 QAPI Device information cause of WMS TL failure.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_ADDR_VACANT = 0x00, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_ADDR_TRANSLATION_FAILURE = 0x01, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_NETWORK_RESOURCE_SHORTAGE = 0x02, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_NETWORK_FAILURE = 0x03, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_INVALID_TELESERVICE_ID = 0x04, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_NETWORK_OTHER = 0x05, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_NO_PAGE_RESPONSE = 0x20, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_DEST_BUSY = 0x21, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_NO_ACK = 0x22, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_DEST_RESOURCE_SHORTAGE = 0x23, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_SMS_DELIVERY_POSTPONED = 0x24, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_DEST_OUT_OF_SERV = 0x25, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_DEST_NOT_AT_ADDR = 0x26, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_DEST_OTHER = 0x27, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_RADIO_IF_RESOURCE_SHORTAGE = 0x40, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_RADIO_IF_INCOMPATABILITY = 0x41, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_RADIO_IF_OTHER = 0x42, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_ENCODING = 0x60, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_SMS_ORIG_DENIED = 0x61, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_SMS_TERM_DENIED = 0x62, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_SUPP_SERV_NOT_SUPP = 0x63, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_SMS_NOT_SUPP = 0x64, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_MISSING_EXPECTED_PARAM = 0x65, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_MISSING_MAND_PARAM = 0x66,
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_UNRECOGNIZED_PARAM_VAL = 0x67, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_UNEXPECTED_PARAM_VAL= 0x68, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_USER_DATA_SIZE_ERR = 0x69, 
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_GENERAL_OTHER = 0x6A,       
  QAPI_DEVICE_INFO_WMS_TL_CAUSE_CODE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_TL_Cause_Code_e;

/**
 QAPI Device information WMS error class.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_ERROR_CLASS_SEND_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_ERROR_CLASS_TEMPORARY = 0x00, /**< Temporary message send error. */
  QAPI_DEVICE_INFO_WMS_ERROR_CLASS_PERMANENT = 0x01, /**< Permanent message send error. */
  QAPI_DEVICE_INFO_WMS_ERROR_CLASS_SEND_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Error_Class_Send_e;

/**
 QAPI Device information WMS message delivery failure type.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_DELIVERY_FAILURE_TYPE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_DELIVERY_FAILURE_TEMPORARY = 0x00, /**< Temporary message delivery failure. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_DELIVERY_FAILURE_PERMANENT = 0x01, /**< Permanent message delivery failure. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_DELIVERY_FAILURE_TYPE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Message_Delivery_Failure_Type_e;

/**
 QAPI Device information cause of WMS message delivery failure.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_MESSAGE_DELIVERY_FAILURE_CAUSE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_BLOCKED_DUE_TO_CALL_CONTROL = 0x00,  /**< Delivery failure due to call control. */
  QAPI_DEVICE_INFO_WMS_MESSAGE_DELIVERY_FAILURE_CAUSE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Message_Delivery_Failure_Cause_e;

/**
 QAPI Device information cause of WMS acknowledgement failure.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_ACK_FAILURE_CAUSE_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_ACK_FAILURE_NO_NETWORK_RESPONSE = 0x00,   /**< No network response acknowledgement failure. */
  QAPI_DEVICE_INFO_WMS_ACK_FAILURE_NETWORK_RELEASED_LINK = 0x01, /**< Network released acknowledgement failure. */
  QAPI_DEVICE_INFO_WMS_ACK_FAILURE_ACK_NOT_SENT = 0x02,          /**< Acknowledgement not sent failure. */
  QAPI_DEVICE_INFO_WMS_ACK_FAILURE_CAUSE_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
} qapi_Device_Info_WMS_Ack_Failure_Cause_e;

/**
 QAPI Device information WMS acknowledgement indicator.
*/
typedef enum {
  QAPI_DEVICE_INFO_WMS_ACK_INDICATOR_ENUM_MIN_ENUM_VAL = -2147483647, /**< Minimum value. Do not use. */
  QAPI_DEVICE_INFO_WMS_ACK_INDICATOR_SEND_ACK = 0x00,        /**< Client must send acknowledgement. */
  QAPI_DEVICE_INFO_WMS_ACK_INDICATOR_DO_NOT_SEND_ACK = 0x01, /**< Client does not need to send acknowledgement. */
  QAPI_DEVICE_INFO_WMS_ACK_INDICATOR_ENUM_MAX_ENUM_VAL = 2147483647 /**< Maximum value. Do not use. */
}qapi_Device_Info_WMS_Ack_Indicator_e;


#define QAPI_DEVICE_INFO_WMS_ALPHA_ID_LENGTH_MAX 255 
/**< Maximum WMS alpha ID length. */

#define QAPI_DEVICE_INFO_WMS_ADDRESS_DIGIT_MAX 21
/**< Maxixmum WMS address digit. */

#define QAPI_DEVICE_INFO_WMS_ADDRESS_TYPE_MAX 3
/**< Maximum WMS address type. */

/**
 QAPI Device information WMS call control modified information.
*/
typedef struct {
  uint32_t alpha_id_len;  /**< Must be set to the number of elements in alpha_id. */
  uint8_t alpha_id[QAPI_DEVICE_INFO_WMS_ALPHA_ID_LENGTH_MAX];
  /**<   Alpha ID. */
}qapi_Device_Info_WMS_Call_Control_Modified_Info_t;

/**
 QAPI Device information WMS message tuple.
*/
typedef struct {
  uint32_t message_index;
  /**< Message index of each matched message. */

  qapi_Device_Info_WMS_Message_Tag_Type_e tag_type;
  /**<   Message tag. Values: \n 
         - 0x00 -- TAG_TYPE_MT_READ
         - 0x01 -- TAG_TYPE_MT_NOT_READ 
         - 0x02 -- TAG_TYPE_MO_SENT 
         - 0x03 -- TAG_TYPE_MO_NOT_SENT
    */
}qapi_Device_Info_WMS_Message_Tuple_t;

/**
 QAPI device information phone number structure.
*/
typedef struct {
  uint8_t length;
  /**< Length of the phone number. */

  uint8_t number_type;
  /**< International or national phone number type. */

  char *number;
  /**< Phone number. */
}qapi_Device_Info_SMS_Number_Type_t;

/**
 QAPI device information SMS validity structure.
*/
typedef struct {
  uint8_t length;
  /**< Length of the validity period. */

  char validity[7];
  /**< Validity period. */
}qapi_Device_Info_SMS_Validity_t;


/**
 QAPI device information Send SMS request structure.
*/
typedef struct {
  /* Mandatory */
  /* Destination phone number */
  qapi_Device_Info_SMS_Number_Type_t dest_number;
  /**< Phone number of destination. */

  /* Mandatory */
  /* SMS payload length */
  uint8_t sms_payload_length;
  /**< SMS message payload length. */

  /* Mandatory */
  /* SMS payload */
  char *sms_payload;
  /**< SMS message payload. */

  /* Mandatory */
  /* Message format */
  qapi_Device_Info_WMS_Message_Format_e message_format;
  /**<   Message format: \n
         - 0x00 -- MESSAGE_FORMAT_CDMA -- CDMA \n
         - 0x02 to 0x05 -- Reserved \n
         - 0x06 -- MESSAGE_FORMAT_GW_PP -- GW_PP \n
         - 0x07 -- MESSAGE_FORMAT_GW_ BC -- GW_BC 
    */

  /* Optional */
  /* SMSC phone number */
  uint8_t smsc_number_is_valid;
  /**< Must be set to TRUE if smsc_number is being passed. */
  qapi_Device_Info_SMS_Number_Type_t smsc_number;
  /**< Phone number of Short Message Service Center (SMSC). */

  /* Optional */
  /* First octet */
  uint8_t first_octet_is_valid;
  /**< Must be set to TRUE if first_octet is being passed. */
  uint8_t first_octet;
  /**< Message type that contains information about status report, User Data Header Indicator (UDHI), and validity. */

  /* Optional */
  /* Message reference */
  uint8_t message_ref_is_valid;
  /**< Must be set to TRUE if message_ref is being passed. */
  uint8_t message_ref;
  /**< Message refernce value. Value 00 lets UE set the message reference number itself. */

  /* Optional */
  /* Protocol identifier data */
  uint8_t pid_is_valid;
  /**< Must be set to TRUE if pid is being passed. */
  uint8_t pid;
  /**< Default value is 00. */

  /* Optional */
  /* Data coding scheme */
  uint8_t dcs_is_valid;
   /**< Must be set to TRUE if dcs is being passed. */
  uint8_t dcs;
  /**< Data Coding Scheme (DCS) contains information about message encoding. For example, GSM 7-bit, GSM 8-bit, UCS2 encodings. */

  /* Optional */
  /* Validity period */
  uint8_t validity_is_valid;
  /**< Must be set to TRUE if validity is being passed. */
  qapi_Device_Info_SMS_Validity_t validity;
  /**< Optional field and will be set based on bit 3 and 4 of first octet field. */

  /* Optional */
  /* SMS on IP Multimedia Subsystem (IMS). */
  uint8_t sms_on_ims_is_valid;
   /**< Must be set to TRUE if sms_on_ims is being passed. */
  uint8_t sms_on_ims;
  /**< Indicates whether the message is to be sent on IMS: \n
         - 0x00 -- Message is not to be sent on IMS. \n
         - 0x01 -- Message is to be sent on IMS. \n
         - 0x02 to 0xFF -- Reserved. \n
    */
} qapi_Device_Info_Send_SMS_Req_t;

/**
 QAPI device information Write SMS request structure.
*/
typedef struct {
  /* Mandatory */
  /* Destination phone number */
  qapi_Device_Info_SMS_Number_Type_t dest_number;
  /**< Phone number of destination. */

  /* Mandatory */
  /* SMS payload length */
  uint8_t sms_payload_length;
  /**< SMS payload length of phone number. */

  /* Mandatory */
  /* SMS payload */
  char *sms_payload;
  /**< SMS payload of phone number. */

  /* Mandatory */
  /* Message format */
  qapi_Device_Info_WMS_Message_Format_e message_format;
  /**<   Message format: \n
         - 0x00 -- MESSAGE_FORMAT_CDMA -- CDMA \n
         - 0x02 to 0x05 -- Reserved \n
         - 0x06 -- MESSAGE_FORMAT_GW_PP -- GW_PP \n
         - 0x07 -- MESSAGE_FORMAT_GW_ BC -- GW_BC 
    */

  /* Optional */
  /* SMSC phone number */
  uint8_t smsc_number_is_valid;
  /**< Must be set to true if smsc_number is being passed. */
  qapi_Device_Info_SMS_Number_Type_t smsc_number;
  /**< Phone number of SMSC. */

  /* Optional */
  /* First octet */
  uint8_t first_octet_is_valid;
  /**< Must be set to true if first_octet is being passed. */
  uint8_t first_octet;
  /**< Message type that contains information about status report, UDHI, and validity. */

  /* Optional */
  /* Message reference */
  uint8_t message_ref_is_valid;
  /**< Must be set to true if message_ref is being passed. */
  uint8_t message_ref;
  /**< Message refernce value. Value 00 lets UE set the message reference number itself. */

  /* Optional */
  /* Protocol identifier data */
  uint8_t pid_is_valid;
  /**< Must be set to true if pid is being passed. */
  uint8_t pid;
  /**< Protocol identifier data (PID) refers to the higher layer protocol being used,
       indicates interworking with a certain type of telematic device, 
	   specifies the replace type of the message, or allows the download of configuration parameters to the SIM card.
	   Default value is 00. */

  /* Optional */
  /* Data coding scheme */
  uint8_t dcs_is_valid;
  /**< Must be set to TRUE if dcs is being passed. */
  uint8_t dcs;
  /**< DCS contains information about message encoding. For example, GSM 7-bit, GSM 8-bit, UCS2 encodings. */

  /* Optional */
  /* Validity period */
  uint8_t validity_is_valid;
  /**< Must be set to TRUE if validity is being passed. */
  qapi_Device_Info_SMS_Validity_t validity;
  /**< Optional field that will be set based on bit 3 and 4 of first octet field. */

  /* Mandatory */
  /* Storage type */
  qapi_Device_Info_WMS_Storage_Type_e   storage_type;
  /**<   Memory storage types: \n
         - 0x00 -- STORAGE_TYPE_UIM -- UIM \n
         - 0x01 -- STORAGE_TYPE_NV  -- NV */

  /* Optional */
  /* Tag type */
  uint8_t tag_type_is_valid;
  /**< Must be set to TRUE if tag_type is being passed. */
  qapi_Device_Info_WMS_Message_Tag_Type_e tag_type;
  /**<   Message tag types: \n 
         - 0x00 -- TAG_TYPE_MT_READ \n 
         - 0x01 -- TAG_TYPE_MT_NOT_READ \n 
         - 0x02 -- TAG_TYPE_MO_SENT \n 
         - 0x03 -- TAG_TYPE_MO_NOT_SENT
    */
} qapi_Device_Info_Write_SMS_Req_t;

/**
 QAPI device information Read SMS request structure.
*/
typedef struct {
  /* Mandatory */
  /* Storage type */
  qapi_Device_Info_WMS_Storage_Type_e   storage_type;
  /**<   Memory storage types: \n
         - 0x00 -- STORAGE_TYPE_UIM -- UIM \n
         - 0x01 -- STORAGE_TYPE_NV  -- NV */

  /* Mandatory */
  /* Memory index */
  uint32_t                              storage_index;
  /**< SMS index in a stored location. */

  /* Optional */
  /*  Message Mode */
  uint8_t message_mode_is_valid;
  /**< Must be set to TRUE if message_mode is being passed. */
  qapi_Device_Info_WMS_Message_Mode_e   message_mode;
  /**<   Message modes: \n 
         - 0x00 -- MESSAGE_MODE_CDMA -- CDMA \n 
         - 0x01 -- MESSAGE_MODE_GW   -- GW
    */

  /* Optional */
  /* SMS on IMS */
  uint8_t sms_on_ims_is_valid;
  /**< Must be set to TRUE if sms_on_ims is being passed. */
  uint8_t sms_on_ims;
  /**< Indicates whether the message is to be sent on IMS: \n
         - 0x00 -- Message is not to be sent on IMS. \n
         - 0x01 -- Message is to be sent on IMS. \n
         - 0x02 to 0xFF -- Reserved. \n
    */
} qapi_Device_Info_Read_SMS_Req_t;

/**
 QAPI device information List SMS request structure.
*/
typedef struct {
  /* Mandatory */
  /* Storage type */
  qapi_Device_Info_WMS_Storage_Type_e   storage_type;
  /**<   Memory storage types: \n
         - 0x00 -- STORAGE_TYPE_UIM -- UIM \n
         - 0x01 -- STORAGE_TYPE_NV  -- NV */

  /* Optional */
  /* Tag type */
  uint8_t                                 tag_type_is_valid;
  /**< Must be set to TRUE if tag_type is being passed. */
  qapi_Device_Info_WMS_Message_Tag_Type_e tag_type;
  /**<   Message tag types: \n 
         - 0x00 -- TAG_TYPE_MT_READ \n 
         - 0x01 -- TAG_TYPE_MT_NOT_READ \n 
         - 0x02 -- TAG_TYPE_MO_SENT \n 
         - 0x03 -- TAG_TYPE_MO_NOT_SENT
    */

  /* Optional */
  /*  Message Mode */
  uint8_t                               message_mode_is_valid;
  /**< Must be set to TRUE if message_mode is being passed. */
  qapi_Device_Info_WMS_Message_Mode_e   message_mode;
  /**<   Message modes: \n 
         - 0x00 -- MESSAGE_MODE_CDMA -- CDMA \n 
         - 0x01 -- MESSAGE_MODE_GW   -- GW
    */
} qapi_Device_Info_List_SMS_Req_t;

/**
 QAPI device information Delete SMS request structure.
*/
typedef struct {

  /* Mandatory */
  /* Storage type */
  qapi_Device_Info_WMS_Storage_Type_e   storage_type;
  /**<   Memory storage types: \n
         - 0x00 -- STORAGE_TYPE_UIM -- UIM \n
         - 0x01 -- STORAGE_TYPE_NV  -- NV */

  /* Optional */
  /*  Memory Index */
  uint8_t                               storage_index_is_valid;
  /**< Must be set to TRUE if storage_index is being passed. */
  uint32_t                              storage_index;
  /**< Indicates the storage index of the relevant message. */

  /* Optional */
  /*  Message Tag */
  uint8_t                                tag_type_is_valid;
  /**< Must be set to TRUE if tag_type is being passed. */
  qapi_Device_Info_WMS_Message_Tag_Type_e tag_type;
  /**<   Message tag types: \n 
         - 0x00 -- TAG_TYPE_MT_READ \n 
         - 0x01 -- TAG_TYPE_MT_NOT_READ \n 
         - 0x02 -- TAG_TYPE_MO_SENT \n 
         - 0x03 -- TAG_TYPE_MO_NOT_SENT
    */

  /* Optional */
  /*  Message Mode */
  uint8_t                               message_mode_is_valid;
  /**< Must be set to TRUE if message_mode is being passed. */
  qapi_Device_Info_WMS_Message_Mode_e   message_mode;
  /**<   Message modes: \n 
         - 0x00 -- MESSAGE_MODE_CDMA -- CDMA \n 
         - 0x01 -- MESSAGE_MODE_GW   -- GW
    */
} qapi_Device_Info_Delete_SMS_Req_t;

/**
 QAPI device information Send SMS ACK request structure.
*/
typedef struct {
  /* Mandatory */
  /* Transaction id */
  uint32_t                              transaction_id;
  /**< Transaction ID of the message for which ACK is to be sent.
  */

  /* Mandatory */
  /* Message protocol  */
  qapi_Device_Info_WMS_Message_Protocol_e message_protocol;
  /**<   WMS message protocols: \n 
         - 0x00 -- MESSAGE_PROTOCOL_ CDMA \n 
         - 0x01 -- MESSAGE_PROTOCOL_ WCDMA
    */

  /* Mandatory */
  /* Success  */
  uint8_t                               success;
  /**< Indicates whether the MT message processed successfully: \n
       - 0x00 -- Failure \n 
       - 0x01 -- Success
  */

  /* Optional */
  /* 3GPP Failure Information */
  uint8_t                         rp_cause_is_valid;
  /**< Must be set to TRUE if rp_cause is being passed. */
  qapi_Device_Info_WMS_RP_Cause_e rp_cause;
  /**< GW RP cause defined in #qapi_Device_Info_WMS_RP_Cause_e. */

  /* Optional */
  /* 3GPP Failure Information */
  uint8_t                         tp_cause_is_valid;
  /**< Must be set to TRUE if tp_cause is being passed. */
  qapi_Device_Info_WMS_TP_Cause_e tp_cause;
  /**< GW TP cause defined in #qapi_Device_Info_WMS_TP_Cause_e. */

  /* Optional */
  /* SMS on IMS */
  uint8_t sms_on_ims_is_valid;
  /**< Must be set to TRUE if sms_on_ims is being passed. */
  uint8_t sms_on_ims;
  /**< Indicates whether the message is to be sent on IMS: \n
       - 0x00 -- Message is not to be sent on IMS \n
       - 0x01 -- Message is to be sent on IMS \n
       - 0x02 to 0xFF -- Reserved \n
    */
} qapi_Device_Info_Send_SMS_Ack_Req_t;

/**
 QAPI device information Get SMS routes request structure.
*/
typedef struct {
  char __placeholder;
  /**< This element is a placeholder to prevent the declaration of an empty structure. */
} qapi_Device_Info_Get_SMS_Routes_Req_t;

/**
 QAPI device information WMS route information.
*/
typedef struct {
  qapi_Device_Info_WMS_Message_Type_e     msg_type;
  /**<  Message type matching this route: \n 
        - 0x00 -- MESSAGE_TYPE_POINT_ TO_POINT
  */

  qapi_Device_Info_WMS_Message_Class_e    msg_class;
  /**<  Message class. Values: \n 
       - 0x00 -- MESSAGE_CLASS_0 
       - 0x01 -- MESSAGE_CLASS_1 
       - 0x02 -- MESSAGE_CLASS_2 
       - 0x03 -- MESSAGE_CLASS_3
       - 0x04 -- MESSAGE_CLASS_NONE
       - 0x05 -- MESSAGE_CLASS_CDMA 
  */

  qapi_Device_Info_WMS_Storage_Type_e     storage_type;
  /**<   Memory storage types: \n
         - 0x00 -- STORAGE_TYPE_UIM \n
         - 0x01 -- STORAGE_TYPE_NV 
  */

  qapi_Device_Info_WMS_Receipt_Action_e   receipt_action;
  /**<  Action to be taken on receipt of a message
       matching the specified type and class for this route. Values: \n  
       - 0x00 -- DISCARD          -- Incoming messages for this route are
                                     discarded by the WMS service without 
                                     notifying QMI_WMS clients. \n 
       - 0x01 -- STORE_AND_NOTIFY -- Incoming messages for this route are
                                     stored to the specified device 
                                     memory, and new message notifications
                                     are sent to registered clients. \n 
       - 0x02 -- TRANSFER_ONLY    -- Incoming messages for this route are
                                     transferred to the client, and the
                                     client is expected to send ACK to
                                     the network. \n 
       - 0x03 -- TRANSFER_AND_ACK -- Incoming messages for this route are
                                     transferred to the client, and ACK is
                                     sent to the network.
  */
} qapi_Device_Info_WMS_Route_t;

/**
 QAPI device informationSet SMS routes request structure.
*/
typedef struct {
  /* Mandatory */
  /* Route list count */
  uint32_t                      route_list_cnt;
  /**< Must be set to the number of elements in route_list field. */

  /* Mandatory */
  /* Route list */
  qapi_Device_Info_WMS_Route_t  *route_list;
  /**< WMS route information defined in #qapi_Device_Info_WMS_Route_t. */

  /* Optional */
  /* Transfer indication */
  uint8_t                                 transfer_ind_is_valid;
  /**< Must be set to TRUE if transfer_ind is being passed. */
  qapi_Device_Info_WMS_Transfer_Ind_e     transfer_ind;
  /**< 0x01 -- TRANSFER_IND_CLIENT -- Status reports are transferred to the client. */

} qapi_Device_Info_Set_SMS_Routes_Req_t;

/**
 QAPI device information Get SMSC request structure.
*/
typedef struct {
  /* Optional */
  /*  SMSC Address Index */
  uint8_t index_is_valid;
  /**< Must be set to TRUE if index is being passed. */
  uint8_t index;
  /**< Memory index to read an SMSC address from a specific index in EF-SMSP.
  */
}qapi_Device_Info_Get_SMSC_Req_t;

/**
 QAPI device information Set SMSC request structure.
*/
typedef struct {
  /* Mandatory */
  /*  SMSC Address */
  char smsc_address_digits[QAPI_DEVICE_INFO_WMS_ADDRESS_DIGIT_MAX + 1];
  /**< NULL-terminated string containing the SMSC address in ASCII digits 
       that can be prefixed with + (maximum 20 digits, not including the +).
  */

  /* Optional */
  /* SMSC Address Type */
  uint8_t smsc_address_type_is_valid;
  /**< Must be set to TRUE if smsc_address_type is being passed. */
  char smsc_address_type[QAPI_DEVICE_INFO_WMS_ADDRESS_TYPE_MAX + 1];
  /**< NULL-terminated string containing the type of SMSC address
       in ASCII digits (maximum three digits).
  */

  /* Optional */
  /* SMSC Address Index */
  uint8_t index_is_valid;
  /**< Must be set to TRUE if index is being passed. */
  uint8_t index;
  /**< Indicates the record index where the SMSC address must be written. */

}qapi_Device_Info_Set_SMSC_Req_t;

/** 
  QAPI device information WMS response structure.
 */
typedef struct
{
  int8_t result;
  /**< Result code: RESULT_SUCCESS (0) or RESULT_FAILURE (-1).
  */
  int32_t error;
  /**< Error code. */
} qapi_Device_Info_WMS_Response_t;

/** 
  QAPI device information incoming SMS indication structure.
 */
typedef struct {
  /* Mandatory */
  /* Storage type */
  qapi_Device_Info_WMS_Storage_Type_e   storage_type;
  /**<   Memory storage types: \n
         - 0x00 -- STORAGE_TYPE_UIM  \n
         - 0x01 -- STORAGE_TYPE_NV
   */

  /* Mandatory */
  /* Memory index */
  uint32_t                              storage_index;
  /**< Storage index of incoming SMS. */
} qapi_Device_Info_Incoming_SMS_Ind_t;

/** 
  QAPI device information SMS timestamp structure.
 */
typedef struct
{
  uint16_t year;
  /**< Year of message. */
  uint8_t month;
  /**< Month of message. */
  uint8_t day;
  /**< Day of message. */
  uint8_t hour;
  /**< Hour of message. */
  uint8_t minute;
  /**< Minutes of message. */
  uint8_t second;
  /**< Seconds of message. */
  uint64_t tzOffset;
  /**< Offset of message. */
  uint64_t timeStampInMs;
}qapi_Device_Info_SMS_TimeStamp_t;

/** 
  QAPI device information route incoming SMS indication structure.
 */
typedef struct {
  /* Mandatory */
  /* ACK Indicator */
  qapi_Device_Info_WMS_Ack_Indicator_e ack_indicator;
  /**<   Parameter that indicates if ACK must be sent by the application: \n
         - 0x00 -- ACK_INDICATOR_SEND_ACK \n
         - 0x01 -- ACK_INDICATOR_DO_NOT_SEND_ACK 
    */

  /* Mandatory */
  /* Transaction id */
  uint32_t                              transaction_id;
  /**<   Transaction ID of the message for which ACK is to be sent.
  */

  /* Mandatory */
  /* SMSC number */
  qapi_Device_Info_SMS_Number_Type_t      smsc_number;
  /**< SMSC number of incoming message. */

  /* Mandatory */
  /* First octet */
  uint8_t                                 first_octet;
  /**< First octet of incoming message. */

  /* Mandatory */
  /* Sender number */
  qapi_Device_Info_SMS_Number_Type_t      sender_number;
  /**< Sender number of incoming message. */

  /* Mandatory */
  /* Protocol identifier data */
  uint8_t                                 pid;
  /**< PID of incoming message. */

  /* Mandatory */
  /* Data coding scheme */
  uint8_t                                 dcs;
  /**< DCS of incoming message. */

  /* Optional */
  /* Timestamp */
  uint8_t                                 timestamp_is_valid;
  /**< Must be set to TRUE if timestamp is being passed. */
  qapi_Device_Info_SMS_TimeStamp_t                                  timestamp;
  /**< Timestamp of incoming message received at Service Center. */

  /* Mandatory */
  /* SMS payload length */
  uint8_t sms_payload_length;
  /**< SMS payload length of incoming message. */

  /* Mandatory */
  /* SMS payload */
  char *sms_payload;
  /**< SMS payload of incoming message. */

  /* Mandatory */
  /* Message format */
  qapi_Device_Info_WMS_Message_Format_e   format;
  /**<   Message formats: \n 
         - 0x00 -- MESSAGE_FORMAT_CDMA \n 
         - 0x02 to 0x05 -- Reserved \n 
         - 0x06 -- MESSAGE_FORMAT_GW_PP \n 
         - 0x08 - MESSAGE_FORMAT_MWI
    */
} qapi_Device_Info_Route_Incoming_SMS_Ind_t;

/** 
  QAPI device information SMS status report structure.
 */
typedef struct {
  /* Mandatory */
  /* ACK Indicator */
  qapi_Device_Info_WMS_Ack_Indicator_e ack_indicator;
  /**<   Parameter that indicates if ACK must be sent by the application. \n
         - 0x00 -- ACK_INDICATOR_SEND_ACK \n
         - 0x01 -- ACK_INDICATOR_DO_NOT_SEND_ACK
    */

  /* Mandatory */
  /* Transaction id */
  uint32_t                              transaction_id;
  /**<   Transaction ID of the SMS status report message.
  */

  /* Mandatory */
  /* SMSC number */
  qapi_Device_Info_SMS_Number_Type_t      smsc_number;
  /**< SMSC number of SMS status report message. */

  /* Mandatory */
  /* First octet */
  uint8_t                                 first_octet;
  /**< First octet number of SMS status report message. */

  /* Mandatory */
  /* Message reference  */
  uint8_t                                 msg_ref;
  /**< Message reference of SMS status report message. */

  /* Mandatory */
  /* Receiver number */
  qapi_Device_Info_SMS_Number_Type_t      reciever_number;
  /**< Receiver number of SMS status report message.. */

  /* Optional */
  /* Protocol identifier data */
  uint8_t                                 pid_is_valid;
  /**< Must be set to TRUE if pid is being passed. */
  uint8_t                                 pid;
  /**< PID of SMS status report message. */
  
  /* Mandatory */
  /* Data coding scheme */
  uint8_t                                 dcs_is_valid;
  /**< Must be set to TRUE if dcs is being passed. */
  uint8_t                                 dcs;
  /**< DCS of SMS status report message. */
  
  /* Mandatory */
  /* SMSC Timestamp */
  qapi_Device_Info_SMS_TimeStamp_t        sc_timestamp;
  /**< SMSC timestamp. */

  /* Mandatory */
  /* DT Timestamp */
  qapi_Device_Info_SMS_TimeStamp_t        dt_timestamp;
  /**< Discharge time (DT) timestamp */

  /* Mandatory */
  /* Status  */
  uint8_t                                 status;
  /**< Identifies status of the previously sent mobile originated short message. */

  /* Mandatory */
  /* SMS payload length */
  uint8_t sms_payload_length;
  /**< SMS payload length. */

  /* Mandatory */
  /* SMS payload */
  char *sms_payload;
  /**< SMS payload. */

  /* Mandatory */
  /* Message format */
  qapi_Device_Info_WMS_Message_Format_e   format;
  /**<   Message formats: \n 
         - 0x00 -- MESSAGE_FORMAT_CDMA \n 
         - 0x02 to 0x05 -- Reserved \n 
         - 0x06 -- MESSAGE_FORMAT_GW_PP \n 
         - 0x08 - MESSAGE_FORMAT_MWI
    */
} qapi_Device_Info_SMS_Staus_Report_t;

/**
 QAPI device information Send SMS response structure.
*/
typedef struct {
  /* Mandatory */
  /* Response code */
  qapi_Device_Info_WMS_Response_t result;
  /**< Response code. */

  /* Mandatory */
  /* Message ID */
  uint16_t message_id;
  /**< WMS message ID. */

  /* Optional */
  /* Cause Code* */
  uint8_t cause_code_valid;  
  /**< Must be set to TRUE if cause_code is being passed. */
  qapi_Device_Info_WMS_TL_Cause_Code_e cause_code;
  /**< GW TL cause defined in #qapi_Device_Info_WMS_TL_Cause_Code_e. */

  /* Optional */
  /*  Error Class* */
  uint8_t error_class_valid;  
  /**< Must be set to TRUE if error_class is being passed. */
  qapi_Device_Info_WMS_Error_Class_Send_e error_class;
  /**<  Error classes: \n
       - 0x00 -- ERROR_CLASS_ TEMPORARY \n
       - 0x01 -- ERROR_CLASS_ PERMANENT 
  */

  /* Optional */
  /* RP Cause Info** */
  uint8_t rp_cause_info_valid;  
  /**< Must be set to TRUE if rp_cause_info is being passed. */
  qapi_Device_Info_WMS_RP_Cause_e rp_cause;
  /**< GW RP cause defined in #qapi_Device_Info_WMS_RP_Cause_e. */

  /* Optional */
  /*  TP Cause Info** */
  uint8_t tp_cause_info_valid;  
  /**< Must be set to TRUE if tp_cause_info is being passed. */
  qapi_Device_Info_WMS_TP_Cause_e tp_cause;
  /**< GW TP cause defined in #qapi_Device_Info_WMS_TP_Cause_e. */

  /* Optional */
  /*  Message Delivery Failure Type */
  uint8_t message_delivery_failure_type_valid;  
  /**< Must be set to TRUE if message_delivery_failure_type is being passed. */
  qapi_Device_Info_WMS_Message_Delivery_Failure_Type_e msg_failure_type;
  /**< Message delivery failure type. \n
       - 0x00 -- WMS_MESSAGE_ DELIVERY_FAILURE_TEMPORARY \n
       - 0x01 -- WMS_MESSAGE_ DELIVERY_FAILURE_PERMANENT
  */

  /* Optional */
  /* Message Delivery Failure Cause */
  uint8_t message_delivery_failure_cause_valid;  
  /**< Must be set to TRUE if message_delivery_failure_cause is being passed. */
  qapi_Device_Info_WMS_Message_Delivery_Failure_Cause_e msg_failure_cause;
  /**< Message delivery failure cause.  Values: \n
       - 0x00 -- WMS_MESSAGE_ BLOCKED_DUE_TO_CALL_ CONTROL
    */

  /* Optional */
  /* Call Control Modified Information */
  uint8_t call_control_modified_info_valid;  
  /**< Must be set to TRUE if call_control_modified_info is being passed. */
  qapi_Device_Info_WMS_Call_Control_Modified_Info_t call_modified_info;
  /**< Call control modified information. */
}qapi_Device_Info_Send_SMS_Rsp_t;

/**
 QAPI device information Read SMS response structure.
*/
typedef struct {
  /* Optional */
  /* SMSC number */
  qapi_Device_Info_SMS_Number_Type_t      smsc_number;

  /* Mandatory */
  /* First octet */
  uint8_t                                 first_octet;
  /**< First octet of incoming message. */

  /* Mandatory */
  /* Sender number */
  qapi_Device_Info_SMS_Number_Type_t      sender_number;
  /**< Sender number of incoming message. */

  /* Mandatory */
  /* Protocol identifier data */
  uint8_t                                 pid;
  /**< PID of incoming message. */

  /* Mandatory */
  /* Data coding scheme */
  uint8_t                                 dcs;
  /**< DCS of incoming message. */

  /* Optional */
  /* Timestamp */
  uint8_t                                 timestamp_is_valid;
  qapi_Device_Info_SMS_TimeStamp_t        timestamp;
  /**< Must be set to TRUE if timestamp is being passed. */

  /* Mandatory */
  /* SMS payload length */
  uint8_t sms_payload_length;
  /**< SMS payload length of incoming message. */

  /* Mandatory */
  /* SMS payload */
  char *sms_payload;
  /**< SMS payload of incoming message. */

  /* Mandatory */
  /* Message format */
  qapi_Device_Info_WMS_Message_Format_e   format;
  /**<   Message format. Value: \n 
         - 0x00 -- MESSAGE_FORMAT_CDMA \n 
         - 0x02 to 0x05 -- Reserved \n 
         - 0x06 -- MESSAGE_FORMAT_GW_PP \n 
         - 0x08 - MESSAGE_FORMAT_MWI
    */

  /* Optional */
  /* Message reference */
  uint8_t message_ref_is_valid;
  /**< Must be set to TRUE if message_ref is being passed. */
  uint8_t message_ref;
  /**< Message reference value. Value 00 lets UE set the message reference number itself. */

  /* Optional */
  /* Validity period */
  uint8_t validity_is_valid;
  /**< Must be set to TRUE if validity is being passed. */
  qapi_Device_Info_SMS_Validity_t validity;
  /**< Optional field and will be set based on bit 3 and 4 of first octet field. */

  /* Mandatory */
  /*  Tag type */
  qapi_Device_Info_WMS_Message_Tag_Type_e tag_type;
  /**<   Message tag. Values: \n 
         - 0x00 -- TAG_TYPE_MT_READ \n 
         - 0x01 -- TAG_TYPE_MT_NOT_READ \n 
         - 0x02 -- TAG_TYPE_MO_SENT \n 
         - 0x03 -- TAG_TYPE_MO_NOT_SENT
    */
}qapi_Device_Info_Read_SMS_Rsp_t;

/**
 QAPI device information Write SMS response structure.
 */
typedef struct {
  /* Mandatory */
  /* Message Memory Storage Identification */
  uint32_t storage_index;
  /**< Memory index. */
}qapi_Device_Info_Write_SMS_Rsp_t;

/**
 QAPI device information List SMS response structure.
*/
typedef struct {
  /* Mandatory */
  /*  Message List */
  uint32_t message_tuple_len;
  /**< Must be set to the number of elements in message_tuple. */
  qapi_Device_Info_WMS_Message_Tuple_t *message_tuple;
  /**< Structure based on #qapi_Device_Info_WMS_Message_Tuple_t. */
} qapi_Device_Info_List_SMS_Rsp_t;

/**
 QAPI device info Delete SMS response structure.
*/
typedef struct {
  char __placeholder;
  /**< This element is a placeholder to prevent the declaration of an empty structure. */
} qapi_Device_Info_Delete_SMS_Rsp_t;

/**
 QAPI device information Send Ack SMS response structure.
*/
typedef struct {
  /* Optional */
  /*  ACK Failure Cause */
  uint8_t failure_cause_valid;  
  /**< Must be set to TRUE if failure_cause is being passed. */
  qapi_Device_Info_WMS_Ack_Failure_Cause_e failure_cause;
  /**< ACK failure cause. Values: \n 
       - 0x00 -- ACK_FAILURE_NO_ NETWORK_ RESPONSE \n 
       - 0x01 -- ACK_FAILURE_NETWORK_ RELEASED_LINK \n 
       - 0x02 -- ACK_FAILURE_ACK_ NOT_SENT*/
} qapi_Device_Info_Send_SMS_Ack_Rsp_t;

/**
 QAPI device information SMS route get response structure.
*/
typedef struct {
  /* Mandatory */
  /*  Route List count */
  uint32_t                      route_list_cnt;
  /**< Must be set to the number of elements in route_list. */

  /* Mandatory */
  /* Route List */
  qapi_Device_Info_WMS_Route_t  *route_list;
  /**< Route list based on #qapi_Device_Info_WMS_Route_t. */

  /* Optional */
  /*  Transfer Status Report */
  uint8_t transfer_ind_valid;  
  /**< Must be set to TRUE if transfer_ind is being passed. */
  qapi_Device_Info_WMS_Transfer_Ind_e     transfer_ind;
  /**<   Values: \n
         - 0x00 -- TRANSFER_IND_SIM    -- Status reports are stored on the
                                          SIM if a matching MO record is found
                                          on the SIM; otherwise, status reports
                                          are transferred to the client. \n 
         - 0x01 -- TRANSFER_IND_CLIENT -- Status reports are transferred
                                          to the client.
    */
} qapi_Device_Info_Get_SMS_Routes_Rsp_t;

/**
 QAPI device information SMS route set response structure.
*/
typedef struct {
  char __placeholder;
  /**< This element is a placeholder to prevent the declaration of an empty structure. */
} qapi_Device_Info_Set_SMS_Routes_Rsp_t;

/**
 QAPI device information SMSC get response structure.
*/
typedef struct {
  /* Mandatory */
  /*  SMSC Address type */
  char smsc_address_type[QAPI_DEVICE_INFO_WMS_ADDRESS_TYPE_MAX];
  /**< SMSC address type in ASCII digits (must be three digits long, with 
       leading zeros used as placeholders).
  */

  /* Mandatory */
  /*  SMSC Address digit */
  uint32_t smsc_address_digits_len;  
  /**< Must be set to the number of elements in smsc_address_digits. */
  char smsc_address_digits[QAPI_DEVICE_INFO_WMS_ADDRESS_DIGIT_MAX];
  /**< SMSC address in ASCII digits; can be prefixed with + (maximum 20 digits, not 
       including the +).
  */
} qapi_Device_Info_Get_SMSC_Rsp_t;

/**
 QAPI device information SMSC set response structure.
*/
typedef struct {
  char __placeholder;
  /**< This element is a placeholder to prevent the declaration of an empty structure. */
} qapi_Device_Info_Set_SMSC_Rsp_t;

/**
 QAPI device information request structure.
*/
typedef struct {
    union {
      /*UIM request*/
      qapi_Device_Info_Req_Verify_Pin_t            *pin_verify_req;           /**< PIN verify request data. */
      qapi_Device_Info_Req_Change_Pin_t            *pin_change_req;           /**< PIN change request data. */
      qapi_Device_Info_Req_Unblock_Pin_t           *pin_unblock_req;          /**< PIN unblock request data . */
      qapi_Device_Info_Req_Protect_Pin_t           *pin_protect_req;          /**< PIN protect request data. */
      qapi_Device_Info_Req_Set_FDN_t               *set_fdn_req;              /**< Sets FDN request data. */
      qapi_Device_Info_APDU_Command_t              *apdu_cmd_req;             /**< Sends APDU command request data. */
      qapi_Device_Info_Preferred_PLMN_Set_Req_t    *preferred_plmn_set_req;   /**< Sets preferred PLMN items request data. */
      qapi_Device_Info_Preferred_PLMN_Read_Req_t   *preferred_plmn_read_req;  /**< Reads preferred PLMN items request data. */
      /*NAS request*/
      qapi_Device_Info_Req_EDRX_Set_t              *set_edrx_req;             /**< EDRX sets request data. */
      qapi_Device_Info_Req_EDRX_Get_t              *get_edrx_req;             /**< EDRX gets request data. */
      qapi_Device_Info_Jamming_Status_Get_Req_t    *jamming_status_req;       /**< Gets jamming status request data. */
      /*Voice request*/
      qapi_Device_Info_Call_Req_t                  *call_req ;                /**< VoLTE call request data. */
       /*Data Profile request*/
      qapi_Device_Info_Set_QCPDPP_Req_t*            set_qcpdpp_req;           /**< QCPDPP command set request data. */
      qapi_Device_Info_Get_QCPDPP_Req_t*            get_qcpdpp_req;           /**< QCPDPP command get request data. */
      qapi_Device_Info_Set_CGDCONT_Req_t*           set_cgdcont_req;          /**< CGDCONT command set request data. */
      qapi_Device_Info_Get_CGDCONT_Req_t*           get_cgdcont_req;          /**< CGDCONT command get request data. */
      qapi_Device_Info_Set_QCPDPIMSCFGE_Req_t*      set_qcpdpimscfg_req;      /**< QCPDPIMSCFGE command set request data. */
      qapi_Device_Info_Get_QCPDPIMSCFGE_Req_t*      get_qcpdpimscfg_req;      /**< QCPDPIMSCFGE command get request data. */
      qapi_Device_Info_Set_QCPDPCFGE_Req_t*         set_qcpdpcfg_req;         /**< QCPDPCFGE command set request data. */
      qapi_Device_Info_Get_QCPDPCFGE_Req_t*         get_qcpdpcfg_req;         /**< QCPDPCFGE command get request data. */
      /*NAS FSK request*/
      qapi_Device_Info_FSK_Start_Req_t*             fsk_start_req;            /**< FSK start request data. */
      qapi_Device_Info_FSK_Data_Req_t*              fsk_data_req;             /**< FSK data request data. */
      qapi_Device_Info_Set_FSK_Debug_Req_t*         set_fsk_debug_req;        /**< FSK set debug parameters request data. */
      qapi_Device_Info_Set_FSK_Hoptable_Req_t*      set_fsk_hoptable_req;     /**< FSK set hop table parameters request data. */
      qapi_Device_Info_Set_FSK_PCL_Req_t*           set_fsk_pcl_req;          /**< FSK set pcl parameters request data. */
	  /*NAS qRxlevmin request*/
      qapi_Device_Info_Get_QRXLEVMIN_Req_t*         get_qRxlevmin_req;        /**< qRxlevmin get request data. */
      /*IMS_IOT response*/
      qapi_Device_Info_IMS_IOT_Ind_Register_Req_t  *ind_register_req;         /**< Set indication registration status. */
	  qapi_Device_Info_Config_Signal_strength_Req_t *sig_config_req;          /**< Set signal strength thresholds for indication. */
	  qapi_Device_Info_Get_Srxlev_Req_t			   *get_srxlev_req;			  /**< SRXLEV get request data. */
	  /* WMS request*/
      qapi_Device_Info_Send_SMS_Req_t               *send_sms_req;            /**< Send SMS request data. */
      qapi_Device_Info_Read_SMS_Req_t               *read_sms_req;            /**< Read SMS request data. */
      qapi_Device_Info_Write_SMS_Req_t              *write_sms_req;           /**< Write SMS request data. */
      qapi_Device_Info_List_SMS_Req_t               *list_sms_req;            /**< List SMS request data. */
      qapi_Device_Info_Delete_SMS_Req_t             *delete_sms_req;          /**< Delete SMS request data. */
      qapi_Device_Info_Send_SMS_Ack_Req_t           *send_sms_ack_req;        /**< Send SMS Ack request data. */
      qapi_Device_Info_Set_SMS_Routes_Req_t         *set_sms_route_req;       /**< SMS route set request data. */
      qapi_Device_Info_Get_SMSC_Req_t               *get_smsc_req;            /**< SMSC get request data. */
      qapi_Device_Info_Set_SMSC_Req_t               *set_smsc_req;            /**< SMSC set request data. */
    }req; /**< Request structure for qapi_Device_Info_Request() @newpagetable */
} qapi_Device_Info_Request_t;

/**
 QAPI device information response structure.
*/
typedef struct {
    union {
      /*UIM response*/
      qapi_Device_Info_Pin_Response_t                *pin_resp;                  /**< PIN operation response data. */
      qapi_Device_Info_Get_FDN_Rsponse_t             *get_fdn_rsp;               /**< Gets FDN response data. */
      qapi_Device_Info_APDU_Response_t               *apdu_cmd_rsp;              /**< Sends APDU command response data. */
      qapi_Device_Info_UIM_Response_t                *preferred_plmn_set_rsp;    /**< Sets preferred PLMN response data. */
      qapi_Device_Info_Preferred_PLMN_Read_Rsp_t     *preferred_plmn_read_rsp;   /**< Reads preferred PLMN items response data. */ 
      qapi_Device_Info_UICC_Application_Rsp_t        *uicc_apps_read_rsp;        /**< Reads UICC application response data. */
      /*NAS request*/
      qapi_Device_Info_Rsp_EDRX_Get_t                *get_edrx_rsp;              /**< EDRX get response data. */    
      qapi_Device_Info_Jamming_Status_Get_Rsp_t      *jamming_status_rsp;        /**< Gets jamming status response data. */
      /*IMS_IOT response*/
      qapi_Device_Info_IMS_Registration_Status_Rsp_t *ims_registration_rsp;      /**< Gets IMS registration status. */
      /*Voice response*/
      qapi_Device_Info_Call_Status_Rsp_t             *call_status_rsp;           /**< Gets VoLTE call status */
      /*Data Profile request*/
      qapi_Device_Info_Set_QCPDPP_Rsp_t*           set_qcpdpp_rsp;            /**< QCPDPP command set response data. */
      qapi_Device_Info_Get_QCPDPP_Rsp_t*           get_qcpdpp_rsp;            /**< QCPDPP command get response data. */
      qapi_Device_Info_Set_CGDCONT_Rsp_t*          set_cgdcont_rsp;           /**< CGDCONT command set response data. */
      qapi_Device_Info_Get_CGDCONT_Rsp_t*          get_cgdcont_rsp;           /**< CGDCONT command get response data. */
      qapi_Device_Info_Set_QCPDPIMSCFGE_Rsp_t*     set_qcpdpimscfg_rsp;       /**< QCPDPIMSCFGE command set response data. */
      qapi_Device_Info_Get_QCPDPIMSCFGE_Rsp_t*     get_qcpdpimscfg_rsp;       /**< QCPDPIMSCFGE command get response data. */
      qapi_Device_Info_Set_QCPDPCFGE_Rsp_t*        set_qcpdpcfg_rsp;          /**< QCPDPCFGE command set response data. */
      qapi_Device_Info_Get_QCPDPCFGE_Rsp_t*        get_qcpdpcfg_rsp;          /**< QCPDPCFGE command get response data. */
	qapi_Device_Info_Scan_Config_Rsp_t			   *scan_config_rsp;	      /**< Gets scan configurations. */
	  /*NAS FSK response*/
      qapi_Device_Info_Get_FSK_Debug_Rsp_t*         get_fsk_debug_rsp;        /**< FSK get debug parameters response data. */
      qapi_Device_Info_Get_FSK_Hoptable_Rsp_t*      get_fsk_hoptable_rsp;     /**< FSK get hop table parameters response data. */
      qapi_Device_Info_Get_FSK_PCL_Rsp_t*           get_fsk_pcl_rsp;          /**< FSK get pcl parameters response data. @newpagetable */
	  /*NAS qRxlevmin response*/
      qapi_Device_Info_Get_QRXLEVMIN_Rsp_t         *get_qRxlevmin_rsp;        /**< qRxlevmin get response data. */
	  qapi_Device_Info_Get_Srxlev_Rsp_t			   *get_srxlev_rsp; 		  /**< srxlev get response data. */
      /* WMS response*/
      qapi_Device_Info_Read_SMS_Rsp_t                *read_sms_rsp;             /**< Read SMS response data. */
      qapi_Device_Info_Write_SMS_Rsp_t               *write_sms_rsp;            /**< Write SMS response data. */
      qapi_Device_Info_List_SMS_Rsp_t                *list_sms_rsp;             /**< List SMS response data. */
      qapi_Device_Info_Send_SMS_Ack_Rsp_t            *send_sms_ack_rsp;         /**< Send SMS ACK response data. */
      qapi_Device_Info_Get_SMS_Routes_Rsp_t          *get_sms_route_rsp;        /**< SMS route get response data. */
      qapi_Device_Info_Get_SMSC_Rsp_t                *get_smsc_rsp;             /**< SMSC get response data. */
    }resp; /**<  Response structure for qapi_Device_Info_Request() @newpagetable */
} qapi_Device_Info_Response_t;

#define QAPI_PLMN_LIST_MAX_LEN 8 /**< Maximum length of PLMN list. */

typedef enum {
  QAPI_DEVICE_INFO_PLMN_ACT_GSM = 0,
  /**< GSM. */
  QAPI_DEVICE_INFO_PLMN_ACT_LTE_M1 = 7,
  /**< LTE Cat M1. */
  QAPI_DEVICE_INFO_PLMN_ACT_LTE_NB1 = 9,
  /**< LTE Cat NB1. @newpage */
}qapi_Device_Info_PLMN_Act_Type_t;

/**
 QAPI PLMN scan result information structure.
*/
typedef struct 
{
  uint16_t mcc;  /**< A 16-bit integer representation of MCC. Range: 0 to 999. */
  uint16_t mnc;  /**< A 16-bit integer representation of MNC. Range: 0 to 999. */
  qapi_Device_Info_PLMN_Act_Type_t Act; /**< 0 - GSM, 7 - LTE_M1, 9 - LTE_NB1. @newpagetable */
}qapi_Device_Info_PLMN_Info_t;

/**
 QAPI PLMN information list structure.
*/
typedef struct
{
  uint32_t list_len;                                               /**< Set to number of elements in PLMN_list. */
  qapi_Device_Info_PLMN_Info_t plmn_list[QAPI_PLMN_LIST_MAX_LEN];  /**< PLMN scan result, plmn_list. */
}qapi_Device_Info_PLMN_Info_List_t;


static __inline int device_info_set_byte_pool(qapi_Device_Info_Hndl_t hndl, void *device_info_byte_pool_ptr);
static __inline int device_info_release_indirection(qapi_Device_Info_Hndl_t hndl);


#ifdef  QAPI_TXM_MODULE

static __inline void qapi_device_info_cb_uspace_dispatcher(UINT cb_id,
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
  void (*pfn_app_cb1) (qapi_Device_Info_Hndl_t , qapi_Device_Info_t *);
  qapi_Device_Info_t *info = NULL;
  if(cb_id == DATA_CUSTOM_CB_SET_DEVICE_INFO)
  {
    pfn_app_cb1 = (void (*)(qapi_Device_Info_Hndl_t, qapi_Device_Info_t *))app_cb;
	info = (qapi_Device_Info_t *)cb_param2;
   (pfn_app_cb1)((qapi_Device_Info_Hndl_t)cb_param1, 
                       (qapi_Device_Info_t *)cb_param2);
    if(info)
    {
      if((info->info_type == QAPI_DEVICE_INFO_TYPE_EDRX_INFO_E ||
          info->info_type == QAPI_DEVICE_INFO_TYPE_JAMMING_STATUS_E ||
          info->info_type == QAPI_DEVICE_INFO_TYPE_IMS_REGISTRATION_STATUS_E ||
          info->info_type == QAPI_DEVICE_INFO_TYPE_VOLTE_CALL_STATUS_E||
          info->info_type == QAPI_DEVICE_INFO_TYPE_INCOMING_SMS_E ||
          info->info_type == QAPI_DEVICE_INFO_TYPE_ROUTE_INCOMING_SMS_E ||
          info->info_type == QAPI_DEVICE_INFO_TYPE_SEND_SMS_RSP_E||
          info->info_type == QAPI_DEVICE_INFO_TYPE_SMS_STATUS_REPORT_E) &&
          info->u.device_info_extn != NULL)
      {
        if(info->info_type == QAPI_DEVICE_INFO_TYPE_ROUTE_INCOMING_SMS_E)
        {
          qapi_Device_Info_Route_Incoming_SMS_Ind_t *u_ind = info->u.device_info_extn;
          if(u_ind->smsc_number.length && u_ind->smsc_number.number)
          {
            tx_byte_release(u_ind->smsc_number.number);
            u_ind->smsc_number.number = NULL;
          }
          if(u_ind->sender_number.length && u_ind->sender_number.number)
          {
            tx_byte_release(u_ind->sender_number.number);
            u_ind->sender_number.number = NULL;
          }
          if(u_ind->sms_payload_length && u_ind->sms_payload)
          {
            tx_byte_release(u_ind->sms_payload);
            u_ind->sms_payload = NULL;
          }
        }
        else if(info->info_type == QAPI_DEVICE_INFO_TYPE_SMS_STATUS_REPORT_E)
        {
          qapi_Device_Info_SMS_Staus_Report_t *u_ind = info->u.device_info_extn;
          if(u_ind->smsc_number.length && u_ind->smsc_number.number)
          {
            tx_byte_release(u_ind->smsc_number.number);
            u_ind->smsc_number.number = NULL;
          }
          if(u_ind->reciever_number.length && u_ind->reciever_number.number)
          {
            tx_byte_release(u_ind->reciever_number.number);
            u_ind->reciever_number.number = NULL;
          }
          if(u_ind->sms_payload_length && u_ind->sms_payload)
          {
            tx_byte_release(u_ind->sms_payload);
            u_ind->sms_payload = NULL;
          }
        }
        tx_byte_release(info->u.device_info_extn);
      }
      tx_byte_release(info);
    }
  }
}


#define qapi_Device_Info_Init_v2(a)               ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_INIT2,    (ULONG) a, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Device_Info_Get_v2(a, b, c)          ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_GET2,     (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Device_Info_Release_v2(a)            device_info_release_indirection(a)
#define qapi_Device_Info_Set_Callback_v2(a, b, c) ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_SET_CB2,  (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) qapi_device_info_cb_uspace_dispatcher, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Device_Info_Reset_v2(a)              ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_RESET2,   (ULONG)a, (ULONG)0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Device_Info_Set(a, b, c, d)          ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_SET,     (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Device_Info_Request(a, b, c, d)      ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_REQUEST, (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Device_Info_Clear_Callback_v2(a, b)  ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_CLEAR_CB2, (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))

/*
 * Macro that Passes Byte Pool Pointer for Application invoking device info QAPI's
 * Parameter 'a' : Handle
 * Parameter 'b' : Pointer to Byte Pool
 * On success, QAPI_OK is returned. On error, appropriate QAPI_ERR_ code(qapi_status.h) is returned.
 * (This Macro is only used in DAM Space)
 */

#define qapi_Device_Info_Pass_Pool_Ptr(a,b)             device_info_set_byte_pool(a,b)

/** @} */

static __inline int device_info_set_byte_pool(qapi_Device_Info_Hndl_t hndl, void *device_info_byte_pool_ptr)
{
  qapi_cb_params_uspace_ptr_t *uspace_memory_device_info = NULL;
  int ret = QAPI_ERROR;

  /* input parameter validation*/
  if((hndl == NULL) || device_info_byte_pool_ptr == NULL)
  {
	return QAPI_ERR_INVALID_PARAM;
  }
  

  /* Retrieve the user space information stored internally */
  qapi_data_map_handle_to_u_addr(hndl, QAPI_APP_DEVICE_INFO, (void**)&uspace_memory_device_info);
  /* Handle to user space information already exists */
  if (uspace_memory_device_info)
  {
    return QAPI_OK;
  }

  tx_byte_allocate(device_info_byte_pool_ptr, (VOID **) &uspace_memory_device_info,(sizeof(qapi_cb_params_uspace_ptr_t)), TX_NO_WAIT);
  if (NULL == uspace_memory_device_info)
  {
    return QAPI_ERR_NO_MEMORY;
  }

  memset (uspace_memory_device_info,0, sizeof(qapi_cb_params_uspace_ptr_t));

  
  /* Initialize user space information */
  uspace_memory_device_info->usr_pool_ptr = device_info_byte_pool_ptr;  
  
  /* Map the user space information to the handle internally and store */ 
  ret = qapi_data_map_u_addr_to_handle((void *)(hndl), QAPI_APP_DEVICE_INFO, uspace_memory_device_info, 0x00);
  if (ret != QAPI_OK)
  {
    /* Release the allocated resources */
    if (uspace_memory_device_info)
      tx_byte_release(uspace_memory_device_info);
  }
  
  return ret;
  
}

/*
 * Releases the user space allocated resources and dispatches the call to qapi_Devive_Info_Release2 
 * Parameter 'a' : Handle
 * On success, #QAPI_OK is returned. On error, appropriate QAPI_ERR_ code(qapi_status.h) is returned.
 * (This is only used in DAM Space)
 */
static __inline int device_info_release_indirection(qapi_Device_Info_Hndl_t hndl)
{
  int ret = QAPI_ERROR;
  qapi_cb_params_uspace_ptr_t *uspace_memory_device_info = NULL;
  
  if(hndl == NULL)
  {
    return QAPI_ERR_INVALID_PARAM;
  }
  
  /* Retrieve the user space information stored internally */
  qapi_data_map_handle_to_u_addr(hndl, QAPI_APP_DEVICE_INFO, (void**)&uspace_memory_device_info);
  
  /* Release the allocated resources */
  if (uspace_memory_device_info)
    tx_byte_release(uspace_memory_device_info);
  
  /* Dispatch the call to the module manager */
  ret = ((qapi_Status_t) (_txm_module_system_call12)(TXM_QAPI_DEVICE_INFO_RELEASE2, (ULONG) hndl, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0));
  return ret;
}

#else

qapi_Status_t qapi_Device_Info_Handler(UINT id, UINT a, UINT b, UINT c, UINT d, UINT e, UINT f, UINT g, UINT h, UINT i, UINT j, UINT k, UINT l);
qapi_Status_t qapi_Device_Info_Handler_v2(UINT id, UINT a, UINT b, UINT c, UINT d, UINT e, UINT f, UINT g, UINT h, UINT i, UINT j, UINT k, UINT l);


/*=======================================================
 * QAPI Device Info APIs                             *
 =======================================================*/

/**
 * Initializes the device information context.
 *
 * @param[in] device_info_hndl Pointer to device info hndl
 *
 * This function must be called before invoking other qapi_Device_Info APIs.
 *
 * @return #QAPI_OK on success, #QAPI_ERROR on failure. @newpage
 */
qapi_Status_t qapi_Device_Info_Init_v2(qapi_Device_Info_Hndl_t *device_info_hndl);


/**
 * Gets the device information for specified ID.
 *
 * @param[in] device_info_hndl  Device info hndl.
 * @param[in] id                Information ID.
 * @param[out] info             Information received for the specified ID.
 *
 * @return #QAPI_OK on success, #QAPI_ERROR on failure.
 *
 * @dependencies
 * Before calling this API, qapi_Device_Info_Init_v2() must have been called. @newpage
 */
qapi_Status_t qapi_Device_Info_Get_v2(qapi_Device_Info_Hndl_t device_info_hndl, qapi_Device_Info_ID_t id, qapi_Device_Info_t *info);


/**
 * Sets device information callback and associated information ID.
 *
 * @param[in] device_info_hndl  Device info hndl.
 * @param[in] id                Information ID.to be set.
 * @param[in] callback          Callback to be registered.
 *
 * @return #QAPI_OK on success, #QAPI_ERROR on failure.
 *
 * @dependencies
 * Before calling this API, qapi_Device_Info_Init_v2() must have been called. @newpage
 */
qapi_Status_t qapi_Device_Info_Set_Callback_v2(qapi_Device_Info_Hndl_t device_info_hndl, qapi_Device_Info_ID_t id, qapi_Device_Info_Callback_t_v2 callback);

/**
 * Releases the device information context.
 *
 * @param[in] device_info_hndl  Device info hndl.
 *
 * @return #QAPI_OK on success, #QAPI_ERROR on failure.
 *
 * @dependencies
 * Before calling this API, qapi_Device_Info_Init_v2() must have been called.
 */
qapi_Status_t qapi_Device_Info_Release_v2(qapi_Device_Info_Hndl_t device_info_hndl);


/**
 * Resets the device.
 *
 * @param[in] device_info_hndl  Device info hndl.
 *
 * @return #QAPI_OK on success, #QAPI_ERROR on failure.
 */
qapi_Status_t qapi_Device_Info_Reset_v2(qapi_Device_Info_Hndl_t device_info_hndl);

/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Sets device information for specified ID.
 *
 * @param[in] device_info_hndl  The device information handle.
 * @param[in] id                Information ID.
 * @param[in] len               Length of the values to be set for the specified ID.
 * @param[in] *val              Values to be set for the specified ID.
 *
 *  @return #QAPI_OK on success, #QAPI_ERROR on failure.
 *
 * @dependencies
 * Before calling this API, qapi_Device_Info_Init_v2() must have been called. @newpage
 */
qapi_Status_t qapi_Device_Info_Set(qapi_Device_Info_Hndl_t device_info_hndl, qapi_Device_Info_ID_t id, int len, int *val);

/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.6.0  &  Introduced. @tblendline
 * }
 *
 * Sets device information for specified ID.
 *
 * @param [in] device_info_hndl  The device information handle.
 * @param [in] id                Device information request type.
 * @param [in] req               Device information request structure that sets parameters 
 *                               based on request ID.
 * @param [in, out] rsp          Device information response structure that fetches parameters
 *                               based on request ID.
 *
 * @return 
 * #QAPI_OK on success, #QAPI_ERROR on failure.
 *
 * @note1hang The device should be in low power mode with no available subscription for the following IDs.
         Otherwise, QAPI returns "#QAPI_ERR_INVALID_STATE" error. \n
 
 *       - #QAPI_DEVICE_INFO_REQ_FSK_START_E
 *       - #QAPI_DEVICE_INFO_REQ_FSK_STOP_E
 *       - #QAPI_DEVICE_INFO_REQ_GET_FSK_DEBUG_E
 *       - #QAPI_DEVICE_INFO_REQ_SET_FSK_DEBUG_E
 *       - #QAPI_DEVICE_INFO_REQ_GET_FSK_PCL_E
 *       - #QAPI_DEVICE_INFO_REQ_SET_FSK_PCL_E
 *
 *
 * @dependencies
 * Before calling this function, qapi_Device_Info_Init_v2() must be called. @newpage
 */
qapi_Status_t qapi_Device_Info_Request
(
  qapi_Device_Info_Hndl_t      device_info_hndl, 
  qapi_Device_Info_Req_ID_t    id, 
  qapi_Device_Info_Request_t*  req, 
  qapi_Device_Info_Response_t* rsp
);



/**
 * @versiontable{2.0,2.45,
 * Data\_Services 1.6.0  &  Introduced. @tblendline
 * }
 * Clears a particular indication associated with a handle.
 *
 * @param [in] device_info_hndl  The device information handle.
 * @param [in] id                Information ID to be cleared.
 *
 * @return #QAPI_OK on success, #QAPI_ERROR on failure.
 *
 * @dependencies
 * Before calling this function, qapi_Device_Info_Init_v2() must be called.
 * This function clears the indication set by using qapi_Device_Info_Set_Callback_v2().
 */
 qapi_Status_t qapi_Device_Info_Clear_Callback_v2(qapi_Device_Info_Hndl_t device_info_hndl, qapi_Device_Info_ID_t id);

#endif

/** @} */ /* end_addtogroup qapi_device_info */

#endif
