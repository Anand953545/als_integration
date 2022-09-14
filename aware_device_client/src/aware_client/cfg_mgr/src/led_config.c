#include "led_config.h"
#include "stringl.h"

#include "qapi_fs.h"
#include "aware_log.h"
#include "aware_utils.h"
#include "cfg_mgr.h"
#include "config_parser.h"
#include "app_utils_misc.h"
#include "cJSON.h"
#include "app_context.h"
#include "aware_app.h"

int led_config_common_load(led_config_t*);



/*-------------------------------------------------------------------------*/
/**
  @brief    Create led config file
  @param    led_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int led_config_create(led_config_t* led_cfg)
{
	int status = 0;

	if (led_cfg) {
		status = led_config_common_load(led_cfg);
	}

	return status;
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Load led common config data to structure variable
  @param    led_cfg		Pointer to structure to get relevant data
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int led_config_common_load(led_config_t* led_cfg)
{
	qapi_Status_t status = QAPI_OK;

	if (NULL != led_cfg)
	{
		dict_t* d = config_parser_load(AWARE_LED_CONFIG_FILE_PATH);
		led_cfg->led_on_enum = config_parser_get_int(d, "on.s");
		led_cfg->duration_in_sec = config_parser_get_int(d, "on.d");
		led_cfg->led_1_status_enum = config_parser_get_int(d, "st.X.1.c");
		led_cfg->led_1_duration_in_sec = config_parser_get_int(d, "st.X.1.d");
		led_cfg->led_2_status_enum = config_parser_get_int(d, "st.X.2.c");
	    led_cfg->led_2_duration_in_sec = config_parser_get_int(d, "st.X.2.d");
		led_cfg->transition_duration_in_sec = config_parser_get_int(d, "st.d");
		led_cfg->led_1_info_enum = config_parser_get_int(d, "g.c");
		led_cfg->led_1_info_duration_in_sec = config_parser_get_int(d, "g.d");
		led_cfg->led_1_info_pause_in_sec = config_parser_get_int(d, "g.p");
        led_cfg->led_2_info_enum = config_parser_get_int(d, "e.c");
		led_cfg->led_2_info_duration_in_sec = config_parser_get_int(d, "e.d");
		led_cfg->led_2_info_pause_in_sec = config_parser_get_int(d, "e.p");
		led_cfg->led_color_no_of_digits=config_parser_get_int(d, "ct.X.c");
		led_cfg->led_color_digit=config_parser_get_int(d, "ct.X.n");
		led_cfg->led_color_digit_duration_in_sec=config_parser_get_int(d, "ct.dd");
		led_cfg->led_color_blank_duration_in_sec=config_parser_get_int(d, "ct.df");
		led_cfg->led_color_pause_digit=config_parser_get_int(d, "ct.p");
		led_cfg->led_all_digit_color=config_parser_get_int(d, "ct.c");
		led_cfg->led_no_of_min=config_parser_get_int(d, "ct.dc");
		config_parser_free(d);

		LOG_INFO("[aware_app]: ------------------------------------------------");
		LOG_INFO("[aware_app]:               led.conf");
		LOG_INFO("[aware_app]: ------------------------------------------------");
		LOG_INFO("[aware_app]: led on enum: %d", led_cfg->led_on_enum);
		LOG_INFO("[aware_app]: duration in sec: %d", led_cfg->duration_in_sec);
		LOG_INFO("[aware_app]: led 1 status : %d", led_cfg->led_1_status_enum);
		LOG_INFO("[aware_app]: led 1 duration in sec: %d", led_cfg->led_1_duration_in_sec);
		LOG_INFO("[aware_app]: led 2 status: %d", led_cfg->led_2_status_enum);
		LOG_INFO("[aware_app]: led 2 duration in sec: %d", led_cfg->led_2_duration_in_sec);
		LOG_INFO("[aware_app]: transition duration in sec: %d", led_cfg->transition_duration_in_sec);
		LOG_INFO("[aware_app]: led 1 info enum: %d", led_cfg->led_1_info_enum);
		LOG_INFO("[aware_app]: led 1 info duration in sec: %d", led_cfg->led_1_info_duration_in_sec);
		LOG_INFO("[aware_app]: led 1 info pause in sec: %d", led_cfg->led_1_info_pause_in_sec);
		LOG_INFO("[aware_app]: led 2 info enum: %d", led_cfg->led_2_info_enum);
		LOG_INFO("[aware_app]: led 2 info duration in sec: %d", led_cfg->led_2_info_duration_in_sec);
		LOG_INFO("[aware_app]: led 2 info pause in sec: %d", led_cfg->led_2_info_pause_in_sec);
		LOG_INFO("[aware_app]: led color no of digits: %d", led_cfg->led_color_no_of_digits);
		LOG_INFO("[aware_app]: led color digit: %d", led_cfg->led_color_digit);
		LOG_INFO("[aware_app]: led color digit duration in sec: %d", led_cfg->led_color_digit_duration_in_sec);
        LOG_INFO("[aware_app]: led color blank duration in sec: %d", led_cfg->led_color_blank_duration_in_sec);
        LOG_INFO("[aware_app]: led color pause digit: %d",led_cfg->led_color_pause_digit);
        LOG_INFO("[aware_app]: led all digit color: %d", led_cfg->led_all_digit_color);
        LOG_INFO("[aware_app]: led no of min: %d", led_cfg->led_no_of_min);
		LOG_INFO("[aware_app]: ------------------------------------------------");
	}
	else
	{
		status = QAPI_ERR_INVALID_PARAM;
	}

	return status;
}




