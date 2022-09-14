#ifndef _CFG_MGR_UT_H_
#define _CFG_MGR_UT_H_

#include "minunit.h"

//DICTIONARY.C
MU_TEST(streq_success);
MU_TEST(dictionary_has_success);
MU_TEST(dictionary_get_success);
MU_TEST(dictionary_update_success);
MU_TEST(dictionary_len_success);
MU_TEST(dictionary_keys_success);
MU_TEST(dictionary_values_success);

//CONFIG_PARSER.C
MU_TEST(config_parser_get_string_success);
MU_TEST(config_parser_has_key_success);

#endif