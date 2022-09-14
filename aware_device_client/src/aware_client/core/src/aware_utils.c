#include "aware_utils.h"
#include "stringl.h"
#include <float.h>
#include "qapi_fs_types.h"
#include "qapi_fs.h"
#include "cfg_mgr.h"
#include "qapi_timer.h"
#include "qapi_pmapp_vbatt.h"


/*-------------------------------------------------------------------------*/
/**
  @brief	Get the battery level
  @param    battery_level	battey level out parameter
  @return   Success or failure
 */
/*--------------------------------------------------------------------------*/
int get_battery_level(int* battery_level)
{
	int battery_status = qapi_Pmapp_Vbatt_Get_Battery_Status(battery_level);

	if(( QAPI_OK !=  battery_status) && (QAPI_ERR_BATT_ABSENT != battery_status))
	{
		battery_level = 0;
		battery_status = -1;
	}
	else if( QAPI_ERR_BATT_ABSENT == battery_status )
	{
		battery_level = 0;
		battery_status = -1;
	}

	return battery_status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Gets free sapce in efs file
  @param    file_statvfs	Structure of file system information
  @return   Success or failure
 */
/*--------------------------------------------------------------------------*/
int get_free_space_in_EFS(struct qapi_FS_Statvfs_Type_s* file_statvfs)
{
	int status=QAPI_OK;
	//struct qapi_FS_Statvfs_Type_s file_statvfs;
	memset(file_statvfs, 0, sizeof(struct qapi_FS_Statvfs_Type_s));
    status = qapi_FS_Statvfs(AWARE_DEVICE_HC_UNIQUE_CONFIG_FILE_PATH, file_statvfs);
    //if (status != QAPI_OK)
	//{
		//totalbytes=0;
		//status = QAPI_ERR_INVALID_PARAM;
	//	total_free_bytes_efs=0;
		//LOG_ERROR("Filestatvfs not working ! %d\n", status);
	
	//}
	//else
	//{
         // total_free_bytes_efs=(file_statvfs.f_bsize*file_statvfs.f_bfree);
		//  LOG_INFO("file system block size ! %ld\n", file_statvfs.f_bsize);
        //  LOG_INFO("Maximum possible number of blocks available! %ld\n",file_statvfs.f_blocks);
        //  LOG_INFO("Total number of free blocks! %ld\n",file_statvfs.f_bfree);
        //  LOG_INFO("Number of free blocks currently available! %ld\n",file_statvfs.f_bavail);
         
		 
		// // LOG_INFO("Total number of free bytes available on EFS! %ld\n",total_free_bytes_efs);
         
		//  //LOG_INFO("Total number of free file serial numbers! %ld\n",file_statvfs.f_ffree);
        //  LOG_INFO("Number of file serial numbers available ! %ld\n",file_statvfs.f_favail);
        //  LOG_INFO("Blocks allocated in the general pool ! %ld\n",file_statvfs.f_balloc);

    //}

	return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Gets the current time in milliseconds
  @return   Time in milliseconds
 */
/*--------------------------------------------------------------------------*/
uint64_t get_current_time_in_milliseconds()
{
   uint16_t y;
   uint16_t m;
   uint16_t d;
   uint64_t t;
 
 	qapi_time_get_t time_info;
	qapi_time_get(QAPI_TIME_JULIAN, &time_info);

   //Year
   y = time_info.time_julian.year;
   //Month of year
   m = time_info.time_julian.month;
   //Day of month
   d = time_info.time_julian.day;
 
   //January and February are counted as months 13 and 14 of the previous year
   if(m <= 2)
   {
      m += 12;
      y -= 1;
   }
 
   //Convert years to days
   t = (365 * y) + (y / 4) - (y / 100) + (y / 400);
   //Convert months to days
   t += (30 * m) + (3 * (m + 1) / 5) + d;
   //Unix time starts on January 1st, 1970
   t -= 719561;
   //Convert days to seconds
   t *= 86400;
   //Add hours, minutes and seconds
   t += (3600 * time_info.time_julian.hour) + (60 * time_info.time_julian.minute) + time_info.time_julian.second;
 
	//convert seconds to milliseconds
	t *= 1000;

   //Return current milliseconds
   return t;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Starts the timer
  @param 	initial_delay Initial delay
  @param 	sigs_func_ptr Timer expiry callback function
  @return   Timer handle
 */
/*--------------------------------------------------------------------------*/
qapi_TIMER_handle_t start_app_timer(uint32_t initial_delay, void* sigs_func_ptr)
{
    qapi_TIMER_handle_t timer_handle;
    qapi_TIMER_define_attr_t timer_def_attr;
    qapi_TIMER_set_attr_t timer_set_attr;

    timer_def_attr.cb_type = QAPI_TIMER_FUNC1_CB_TYPE; // notification type
    timer_def_attr.sigs_func_ptr = sigs_func_ptr; // callback to call when
                                                  // the timer expires
    timer_def_attr.sigs_mask_data = 0;  // this data will be returned in
                                        // the callback
    timer_def_attr.deferrable = false;  // set to true for nondeferrable timer
                                        // define the timer. Note: This call allocates memory and hence
                                        // qapi_Timer_Undef() should be called whenever the timer usage is done.
    qapi_Timer_Def(&timer_handle, &timer_def_attr);
    timer_set_attr.reload = false; // Do not restart timer after it expires
    timer_set_attr.time = initial_delay;
    timer_set_attr.unit = QAPI_TIMER_UNIT_SEC;
    // set or start the timer
    qapi_Timer_Set(timer_handle, &timer_set_attr);

    return timer_handle;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Stops the timer
  @param 	timer_handle Timer handle input
  @return   Void
 */
/*--------------------------------------------------------------------------*/
void stop_app_timer(qapi_TIMER_handle_t timer_handle)
{
    // stop a running timer
    qapi_Timer_Stop(timer_handle);
    // Undef the timer. Releases memory allocated in qapi_Timer_Def()
    qapi_Timer_Undef(timer_handle);
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Restarts the timer base on the timeout interval
  @param 	timer_handle Timer handle
  @param 	timeout Timeout in seconds
  @return   Success or failure
 */
/*--------------------------------------------------------------------------*/
int restart_app_timer(qapi_TIMER_handle_t timer_handle, uint32_t timeout)
{
	qapi_TIMER_set_attr_t timer_set_attr;

	timer_set_attr.reload = true;
	timer_set_attr.time = timeout;
	timer_set_attr.unit = QAPI_TIMER_UNIT_SEC;

	// set or start the timer
	qapi_Timer_Set(timer_handle, &timer_set_attr);

    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	String to Integer conversion utility
  @param 	str String
  @return   Integer
 */
/*--------------------------------------------------------------------------*/
int aware_atoi(const char *str)
{
  int res = 0; // Initializer
  int sign = 1, i;

  if(str == NULL)
    return 0;
    
  /* remove trailing whitespace */
  for(i = 0; str[i]; i++) {
    if(str[i] == ' ')
      continue;
    else if(str[i] == '-') {
      sign = -1;
      break;
    } else
      break;
  }

  for(; str[i] && str[i] >= '0' && str[i] <= '9'; i++)
    res = res * 10 + str[i] - '0';
  return res * sign;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Converts integer to string
  @param 	data 	 Integer input
  @param 	string	 Output string
  @param 	length	 Output string length
  @return   String length
 */
/*--------------------------------------------------------------------------*/
size_t aware_int_to_str(int64_t data, uint8_t *string, size_t length)
{
	int index = 0;
	boolean minus = FALSE;
	size_t result = 0;
	if(string == NULL)
	{
	  return 0;
	}
  
	if (data < 0)
	{
	  minus = TRUE;
	  data = 0 - data;
	}
	else
	{
	  minus = FALSE;
	}
  
	index = length - 1;
	do
	{
	  string[index] = '0' + data%10;
	  data /= 10;
	  index --;
	} while(index >= 0 && data > 0);
  
	if(data > 0)
	  return 0;
  
	if(minus == TRUE)
	{
	  if(index == 0)
	  return 0;
	  string[index] = '-';
	}
	else
	{
	  index++;
	}
  
	result = length - index;
  
	if (result < length)
	{
	  memscpy(string, length, string + index, result);
	  string[result] = '\0';
	}
  
	return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Converts a floating point number to string
  @param 	data 	 Floating point number
  @param 	string	 Converted string output
  @param 	length	 Converted string length
  @return   String length
 */
/*--------------------------------------------------------------------------*/
size_t aware_float_to_str(double data, uint8_t * string, size_t length)
{
	size_t intLength = 0;
	size_t decLength = 0;
	int64_t intPart = 0;
	double decPart = 0;
  
	if(string == NULL)
	{
	  return 0;
	}
  
	if (data <= (double)INT64_MIN || data >= (double)INT64_MAX) 
    return 0;
  
	intPart = (int64_t)data;
	decPart = data - intPart;
	if (decPart < 0)
	{
	  decPart = 1 - decPart;
	}
	else
	{
	  decPart = 1 + decPart;
	}
  
	if (decPart <= 1 + FLT_EPSILON)
	{
	  decPart = 0;
	}
  
	if (intPart == 0 && data < 0)
	{
	  // deal with numbers between -1 and 0
	  if (length < 4) return 0;   // "-0.n"
	  string[0] = '-';
	  string[1] = '0';
	  intLength = 2;
	}
	else
	{
	  intLength = aware_int_to_str(intPart, string, length);
	  if (intLength == 0) return 0;
	}
	
  decLength = 0;

	if (decPart >= FLT_EPSILON)
	{
	  int i;
	  double noiseFloor;
  
	  if (intLength >= length - 1) 
      return 0;
  
	  i = 0;
	  noiseFloor = FLT_EPSILON;
	  do
	  {
      decPart *= 10;
      noiseFloor *= 10;
      i++;
	  } while (decPart - (int64_t)decPart > noiseFloor);
  
	  decLength = aware_int_to_str(decPart, string + intLength, length - intLength);
	  if (decLength <= 1) 
      return 0;
  
	  // replace the leading 1 with a dot
	  string[intLength] = '.';
	}
  
	return intLength + decLength;
}
 
/*-------------------------------------------------------------------------*/
/**
  @brief	Gets the token from the string based on a delimitter
  @param 	str 	Source input string
  @param 	delim   Delimitter to find in the source string
  @return   The token
 */
/*--------------------------------------------------------------------------*/
char * strtok2(char *str, const char *delim)
{
	static char *nxt; /* static variable used to advance the string to replace delimiters */
	static int size;  /* static variable used to count until the end of the string        */

	/* IMPORTANT: any advance to 'nxt' must be followed by a diminution of 'size', and vice verce */

	int i; /* counter of delimiter(s) in string */

	/* initialize the string when strtok2 is first calles and supplied with a valid string */
	if(str != NULL)
	{ 
		nxt = str;
		size = strlen(str);
	}

	/* if we havn't reached the end of the string, any other call to strtok2 with NULL will come here */
	else if(size > 0)
	{
		nxt++;      /* last run left nxt on a null terminator, so we have to advance it */
		size--;     /* any advancement must follow diminution of size                   */
		str = nxt;  /* string now points to the first char after the last delimiter     */ 
	}

	/* if we reached the end of string, return NULL pointer */
	else
	{
		str = NULL;
	}

	/* nxt is used to advance the string until a delimiter or a series of delimiters are encountered; 
	* it then stops on the last delimiter which has turned to NULL terminator
	*/
	while(*nxt)
	{
		i = strspn(nxt, delim);
		while(i > 1)        /* turns delimiters to NULL terminator (except the last one) */
		{
			*nxt = '\0';
			nxt++;
			size--;
			i--;
		}                   /* in the last delimiter we have to do something a      */
		if(1 == i)          /* bit different we have to actually take nxt backwards */
		{                   /* one step, to break the while(*nxt) loop              */
			*nxt = '\0';
			if(size > 1)    /* if the delimiter is last char, don't do next lines   */
			{
				nxt--;
				size++;     /* nxt is diminished so size increases                    */
			}
		}
		nxt++;          /* if no delimiter is found, advance nxt                  */
		size--;         /* in case of the last delimiter in a series, we took nxt */
	}                   /* a step back, so now moving it a step forward means     */
						/* it rests on a NULL terminator                          */
	return str;
}