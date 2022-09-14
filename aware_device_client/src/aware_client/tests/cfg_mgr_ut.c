#include "cfg_mgr_ut.h"
#include "dictionary.h"
#include "config_parser.h"


//DICTIONARY.C

MU_TEST(streq_success)
{
	int status = streq("aware_app", "aware_app");
    mu_check(status == 1);
    LOG_INFO("[UNIT TEST]: streq_success");
}

MU_TEST(dictionary_has_success)
{
    dict_t UT={"aware_app"};
	int status = dictionary_has(&UT, "a");
    mu_check(status == 1);
    LOG_INFO("[UNIT TEST]: dictionary_has_success");
}

MU_TEST(dictionary_get_success)
{
    dict_t UT={"aware_app"};
	char* status = dictionary_get(&UT, "a");
    mu_check(status != 0);
    LOG_INFO("[UNIT TEST]: dictionary_get_success");
}

MU_TEST(dictionary_update_success)
{
    dict_t UT={"aware_app"};
	int status = dictionary_update(&UT, "a","NULL");
    mu_check(status == 1);
    LOG_INFO("[UNIT TEST]: dictionary_update_success");
}

MU_TEST(dictionary_len_success)
{
    dict_t UT={"aware_app"};
	int status = dictionary_len(&UT);
    mu_check(status != 0);
    LOG_INFO("[UNIT TEST]: dictionary_len_success");
}

MU_TEST(dictionary_keys_success)
{
    dict_t UT={"aware_app"};
	char** status = dictionary_keys(&UT);
    mu_check(status != NULL);
    LOG_INFO("[UNIT TEST]: dictionary_keys_success");
}

MU_TEST(dictionary_values_success)
{
    dict_t UT={"aware_app","a"};
	char** status = dictionary_values(&UT);
    mu_check(status != NULL);
    LOG_INFO("[UNIT TEST]: dictionary_values_success");
}


//CONFIG_PARSER.C
MU_TEST(config_parser_get_string_success)
{
    dict_t UT={"aware_app"};
	char* status = config_parser_get_string(&UT, "a");
    mu_check(status != NULL);
    LOG_INFO("[UNIT TEST]: config_parser_get_string_success");
}

MU_TEST(config_parser_has_key_success)
{
    dict_t UT={"aware_app"};
	int status = config_parser_has_key(&UT, "a");
    mu_check(status == 1);
    LOG_INFO("[UNIT TEST]: config_parser_has_key_success");
}