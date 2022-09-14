#include "sensor_mgr_utils.h"

int get_pre_alert_threshold_limit(int alert_threshold_high, int alert_threshold_low, int percent)
{
   int delta = (alert_threshold_high-alert_threshold_low)/percent;

   return delta;
}

int get_pre_alert_threshold_high(int alert_threshold_high, int alert_threshold_low)
{
    int threshold_limit = get_pre_alert_threshold_limit(alert_threshold_high, alert_threshold_low, 10);

    int variation = (1 > threshold_limit)? 1:threshold_limit;
    int pre_alert_threshold_high = alert_threshold_high - variation;

    return pre_alert_threshold_high;
}

int get_pre_alert_threshold_low(int alert_threshold_high, int alert_threshold_low)
{
    int threshold_limit = get_pre_alert_threshold_limit(alert_threshold_high, alert_threshold_low, 10);

    int variation = ( 1 > threshold_limit)? 1:threshold_limit;
    int pre_alert_threshold_low = alert_threshold_low + variation;

    return pre_alert_threshold_low;
} 

int get_humidity_pre_alert_threshold_high(int alert_threshold_high, int alert_threshold_low)
{
    int threshold_limit = get_pre_alert_threshold_limit(alert_threshold_high, alert_threshold_low, 10);

    int variation = (3 > threshold_limit)? 3:threshold_limit;
    int pre_alert_threshold_high = alert_threshold_high - variation;

    return pre_alert_threshold_high;
}

int get_humidity_pre_alert_threshold_low(int alert_threshold_high, int alert_threshold_low)
{
    int threshold_limit = get_pre_alert_threshold_limit(alert_threshold_high, alert_threshold_low, 10);

    int variation = ( 3 > threshold_limit)? 3:threshold_limit;
    int pre_alert_threshold_low = alert_threshold_low + variation;

    return pre_alert_threshold_low;
}