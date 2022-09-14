#include "aware_health_check.h"
#include "aware_log.h"

#include "qapi_status.h"
#include "qapi_fs.h"
#include "qapi_types.h"
#undef true
#undef false
#include "qapi_device_info.h"
#include "aware_utils.h"
#include "app_utils_misc.h"
#include "aware_device_info.h"

#include "cfg_mgr.h"

void print_sim_status(int);




/*-------------------------------------------------------------------------*/
/**
  @brief	Validates and gives config parameters in the config file
  @param	key Config key
  @param	filepath File location
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
boolean valid_config(const char* key, char *filepath)
{
	boolean result = FALSE;

	qapi_Status_t status = QAPI_OK;

	if ((strlen(key) > 0) && (NULL != filepath))
	{
		char str[MAXSTR] = "";
		char fileread[MAXFILESIZE + 1] = "";
		int fp = -1;
		char *token;
		uint32_t token_len = 0;
		uint32_t bytes_read = 0;
		uint32_t row_idx = 0;
		uint32_t char_idx = 0;

		status = qapi_FS_Open(filepath, QAPI_FS_O_RDONLY_E, &fp);
		if (status != QAPI_OK)
		{
			LOG_INFO("Unable to open file %s ! %d\n", filepath, status);
		}
		else
		{
			struct qapi_FS_Stat_Type_s file_stat;
			memset(&file_stat, 0, sizeof(struct qapi_FS_Stat_Type_s));

			if (QAPI_OK == (qapi_FS_Stat(filepath, &file_stat)))
			{
				const char *delim = "=\n";
				if ((QAPI_OK) == (qapi_FS_Read(fp, fileread, file_stat.st_size, &bytes_read)))
				{
					for (char_idx = 0; char_idx < (file_stat.st_size + 1); char_idx++)
					{
						str[row_idx] = fileread[char_idx];
						if (str[row_idx] == '\n' || str[row_idx] == 0x0) {
							if (strchr(str, '=')) {
								char key_name[5];

								token = strtok2(str, delim);
								token_len = strlen(token)+1;
								strlcpy(key_name, token, token_len);

								token = strtok2(NULL, delim);
								trim_white_spaces(token);

								if(strcasecmp(key_name, key) == 0 && strlen(token) > 0)	{
									if(strcasecmp("sn", key) == 0) {
										char* imei = get_aware_device_info(AWARE_DEVICE_INFO_IMEI);
										if(strcasecmp(token, imei) == 0){
											result = TRUE;
										} else {
											LOG_INFO("[aware_app]: serial number doesn't match %s, %s", token, imei);
										}
										app_utils_byte_release(imei);
									} else {
										result = TRUE;
									}
									break;
								}
							}
							row_idx = 0;
							memset(str, '\0', sizeof(str));
						}
						else {
							row_idx++;
						}
					}
				}
			}
			qapi_FS_Close(fp);
		}
	}
	else {
		status = QAPI_ERR_INVALID_PARAM;
	}

	return result;
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Health check routine
  @return   success or failure
 */
/*--------------------------------------------------------------------------*/
int perform_health_check(app_context_t* app_ctx)
{
	qapi_Status_t status = QAPI_OK;
	app_ctx->init_error = INTI_ERROR_NONE;

	boolean dhu_config = FALSE;
	boolean dhg_config = FALSE;
	boolean dsc_config = FALSE;

	LOG_INFO("[aware_app]: ------------------------------------------------");
	LOG_INFO("[aware_app]:               Health Check Results              ");
	LOG_INFO("[aware_app]: ------------------------------------------------");

	/* stat for config files*/
	struct qapi_FS_Stat_Type_s file_stat;
	memset(&file_stat, 0, sizeof(struct qapi_FS_Stat_Type_s));
	if (QAPI_OK == (qapi_FS_Stat(AWARE_DEVICE_HC_GENERAL_CONFIG_FILE_PATH, &file_stat))) {
		dhg_config = TRUE;
		LOG_INFO("[aware_app]: %s config file : OK", AWARE_DEVICE_HC_GENERAL_CONFIG_FILE_PATH);
	}
	else
	{
		app_ctx->init_error = INTI_ERROR_NO_AWARE_INFO;
		LOG_ERROR("[aware_app]: %s config file : Missing!", AWARE_DEVICE_HC_GENERAL_CONFIG_FILE_PATH);
	}

	memset(&file_stat, 0, sizeof(struct qapi_FS_Stat_Type_s));
	if (QAPI_OK == (qapi_FS_Stat(AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH, &file_stat)))
	{
		dhu_config = TRUE;
		LOG_INFO("[aware_app]: %s config file : OK", AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH);
	}
	else
	{
		app_ctx->init_error = INTI_ERROR_NO_AWARE_INFO;
		LOG_ERROR("[aware_app]: %s config file : Missing!", AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH);
	}

	memset(&file_stat, 0, sizeof(struct qapi_FS_Stat_Type_s));
	if (QAPI_OK == (qapi_FS_Stat(AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH, &file_stat)))
	{
		dsc_config = TRUE;
		LOG_INFO("[aware_app]: %s config file : OK", AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);
	}
	else
	{
		app_ctx->init_error = INTI_ERROR_NO_AWARE_INFO;
		LOG_ERROR("[aware_app]: %s config file : Missing!", AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);
	}

	/*check security files */
	memset(&file_stat, 0, sizeof(struct qapi_FS_Stat_Type_s));
	if (QAPI_OK == (qapi_FS_Stat(AWARE_SECURITY_FILE_PATH, &file_stat)))
	{
		LOG_INFO("[aware_app]: Security files : OK");
	}
	else
	{
		app_ctx->init_error = INTI_ERROR_NO_SECURITY;
		LOG_ERROR("[aware_app]: Security files : Missing!");
	}

	if(INTI_ERROR_NONE != app_ctx->init_error) {
		return EFAILURE;
	}

	/* Check for value in config files*/
	if (dhu_config && dhg_config && dsc_config)
	{
		boolean device_id_status = valid_config("ai", AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH);
		boolean serial_number_status = valid_config("sn", AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH);
		boolean aware_url_status = valid_config("u.1", AWARE_DEVICE_STATIC_COMMON_CONFIG_FILE_PATH);
	
		if (FALSE == device_id_status || FALSE == serial_number_status || FALSE == aware_url_status)
		{
			app_ctx->init_error = INTI_ERROR_INVALID_CONF_FILES;
			if(FALSE == device_id_status)
			{
				LOG_ERROR("[aware_app]: Aware device id : Missing!");
			}
			else if(FALSE == serial_number_status)
			{
				LOG_ERROR("[aware_app]: Aware device serial number : Missing or Doesn't match!");
			}
			else if(FALSE == aware_url_status)
			{
				LOG_ERROR("[aware_app]: Aware URL : Missing!");
			}
		}
	}

	if(INTI_ERROR_NONE != app_ctx->init_error) {
		return EFAILURE;
	}

	int sim_state = get_aware_device_sim_status();
	print_sim_status(sim_state);

	if(sim_state != QAPI_DEVICE_INFO_SIM_STATE_READY)
		app_ctx->init_error = INTI_ERROR_NO_SIM;

	LOG_INFO("[aware_app]: ------------------------------------------------\n");

	if(INTI_ERROR_NONE != app_ctx->init_error) {
		return EFAILURE;
	}

	return ESUCCESS;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Prints the sim card status description
  @param    sim_state Sim status input
  @return   void
 */
/*--------------------------------------------------------------------------*/
void print_sim_status(int sim_state)
{
	switch(sim_state)
	{
		case QAPI_DEVICE_INFO_SIM_STATE_UNKNOWN:
			LOG_INFO("[aware_app]: Sim State: Unknown.");
		break;
		case QAPI_DEVICE_INFO_SIM_STATE_DETECTED:
			LOG_INFO("[aware_app]: Sim State: Detected.");
		break;
		case QAPI_DEVICE_INFO_SIM_STATE_PIN1_OR_UPIN_REQ:
			LOG_INFO("[aware_app]: Sim State: PIN1 or UPIN is required.");
		break;
		case QAPI_DEVICE_INFO_SIM_STATE_PUK1_OR_PUK_REQ:
			LOG_INFO("[aware_app]: Sim State: PUK1 or PUK for UPIN is required.");
		break;
		case QAPI_DEVICE_INFO_SIM_STATE_PERSON_CHECK_REQ:
			LOG_INFO("[aware_app]: Sim State: Personalization state must be checked.");
		break;
		case QAPI_DEVICE_INFO_SIM_STATE_PIN1_PERM_BLOCKED:
			LOG_INFO("[aware_app]: Sim State: PIN1 is blocked.");
		break;
		case QAPI_DEVICE_INFO_SIM_STATE_ILLEGAL:
			LOG_INFO("[aware_app]: Sim State: Illegal.");
		break;
		case QAPI_DEVICE_INFO_SIM_STATE_READY:
			LOG_INFO("[aware_app]: Sim State: Ready.");
		break;
	}
}



/*-------------------------------------------------------------------------*/
/**
  @brief    writes logs to file
  @param    buf_ptr		Pointer to buffer string
  @return   0 if success or -1 if failure
 */
/*--------------------------------------------------------------------------*/
int hc_log_to_file(char *buf_ptr)
{
	int ret = QAPI_ERROR;
	int fd = -1;
	uint32_t bytes_written;
	int buf_size = strlen(buf_ptr);

	ret = qapi_FS_Open_With_Mode(AWARE_HEALTH_CHECK_LOG_PATH, QAPI_FS_O_RDWR_E | QAPI_FS_O_CREAT_E | QAPI_FS_O_APPEND_E, QAPI_FS_S_IWUSR_E, &fd);
	if(ret != QAPI_OK)
	{
		LOG_ERROR("[aware_app]: opening file write_to_health_check_log_file : failed, qapi_FS_Open ret = %d\n", ret);
		return -1;
	}

	ret = qapi_FS_Write(fd, buf_ptr, buf_size, &bytes_written);
	if(ret != QAPI_OK)
	{
		LOG_ERROR("[aware_app]: writing write_to_health_check_log_file : failed, qapi_FS_Write ret = %d\n", ret);
		qapi_FS_Close(fd);
		return -1;
	}

	LOG_INFO("[aware_app]: write_to_health_check_log_file : success, qapi_FS_Write written bytes = %d\n", bytes_written);
	qapi_FS_Close(fd);

	return 0;
}