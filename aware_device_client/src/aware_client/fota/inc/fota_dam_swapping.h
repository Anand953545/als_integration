#ifndef _FOTA_DAM_SWAPPING_H_
#define _FOTA_DAM_SWAPPING_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "app_types.h"
#include "qapi_types.h"

int swap_dam_file(void);
int setup_dam_ota_update_dir(void);

#endif