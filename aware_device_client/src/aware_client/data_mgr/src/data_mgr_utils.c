/*
 * @file data_mgr_utils.c
 * @brief DATA_MGR Utils implementations
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "qapi_fs.h"
#include "data_mgr_dss.h"
#include "data_mgr.h"
#include "aware_log.h"
#include "app_utils_misc.h"

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

char* base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

/*-------------------------------------------------------------------------*/
/**
  @brief Init signal
  @param signal Signal/event to be inited
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_signal_init(TX_EVENT_FLAGS_GROUP *signal)
{
	LOG_INFO("[data_mgr]: data_mgr_signal_init : Entry\n");

#ifdef QAPI_TXM_MODULE
  if(TX_SUCCESS != txm_module_object_allocate((void **)&signal, sizeof(TX_EVENT_FLAGS_GROUP))) 
  {
    return;
  }
#endif
  tx_event_flags_create(signal, "data_mgr_signal");
}

/*-------------------------------------------------------------------------*/
/**
  @brief Destroy signal
  @param signal Signal pointer
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_signal_destroy(TX_EVENT_FLAGS_GROUP *signal)
{
  if(NULL != signal) {
    tx_event_flags_delete(signal);
  }
}

/*-------------------------------------------------------------------------*/
/**
  @brief Wait on a signal
  @param signal Signal pointer
  @param mask Signal mask
  @param attribute Wait attributes
  @return Set signals
 */
/*--------------------------------------------------------------------------*/
uint32
data_mgr_signal_wait(TX_EVENT_FLAGS_GROUP *signal, uint32 mask, uint32 attribute)
{
  uint32 set_signal = 0;

  data_mgr_signal_wait_timed(signal, mask, attribute, &set_signal, TX_WAIT_FOREVER);
  return set_signal;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Wait on signal for particular time
  @param signal Signal pointer
  @param mask Signal mask
  @param attribute Signal attributes
  @param curr_signals Signals set
  @param timeout Timeout value in secs
  @return Success or fail
 */
/*--------------------------------------------------------------------------*/
int data_mgr_signal_wait_timed(TX_EVENT_FLAGS_GROUP *signal,
                         uint32 mask,
                         uint32 attribute,
                         uint32 *curr_signals,
                         uint32 timeout)
{
  UINT get_option = 0xFFFFFFFF;
  ULONG set_signal = 0;
  UINT ret = TX_GROUP_ERROR;
  //UINT timer_ticks;
  //UINT tick_remain;

  if(NULL == signal) {
    return -1;
  }


  if(attribute & DM_SIGNAL_ATTR_WAIT_ALL) {
    if(attribute & DM_SIGNAL_ATTR_CLEAR_MASK) {
      get_option = TX_AND_CLEAR;
    } else {
      get_option = TX_AND;
    }
  } else {
    if(attribute & DM_SIGNAL_ATTR_CLEAR_MASK) {
      get_option = TX_OR_CLEAR;
    } else {
      get_option = TX_OR;
    }
  }

  if(timeout == TX_WAIT_FOREVER) {
    ret = tx_event_flags_get(signal, mask, get_option, (ULONG *)&set_signal,
                             TX_WAIT_FOREVER); // Wait for ever
  } else {

    timeout = timeout * 100; /*Converting sec to ticks*/  
    ret = tx_event_flags_get(signal, mask, get_option, (ULONG *)&set_signal, timeout);
  }

  if(TX_SUCCESS != ret) {
    if(TX_NO_EVENTS == ret) {
      return TX_NO_EVENTS;
    }
    return TX_NO_INSTANCE;
  }

  *curr_signals = set_signal;
  return TX_SUCCESS;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Set signal mask
  @param signal Signal pointer
  @param mask Signal mask
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_signal_set(TX_EVENT_FLAGS_GROUP *signal, uint32 mask)
{
  tx_event_flags_set(signal, mask, TX_OR); // Donot clear
}

/*-------------------------------------------------------------------------*/
/**
  @brief Clear signal mask
  @param signal Signal pointer
  @param mask Signal mask
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_signal_clear(TX_EVENT_FLAGS_GROUP *signal, uint32 mask)
{
	LOG_INFO("[data_mgr]: data_mgr_signal_clear : Entry\n");

  tx_event_flags_set(signal, ~mask, TX_AND);
}

/*-------------------------------------------------------------------------*/
/**
  @brief Get signals 
  @param signal Signal pointer
  @return Signal mask
 */
/*--------------------------------------------------------------------------*/
uint32 data_mgr_signal_get(TX_EVENT_FLAGS_GROUP *signal)
{
  ULONG get_signal = 0;

  tx_event_flags_info_get(signal, TX_NULL, &get_signal, TX_NULL, TX_NULL,
                          TX_NULL);

  return (uint32)get_signal;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Integer to string Conversion
  @param str Output string
  @param num Input number
  @return Void
 */
/*--------------------------------------------------------------------------*/
void data_mgr_util_int_to_str(char* str, int num)
{
  int i = 0, rem = 0, len = 0, n = 0;
  
  if(str == NULL)
    return;
    
  n = num;
  while(n != 0) {
    len++;
    n /= 10;
  }
  for(i = 0; i < len; i++) {
    rem = num % 10;
    num = num / 10;
    str[len - (i + 1)] = rem + '0';
  }
  str[len] = '\0';
}

#ifdef QAPI_TXM_MODULE
unsigned long __stack_chk_guard __attribute__((weak)) = 0xDEADDEAD;

void __attribute__((weak)) __stack_chk_fail (void)
{
  return;
}


/*-------------------------------------------------------------------------*/
/**
  @brief Allocate memory by Concatinate two string
  @param s1 string1
  @param n1 size of string1
  @param s2 string2
  @param n2 size of string2
  @return void
 */
/*--------------------------------------------------------------------------*/
void *memscat(void *s1, size_t n1, void *s2, size_t n2) {
	void *target = (char*)s1 + n1;
	memcpy(target, s2, n2);
	return s1;
}

#endif


/*-------------------------------------------------------------------------*/
/**
  @brief Encode base64 data
  @param data Data which has to be encoded
  @param input_length Length of the input data
  @param output_length Lenght of output data
  @return Encoded data
 */
/*--------------------------------------------------------------------------*/
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = app_utils_mem_alloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}