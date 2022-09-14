#include "qapi_fs.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "app_utils_misc.h"
#include "qapi_diag.h"
#include "qapi_location.h"

#include "aware_log.h"

char* LOG_PREPEND = "";
char* LOG_PREPEND_ACTIVE = "==== ";
char* LOG_QAPI_PREPEND_ACTIVE = ">>>> ";
char* LOG_PREPEND_INACTIVE = "";

/***************************************************************************
    Logging Utilities
****************************************************************************/

/*-------------------------------------------------------------------------*/
/**
  @brief	Log the location information 
  @param    location Structure variable where all location information are available
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void app_utils_log_location(qapi_Location_t location)
{
    uint64_t ts_sec = location.timestamp / 1000;

    LOG_INFO("Time(HH:MM:SS): %02d:%02d:%02d",
        (int)((ts_sec / 3600) % 24), (int)((ts_sec / 60) % 60), (int)(ts_sec % 60));

    if (location.flags & QAPI_LOCATION_HAS_LAT_LONG_BIT) {
        LOG_INFO("LATITUDE: %d.%d",
            (int)location.latitude, (abs((int)(location.latitude * 100000))) % 100000);
        LOG_INFO("LONGITUDE: %d.%d",
            (int)location.longitude, (abs((int)(location.longitude * 100000))) % 100000);
    }
    else {
        LOG_INFO("Latitude and longitude are not provided");
    }

    if (location.flags & QAPI_LOCATION_HAS_ALTITUDE_BIT) {
        LOG_INFO("ALTITUDE: %d.%d",
            (int)location.altitude, (abs((int)(location.altitude * 100))) % 100);
        LOG_INFO("ALTITUDE MSL: %d.%d",
            (int)location.altitudeMeanSeaLevel, (abs((int)(location.altitudeMeanSeaLevel * 100))) % 100);
    }
    else {
        LOG_INFO("Altitude is not provided");
    }

    if (location.flags & QAPI_LOCATION_HAS_SPEED_BIT) {
        LOG_INFO("SPEED: %d.%d",
            (int)location.speed, (abs((int)(location.speed * 100))) % 100);
    }
    else {
        LOG_INFO("Speed is not provided");
    }

    if (location.flags & QAPI_LOCATION_HAS_BEARING_BIT) {
        LOG_INFO("BEARING: %d.%d",
            (int)location.bearing, (abs((int)(location.bearing * 100))) % 100);
    }
    else {
        LOG_INFO("Bearing is not provided");
    }

    if (location.flags & QAPI_LOCATION_HAS_ACCURACY_BIT) {
        LOG_INFO("ACCURACY: %d.%d",
            (int)location.accuracy, (abs((int)(location.accuracy * 100))) % 100);
    }
    else {
        LOG_INFO("Accuracy is not provided");
    }

    if (location.flags & QAPI_LOCATION_HAS_VERTICAL_ACCURACY_BIT) {
        LOG_INFO("VERTICAL ACCURACY: %d.%d",
            (int)location.verticalAccuracy, (abs((int)(location.verticalAccuracy * 100))) % 100);
    }
    else {
        LOG_INFO("Vertical accuracy is not provided");
    }

    if (location.flags & QAPI_LOCATION_HAS_SPEED_ACCURACY_BIT) {
        LOG_INFO("SPEED ACCURACY: %d.%d",
            (int)location.speedAccuracy, (abs((int)(location.speedAccuracy * 100))) % 100);
    }
    else {
        LOG_INFO("Speed accuracy is not provided");
    }

    if (location.flags & QAPI_LOCATION_HAS_BEARING_ACCURACY_BIT) {
        LOG_INFO("BEARING ACCURACY: %d.%d",
            (int)location.bearingAccuracy, (abs((int)(location.bearingAccuracy * 100))) % 100);
    }
    else {
        LOG_INFO("Bearing accuracy is not provided");
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Log the GTP Data information 
  @param    gtpdata Structure variable where all gtp data information are available
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void app_utils_log_gtpdata(gtp_data_t gtpdata)
{
    int i;

    LOG_INFO("GTP Data Size: %d", gtpdata.dataBufferSize);
    for(i=0;i<gtpdata.dataBufferSize;i++)
    {
        LOG_INFO("GTP Data[%d]: 0x%02X", i, ((uint8_t *)(gtpdata.dataBufferPtr))[i]);
    }
    
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Log the NMEA data
  @param	nmea Structure variable for NMEA data.
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void app_utils_log_nmea(qapi_Gnss_Nmea_t nmea) {
    LOG_INFO("NMEA: [%s] Length: [%d]", nmea.nmea, nmea.length);
}
