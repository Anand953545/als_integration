#include "app_types.h"
#include "app_utils_misc.h"
//#include "aware_log.h"
#include "log.h"

char * strdup_s(const char * s)
{
    char * t ;
    size_t len ;

    if (!s)
        return NULL ;

    len = strlen(s) + 1 ;
    app_utils_byte_allocate((void**)&t, len);
    if (t) {
        memscpy(t, len, s, len) ;
    }

    return t ;
}

char * trim_white_spaces(char *input)
{
  char *output = input, *target = input;

  for(; *input != '\0'; ++input)
  {
    if(*input != ' ' && *input != '\t' && *input != '\r' && *input != '\n')
      *target++ = *input;
  }
  *target = '\0';

  return output;
}

// Pass in len as 0 for it to be auto computed
uint64_t app_utils_string_to_uint64(char* str, uint32 strLen)
{
    int i = 0;
    uint64_t value = 0;
    char digit = '\0';

    if(NULL == str){
        return 0;
    }
    if (strLen == 0) {
        strLen = strlen(str);
    }
    if(strLen > 19 || strLen <= 0){
        return 0;
    }

    // traverse all characters in string
    for(i=0; i < strLen; i++){
        digit = str[i];
        if(digit < 48 || digit > 57){
            return 0;
        }
        value = (value*10) + (digit-48);
    }

    return value;
}

// returns TRUE if string is unsigned int, or else FALSE
int app_utils_is_string_uint(char* str, uint32 strLen)
{
    uint32 i = 0;
    char digit = '\0';

    if (NULL == str)
    {
        return FALSE;
    }
    if (strLen == 0)
    {
        strLen = strlen(str);
    }
    if(strLen > 19 || strLen == 0)
    {
        return FALSE;
    }

    // traverse all characters in string
    for(i = 0; i < strLen; i++)
    {
        digit = str[i];
        if(digit < 48 || digit > 57){
            return FALSE;
        }
    }

    return TRUE;
}

float app_utils_string_to_float(char* str) {
  float value = 0, fact = 1;

  if (*str == '-') {
    str++;
    fact = -1;
  }

  for (int fraction = 0; *str; str++) {
    if (*str == '.') {
      fraction = 1; 
      continue;
    }

    int d = *str - '0';
    if (d >= 0 && d <= 9) {
      if (fraction) fact /= 10.0f;
      value = value * 10.0f + (float)d;
    }
  }

  return value * fact;
}

void print_byte_array(uint8* src, size_t len)
{
    for(int i = 0; i < len; i++)
    {
        LOG_INFO("%d", src[i]);
    }
}