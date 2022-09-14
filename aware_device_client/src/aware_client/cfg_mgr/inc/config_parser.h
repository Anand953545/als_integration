#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_

#include "dictionary.h"

#ifdef __cplusplus
extern "C" {
#endif

void config_parser_print(dict_t* d);

/*-------------------------------------------------------------------------*/
/**
  @brief    Save a dictionary to a loadable ini file
  @param    d   Dictionary to dump
  @param    f   Opened file pointer to dump to
  @return   void
 */
/*--------------------------------------------------------------------------*/

void config_parser_save(dict_t * d, const char* file_path);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the binary string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   integer
 */
/*--------------------------------------------------------------------------*/
int config_parser_get_bitmask(dict_t * d, char * key);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @return   pointer to statically allocated character string
 */
/*--------------------------------------------------------------------------*/
char * config_parser_get_string(dict_t * d, char * key);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   integer
 */
/*--------------------------------------------------------------------------*/
int config_parser_get_int(dict_t * d, char * key);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to an long int
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   integer
 */
/*--------------------------------------------------------------------------*/
long int config_parser_get_longint(dict_t * d, char * key);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a float
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   float
 */
/*--------------------------------------------------------------------------*/
float config_parser_get_float(dict_t * d, char * key);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a double
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   double
 */
/*--------------------------------------------------------------------------*/
double config_parser_get_double(dict_t * d, char * key);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a boolean
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   integer
 */
/*--------------------------------------------------------------------------*/
int config_parser_get_boolean(dict_t * d, char * key);


/*-------------------------------------------------------------------------*/
/**
  @brief    Set an entry in a dictionary.
  @param    ini     Dictionary to modify.
  @param    entry   Entry to modify (entry name)
  @param    val     New value to associate to the entry.
  @return   int     0 if Ok, -1 otherwise.
 */
/*--------------------------------------------------------------------------*/
int config_parser_add(dict_t * d, char * entry, char * val);

int config_parser_set_string(dict_t * d, char * entry, char * val);

int config_parser_set_int(dict_t * d, char * entry, int val);
/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an entry in a dictionary
  @param    ini     Dictionary to modify
  @param    entry   Entry to delete (entry name)
  @return   void
 */
/*--------------------------------------------------------------------------*/
void config_parser_remove(dict_t * d, char * entry);

/*-------------------------------------------------------------------------*/
/**
  @brief    Finds out if a given entry exists in a dictionary
  @param    ini     Dictionary to search
  @param    entry   Name of the entry to look for
  @return   integer 1 if entry exists, 0 otherwise
 */
/*--------------------------------------------------------------------------*/
int config_parser_has_entry(dict_t * d, char * entry);

/*-------------------------------------------------------------------------*/
/**
  @brief    Parse an ini file and return an allocated dictionary object
  @param    ininame Name of the ini file to read.
  @return   Pointer to newly allocated dictionary
 */
/*--------------------------------------------------------------------------*/
dict_t * config_parser_load(const char * file_name);

/*-------------------------------------------------------------------------*/
/**
  @brief    Free all memory associated to a dictionary
  @param    d Dictionary to free
  @return   void
 */
/*--------------------------------------------------------------------------*/
void config_parser_free(dict_t* d);

#ifdef __cplusplus
}
#endif

#endif
