#include "dictionary.h"
#include "app_utils_misc.h"
#include "aware_log.h"

/*-------------------------------------------------------------------------*/
/**
  @brief    Compares two strings
  @param    str1      Buffer pointer to string
  @param    str2      Buffer pointer to string
  @return   integer value 1 if strings are equal,  0 if strings are not equal
 */
/*--------------------------------------------------------------------------*/
int streq(char *str1, char *str2)
{
    while(*str1 == *str2 && *str1 != '\0' && *str2 != '\0')
        str1++, str2++;

    if(*str1 == '\0' && *str2 == '\0') return 1; // equal

    return 0; // not equal
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Creates new dictionary
  @return   d      Pointer to newly allocated dictionary
 */
/*--------------------------------------------------------------------------*/
dict_t *dictionary_new()
{
    dict_t *d = NULL;
    int result = app_utils_byte_allocate((void**)&d, sizeof(dict_t));
    strlcpy(d->key, "", 1);
    d->next = 0;

    return d;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Free the dictionary
  @param   d      Pointer to a dictionary
 */
/*--------------------------------------------------------------------------*/
void dictionary_free(dict_t *d)
{
    if((int)(d->next) != 0) {
        dictionary_free(d->next);
    }

    app_utils_byte_release(d);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Check the string associated to a key
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @return   integer 1 if key is present, or string present in the dictionary
 */
/*--------------------------------------------------------------------------*/
int dictionary_has(dict_t *d, char *key)
{
    if(streq(d->key, key)) {
        return 1;
    } else if((int)(d->next) != 0) {
        return dictionary_has(d->next, key);
    }

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @return   Pointer to statically allocated character string
 */
/*--------------------------------------------------------------------------*/
char *dictionary_get(dict_t *d, char *key)
{
    if(streq(d->key, key)) {
        char *val = NULL;
        int result = app_utils_byte_allocate((void**)&val, sizeof(d->val));
        if(result >= 0) {
            strlcpy(val, d->val, VALUE_LENGTH);
        }

        return val;
    } else {
        if((int)(d->next) != 0) {
            return dictionary_get(d->next, key);
        }
    }

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Update the dictionary with string associated to key with string associated to val
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @param    val     String value to replace in place of key string
  @return   integer 1 if value string is updated successfully, 0 if not updated
 */
/*--------------------------------------------------------------------------*/
int dictionary_update(dict_t *d, char *key, char *val)
{
    int ret = 0;

    if(streq(key, "")) return ret;

    if(streq(d->key, key) || streq(d->key, "")) {
        if(streq(d->key, "")) {
            strlcpy(d->key, key, KEY_LENGTH);
        }
        if(val!=NULL)    
            strlcpy(d->val, val, VALUE_LENGTH);
        else
            d->val[0]='\0';
        //LOG_INFO("key: %s, value: %s", key, val);
        ret = 1;
    } else {
        if((int)(d->next) == 0) {
            d->next = dictionary_new();
        }

        dictionary_update(d->next, key, val);
    }

    return ret;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Remove the string associated to a key from dictionary
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @return   void
 */
/*--------------------------------------------------------------------------*/
void dictionary_remove(dict_t *d, char *key)
{
    if(streq(d->key, key) && (int)(d->next) != 0) {
	    dict_t *n = d->next;
        *d = *n;
        app_utils_byte_release(n);
    } else if((int)(d->next) != 0) {
        dictionary_remove(d->next, key);
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Find the dictionary lenght
  @param    d       Dictionary to search
  @return   i       Dictionary lenght
 */
/*--------------------------------------------------------------------------*/
int dictionary_len(dict_t *d)
{
    int i;

    for(i = 1; (int)(d->next) != 0; i++)
        d = d->next;

    return i;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the list of string in dictionary associated with key
  @param    d       Dictionary to search
  @return   keys    Pointer to list of string
 */
/*--------------------------------------------------------------------------*/
char **dictionary_keys(dict_t *d)
{
    int len = dictionary_len(d);
    char **keys;
    int result = app_utils_byte_allocate((void**)keys, len * sizeof(char *));

    for(int i = 0; i < len; i++) {
        result = app_utils_byte_allocate((void**)&keys[i], KEY_LENGTH);
        strlcpy(keys[i], d->key, KEY_LENGTH);
        d = d->next;
    }

    return keys;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the list of string in dictionary associated with value
  @param    d       Dictionary to search
  @return   values  Pointer to list of string
 */
/*--------------------------------------------------------------------------*/
char **dictionary_values(dict_t *d)
{
    int len = dictionary_len(d);
    char **values;
    int result = app_utils_byte_allocate((void**)values, len * sizeof(char *));

    for(int i = 0; i < len; i++) {
        result = app_utils_byte_allocate((void**)&values[i], VALUE_LENGTH);
        strlcpy(values[i], d->val, VALUE_LENGTH);
        d = d->next;
    }

    return values;
}