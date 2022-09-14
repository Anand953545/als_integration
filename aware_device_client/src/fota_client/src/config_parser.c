#include "config_parser.h"
#include "app_utils_misc.h"
#include "stringl.h"
#include "qapi_fs.h"
#include "log.h"
#include "aware_utils.h"

/*---------------------------- Defines -------------------------------------*/
#define tolower(x) (('A' <= (x) && (x) <= 'Z') ? ((x - 'A') + 'a') : (x))
#define isspace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')

/*-------------------------------------------------------------------------*/
/**
  @brief    Convert a string to lowercase.
  @param    in   String to convert.
  @param    out Output buffer.
  @param    len Size of the out buffer.
  @return   ptr to the out buffer or NULL if an error occured.
 */
/*--------------------------------------------------------------------------*/
static const char * strlower(const char * in, char *out, unsigned len)
{
    unsigned i ;

    if (in==NULL || out == NULL || len==0) return NULL ;

    i=0 ;
    while (in[i] != '\0' && i < len-1) {
        out[i] = (char)tolower((int)in[i]);
        i++ ;
    }

    out[i] = '\0';

    return out ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Remove blanks at the beginning and the end of a string.
  @param    str  String to parse and alter.
  @return   unsigned New size of the string.
 */
/*--------------------------------------------------------------------------*/
static unsigned trim_s(char * str)
{
  char *output = str, *target = str;

  for(; *str != '\0'; ++str)
  {
    if (!isspace((int)*str))
      *target++ = *str;
  }
  *target = '\0';

  return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Prints the elements in the dictionary.
  @param    d  Pointer to a dictionary.
  @return   void
 */
/*--------------------------------------------------------------------------*/
void config_parser_print(dict_t* d)
{
    int len = dictionary_len(d);

    for(int i = 0; i < len; i++) {
        if (streq(d->key, ""))
            continue;
        LOG_INFO("%s=%s", d->key, d->val);
        d = d->next;
    }
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Save a dictionary to a file.
  @param    d   Dictionary to dump.
  @param    path   file location to dump to.
  @return   void
 */
/*--------------------------------------------------------------------------*/
void config_parser_save(dict_t * d, const char* config_file_path)
{
    int i, fd = -1, bytes_written = 0;
    char config_buffer[256];

    qapi_FS_Status_t ret = qapi_FS_Open(config_file_path, QAPI_FS_O_WRONLY_E | QAPI_FS_O_TRUNC_E, &fd);
    if (d == NULL || ret > 0) return ;

    int len = dictionary_len(d);
    if(len > 0) {
        for(int i = 0; i < len; i++) {
            if (streq(d->key, ""))
                continue;
            
            memset(config_buffer, 0, 256);
            snprintf(config_buffer, 256, "%s=%s\n", d->key, d->val);
            qapi_FS_Write(fd, config_buffer, strlen(config_buffer), &bytes_written);
            d = d->next;
        }
    }

    qapi_FS_Close(fd);

    return ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @return   pointer to statically allocated character string
 */
/*--------------------------------------------------------------------------*/
char * config_parser_get_string(dict_t * d, char * key)
{
    const char * def = "";
    char * sval ;

    if (d == NULL || key == NULL)
        return (char*)def ;

    sval = dictionary_get(d, key);

    return sval ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to an long int
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   long integer
 */
/*--------------------------------------------------------------------------*/
long int config_parser_get_longint(dict_t * d, char * key)
{
    char* str = config_parser_get_string(d, key);

    if (streq((char*)str, "")) return -1 ;

    long int value = strtol(str, NULL, 0);

    app_utils_byte_release(str);

    return value;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the binary string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   integer
 */
/*--------------------------------------------------------------------------*/
int config_parser_get_bitmask(dict_t * d, char * key)
{
    char * str = config_parser_get_string(d, key);

    if (streq((char*)str, "")) 
    {    
        
        app_utils_byte_release(str);
        return -1;
    }
    int value = strtol(str, NULL, 2);

    app_utils_byte_release(str);

    return value;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   integer
 */
/*--------------------------------------------------------------------------*/
int config_parser_get_int(dict_t * d, char * key)
{
    char * str = config_parser_get_string(d, key);

    if (streq((char*)str, "")) return -1 ;

    int value = strtol(str, NULL, 0);

    app_utils_byte_release(str);

    return value;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a double
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   double
 */
/*--------------------------------------------------------------------------*/
double config_parser_get_double(dict_t * d, char * key)
{
    char * str = config_parser_get_string(d, key);

    if (streq((char*)str, "")) return 0;

    double value = atof(str);

    app_utils_byte_release(str);

    return value;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a boolean
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   integer
 */
/*--------------------------------------------------------------------------*/
int config_parser_get_boolean(dict_t * d, char * key)
{
    int          ret ;
    char * str = config_parser_get_string(d, key);

    if (streq((char*)str, "")) return 0 ;

    if (streq((char*)str, "TRUE")) {
        ret = 1 ;
    } else if (streq((char*)str, "FALSE")) {
        ret = 0 ;
    } else {
        ret = -1 ;
    }

    app_utils_byte_release(str);

    return ret;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Finds out if a given entry exists in a dictionary
  @param    d       Dictionary to search
  @param    entry   Name of the entry to look for
  @return   integer 1 if entry exists, 0 otherwise
 */
/*--------------------------------------------------------------------------*/
int config_parser_has_key(dict_t * d, char * key)
{
    int found = dictionary_has(d, key) ;

    return found ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Set an entry in a dictionary.
  @param    ini     Dictionary to modify.
  @param    key     Entry to modify (entry name)
  @param    val     New value to associate to the entry.
  @return   int 0 if Ok, -1 otherwise.
 */
/*--------------------------------------------------------------------------*/
int config_parser_add(dict_t * d, char * key, char * val)
{
    return dictionary_update(d, key, val);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an entry in a dictionary
  @param    d       Dictionary to modify
  @param    key     Entry to delete (entry name)
  @return   void
 */
/*--------------------------------------------------------------------------*/
void config_parser_remove(dict_t * d, char * key)
{
    dictionary_remove(d, key);
}

int config_parser_set_string(dict_t * d, char * key, char * val)
{
    if(dictionary_has(d, key)) {
        return dictionary_update(d, key, val);
    }

    return 0;
}

int config_parser_set_int(dict_t * d, char * key, int value)
{
    if(dictionary_has(d, key)) {
        char str[16];

        memset(str, 0, sizeof(str));
        snprintf(str, sizeof(str), "%d", value);

        return dictionary_update(d, key, str);
    }

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Open and parse an config file and return an allocated dictionary object
  @param    config_file_path Path of the config file to read.
  @return   Pointer to newly allocated dictionary
 */
/*--------------------------------------------------------------------------*/
dict_t* config_parser_load(const char * config_file_path)
{
    dict_t* kv_dict = NULL;
    int fd = -1;
    uint32_t kv_idx = 0;
    uint32_t char_idx = 0;
    char file_line[128] = {0};
    char file_contents[2048] = {0};

    qapi_FS_Status_t ret = qapi_FS_Open(config_file_path, QAPI_FS_O_RDONLY_E, &fd);
    LOG_INFO("Opening file %s, status %d", config_file_path, ret);

    if(QAPI_OK == ret) {
        kv_dict = dictionary_new();

        struct qapi_FS_Stat_Type_s file_stat;
        memset(&file_stat, 0, sizeof(struct qapi_FS_Stat_Type_s));
        if (QAPI_OK == (qapi_FS_Stat_With_Handle(fd, &file_stat)))
        {
            LOG_INFO("Opened cofnig file %s, size %d", config_file_path, file_stat.st_size);
            const char *delim = "=\n";
            char *token;
            uint32_t bytes_read = 0, token_len = 0;
            if ((QAPI_OK) == (qapi_FS_Read(fd, file_contents, file_stat.st_size, &bytes_read)))
            {
                for (char_idx = 0; char_idx < (file_stat.st_size + 1); char_idx++)
                {
                    file_line[kv_idx] = file_contents[char_idx];
                    if (file_line[kv_idx] == '\n' || file_line[kv_idx] == 0x0)
                    {
                        if (strchr(file_line, '='))
                        {
                            char key_entry[16];

                            token = strtok2(file_line, delim);
                            token_len = strlen(token)+1;
                            strlcpy(key_entry, token, token_len);

                            token = strtok2(NULL, delim);
                            
                            if(token!=NULL)
                                trim_white_spaces(token);

                            config_parser_add(kv_dict, key_entry, token);
                        }

                        kv_idx = 0;
                        memset(file_line, '\0', sizeof(file_line));
                    }
                    else
                    {
                        kv_idx++;
                    }
                }
            }
        }
        qapi_FS_Close(fd);
    }

    return kv_dict;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Free all memory associated to a dictionary
  @param    d Dictionary to free
  @return   void
 */
/*--------------------------------------------------------------------------*/
void config_parser_free(dict_t * d)
{
    dictionary_free(d);
}
