#include "cfg_mgr.h"
#include "cJSON.h"
#include "stringl.h"

#include "aware_app.h"
#include "device_common_config.h"
#include "device_general_config.h"
#include "app_config.h"
#include "aware_log.h"
#include "app_utils_misc.h"

#include <string.h>

int cfg_mgr_apply_update(char* msg, int msg_length)
{
	cJSON *root = cJSON_Parse(msg);
    cJSON *node = root->child;

    char* pch = NULL;
    
    for (; node != NULL; node = node->next)
    {
        if(node->type == cJSON_String)
            LOG_INFO("[cfg_mgr]: Config update with key : %s, and value: %s", node->string, node->valuestring);

        if(strcasecmp(node->string, AWARE_ENDPOINT_URL_CONFIG_KEY) == 0) {
            device_static_common_update(&app_ctx.dhc_cfg, node->string, node->valuestring);
        } else if(strcasecmp(node->string, AWARE_ACTIVE_NORMAL_CHECKIN_PERIOD_CONFIG_KEY) == 0) {
            app_config_update(&app_ctx.app_cfg, node->string, node->valuestring);
        }
    } 

    if(root)
	    cJSON_Delete(root);

    return 0;
}