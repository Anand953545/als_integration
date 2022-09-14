/* Copyright (c) 2017-2022 Qualcomm Technologies, Inc. and/or its
subsidiaries. All rights reserved.
Confidential - Qualcomm Technologies, Inc. - May Contain Trade Secrets.
*/

/**
 * @file qapi_location.h
 *
 * @details Functions for GNSS location driver.
 *
 */

#ifndef _QAPI_LOCATION_H_
#define _QAPI_LOCATION_H_

#include <stdint.h>
#include <stddef.h>
#include "qapi_txm_base.h"
#include "stringl.h"

/*
!! IMPORTANT NOTE: "LOCATION" VERSION CATEGORY.
*/

#ifdef __cplusplus
extern "C" {
#endif

#define TXM_QAPI_LOCATION_INIT                         TXM_QAPI_LOCATION_BASE
#define TXM_QAPI_LOCATION_DEINIT                       TXM_QAPI_LOCATION_BASE + 1
#define TXM_QAPI_LOCATION_START_TRACKING               TXM_QAPI_LOCATION_BASE + 2
#define TXM_QAPI_LOCATION_STOP_TRACKING                TXM_QAPI_LOCATION_BASE + 3
#define TXM_QAPI_LOCATION_UPDATE_TRACKING_OPTIONS      TXM_QAPI_LOCATION_BASE + 4
#define TXM_QAPI_LOCATION_START_BATCHING               TXM_QAPI_LOCATION_BASE + 5
#define TXM_QAPI_LOCATION_STOP_BATCHING                TXM_QAPI_LOCATION_BASE + 6
#define TXM_QAPI_LOCATION_UPDATE_BATCHING_OPTIONS      TXM_QAPI_LOCATION_BASE + 7
#define TXM_QAPI_LOCATION_GET_BATCHED_LOCATIONS        TXM_QAPI_LOCATION_BASE + 8
#define TXM_QAPI_LOCATION_ADD_GEOFENCES                TXM_QAPI_LOCATION_BASE + 9
#define TXM_QAPI_LOCATION_REMOVE_GEOFENCES             TXM_QAPI_LOCATION_BASE + 10
#define TXM_QAPI_LOCATION_MODIFY_GEOFENCES             TXM_QAPI_LOCATION_BASE + 11
#define TXM_QAPI_LOCATION_PAUSE_GEOFENCES              TXM_QAPI_LOCATION_BASE + 12
#define TXM_QAPI_LOCATION_RESUME_GEOFENCES             TXM_QAPI_LOCATION_BASE + 13
#define TXM_QAPI_LOCATION_GET_SINGLE_SHOT              TXM_QAPI_LOCATION_BASE + 14
#define TXM_QAPI_LOCATION_CANCEL_SINGLE_SHOT           TXM_QAPI_LOCATION_BASE + 15
#define TXM_QAPI_LOCATION_SET_USER_BUFFER              TXM_QAPI_LOCATION_BASE + 16
#define TXM_QAPI_LOCATION_START_GET_GNSS_DATA          TXM_QAPI_LOCATION_BASE + 17
#define TXM_QAPI_LOCATION_STOP_GET_GNSS_DATA           TXM_QAPI_LOCATION_BASE + 18
#define TXM_QAPI_LOCATION_GET_BACKOFF_TIME             TXM_QAPI_LOCATION_BASE + 19
#define TXM_QAPI_LOCATION_GET_BEST_AVAILABLE_POSITION  TXM_QAPI_LOCATION_BASE + 20
#define TXM_QAPI_LOCATION_GET_SINGLE_SHOT_V2           TXM_QAPI_LOCATION_BASE + 21
#define TXM_QAPI_LOCATION_START_MOTION_TRACKING        TXM_QAPI_LOCATION_BASE + 22
#define TXM_QAPI_LOCATION_STOP_MOTION_TRACKING         TXM_QAPI_LOCATION_BASE + 23
#define TXM_QAPI_LOCATION_START_NTP_DOWNLOAD           TXM_QAPI_LOCATION_BASE + 24
#define TXM_QAPI_LOCATION_GET_GNSS_ENERGY_CONSUMED     TXM_QAPI_LOCATION_BASE + 25
#define TXM_QAPI_LOCATION_INIT_V2                      TXM_QAPI_LOCATION_BASE + 26
#define TXM_QAPI_LOCATION_GET_SINGLE_SHOT_V3           TXM_QAPI_LOCATION_BASE + 27
#define TXM_QAPI_LOCATION_SET_GNSS_CONFIG              TXM_QAPI_LOCATION_BASE + 28
#define TXM_QAPI_LOCATION_GET_AD_STATUS                TXM_QAPI_LOCATION_BASE + 29
#define TXM_QAPI_LOCATION_TRIGGER_AD_UPDATE            TXM_QAPI_LOCATION_BASE + 30
#define TXM_QAPI_LOCATION_CLIENT_REQUEST_CB_RESPONSE   TXM_QAPI_LOCATION_BASE + 31
#define TXM_QAPI_LOCATION_GET_GNSS_CONFIG              TXM_QAPI_LOCATION_BASE + 32

#define CUSTOM_CB_QAPI_LOCATION_CAPABILITIES        CUSTOM_CB_QAPI_LOCATION_BASE
#define CUSTOM_CB_QAPI_LOCATION_RESPONSE            CUSTOM_CB_QAPI_LOCATION_BASE + 1
#define CUSTOM_CB_QAPI_LOCATION_COLLECTIVE_RESPONSE CUSTOM_CB_QAPI_LOCATION_BASE + 2
#define CUSTOM_CB_QAPI_LOCATION_TRACKING            CUSTOM_CB_QAPI_LOCATION_BASE + 3
#define CUSTOM_CB_QAPI_LOCATION_BATCHING            CUSTOM_CB_QAPI_LOCATION_BASE + 4
#define CUSTOM_CB_QAPI_LOCATION_GEOFENCE_BREACH     CUSTOM_CB_QAPI_LOCATION_BASE + 5
#define CUSTOM_CB_QAPI_LOCATION_SINGLE_SHOT         CUSTOM_CB_QAPI_LOCATION_BASE + 6
#define CUSTOM_CB_QAPI_LOCATION_GNSS_DATA           CUSTOM_CB_QAPI_LOCATION_BASE + 7
#define CUSTOM_CB_QAPI_LOCATION_META_DATA           CUSTOM_CB_QAPI_LOCATION_BASE + 8
#define CUSTOM_CB_QAPI_LOCATION_GNSS_NMEA           CUSTOM_CB_QAPI_LOCATION_BASE + 9
// Use Location Extended Base for further callbacks
#define CUSTOM_CB_QAPI_LOCATION_MOTION_TRACKING     CUSTOM_CB_QAPI_LOCATION_EXT_BASE
#define CUSTOM_CB_QAPI_LOCATION_CLIENT_REQUEST      CUSTOM_CB_QAPI_LOCATION_EXT_BASE + 1

 /** @addtogroup qapi_location
 @{ */

 /**
  * @versiontable{2.3,2.7,
  * Location 1.2.0  &  Added last 4 enums. @tblendline
  * Location 1.5.0  &  Added last 2 enums. @tblendline
  * }
  * GNSS location error codes.
  */
typedef enum {
    QAPI_LOCATION_ERROR_SUCCESS                   = 0,  /**< Success. */
    QAPI_LOCATION_ERROR_GENERAL_FAILURE           = 1,  /**< General failure. */
    QAPI_LOCATION_ERROR_CALLBACK_MISSING          = 2,  /**< Callback is missing. */
    QAPI_LOCATION_ERROR_INVALID_PARAMETER         = 3,  /**< Invalid parameter. */
    QAPI_LOCATION_ERROR_ID_EXISTS                 = 4,  /**< ID already exists. */
    QAPI_LOCATION_ERROR_ID_UNKNOWN                = 5,  /**< ID is unknown. */
    QAPI_LOCATION_ERROR_ALREADY_STARTED           = 6,  /**< Already started. */
    QAPI_LOCATION_ERROR_NOT_INITIALIZED           = 7,  /**< Not initialized. */
    QAPI_LOCATION_ERROR_GEOFENCES_AT_MAX          = 8,  /**< Maximum number of geofences reached. */
    QAPI_LOCATION_ERROR_NOT_SUPPORTED             = 9,  /**< Not supported. */
    QAPI_LOCATION_ERROR_TIMEOUT                   = 10, /**< Timeout when asking single shot. Added in Location 1.2.0. */
    QAPI_LOCATION_ERROR_LOAD_FAILURE              = 11, /**< GNSS engine could not get loaded. Added in Location 1.2.0. */
    QAPI_LOCATION_ERROR_LOCATION_DISABLED         = 12, /**< Location module license is disabled. Added in Location 1.2.0. */
    QAPI_LOCATION_ERROR_BEST_AVAIL_POS_INVALID    = 13, /**< Best available position is invalid. Added in Location 1.2.0. */
    QAPI_LOCATION_ERROR_TIMEOUT_ACC_CHECK_FAILED  = 14, /**< Timeout when asking for a single shot. Added in Location 1.5.0. */
    QAPI_LOCATION_ERROR_THRESHOLD_REACHED         = 15, /**< GTP 2.0 threshold reached. Added in Location 1.5.0. */
    QAPI_LOCATION_ERROR_TIME_UPDATE_REQUIRED      = 16, /**< Time update required for the requested session.
                                                             Added in Location 1.9.0 */
    QAPI_LOCATION_ERROR_XTRA_UPDATE_REQUIRED      = 17, /**< XTRA update required for the requested session.
                                                             Added in Location 1.11.0 */
    QAPI_LOCATION_ERROR_XTRA_DISABLED             = 18, /**< XTRA download is disabled.
                                                             Added in Location 1.11.0 */
    QAPI_LOCATION_ERROR_API_RESTRICTED_TO_MASTER_CLIENT = 19, /**< API can only be invoked by master client.
                                                             Added in Location 1.11.0 */
    QAPI_LOCATION_ERROR_MASTER_CLIENT_ALREADY_CREATED   = 20, /**< Master client has already registered.
                                                             Added in Location 1.12.0 */
} qapi_Location_Error_t;

typedef uint16_t qapi_Location_Flags_Mask_t;
/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Added MSL and best available position bits. @tblendline
 * }
 *
 * Flags to indicate which values are valid in a location.
 */
typedef enum {
    QAPI_LOCATION_HAS_LAT_LONG_BIT =          (1 << 0),   /**< Location has a valid latitude and longitude. */
    QAPI_LOCATION_HAS_ALTITUDE_BIT =          (1 << 1),   /**< Location has a valid altitude. */
    QAPI_LOCATION_HAS_SPEED_BIT =             (1 << 2),   /**< Location has a valid speed. */
    QAPI_LOCATION_HAS_BEARING_BIT =           (1 << 3),   /**< Location has a valid bearing. */
    QAPI_LOCATION_HAS_ACCURACY_BIT =          (1 << 4),   /**< Location has valid accuracy. */
    QAPI_LOCATION_HAS_VERTICAL_ACCURACY_BIT = (1 << 5),   /**< Location has valid vertical accuracy. */
    QAPI_LOCATION_HAS_SPEED_ACCURACY_BIT =    (1 << 6),   /**< Location has valid speed accuracy. */
    QAPI_LOCATION_HAS_BEARING_ACCURACY_BIT =  (1 << 7),   /**< Location has valid bearing accuracy. */
    QAPI_LOCATION_HAS_ALTITUDE_MSL_BIT =      (1 << 8),   /**< Location has valid altitude with regard to mean sea level. */
    QAPI_LOCATION_IS_BEST_AVAIL_POS_BIT =     (1 << 9),   /**< Location is the currently best available position. */
    QAPI_LOCATION_HAS_CONFORMITY_INDEX_BIT  = (1 <<10),   /**< RL2.x is enabled, hence location has Conformity index. */
    QAPI_LOCATION_HAS_SPOOFING_MASK_BIT     = (1 <<11),   /**< RL1.x is enabled, hence location has spoofing mask. */
} qapi_Location_Flags_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Enum representing the location technologies.
 */
typedef enum {
    QAPI_LOCATION_TECH_GNSS_BIT          =  (1 << 0),  /**< GNSS */
    QAPI_LOCATION_TECH_GTP_1_0_BIT       =  (1 << 1),  /**< GTP WWAN 1.0 */
    QAPI_LOCATION_TECH_GTP_2_0_BIT       =  (1 << 2),  /**< GTP WWAN 2.0 */
    QAPI_LOCATION_TECH_WIFI_BIT          =  (1 << 3),  /**< WIFI Positioning */
    QAPI_LOCATION_TECH_BEST_AVAIL_BIT    =  (1 << 4),  /**< WIFI Positioning */
} qapi_Location_Tech_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Mask comprising of bits from #qapi_Location_Tech_t
 */
typedef uint32_t qapi_Location_Tech_Mask_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Flags that indicate which values are valid in metadata.
 */
typedef enum {
    QAPI_LOCATION_META_DATA_HAS_BACKOFF_TIME                  =  (1 << 0),   /**< Metadata has backoff time. */
    QAPI_LOCATION_META_DATA_HAS_DISTANCE_TO_NEAREST_GEOFENCE  =  (1 << 1),   /**< Metadata has distance to nearest geofence. */
    QAPI_LOCATION_META_DATA_ENERGY_CONSUMED_SINCE_LAST_BOOT   =  (1 << 2),   /**< Metadata has energy consumed since last boot. */
    QAPI_LOCATION_META_DATA_ENERGY_CONSUMED_SINCE_FIRST_BOOT  =  (1 << 3),   /**< Metadata has energy consumed since first boot. */
    QAPI_LOCATION_META_DATA_HAS_ENGINE_STATUS                 =  (1 << 4),   /**< Metadata has location engine status. */
    QAPI_LOCATION_META_DATA_HAS_AD_STATUS_INFO                =  (1 << 5),   /**< Metadata has Assistance Data status. */
    QAPI_LOCATION_META_DATA_HAS_GNSS_CONFIG                   =  (1 << 6),   /**< Metadata has GNSS Config. */
} qapi_Location_Meta_Data_Flags_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Mask comprising of bits from #qapi_Location_Meta_Data_Flags_t
 */
typedef uint32_t qapi_Location_Meta_Data_Flags_Mask_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.3.0  &  Introduced. @tblendline
 * }
 *
 * Location engine status events.
 */
typedef enum {
    QAPI_LOCATION_ENGINE_STATUS_UNKNOWN = 0,
    QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_SUCCESS,      /**< Data call for assistance data DL successful. */
    QAPI_LOCATION_ENGINE_STATUS_DATA_CALL_FAILED,       /**< Data call for assistance data DL failed. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_START,          /**< XTRA assistance data DL started. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_SUCCESS,        /**< XTRA assistance data DL successful. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_FAILED,         /**< XTRA assistance data DL failed. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_DL_THROTTLED,      /**< XTRA assistance data DL throttled. */
    QAPI_LOCATION_ENGINE_STATUS_NTP_DL_START,           /**< Time info DL started. */
    QAPI_LOCATION_ENGINE_STATUS_NTP_DL_SUCCESS,         /**< Time info DL successful. */
    QAPI_LOCATION_ENGINE_STATUS_NTP_DL_FAILED,          /**< Time info DL failed. */
    QAPI_LOCATION_ENGINE_STATUS_NTP_DL_THROTTLED,       /**< Time info DL throttled. */
    QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_START,      /**< GTP WWAN positioning started. */
    QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_SUCCESS,    /**< GTP WWAN positioning successful. */
    QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_POS_FAILED,     /**< GTP WWAN positioning failed. */
    QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_START,         /**< WIFI positioning started. */
    QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_SUCCESS,       /**< WIFI positioning successful. */
    QAPI_LOCATION_ENGINE_STATUS_WIFI_POS_FAILED,        /**< WIFI positioning failed. */
    QAPI_LOCATION_ENGINE_STATUS_LOAD_SUCCESS,           /**< Location engine started position tracking. */
    QAPI_LOCATION_ENGINE_STATUS_LOAD_FAILED,            /**< Location engine failed to start position tracking. */
    QAPI_LOCATION_ENGINE_STATUS_UNLOADED,               /**< Location engine interrupted while deriving position. */
    QAPI_LOCATION_ENGINE_STATUS_FIX_FAILED,             /**< Location engine failed to derive a position. */
    QAPI_LOCATION_ENGINE_STATUS_POS_ACC_CHECK_FAILED,   /**< Position report generated but failed accuracy check. */
    QAPI_LOCATION_ENGINE_STATUS_FIX_SUCCESS,            /**< Location engine successful in deriving a position. */
    QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_START,    /**< GTP WWAN maintenance started. */
    QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_SUCCESS,  /**< GTP WWAN maintenance successful.*/
    QAPI_LOCATION_ENGINE_STATUS_GTPWWAN_MAINT_FAILED,   /**< GTP WWAN maintenance failed. */
    QAPI_LOCATION_ENGINE_STATUS_WEAK_GNSS_SIGNALS,      /**< GNSS signal strength is weak. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_START,      /**< XTRA Integrity assistance data DL started. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_SUCCESS,    /**< XTRA Integrity assistance data DL successful. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_FAILED,     /**< XTRA Integrity assistance data DL failed. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_THROTTLED,  /**< XTRA Integrity assistance data DL throttled. */
    QAPI_LOCATION_ENGINE_STATUS_XTRA_INT_DL_DISABLED,   /**< XTRA Integrity assistance data DL disabled. */
} qapi_Location_Meta_Data_Engine_Status_t;

/**
 * Flags to indicate Geofence breach status.
 */
typedef enum {
    QAPI_GEOFENCE_BREACH_ENTER = 0, /**< Entering Geofence breach. */
    QAPI_GEOFENCE_BREACH_EXIT,      /**< Exiting Geofence breach. */
    QAPI_GEOFENCE_BREACH_DWELL_IN,  /**< Dwelling in a breached Geofence. */
    QAPI_GEOFENCE_BREACH_DWELL_OUT, /**< Dwelling outside of a breached Geofence. */
    QAPI_GEOFENCE_BREACH_UNKNOWN,   /**< Breach is unknown. */
} qapi_Geofence_Breach_t;

typedef uint16_t qapi_Geofence_Breach_Mask_t;
/** Flags to indicate Geofence breach mask bit. */
typedef enum {
    QAPI_GEOFENCE_BREACH_ENTER_BIT =     (1 << 0),   /**< Breach enter bit. */
    QAPI_GEOFENCE_BREACH_EXIT_BIT =      (1 << 1),   /**< Breach exit bit. */
    QAPI_GEOFENCE_BREACH_DWELL_IN_BIT =  (1 << 2),   /**< Breach dwell in bit. */
    QAPI_GEOFENCE_BREACH_DWELL_OUT_BIT = (1 << 3),   /**< Breach dwell out bit. */
} qapi_Geofence_Breach_Mask_Bits_t;

typedef uint32_t qapi_Location_Capabilities_Mask_t;
/** Flags to indicate the capabilities bit. */
typedef enum {
    QAPI_LOCATION_CAPABILITIES_TIME_BASED_TRACKING_BIT =     (1 << 0),  /**< Capabilities time-based tracking bit. */
    QAPI_LOCATION_CAPABILITIES_TIME_BASED_BATCHING_BIT =     (1 << 1),  /**< Capabilities time-based batching bit. */
    QAPI_LOCATION_CAPABILITIES_DISTANCE_BASED_TRACKING_BIT = (1 << 2),  /**< Capabilities distance-based tracking bit. */
    QAPI_LOCATION_CAPABILITIES_DISTANCE_BASED_BATCHING_BIT = (1 << 3),  /**< Capabilities distance-based batching bit. */
    QAPI_LOCATION_CAPABILITIES_GEOFENCE_BIT =                (1 << 4),  /**< Capabilities Geofence bit. */
    QAPI_LOCATION_CAPABILITIES_GNSS_DATA_BIT =               (1 << 5),  /**< Capabilities GNSS data bit. */
} qapi_Location_Capabilities_Mask_Bits_t;

/** Flags to indicate the constellation type. */
typedef enum {
    QAPI_GNSS_SV_TYPE_UNKNOWN = 0, /**< Unknown. */
    QAPI_GNSS_SV_TYPE_GPS,         /**< GPS. */
    QAPI_GNSS_SV_TYPE_SBAS,        /**< SBAS. */
    QAPI_GNSS_SV_TYPE_GLONASS,     /**< GLONASS. */
    QAPI_GNSS_SV_TYPE_QZSS,        /**< QZSS. */
    QAPI_GNSS_SV_TYPE_BEIDOU,      /**< BEIDOU. */
    QAPI_GNSS_SV_TYPE_GALILEO,     /**< GALILEO. */
    QAPI_GNSS_SV_TYPE_NAVIC,       /**< GALILEO. */
    QAPI_MAX_NUMBER_OF_CONSTELLATIONS /**< Maximum number of constellations. */
} qapi_Gnss_Sv_t;

/** Flags to indicate allowed power levels for fix computation. */
typedef enum {
    QAPI_LOCATION_POWER_HIGH = 0,           /**< Use all technologies available to calculate location.   */
    QAPI_LOCATION_POWER_LOW,                /**< Use all low power technologies to calculate location.   */
    QAPI_LOCATION_POWER_MED,                /**< Use only low and medium power technologies to calculate location */
} qapi_Location_Power_Level_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Flags to indicate the desired level of accuracy for fix computation.
 */
typedef enum {
    QAPI_LOCATION_ACCURACY_UNKNOWN = 0,     /**< Accuracy is not specified, use default. */
    QAPI_LOCATION_ACCURACY_LOW,             /**< Low Accuracy for location is acceptable. */
    QAPI_LOCATION_ACCURACY_MED,             /**< Medium Accuracy for location is acceptable. */
    QAPI_LOCATION_ACCURACY_HIGH,            /**< Only High Accuracy for location is acceptable. */
} qapi_Location_Accuracy_Level_t;

typedef uint32_t qapi_Location_Motion_Type_Mask_t;
/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Flags to indicate the type of motion.
 */
typedef enum {
    QAPI_LOCATION_MOTION_TYPE_REST =              (1 << 0),   /**< Device at rest. */
    QAPI_LOCATION_MOTION_TYPE_MOVE_ABSOLUTE =     (1 << 1),   /**< Device in motion. */
    QAPI_LOCATION_MOTION_TYPE_MOVE_SIGNIFICANT =  (1 << 2),   /**< Device in significant motion,
                                                                   where significant motion is
                                                                   identified by the device being
                                                                   in motion for a certain amount
                                                                   of time. @newpage
                                                                   */
} qapi_Location_Motion_Type_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Enum indicating spoofed location data.
 */
typedef enum {
    QAPI_LOCATION_SPOOF_BIT_POSTION_SPOOFED             = (1<<0), /**< Location position spoofed */
    QAPI_LOCATION_SPOOF_BIT_TIME_SPOOFED                = (1<<1), /**< Location time spoofed */
    QAPI_LOCATION_SPOOF_BIT_NAVIGATION_DATA_SPOOFED     = (1<<2), /**< Location navigation data spoofed */
} qapi_Location_Spoof_Bits_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Enum indicating GNSS Constellation types.
 */
typedef enum {
    QAPI_GNSS_SV_MASK_BIT_GLO      = (1<<0),
    QAPI_GNSS_SV_MASK_BIT_BDS      = (1<<1),
    QAPI_GNSS_SV_MASK_BIT_QZSS     = (1<<2),
    QAPI_GNSS_SV_MASK_BIT_GAL      = (1<<3),
    QAPI_GNSS_SV_MASK_BIT_NAVIC    = (1<<4),
} qapi_Gnss_Sv_Mask_Bits_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Enum of different XTRA config items which can be dynamically
 * updated.
 */
typedef enum {
    QAPI_XTRA_CONFIG_TYPE_XTRA_FILE_DURATION         = (1<<0),
    QAPI_XTRA_CONFIG_TYPE_XTRA_CONSTELLATION_MASK    = (1<<1),
    QAPI_XTRA_CONFIG_TYPE_XTRA_INT_ENABLED           = (1<<2),
} qapi_Xtra_Config_Type_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Enum indicating XTRA file duration config.
 */
typedef enum {
    QAPI_XTRA_CONFIG_FILE_DURATION_24_HRS = 1,
    QAPI_XTRA_CONFIG_FILE_DURATION_72_HRS = 2,
    QAPI_XTRA_CONFIG_FILE_DURATION_168_HRS = 3
} qapi_Xtra_Config_File_Duration_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Enum indicating different Assistance Data types.
 */
typedef enum {
    QAPI_AD_TYPE_GTP                = (1<<0),
    QAPI_AD_TYPE_XTRA               = (1<<1),
    QAPI_AD_TYPE_XTRA_INTEGRITY     = (1<<2),
    QAPI_AD_TYPE_TIME               = (1<<3),
    QAPI_AD_TYPE_GTP_CS             = (1<<4),
} qapi_AD_Type_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Spoof mask comprising of bits from #qapi_Location_Spoof_Bits_t
 */
typedef uint32_t qapi_Location_Spoof_Mask_t;

/** Structure for location information. */
typedef struct {
    size_t size;                             /**< Size. Set to the size of qapi_Location_t. */
    qapi_Location_Flags_Mask_t flags;        /**< Bitwise OR of qapi_Location_Flags_t. */
    uint64_t timestamp;                      /**< UTC timestamp for a location fix; milliseconds since Jan. 1, 1970. */
    double latitude;                         /**< Latitude in degrees. */
    double longitude;                        /**< Longitude in degrees. */
    double altitude;                         /**< Altitude in meters above the WGS 84 reference ellipsoid. */
    double altitudeMeanSeaLevel;             /**< Altitude in meters with respect to mean sea level. */
    float speed;                             /**< Speed in meters per second. */
    float bearing;                           /**< Bearing in degrees; range: 0 to 360. */
    float accuracy;                          /**< Accuracy in meters. */
    float verticalAccuracy;                  /**< Vertical accuracy in meters. */
    float speedAccuracy;                     /**< Speed accuracy in meters/second. */
    float bearingAccuracy;                   /**< Bearing accuracy in degrees (0 to 359.999). */
    float conformityIndex;                   /**< Conformity Index in range [0, 1] */
    qapi_Location_Spoof_Mask_t spoofMask;    /**< Mask indicating which position parameter is spoofed. */
} qapi_Location_t;

/** Structure for GNSS data information. */
typedef struct {
    size_t size;                                            /**< Size. Set to the size of #qapi_Gnss_Data_t. */
    uint32_t jammerInd[QAPI_MAX_NUMBER_OF_CONSTELLATIONS];  /**< JammerInd is an array. */
} qapi_Gnss_Data_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Mask comprising of bits from #qapi_AD_Type_t
 */
typedef uint32_t qapi_AD_Type_Mask_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for XTRA Assistance Status Info. 
 */
typedef struct {
    size_t size;                                /**< Size. Set to the size of
                                                     #qapi_Xtra_AD_Status_t. */

    uint64_t             xtraDataStartTime;     /**< UTC millis since epoch value for XTRA data start.
                                                     This value will be decoded after a successful XTRA DL. */

    uint32_t             xtraFileDurationHrs;   /**< Validity duration of last downloaded XTRA file in hours. */

    uint64_t             xtraFileGenerationTime;  /**< UTC millis since epoch value for when the last downloaded
                                                       XTRA file was generated on the server.
                                                       This value will be decoded after a successful XTRA DL */
} qapi_Xtra_AD_Status_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for XTRA Integrity Assistance Status Info. 
 */
typedef struct {
    size_t size;                                /**< Size. Set to the size of
                                                     #qapi_Xtra_Int_AD_Status_t. */

    uint64_t             lastXtraIntDlSuccess;  /**< UTC millis since epoch value for last successful
                                                     XTRA Integrity download */
} qapi_Xtra_Int_AD_Status_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for GTP Assistance Status Info. 
 */
typedef struct {
    size_t size;                            /**< Size. Set to the size of
                                                 #qapi_Gtp_AD_Status_t. */

    uint8_t           gtpCellAdValid;       /**< Flag indicating whether GTP Cell AD is valid */
    uint8_t           gtpL3AdValid;         /**< Flag indicating whether GTP L3 AD is valid */
    uint8_t           gtpScanAdValid;       /**< Flag indicating whether GTP Scan AD is valid */
    uint8_t           gtpWifiAdValid;       /**< Flag indicating whether GTP WIFI AD is valid */
    uint8_t           gtpCsRequired;        /**< Flag indicating whether GTP Crowdsourcing is required */

} qapi_Gtp_AD_Status_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for Time Assistance Status Info. 
 */
typedef struct {
    size_t        size;                  /**< Size. Set to the size of
                                              #qapi_Time_AD_Status_t. */

    uint32_t      tuncMs;                /**< Time uncertainty in milliseconds.
                                              Value of 0 denotes that no time is available with GNSS engine.
                                              Client must use this value to determine when to trigger the 
                                              NTP download. */
} qapi_Time_AD_Status_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for Assistance Status Info. 
 * Identifies the assistance data status of various positioning techs in location module.
 * The AD Type mask indicates the fields that are set.
 */
typedef struct {
    size_t size;                                    /**< Size. Set to the size of
                                                         #qapi_AD_Status_Info_t. */

    qapi_AD_Type_Mask_t         validAdTypeMask;    /**< Assistance types for which the status is set in this struct. */

    qapi_Gtp_AD_Status_t        gtpAdStatus;        /**< GTP AD status */    
    qapi_Xtra_AD_Status_t       xtraAdStatus;       /**< XTRA AD status */
    qapi_Xtra_Int_AD_Status_t   xtraIntAdStatus;    /**< XTRA Integrity AD status */
    qapi_Time_AD_Status_t       timeAdStatus;       /**< Time AD status */

} qapi_AD_Status_Info_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Mask comprising of bits from #qapi_Gnss_Sv_Mask_Bits_t
 */
typedef uint32_t qapi_Gnss_Sv_Type_Mask_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for GNSS constellation config.
 */
typedef struct {
    size_t size;                                   /**< Size. Set to the size of
                                                         #qapi_Gnss_Sv_Type_Config_t. */
    qapi_Gnss_Sv_Type_Mask_t enabledSvTypesMask;   /**< Constellations to be enabled. */

} qapi_Gnss_Sv_Type_Config_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Mask comprising of bits from #qapi_Xtra_Config_Type_t
 */
typedef uint32_t qapi_Xtra_Config_Type_Mask_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for specifying XTRA Config details.
 * One or more config values can be specified at the same time.
 * The config items being updated are identified by the configMask value.
 */
typedef struct {
    size_t size;                                            /**< Size. Set to the size of
                                                                 #qapi_Xtra_Config_t. */
    qapi_Xtra_Config_Type_Mask_t       configMask;         /**< Config items being updated. */

    qapi_Xtra_Config_File_Duration_t   xtraFileDuration;   /**< XTRA file validity duration in hours. */

    qapi_Gnss_Sv_Type_Mask_t           xtraConstellationMask;  /**< XTRA constellation mask */

    uint8_t                            xtraIntEnabled;     /**< Is XTRA Integrity enabled or disabled. */

} qapi_Xtra_Config_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Enum indicating the GNSS config type.
 */
typedef enum {
    QAPI_GNSS_CONFIG_TYPE_GNSS_SV_TYPE_CONFIG     = (1<<0),
    QAPI_GNSS_CONFIG_TYPE_XTRA_CONFIG             = (1<<1),
} qapi_Gnss_Config_Type_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Mask comprising of bits from #qapi_Gnss_Config_Type_t
 */
typedef uint32_t qapi_Gnss_Config_Type_Mask_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for GNSS config.
 */
typedef struct {
    size_t size;                                    /**< Size. Set to the size of
                                                         #qapi_Location_Gnss_Config_t. */
    qapi_Gnss_Config_Type_Mask_t    configMask;     /**< Config items being updated. */

    qapi_Gnss_Sv_Type_Config_t      svTypeConfig;   /**< SV Type configuration */
    qapi_Xtra_Config_t              xtraConfig;     /**< XTRA configuration */

} qapi_Gnss_Config_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Structure for Location Meta data information.
 */
typedef struct {
    size_t size;                                  /**< Size. Set to size of qapi_Meta_Data_t. */
    qapi_Location_Meta_Data_Flags_Mask_t flags;   /**< Bitwise OR of qapi_Location_Meta_Data_Flags_t. */
    uint32_t backoffTimeMillis;                   /**< Backoff value in milliseconds. */
    uint32_t nearestGeofenceMeters;               /**< Distance to nearest geofence in meters. */
    uint64_t energyConsumedSinceLastBoot;         /**<Energy consumed by the GNSS engine since
                                                      bootup in units of 0.1 milliwatt seconds. */
    uint64_t energyConsumedSinceFirstBoot;        /**<Energy consumed by the GNSS engine since
                                                      the first bootup in units of 0.1 milliwatt seconds. */
    qapi_Location_Meta_Data_Engine_Status_t engineStatus;  /**< Location Engine status. */
    qapi_AD_Status_Info_t adStatusInfo;           /**< Assistance data status. */
    qapi_Gnss_Config_t gnssConfig;                /**< GNSS Config */
} qapi_Location_Meta_Data_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 * Source of the NMEA string, Whether Final position, Intermediate position, or SV Information. */
typedef enum {
    QAPI_NMEA_SOURCE_UNKNOWN = 0,
    QAPI_NMEA_SOURCE_POSITION_FINAL,
    QAPI_NMEA_SOURCE_POSITION_INTERMEDIATE,
    QAPI_NMEA_SOURCE_SV_INFO,
    QAPI_NMEA_SOURCE_GNSS_ENGINE,
} qapi_Nmea_Source;

/** 
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Structure for NMEA data. */
typedef struct {
    size_t size;              /**< Size. Set to the size of #qapi_Gnss_Nmea_t. */
    uint64_t timestamp;       /**< Timestamp when NMEA is generated (milliseconds since Jan. 1, 1970). */
    const char* nmea;         /**< NMEA sentence string. */
    size_t length;            /**< Length of the NMEA sentence string. */
    qapi_Nmea_Source source;  /**< NMEA Source. */
} qapi_Gnss_Nmea_t;

/** 
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Added accuracyLevel parameter only. @tblendline
 * }
 *
 * Structure for location options. */
typedef struct {
    size_t size;          /**< Size. Set to the size of #qapi_Location_Options_t. */
    uint32_t minInterval; /**<  There are three different interpretations of this field, 
                                depending if minDistance is 0 or not: \n
                                1. Time-based tracking (minDistance = 0). minInterval is the minimum 
                                time interval in milliseconds that must elapse between final position reports. \n
                                2. Distance-based tracking (minDistance > 0). minInterval is the
                                maximum time period in milliseconds after the minimum distance
                                criteria has been met within which a location update must be provided. 
                                If set to 0, an ideal value will be assumed by the engine. \n
                                3. Batching. minInterval is the minimum time interval in milliseconds that 
                                must elapse between position reports.
                                */
    uint32_t minDistance; /**< Minimum distance in meters that must be traversed between position reports. 
                               Setting this interval to 0 will be a pure time-based tracking/batching.
                               */
    qapi_Location_Accuracy_Level_t accuracyLevel; /**< Accuracy level required for fix computation. */
} qapi_Location_Options_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.4.0  &  Introduced. @tblendline
 * }
 *
 * Structure for GTP positioning options.
 */
typedef struct {
    size_t   size;                  /**< Size. Set to the size of #qapi_Location_Gtp_Options_t. */
    uint8_t  requestTag[5];         /**< Tag to uniquely identify an MCS
                                         position request. */
} qapi_Location_Gtp_Options_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Structure for singleshot request options.
 */
typedef struct {
    size_t size;                                    /**< Size. Set to the size of #qapi_Singleshot_Options_t. */
    qapi_Location_Power_Level_t    powerLevel;      /**< Power level required for fix computation. */
    qapi_Location_Accuracy_Level_t accuracyLevel;   /**< Accuracy level required for fix computation. */
    uint32_t                       timeout;         /**< Timeout (in milliseconds) for singleshot fix. */
    qapi_Location_Gtp_Options_t    gtpOptions;      /**< GTP positioning specific options (optional). */
} qapi_Singleshot_Options_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for singleshot v3 request options.
 */
typedef struct {
    size_t size;                                        /**< Size. Set to the size of #qapi_Singleshot_v3_Options_t. */
    qapi_Location_Tech_t           locationTech;        /**< Location tech to be used for positioning. */
    uint32_t                       gnssAccuracyMeters;  /**< Desired accuracy level required from GNSS tech.
                                                             Accuracy level is only applicable when locationTech is 
                                                             specified as GNSS. */
    uint32_t                       timeout;             /**< Timeout (in milliseconds) for singleshot fix. */
} qapi_Singleshot_v3_Options_t;

/** Structure for Geofence options. */
typedef struct {
    size_t size;                                /**< Size. Set to the size of #qapi_Geofence_Option_t. */
    qapi_Geofence_Breach_Mask_t breachTypeMask; /**< Bitwise OR of #qapi_Geofence_Breach_Mask_Bits_t bits. */
    uint32_t responsiveness;                    /**< Specifies in milliseconds the user-defined rate of detection for a Geofence
                                                     breach. This may impact the time lag between the actual breach event and
                                                     when it is reported. The gap between the actual breach and the time it 
                                                     is reported depends on the user setting. The power implication is 
                                                     inversely proportional to the responsiveness value set by the user.
                                                     The higher the responsiveness value, the lower the power implications, 
                                                     and vice-versa. */
    uint32_t dwellTime;                         /**< Dwell time is the time in milliseconds a user spends in the Geofence before 
                                                     a dwell event is sent. */
} qapi_Geofence_Option_t;

/** Structure for Geofence information. */
typedef struct {
    size_t size;      /**< Size. Set to the size of #qapi_Geofence_Info_t. */
    double latitude;  /**< Latitude of the center of the Geofence in degrees. */
    double longitude; /**< Longitude of the center of the Geofence in degrees. */
    double radius;    /**< Radius of the Geofence in meters. */
} qapi_Geofence_Info_t;

/** Structure for Geofence breach notification. */
typedef struct {
    size_t size;                    /**< Size. Set to the size of #qapi_Geofence_Breach_Notification_t. */
    size_t count;                   /**< Number of IDs in the array. */
    uint32_t* ids;                  /**< Array of IDs that have been breached. */
    qapi_Location_t location;       /**< Location associated with a breach. */
    qapi_Geofence_Breach_t type;    /**< Type of breach. */
    uint64_t timestamp;             /**< Timestamp of the breach; milliseconds since Jan. 1, 1970. */
} qapi_Geofence_Breach_Notification_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Structure for location motion reporting options.
 */
typedef struct {
    size_t size;                                       /**< Size. Set to the size of
                                                            #qapi_Location_Motion_Options_t. */
    qapi_Location_Motion_Type_Mask_t motionTypeMask;   /**< Types of motion for which reporting
                                                            is desired. */
} qapi_Location_Motion_Options_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Structure for location motion information.
 */
typedef struct {
    size_t size;                                       /**< Size. Set to the size of
                                                            #qapi_Location_Motion_Info_t. */
    qapi_Location_Motion_Type_t motionType;            /**< Type of motion detected. */
    uint64_t timestamp;                                /**< Timestamp of the motion event;
                                                            milliseconds since Jan. 1, 1970. */
} qapi_Location_Motion_Info_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Type of request being sent from location module to the master QAPI client.
 */
typedef enum {
    QAPI_CLIENT_REQUEST_TYPE_AD                  = (1<<0),
    QAPI_CLIENT_REQUEST_TYPE_GEOFENCE_GNSS_REQ   = (1<<1),
    QAPI_CLIENT_REQUEST_TYPE_GTPWWAN_POS         = (1<<2),
    QAPI_CLIENT_REQUEST_TYPE_GTPWIFI_POS         = (1<<3),
    QAPI_CLIENT_REQUEST_TYPE_GTPGNSS_REQ         = (1<<4),         
} qapi_Client_Request_Type_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Response status for the request sent by location stack to master QAPI client.
 */
typedef enum {
    QAPI_CLIENT_RESPONSE_STATUS_SUCCESS              = (1<<0),
    QAPI_CLIENT_RESPONSE_STATUS_NETWORK_FAILURE      = (1<<1),
} qapi_Client_Response_Status_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for QAPI client request. 
 * Contains details about the request sent by location module to the master QAPI client.
 */
typedef struct {
    size_t size;                                /**< Size. Set to the size of
                                                     #qapi_Client_Request_Info_t. */

    qapi_Client_Request_Type_t requestType;     /**< Type of request being sent to the master QAPI client.
                                                     Rest of the fields in this struct must be 
                                                     interpreted as per this request type. */

    qapi_AD_Type_t adType;                      /**< Assistance type (single, not mask) being requested.
                                                     Request should be for one AD type at a time. */

    void* requestDataBuffer;                    /**< Data blob to be interpreted according to the
                                                     requestType/adType.
                                                     This buffer should be copied by client in the callback
                                                     context, and will be freed by location stack once the 
                                                     callback execution completes. */
    size_t requestDataBufferSize;               /**< Size of above request data buffer */

    qapi_Singleshot_v3_Options_t gnssRequestOptions;  /**< Options for GNSS request when the client request
                                                           type is GEOFENCE_GNSS_REQ */

} qapi_Client_Request_Info_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for Assistance Response Info. 
 * Contains the Assistance data response to be injected in to the location stack, by the QAPI client.
 */
typedef struct {
    size_t size;                                /**< Size. Set to the size of
                                                     #qapi_Client_Response_Info_t. */

    qapi_Client_Request_Type_t requestType;     /**< Type of request being responded to here by the 
                                                     master QAPI client.
                                                     Rest of the fields in this struct must be 
                                                     interpreted as per this request type. */

    qapi_Client_Response_Status_t status;       /**< Status of the request, success or failure. */

    qapi_AD_Type_t adType;                      /**< Assistance type (single, not mask) 
                                                     Response should be for one AD type at a time. */

    void* responseDataBuffer;                   /**< Data blob to be interpreted according to the 
                                                     requestType/adType.
                                                     This buffer would be copied by location stack in the callback
                                                     context, and can be freed by QAPI client once the API 
                                                     execution completes. */
    size_t responseDataBufferSize;              /**< Size of above response data buffer */

} qapi_Client_Response_Info_t;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Structure for options specified while triggering AD update. 
 * Provided by the master QAPI client while triggering an AD update.
 */
typedef struct {
    size_t size;                                /**< Size. Set to the size of
                                                     #qapi_AD_Update_Options_t. */
    qapi_AD_Type_t adType;                      /**< Assistance type (single, not mask) for which update is
                                                     being requested.
                                                     Request should be for one AD type at a time. */
} qapi_AD_Update_Options_t;

/* GNSS Location Callbacks */

/**
*  Provides the capabilities of the system.
         It is called once after qapi_Loc_Init() is called.

  @param[in] capabilitiesMask Bitwise OR of #qapi_Location_Capabilities_Mask_Bits_t.

  @return None.
*/
typedef void(*qapi_Capabilities_Callback)(
    qapi_Location_Capabilities_Mask_t capabilitiesMask
);

/**
*  Response callback, which is used by all tracking, batching 
         and Single Shot APIs.
         It is called for every tracking, batching and single shot API.

  @param[in] err  #qapi_Location_Error_t associated with the request.
                  If this is not QAPI_LOCATION_ERROR_SUCCESS then id is not valid.
  @param[in] id   ID to be associated with the request.

  @return None.
*/
typedef void(*qapi_Response_Callback)(
    qapi_Location_Error_t err,
    uint32_t id
);

/**
*  Collective response callback is used by Geofence APIs.
         It is called for every Geofence API call.

  @param[in] count  Number of locations in arrays.
  @param[in] err    Array of #qapi_Location_Error_t associated with the request.
  @param[in] ids    Array of IDs to be associated with the request.

  @return None.
*/
typedef void(*qapi_Collective_Response_Callback)(
    size_t count,
    qapi_Location_Error_t* err,
    uint32_t* ids
);

/**
*  Tracking callback used for the qapi_Loc_Start_Tracking() API.
         This is an optional function and can be NULL.
         It is called when delivering a location in a tracking session.

  @param[in] location  Structure containing information related to the 
                       tracked location.

  @return None.
*/
typedef void(*qapi_Tracking_Callback)(
    qapi_Location_t location
);

/**
*  Batching callback used for the qapi_Loc_Start_Batching() API.
         This is an optional function and can be NULL.
         It is called when delivering a location in a batching session.

  @param[in] count     Number of locations in an array.
  @param[in] location  Array of location structures containing information
                       related to the batched locations.

  @return None.
*/
typedef void(*qapi_Batching_Callback)(
    size_t count,
    qapi_Location_t* location
);

/**
*  Geofence breach callback used for the qapi_Loc_Add_Geofences() API.
         This is an optional function and can be NULL.
         It is called when any number of geofences have a state change.

  @param[in] geofenceBreachNotification     Breach notification information.

  @return None.
*/
typedef void(*qapi_Geofence_Breach_Callback)(
    qapi_Geofence_Breach_Notification_t geofenceBreachNotification
);

/**
*  Single shot callback used for the qapi_Loc_Get_Single_Shot() API.
        This is an optional function and can be NULL.
        It is called when delivering a location in a single shot session
        broadcasted to all clients, no matter if a session has started by client.

   @param[in] location  Structure containing information related to the
                        tracked location.
   @param[in] err       #qapi_Location_Error_t associated with the request.
                        This could be QAPI_LOCATION_ERROR_SUCCESS (location 
                        is valid) or QAPI_LOCATION_ERROR_TIMEOUT (a timeout
                        occurred, location is not valid).

   @return None.
*/
typedef void(*qapi_Single_Shot_Callback)(
    qapi_Location_t location,
    qapi_Location_Error_t err
);

/**
*  GNSS data callback used for the qapi_Loc_Start_Get_Gnss_Data() API.
         This is an optional function and can be NULL.
         It is called when delivering a GNSS Data structure. The GNSS data
         structure contains useful information (e.g., jammer indication). 
         This callback will be called every second.

  @param[in] gnssData  Structure containing information related to the 
                       requested GNSS Data.

  @return None.
*/
typedef void(*qapi_Gnss_Data_Callback)(
    qapi_Gnss_Data_t gnssData
);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Meta data callback used for below scenarios.
         - qapi_Loc_Get_Backoff_Value() API
         This is an optional function and can be NULL.
         It is called when delivering some location meta data.

  @param[in] metaData  Structure containing meta data related to ongoing
                       location sessions.

  @return None.
*/
typedef void(*qapi_Location_Meta_Data_Callback)(
    qapi_Location_Meta_Data_t metaData
);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * NMEA callback used for reporting NMEA statements.
         This is an optional function and can be NULL.
         It is called when delivering NMEA report from Modem.

  @param[in] nmeaData  Structure containing the NMEA sentence

  @return None.
*/
typedef void(*qapi_Gnss_Nmea_Callback)(
    qapi_Gnss_Nmea_t gnssNmea
);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Tracking callback used for the qapi_Loc_Start_Motion_Tracking() API.
         This is an optional function and can be NULL.
         It is called when delivering motion info in a motion tracking session.

  @param[in] motionInfo  Structure containing information about the
                         detected motion.

  @return None.
*/
typedef void(*qapi_Motion_Tracking_Callback)(
    qapi_Location_Motion_Info_t motionInfo
);

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Client Request callback will be used by location positioning techs to send
 * specific requests to the master QAPI client.
 * Details about the request type and relevant parameters will be in the 
 * clientRequestInfo parameter.

  @param[in] clientRequestInfo  Structure containing information about the
                                request to be handled by the master QAPI
                                client.

  @return None.
*/
typedef void(*qapi_Client_Request_Callback)(
    qapi_Client_Request_Info_t clientRequestInfo
);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Added last 3 callbacks. @tblendline 
 * } 
 *
 * Location callbacks requirements. */
typedef struct {
    size_t size;
    /**< Size. Set to the size of qapi_Location_Callbacks_t. */
    qapi_Capabilities_Callback         capabilitiesCb;
    /**< Capabilities callback is mandatory. */
    qapi_Response_Callback             responseCb;
    /**< Response callback is mandatory. */
    qapi_Collective_Response_Callback  collectiveResponseCb;
    /**< Collective response callback is mandatory. */
    qapi_Tracking_Callback             trackingCb;
    /**< Tracking callback is optional. */
    qapi_Batching_Callback             batchingCb;
    /**< Batching callback is optional. */
    qapi_Geofence_Breach_Callback      geofenceBreachCb;
    /**< Geofence breach callback is optional. */
    qapi_Single_Shot_Callback          singleShotCb;
    /**< Single shot callback is optional. */
    qapi_Gnss_Data_Callback            gnssDataCb;
    /**< GNSS data callback is optional. @newpagetable */
    qapi_Location_Meta_Data_Callback   metaDataCb;
    /**< Metadata callback is optional. Added in Location 1.2.0. */
    qapi_Gnss_Nmea_Callback            gnssNmeaCb;
    /**< GNSS NMEA callback is optional. Added in Location 1.2.0. */
    qapi_Motion_Tracking_Callback      motionTrackingCb;
    /**< Motion tracking callback is optional. Added in Location 1.2.0. */
    qapi_Client_Request_Callback       clientRequestCb;
    /**< Client request callback is optional. Added in Location 1.9.0. */

} qapi_Location_Callbacks_t;

/** Location client identifier. */
typedef uint32_t qapi_loc_client_id;

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline 
 * } 
 *
 * Location QAPI Client Options passed in #qapi_Loc_Init_V2. */
typedef struct {
    size_t size;
    /**< Size. Set to the size of qapi_Location_Client_Options_t. */

    const qapi_Location_Callbacks_t* pCallbacks;
    /**< Client callbacks registered with location module. */

    uint8_t isMasterClient;
    /**< Flag indicating if this is the master client. */

} qapi_Location_Client_Options_t;

#ifdef  QAPI_TXM_MODULE     // USER_MODE_DEFS
static __inline void qapi_location_cb_uspace_dispatcher(UINT cb_id,
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
    void(*pfn_app_cb1) (qapi_Location_Capabilities_Mask_t);
    void(*pfn_app_cb2) (qapi_Location_Error_t, uint32_t);
    void(*pfn_app_cb3) (size_t, qapi_Location_Error_t*, uint32_t*);
    void(*pfn_app_cb4) (qapi_Location_t);
    void(*pfn_app_cb5) (size_t, qapi_Location_t*);
    void(*pfn_app_cb6) (qapi_Geofence_Breach_Notification_t);
    void(*pfn_app_cb7) (qapi_Location_t, qapi_Location_Error_t);
    void(*pfn_app_cb8) (qapi_Gnss_Data_t);
    void(*pfn_app_cb9) (qapi_Location_Meta_Data_t);
    void(*pfn_app_cb10) (qapi_Gnss_Nmea_t);
    void(*pfn_app_cb11) (qapi_Location_Motion_Info_t);
    void(*pfn_app_cb12) (qapi_Client_Request_Info_t);

    qapi_Location_t location;
    qapi_Geofence_Breach_Notification_t notification;
    qapi_Gnss_Data_t gnssData;
    qapi_Location_Meta_Data_t metaData;
    qapi_Gnss_Nmea_t gnssNmea;
    qapi_Location_Motion_Info_t motionInfo;
    qapi_Client_Request_Info_t clientRequestInfo;

    switch (cb_id) {
    case CUSTOM_CB_QAPI_LOCATION_CAPABILITIES:
        pfn_app_cb1 = (void(*)(qapi_Location_Capabilities_Mask_t))app_cb;
        (pfn_app_cb1)((qapi_Location_Capabilities_Mask_t)cb_param1);
        break;

    case CUSTOM_CB_QAPI_LOCATION_RESPONSE:
        pfn_app_cb2 = (void(*)(qapi_Location_Error_t, uint32_t))app_cb;
        (pfn_app_cb2)((qapi_Location_Error_t)cb_param1,
            (uint32_t)cb_param2);
        break;

    case CUSTOM_CB_QAPI_LOCATION_COLLECTIVE_RESPONSE:
        pfn_app_cb3 = (void(*)(size_t, qapi_Location_Error_t*, uint32_t*))app_cb;
        (pfn_app_cb3)((size_t)cb_param1,
            (qapi_Location_Error_t*)cb_param2,
            (uint32_t*)cb_param3);
        break;

    case CUSTOM_CB_QAPI_LOCATION_TRACKING:
        pfn_app_cb4 = (void(*)(qapi_Location_t))app_cb;
        memcpy(&location, (qapi_Location_t*)cb_param1, sizeof(qapi_Location_t));
        (pfn_app_cb4)(location);
        break;

    case CUSTOM_CB_QAPI_LOCATION_BATCHING:
        pfn_app_cb5 = (void(*)(size_t, qapi_Location_t*))app_cb;
        (pfn_app_cb5)((size_t)cb_param1,
            (qapi_Location_t*)cb_param2);
        break;

    case CUSTOM_CB_QAPI_LOCATION_GEOFENCE_BREACH:
        pfn_app_cb6 = (void(*)(qapi_Geofence_Breach_Notification_t))app_cb;
        memcpy(&notification, (qapi_Geofence_Breach_Notification_t*)cb_param1,
                sizeof(qapi_Geofence_Breach_Notification_t));
        (pfn_app_cb6)(notification);
        break;

    case CUSTOM_CB_QAPI_LOCATION_SINGLE_SHOT:
        pfn_app_cb7 = (void(*)(qapi_Location_t, qapi_Location_Error_t))app_cb;
        memcpy(&location, (qapi_Location_t*)cb_param1, sizeof(qapi_Location_t));
        (pfn_app_cb7)(location, (qapi_Location_Error_t)cb_param2);
        break;
    
    case CUSTOM_CB_QAPI_LOCATION_GNSS_DATA:
        pfn_app_cb8 = (void(*)(qapi_Gnss_Data_t))app_cb;
        gnssData = *((qapi_Gnss_Data_t*)cb_param1);
        (pfn_app_cb8)(gnssData);
        break;

    case CUSTOM_CB_QAPI_LOCATION_META_DATA:
        pfn_app_cb9 = (void(*)(qapi_Location_Meta_Data_t))app_cb;
        metaData = *((qapi_Location_Meta_Data_t*)cb_param1);
        (pfn_app_cb9)(metaData);
        break;

    case CUSTOM_CB_QAPI_LOCATION_GNSS_NMEA:
        pfn_app_cb10 = (void(*)(qapi_Gnss_Nmea_t))app_cb;
        gnssNmea = *((qapi_Gnss_Nmea_t*)cb_param1);
        (pfn_app_cb10)(gnssNmea);
        break;

    case CUSTOM_CB_QAPI_LOCATION_MOTION_TRACKING:
        pfn_app_cb11 = (void(*)(qapi_Location_Motion_Info_t))app_cb;
        motionInfo = *((qapi_Location_Motion_Info_t*)cb_param1);
        (pfn_app_cb11)(motionInfo);
        break;

    case CUSTOM_CB_QAPI_LOCATION_CLIENT_REQUEST:
        pfn_app_cb12 = (void(*)(qapi_Client_Request_Info_t))app_cb;
        clientRequestInfo = *((qapi_Client_Request_Info_t*)cb_param1);
        (pfn_app_cb12)(clientRequestInfo);
        break;        

    default:
        break;
    }
}

#define qapi_Loc_Init(a, b)                            ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_INIT                           , (ULONG) a, (ULONG) b, (ULONG) qapi_location_cb_uspace_dispatcher, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Deinit(a)                             ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_DEINIT                         , (ULONG) a, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Start_Tracking(a, b, c)               ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_START_TRACKING                 , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Stop_Tracking(a, b)                   ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_STOP_TRACKING                  , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Update_Tracking_Options(a, b, c)      ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_UPDATE_TRACKING_OPTIONS        , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Start_Batching(a, b, c)               ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_START_BATCHING                 , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Stop_Batching(a, b)                   ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_STOP_BATCHING                  , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Update_Batching_Options(a, b, c)      ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_UPDATE_BATCHING_OPTIONS        , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Batched_Locations(a, b, c)        ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_BATCHED_LOCATIONS          , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Add_Geofences(a, b, c, d, e)          ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_ADD_GEOFENCES                  , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d, (ULONG) e, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Remove_Geofences(a, b, c)             ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_REMOVE_GEOFENCES               , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Modify_Geofences(a, b, c, d)          ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_MODIFY_GEOFENCES               , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Pause_Geofences(a, b, c)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_PAUSE_GEOFENCES                , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Resume_Geofences(a, b, c)             ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_RESUME_GEOFENCES               , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Single_Shot(a, b, c)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_SINGLE_SHOT                , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Cancel_Single_Shot(a, b)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_CANCEL_SINGLE_SHOT             , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Set_User_Buffer(a, b, c)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_SET_USER_BUFFER                , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Start_Get_Gnss_Data(a, b)             ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_START_GET_GNSS_DATA            , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Stop_Get_Gnss_Data(a, b)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_STOP_GET_GNSS_DATA             , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Backoff_Time(a, b)                ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_BACKOFF_TIME               , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Best_Available_Position(a, b)     ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_BEST_AVAILABLE_POSITION    , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Single_Shot_V2(a, b, c)           ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_SINGLE_SHOT_V2             , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Start_Motion_Tracking(a, b, c)        ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_START_MOTION_TRACKING          , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Stop_Motion_Tracking(a, b)            ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_STOP_MOTION_TRACKING           , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Start_Ntp_Download(a, b)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_START_NTP_DOWNLOAD             , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Gnss_Energy_Consumed(a, b)        ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_GNSS_ENERGY_CONSUMED       , (ULONG) a, (ULONG) b, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Init_V2(a, b)                         ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_INIT_V2                        , (ULONG) a, (ULONG) b, (ULONG) qapi_location_cb_uspace_dispatcher, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Single_Shot_V3(a, b, c)           ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_SINGLE_SHOT_V3             , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Set_Gnss_Config(a, b, c)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_SET_GNSS_CONFIG                , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_AD_Status(a, b, c)                ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_AD_STATUS                  , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Trigger_AD_Update(a, b, c)            ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_TRIGGER_AD_UPDATE              , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Client_Request_Cb_Response(a, b, c)   ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_CLIENT_REQUEST_CB_RESPONSE     , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Loc_Get_Gnss_Config(a, b, c)              ((UINT) (_txm_module_system_call12)(TXM_QAPI_LOCATION_GET_GNSS_CONFIG                , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))

#else // QAPI_TXM_MODULE

UINT qapi_location_handler(UINT id, UINT a1, UINT a2, UINT a3, UINT a4, UINT a5, UINT a6, UINT a7, UINT a8, UINT a9, UINT a10, UINT a11, UINT a12);

/**
*  Initializes a location session and registers the callbacks.

  @param[out] pClientId  Pointer to client ID type, where the unique identifier
                         for this location client is returned.
  @param[in] pCallbacks  Pointer to the structure with the callback
                         functions to be registered.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- One of the mandatory
            callback functions is missing. 
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- There is an internal error. 
  - #QAPI_LOCATION_ERROR_ALREADY_STARTED -- A location session has
            already been initialized.
*/
qapi_Location_Error_t qapi_Loc_Init(
        qapi_loc_client_id* pClientId,
        const qapi_Location_Callbacks_t* pCallbacks);

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 * Initializes a location session and registers the callbacks.

  @param[out] pClientId  Pointer to client ID type, where the unique identifier
                         for this location client is returned.
  @param[in]  pOptions   Pointer to the client options structure containing 
                         callbacks and other info about the client.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- One of the mandatory
            callback functions is missing. 
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- There is an internal error. 
  - #QAPI_LOCATION_ERROR_ALREADY_STARTED -- A location session has
            already been initialized.
*/
qapi_Location_Error_t qapi_Loc_Init_V2(
        qapi_loc_client_id* pClientId,
        const qapi_Location_Client_Options_t* pOptions);

/**
*  De-initializes a location session.

  @param[in] clientId  Client identifier for the location client to be
                       deinitialized.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Deinit(
        qapi_loc_client_id clientId);

/**
*  Sets the user buffer to be used for sending back callback data.

  @param[in] clientId   Client ID for which user buffer is to be set
  @param[in] pUserBuffer User memory buffer to hold information passed
                         in callbacks. Note that since buffer is shared by all
                         the callbacks this has to be consumed at the user side
                         before it can be used by another callback to avoid any
                         potential race condition.
  @param[in] bufferSize  Size of user memory buffer to hold information passed
                         in callbacks. This size should be large enough to
                         accomodate the largest data size passed in a callback.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- There is an internal error. 
*/
qapi_Location_Error_t qapi_Loc_Set_User_Buffer(
        qapi_loc_client_id clientId,
        uint8_t* pUserBuffer,
        size_t bufferSize);

/**
*  Starts a tracking session, which returns a 
         session ID that will be used by the other tracking APIs and in 
         the response callback to match the command with a response. Locations are 
         reported on the tracking callback passed in qapi_Loc_Init() periodically
         according to the location options.
           responseCb returns:
                - #QAPI_LOCATION_ERROR_SUCCESS if session was successfully started.
                - #QAPI_LOCATION_ERROR_ALREADY_STARTED if a qapi_Loc_Start_Tracking
        	session is already in progress.
        	- #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no trackingCb was passed in
        	qapi_Loc_Init().
        	- #QAPI_LOCATION_ERROR_INVALID_PARAMETER if pOptions parameter is
        	    invalid.

  @param[in] clientId     Client identifier for the location client.
  @param[in] pOptions     Pointer to a structure containing the options: 
                          - minInterval -- There are two different interpretations of this field, 
                                depending if minDistance is 0 or not:
                                1. Time-based tracking (minDistance = 0). minInterval is the minimum 
                                time interval in milliseconds that must elapse between final position reports.
                                2. Distance-based tracking (minDistance > 0). minInterval is the
                                maximum time period in milliseconds after the minimum distance
                                criteria has been met within which a location update must be provided. 
                                If set to 0, an ideal value will be assumed by the engine.
                          - minDistance -- Minimum distance in meters that must be traversed between position 
                                reports. Setting this interval to 0 will be a pure time-based tracking.
  @param[out] pSessionId   Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Start_Tracking(
        qapi_loc_client_id clientId,
        const qapi_Location_Options_t* pOptions,
        uint32_t* pSessionId);

/**
*  Stops a tracking session associated with the id 
         parameter
           responseCb returns:
                - #QAPI_LOCATION_ERROR_SUCCESS if successful.
        	- #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	    tracking session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the session to be stopped.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Stop_Tracking(
        qapi_loc_client_id clientId,
        uint32_t sessionId);

/**
*  Changes the location options of a 
         tracking session associated with the id parameter responseCb returns:
          - #QAPI_LOCATION_ERROR_SUCCESS if successful.
          - #QAPI_LOCATION_ERROR_INVALID_PARAMETER if pOptions parameter is invalid.
          - #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	tracking session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the session to be changed.
  @param[in] pOptions   Pointer to a structure containing the options:
                          - minInterval -- There are two different interpretations of this field, 
                                depending if minDistance is 0 or not:
                                1. Time-based tracking (minDistance = 0). minInterval is the minimum 
                                time interval in milliseconds that must elapse between final position reports.
                                2. Distance-based tracking (minDistance > 0). minInterval is the
                                maximum time period in milliseconds after the minimum distance
                                criteria has been met within which a location update must be provided. 
                                If set to 0, an ideal value will be assumed by the engine.
                          - minDistance -- Minimum distance in meters that must be traversed between position 
                                reports. Setting this interval to 0 will be a pure time-based tracking.@tablebulletend

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Update_Tracking_Options(
        qapi_loc_client_id clientId,
        uint32_t sessionId,
        const qapi_Location_Options_t* pOptions);

/**
*  Starts a batching session, which returns a 
         session ID that will be used by the other batching APIs and in 
         the response callback to match the command with a response.
         
         Locations are 
         reported on the batching callback passed in qapi_Loc_Init() periodically
         according to the location options. A batching session starts tracking on
         the low power processor and delivers them in batches by the 
         batching callback when the batch is full or when qapi_Loc_Get_Batched_Locations() 
         is called. This allows for the processor that calls this API to sleep 
         when the low power processor can batch locations in the background and 
         wake up the processor calling the API only when the batch is full, 
         thus saving power.
           responseCb returns:
                - #QAPI_LOCATION_ERROR_SUCCESS if session was successfully started.
                - #QAPI_LOCATION_ERROR_ALREADY_STARTED if a qapi_Loc_Start_Batching
        	session is already in progress.
        	- #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no batchingCb was passed in
        	qapi_Loc_Init().
        	- #QAPI_LOCATION_ERROR_INVALID_PARAMETER if pOptions parameter is
        	    invalid.
        	- #QAPI_LOCATION_ERROR_NOT_SUPPORTED if batching is not supported.

  @param[in] clientId    Client identifier for the location client.
  @param[in] pOptions    Pointer to a structure containing the options:
                          - minInterval -- minInterval is the minimum time interval in milliseconds that 
                                must elapse between position reports.
                          - minDistance -- Minimum distance in meters that must be traversed between 
                                position reports.@tablebulletend
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Start_Batching(
        qapi_loc_client_id clientId,
        const qapi_Location_Options_t* pOptions,
        uint32_t* pSessionId);

/**
*  Stops a batching session associated with the id
         parameter.
           responseCb returns:
                - #QAPI_LOCATION_ERROR_SUCCESS if successful.
        	- #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        					batching session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the session to be stopped.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Stop_Batching(
        qapi_loc_client_id clientId,
        uint32_t sessionId);

/**
*  Changes the location options of a 
         batching session associated with the id parameter.
           responseCb returns:
               - #QAPI_LOCATION_ERROR_SUCCESS if successful.
               - #QAPI_LOCATION_ERROR_INVALID_PARAMETER if pOptions parameter is
        	invalid.
               - #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	batching session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the session to be changed.
  @param[in] pOptions   Pointer to a structure containing the options:
                          - minInterval -- minInterval is the minimum time interval in milliseconds that 
                                must elapse between position reports.
                          - minDistance -- Minimum distance in meters that must be traversed between 
                                position reports.@tablebulletend

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Update_Batching_Options(
        qapi_loc_client_id clientId,
        uint32_t sessionId,
        const qapi_Location_Options_t* pOptions);

/**
*  Gets a number of locations that are 
         currently stored or batched on the low power processor, delivered by 
         the batching callback passed to qapi_Loc_Init(). Locations are then 
         deleted from the batch stored on the low power processor.
           responseCb returns:
        	- #QAPI_LOCATION_ERROR_SUCCESS if successful, will be followed by
        	batchingCallback call.
        	- #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no batchingCb was passed in
        	qapi_Loc_Init().
        	- #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	batching session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the session for which the number of locations is requested.
  @param[in] count      Number of requested locations. The client can set this to 
                        MAXINT to get all the batched locations. If set to 0 no location
                        will be present in the callback function.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Get_Batched_Locations(
        qapi_loc_client_id clientId,
        uint32_t sessionId,
        size_t count);

/**
*  Adds a specified number of Geofences and returns an 
         array of Geofence IDs that will be used by the other Geofence APIs, 
         as well as in the collective response callback to match the command with 
         a response. The Geofence breach callback delivers the status of each
         Geofence according to the Geofence options for each.
           collectiveResponseCb returns:
               - #QAPI_LOCATION_ERROR_SUCCESS if session was successful
               - #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no geofenceBreachCb
               - #QAPI_LOCATION_ERROR_INVALID_PARAMETER if any parameters are invalid
               - #QAPI_LOCATION_ERROR_NOT_SUPPORTED if geofence is not supported

  @param[in] clientId  Client identifier for the location client.
  @param[in] count     Number of Geofences to be added.
  @param[in] pOptions  Array of structures containing the options:
                       - breachTypeMask -- Bitwise OR of GeofenceBreachTypeMask bits
                       - responsiveness in milliseconds
                       - dwellTime in seconds @vertspace{-14}
  @param[in] pInfo     Array of structures containing the data:
                       - Latitude of the center of the Geofence in degrees 
                       - Longitude of the center of the Geofence in degrees
                       - Radius of the Geofence in meters @vertspace{-14}
  @param[out] pIdArray  Array of IDs of Geofences to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Add_Geofences(
        qapi_loc_client_id clientId,
        size_t count,
        const qapi_Geofence_Option_t* pOptions,
        const qapi_Geofence_Info_t* pInfo,
        uint32_t** pIdArray);

/**
*  Removes a specified number of Geofences.
           collectiveResponseCb returns:
              - #QAPI_LOCATION_ERROR_SUCCESS if session was successful. 
              - #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
               geofence session.

  @param[in] clientId  Client identifier for the location client.
  @param[in] count     Number of Geofences to be removed.
  @param[in] pIDs      Array of IDs of the Geofences to be removed.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Remove_Geofences(
        qapi_loc_client_id clientId,
        size_t count,
        const uint32_t* pIDs);

/**
*  Modifies a specified number of Geofences.
           collectiveResponseCb returns:
                - # QAPI_LOCATION_ERROR_SUCCESS if session was successful.
                - # QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	geofence session.
        	- #QAPI_LOCATION_ERROR_INVALID_PARAMETER if any parameters are invalid.

  @param[in] clientId  Client identifier for the location client.
  @param[in] count     Number of Geofences to be modified.
  @param[in] pIDs      Array of IDs of the Geofences to be modified.
  @param[in] options  Array of structures containing the options:
                       - breachTypeMask -- Bitwise OR of GeofenceBreachTypeMask bits
                       - responsiveness in milliseconds
                       - dwellTime in seconds @tablebulletend

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Modify_Geofences(
        qapi_loc_client_id clientId,
        size_t count,
        const uint32_t* pIDs,
        const qapi_Geofence_Option_t* options);

/**
*  Pauses a specified number of Geofences, which is 
         similar to qapi_Loc_Remove_Geofences() except that they can be resumed 
         at any time.
           collectiveResponseCb returns:
               - #QAPI_LOCATION_ERROR_SUCCESS if session was successful.
               - #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	   geofence session.

  @param[in] clientId  Client identifier for the location client.
  @param[in] count     Number of Geofences to be paused.
  @param[in] pIDs      Array of IDs of the Geofences to be paused.

  @return
  QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. \n
  QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Pause_Geofences(
        qapi_loc_client_id clientId,
        size_t count,
        const uint32_t* pIDs);

/**
*  Resumes a specified number of Geofences that are 
         paused.
           collectiveResponseCb returns:
              - # QAPI_LOCATION_ERROR_SUCCESS if session was successful 
              - # QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	   geofence session.

  @param[in] clientId  Client identifier for the location client.
  @param[in] count     Number of Geofences to be resumed.
  @param[in] pIDs      Array of IDs of the Geofences to be resumed.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Resume_Geofences(
        qapi_loc_client_id clientId,
        size_t count,
        const uint32_t* pIDs);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Deprecated. @tblendline
 * }
 * Attempts a single location fix. It returns a session ID that 
         will be used by qapi_Loc_Cancel_Single_Shot API and in 
         the response callback to match the command with a response.
           responseCb returns:
                - #QAPI_LOCATION_ERROR_SUCCESS if session was successfully started.
                - #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no singleShotCb passed in
        	qapi_Loc_Init().
        	- #QAPI_LOCATION_ERROR_INVALID_PARAMETER if anyparameter is
         invalid. If responseCb reports LOCATION_ERROR_SUCESS, then the following is what
         can happen to end the single shot session: \n
         1) A location will be reported on the singleShotCb. \n
         2) QAPI_LOCATION_ERROR_TIMEOUT will be reported on the singleShotCb. \n
         3) The single shot session is canceled using the qapi_Loc_Cancel_Single_Shot API
         In either of these cases, the session is considered complete and the session id will
         no longer be valid.

  @param[in]  clientId    Client identifier for the location client.
  @param[in]  powerLevel  Indicates what available technologies to use to compute the location.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.

  @deprecated Use API qapi_Loc_Get_Single_Shot_V2() instead.
*/
qapi_Location_Error_t qapi_Loc_Get_Single_Shot(
    qapi_loc_client_id clientId,
    qapi_Location_Power_Level_t powerLevel,
    uint32_t* pSessionId);

/**
*  Cancels a single shot session.
           responseCb returns:
        	- #QAPI_LOCATION_ERROR_SUCCESS if successful.
        	- #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	single shot session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the single shot session to be cancelled.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Cancel_Single_Shot(
    qapi_loc_client_id clientId,
    uint32_t sessionId);

/**
*  Starts a Get GNSS data session, which returns a 
         session ID that will be used by the qapi_Loc_Stop_Get_Gnss_Data() API and in 
         the response callback to match the command with a response. GNSS data is 
         reported on the GNSS data callback passed in qapi_Loc_Init() periodically
         (every second until qapi_Loc_Stop_Get_Gnss_Data() is called). 
		 
           responseCb returns: \n
                - #QAPI_LOCATION_ERROR_SUCCESS if session was successfully started. 
        	- #QAPI_LOCATION_ERROR_ALREADY_STARTED if a
        	qapi_Loc_Start_Get_Gnss_Data() session is already in progress.
        	- #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no gnssDataCb was passed in
        	qapi_Loc_Init().

  @param[in] clientId     Client identifier for the location client.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Start_Get_Gnss_Data(
        qapi_loc_client_id clientId,
        uint32_t* pSessionId);

/**
*  Stops a Get GNSS data session associated with the ID 
         parameter.
		 
           responseCb returns: \n
               - #QAPI_LOCATION_ERROR_SUCCESS if successful. 
               - #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a Get
        	   GNSS data session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the session to be stopped.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No location session has
            been initialized.
*/
qapi_Location_Error_t qapi_Loc_Stop_Get_Gnss_Data(
        qapi_loc_client_id clientId,
        uint32_t sessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Requests the GNSS engine to provide a backoff time value.
         This backoff value indicates when the client can expect
         the next location callback based on the ongoing sessions.
         The backoff value is provided via qapi_Location_Meta_Data_Callback
         if registered by the client.
         It returns a session ID that will be sent in response callback
         to match the command with a response.

  @param[in]  clientId    Client identifier for the location client.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client id provided. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Metadata callback is not set. 
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                         the request.
*/
qapi_Location_Error_t qapi_Loc_Get_Backoff_Time(
        qapi_loc_client_id clientId,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Fetches the best available position with the GNSS Engine.
         It returns a session ID that will be sent in response callback
         to match the command with a response.
           responseCb returns:
                - #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no singleShotCb passed in
        	qapi_Loc_Init().
        	- #QAPI_LOCATION_ERROR_INVALID_PARAMETER if any parameter is invalid.
        	- #QAPI_LOCATION_ERROR_SUCCESS if request was successfully placed to GNSS
         Engine. If responseCb reports LOCATION_ERROR_SUCESS, then the following is what
         can happen: \n
            A location will be reported on the singleShotCb.
            The location object would have QAPI_LOCATION_IS_BEST_AVAIL_POS_BIT set in the
            flags field to indicate that this position is the best available position and
            not a response to the singleshot request. This location can have a large
            accuracy value, which must be checked by the client if it suffices its
            purpose.

  @param[in]  clientId    Client identifier for the location client.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided.
  - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Singleshot callback is not set. 
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                         the request.
*/
qapi_Location_Error_t qapi_Loc_Get_Best_Available_Position(
        qapi_loc_client_id clientId,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Attempts a single location fix. It returns a session ID that
         will be used by qapi_Loc_Cancel_Single_Shot API and in
         the response callback to match the command with a response.
           responseCb returns:
                - #QAPI_LOCATION_ERROR_SUCCESS if session was successfully started.
                - #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no singleShotCb passed in
        	qapi_Loc_Init().
        	- #QAPI_LOCATION_ERROR_INVALID_PARAMETER if anyparameter is
         invalid. If responseCb reports LOCATION_ERROR_SUCESS, then there are below 3
         possible ends to this singleshot request: \n
         1) A location will be reported on the singleShotCb. \n
         2) QAPI_LOCATION_ERROR_TIMEOUT will be reported on the singleShotCb. \n
         3) The single shot session is canceled using the qapi_Loc_Cancel_Single_Shot API
         In all cases, the session is considered complete and the session id will
         no longer be valid.

  @param[in]  clientId    Client identifier for the location client.
  @param[in]  pOptions    Pointer to a structure containing the options:
                          - powerLevel  Indicates what level of power consumption is acceptable
                                        while computing the fix.
                          - accuracyLevel Indicates what level of accuracy is desired.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Singleshot callback is not set. 
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                         the request.
*/
qapi_Location_Error_t qapi_Loc_Get_Single_Shot_V2(
    qapi_loc_client_id clientId,
    const qapi_Singleshot_Options_t* pOptions,
    uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Attempts a single location fix for the specified location tech. 
         It returns a session ID that can be used by qapi_Loc_Cancel_Single_Shot API 
         and in the response callback to match the command with a response.
           responseCb returns:
                - #QAPI_LOCATION_ERROR_SUCCESS if session was successfully started.
                - #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no singleShotCb passed in
                    qapi_Loc_Init().
                - #QAPI_LOCATION_ERROR_INVALID_PARAMETER if any parameter is invalid. 
                - #QAPI_LOCATION_ERROR_AD_UPDATE_REQUIRED if the session is started 
                    without triggering required AD update first.

         If responseCb reports LOCATION_ERROR_SUCCESS, then there are below 
                    possible ends to this singleshot request: \n
         1) For GNSS tech, 
                - A location will be reported on the singleShotCb. \n
                - QAPI_LOCATION_ERROR_TIMEOUT will be reported on the singleShotCb. \n
                - The single shot session is canceled using the qapi_Loc_Cancel_Single_Shot API
            In all cases, the session is considered complete and the session id will
            no longer be valid.

         2) For GTP WWAN tech,  .. TODO
                - GTP AD will be successfuly sent to the server, no location is derived on device.
                - GTP AD fails to upload to server, and server is not able to derive a fix.

         3) For WIFI Positioning tech,  .. TODO
                - WIFI AD will be successfuly sent to the server, no location is derived on device.
                - WIFI AD fails to upload to server, and server is not able to derive a fix.

  @param[in]  clientId    Client identifier for the location client.
  @param[in]  pOptions    Pointer to a structure containing the singleshot v3 options.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Singleshot callback is not set. 
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                            the request.
*/
qapi_Location_Error_t qapi_Loc_Get_Single_Shot_V3(
    qapi_loc_client_id clientId,
    const qapi_Singleshot_v3_Options_t* pOptions,
    uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Starts a motion tracking session, which returns a
         session ID that will be used by the other tracking APIs and in
         the response callback to match the command with a response.
         Motion Info is reported on the motion tracking callback passed
         in qapi_Loc_Init() when a motion is detected matching the motion
         options specified in this API.
           responseCb returns:
               - #QAPI_LOCATION_ERROR_SUCCESS if session was successfully started.
               - #QAPI_LOCATION_ERROR_CALLBACK_MISSING if no motionTrackingCb was passed
        					     in qapi_Loc_Init().
               - #QAPI_LOCATION_ERROR_INVALID_PARAMETER if pOptions parameter is invalid.

  @param[in]  clientId     Client identifier for the location client.
  @param[in]  pOptions     Pointer to a structure containing the motion detection options.
  @param[out] pSessionId   Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
 - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Motion tracking callback is not set. 
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                         the request.
*/
qapi_Location_Error_t qapi_Loc_Start_Motion_Tracking(
        qapi_loc_client_id clientId,
        const qapi_Location_Motion_Options_t* pOptions,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.2.0  &  Introduced. @tblendline
 * }
 *
 * Stops a motion tracking session associated with the id
         parameter
           responseCb returns:
               - #QAPI_LOCATION_ERROR_SUCCESS if successful.
               - #QAPI_LOCATION_ERROR_ID_UNKNOWN if clientId is not associated with a
        	   tracking session.

  @param[in] clientId   Client identifier for the location client.
  @param[in] sessionId  ID of the session to be stopped.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID or session ID provided.
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                         the request.
*/
qapi_Location_Error_t qapi_Loc_Stop_Motion_Tracking(
        qapi_loc_client_id clientId,
        uint32_t sessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.6.0  &  Introduced. @tblendline
 * }
 *
 * Start a NTP download session

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful.  
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                         the request.
*/
qapi_Location_Error_t qapi_Loc_Start_Ntp_Download(
        qapi_loc_client_id clientId,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.7.0  &  Introduced. @tblendline
 * }
 *
 * Requests energy consumed parameters.

  @param[in]  clientId    Client identifier for the location client.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
  - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Metadata callback is not set. 
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                         the request.
*/
qapi_Location_Error_t qapi_Loc_Get_Gnss_Energy_Consumed(
        qapi_loc_client_id clientId,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Request for updating GNSS Configuration.
         The exact configuration to be updated is specified with the 
         config mask field in the #qapi_Gnss_Config_t struct.
         The corresponding values as per the config to be set must 
         be set in the passed in struct.

  @param[in]  clientId     Client identifier for the location client.
  @param[in]  pGnssConfig  Pointer to GNSS Configuration struct to be updated.
  @param[out] pSessionId   Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                           the request.
*/
qapi_Location_Error_t qapi_Loc_Set_Gnss_Config(
        qapi_loc_client_id clientId,
        const qapi_Gnss_Config_t* pGnssConfig,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.10.0  &  Introduced. @tblendline
 * }
 *
 * Requests current GNSS Config

  @param[in]  clientId    Client identifier for the location client.
  @param[in]  configMask  Mask identifying config that is being requested.
  @param[out] pSessionId  Pointer to the session ID to be returned.

  @return
  - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful.
  - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided.
  - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set.
  - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Metadata callback is not set.
  - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                            the request.
*/
qapi_Location_Error_t qapi_Loc_Get_Gnss_Config(
        qapi_loc_client_id clientId,
        qapi_Gnss_Config_Type_Mask_t configMask,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Request for current Assistance Data Status.
         Assistance data is reported on the meta data callback passed
         in qapi_Loc_Init() in the #qapi_AD_Status_Info_t struct contained 
         in the #qapi_Location_Meta_Data_t struct.

  @param[in]  clientId     Client identifier for the location client.
  @param[in]  adType       Assistance data type for which status is being requested.
  @param[out] pSessionId   Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
 - #QAPI_LOCATION_ERROR_CALLBACK_MISSING -- Meta data callback is not set. 
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                           the request.
*/
qapi_Location_Error_t qapi_Loc_Get_AD_Status(
        qapi_loc_client_id clientId,
        qapi_AD_Type_t adType,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Trigger Assistance data update for a specific AD Type.
         The type of AD requested and specific parameters related to the 
         request should be specified in the options argument.

  @param[in]  clientId     Client identifier for the location client.
  @param[in]  pOptions     Pointer to options specifying the type of AD update
                           requested, and any other relevant parameter.
  @param[out] pSessionId   Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                           the request.
*/
qapi_Location_Error_t qapi_Loc_Trigger_AD_Update(
        qapi_loc_client_id clientId,
        const qapi_AD_Update_Options_t* pOptions,
        uint32_t* pSessionId);

/**
 * @versiontable{2.3,2.7,
 * Location 1.9.0  &  Introduced. @tblendline
 * }
 *
 * Response sent by Master QAPI client for the #qapi_Client_Request_Callback
         The response would depend on the type of request sent
         by the location stack to the master QAPI client.

  @param[in]  clientId     Client identifier for the location client.
  @param[in]  pResponse    Pointer to the response info structure, 
                           #qapi_Client_Response_Info_t
  @param[out] pSessionId   Pointer to the session ID to be returned.

  @return
 - #QAPI_LOCATION_ERROR_SUCCESS -- Operation was successful. 
 - #QAPI_LOCATION_ERROR_ID_UNKNOWN -- Invalid client ID provided. 
 - #QAPI_LOCATION_ERROR_NOT_INITIALIZED -- No user space buffer is set. 
 - #QAPI_LOCATION_ERROR_GENERAL_FAILURE -- Internal failure while processing
                                           the request.
*/
qapi_Location_Error_t qapi_Loc_Client_Request_Cb_Response(
        qapi_loc_client_id clientId,
        const qapi_Client_Response_Info_t* pResponse,
        uint32_t* pSessionId);

#endif // QAPI_TXM_MODULE

/** @} */ /* end_addtogroup qapi_location */

#ifdef __cplusplus
}
#endif

#endif /* _QAPI_LOCATION_H_ */
