/**
 * @file version_dataservice_def.h
 *
 * @brief QAPI versions defintions for category dataservice.
 *
 * @details This file defines the QAPI versions for category dataservice.
 */

/*===========================================================================

  Copyright (c) 2019-2021 Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential - Qualcomm Technologies, Inc. - May Contain Trade Secrets

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/10/21   pvyas       Added support for QAPI for for Read/Write/Delete SMS from UICC.
                       change dataservices version to 1.25.0
30/09/21   hakumar       Added QAPIs Support To enable disable NZ BSR. change dataservices version to 1.24.0
15/09/21   shanu       Added QAPIs Support To Set Signal Strength And Exposing SINR, SRXLEV and CE values. change dataservices version to 1.24.0 

27/07/21   mariapri	   Added QAPI_NET_MQTT_GET_EXTENDED_CONFIG_OPTION option in qapi_Net_MQTT_Get_Extended_Config_Option()
					   change dataservices version to 1.23.0
15/06/21   rtomar      Added QAPI_NET_SSL_EXTENDED_CONFIG_ENABLE_NON_SECURE_TLS12_OPTIONS option in qapi_Net_SSL_Set_Extended_Config_Option() 
                       change dataservices version to 1.22.0
17/05/21   pvyas      Fixed enum typo in qapi_Device_Info_ID_t. Updated QAPI_DEIVCE_INFO_TIME_ZONE_E to QAPI_DEVICE_INFO_TIME_ZONE_E and 
15/06/21   pvyas      Added QAPI_DEVICE_INFO_FIRMWARE_VERSION_E in qapi_Device_Info_Get_v2(). change dataservices version to 1.21.0
09/06/21   xianma     Define new QAPI error code QAPI_DEVICE_INFO_ERR_BAND_NOT_SUPPORTED in device_info module
change dataservices version to 1.20.0
04/06/21   xianma     Added QAPI_NET_SSL_EXTENDED_CONFIG_RFC_COMPATIBLE_SECURE_RENEGOTIATION_FEATURE and
QAPI_NET_SSL_EXTENDED_CONFIG_ENABLE_RENEG_EXTENSION option in qapi_Net_SSL_Set_Extended_Config_Option().
change dataservices version to 1.19.0
17/05/21   pvyas      Fixed enum typo in qapi_Device_Info_ID_t. Updated QAPI_DEIVCE_INFO_TIME_ZONE_E to QAPI_DEVICE_INFO_TIME_ZONE_E and
QAPI_DEIVCE_INFO_ICCID_E to QAPI_DEVICE_INFO_ICCID_E. change dataservices version to 1.18.0
15/04/21   xianma     Added QAPI_NET_SSL_EXTENDED_CONFIG_RFC_COMPATIBLE_EXT_MASTER_SECRET_FEATURE option in qapi_Net_SSL_Set_Extended_Config_Option().
change dataservices version to 1.17.0
23/03/21   xianma     Added QAPI_DEVICE_INFO_LTE_M1_BAND_PREF_EXT_E and QAPI_DEVICE_INFO_LTE_NB1_BAND_PREF_EXT_E in qapi_Device_Info_Get_v2() and qapi_Device_Info_set()
change dataservices version to 1.16.0
10/03/21   pvyas      Added QAPI_COAP_EXTENDED_CONFIG_DTLS_HANDLE_ALERT_WITH_EPOCH_0 option in qapi_Coap_Set_Extended_Config_Option().
09/03/21   xianma     Added QAPI_NET_SSL_EXTENDED_CONFIG_DTLS_HANDLE_ALERT_WITH_EPOCH_0 option in qapi_Net_SSL_Set_Extended_Config_Option().
change dataservices version to 1.15.0
10/02/21   hakumar    Added support for qapi_Sensor_Mgr_Set_Callback_v2(), qapi_Sensor_Mgr_Query_Sensor_Ind_v2() which allows DAM apps to recieve/query indication
of sensor HW fails and sensor going into bad state. Change dataservices version to 1.14.0
06/01/21   baohan     Added support IMS_IOT indication register request in device info. change dataservices version to 1.13.0
04/11/20   shanu      Added support to get qRxlevmin parameter for metering applications. change dataservices version to 1.12.0
12/10/20   pvyas      Added support to get next WWAN sleep time indication and get and set WWAN sleep threshold value. change dataservices version to 1.11.0
18/09/20   more       change dataservices version to 1.10.0
04/09/20   hakumar    Add MQTT config extended options QAPI_NET_MQTT_EXTENDED_CONFIG_MAX_CHUNK,
QAPI_NET_MQTT_EXTENDED_CONFIG_MAX_CHUNK_DELAY,  QAPI_NET_MQTT_EXTENDED_CONFIG_MAX_CHUNK_RETRIES,
change dataservices version to 1.7.0
13/07/20   rtomar     Added support for qapi_select_v2(),qapi_fd_set_v2(),qapi_fd_zero_v2(),qapi_fd_clr_v2(),qapi_fd_isset_v2()
30/06/20   pvyas      Add support for NAS FSK based alarm reporting on Jamming detection.
12/05/20   tjuneja	  Added support in qapi_Device_Info_Request() to fetch scan configurations using qapi_Device_Info_Scan_Config_Rsp_t.
28/04/20   pvyas      Add qapi_Device_Info_Clear_Callback() and qapi_Device_Info_Clear_Callback_v2(), change dataservices version to 1.6.0
07/04/20   more       Added support to set Next activity time through New QAPI qapi_DSS_Set_Transfer_Status_v2()
31/03/20   shanu      Add support for CoAP block-wise transfer
24/02/20   pvyas      Add qapi_Coap_Set_Header_Ext() and qapi_Coap_Get_Header_Ext(), change dataservices version to 1.6.0
30/01/20   shanu      Deprecated the support to get profile index using QAPI device info because profile Index is not supported in MDM9205.
23/01/19   shanu      added qapi_Coap_Parse_Message
09/01/20   pvyas      Add support for new LwM2M registration retry resources
15/12/19   nchagant   added support for setting application priority for GNSS or WWAN and retrieving the current loaded tech.
05/12/19   shanu      add support for retrieving Cell information on cellular parameters for application using qapi_Device_Info_Get_v2()  and qapi_Device_Info_Set()
05/12/19   xianma     Add support for RPK in certificate relate QAPIs and also qapi_Net_SSL_Set_Extended_Config_Option() change dataservices version to 1.6.0
05/12/19   huawenc    Add support for TLS1.3 options in qapi_Net_SSL_Set_Extended_Config_Option(), also deprecated qapi_Net_SSL_Accept() change dataservices version to 1.6.0
05/12/19   baohan     support DELETE method by httpc stack, change dataservices version to 1.6.0 from 1.5.1
20/11/19   shanu      added qapi_DSS_Get_Apn_Rate_Control and qapi_DSS_Get_Splmn_Rate_Control
09/10/19   more       added qapi_Coap_Reconnect in dam module , updated behaviour of qapi_Coap_Get_Header
02/09/19   xianma     Add qapi_DSS_Get_Link_Mtu_per_family(), change dataservices version to 1.3.0
20/06/19   baohan     created this file.
===========================================================================*/
#ifndef __VERSION_DATASERVICE_DEF_H__
#define __VERSION_DATASERVICE_DEF_H__

/*
*
* 3-number version format. For example 1.2.0
*
* 1) The major number 1, indicates backward compatibility.
*    It only gets incremented if the backward compatibility is broken intentionally.
* 2) The minor number 2, indicates a change in API.
*    It gets incremented when there is a change in API, struct, enums
* 3) The patch number 0 indicates non source code chagnes.
*    It gets incremented for non source code changes such as comments
*
* Following is the list of changes that are NOT backward compatible:
*
* 1) Updating/removing the existing enum values or identifiers.
* 2) Adding fields to existing structs.
* 3) Removing fields from existing structs.
* 4) Updating the signature of existing functions
*
*/

/** data service versions */
#define DATA_SERVICE_MAJOR_NUM           1
#define DATA_SERVICE_MINOR_NUM           25
#define DATA_SERVICE_PATCH_NUM           0
#define DATA_SERVICE_VER_STR             DATA_SERVICE_MAJOR_NUM.DATA_SERVICE_MINOR_NUM.DATA_SERVICE_PATCH_NUM

#endif
