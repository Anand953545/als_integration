#include "restore_backup_dam.h"
#include "stringl.h"
#include "log.h"

#include "qapi_fs.h"
#include "aware_utils.h"
#include "qapi_device_info.h"

#define OLD_BACKUP_FILE_PATH "/datatx/aware_client_v1.bin"
#define NEW_BACKUP_FILE_PATH "/datatx/backup_image/aware_client_v1.bin"


int reset_device_info();

static qapi_Device_Info_Hndl_t device_info_hndl;

int restore_backup_dam(void)
{
  qapi_Status_t status = QAPI_OK;

  LOG_INFO("[fota_client]: backup DAM file copied to data DAM file");
  status = qapi_FS_Rename(NEW_BACKUP_FILE_PATH, OLD_BACKUP_FILE_PATH);
  if(status != QAPI_OK)
      return status;
          
  LOG_INFO("[fota_client]: reset device");
  status = reset_device_info();
  if(status != QAPI_OK)
    return status;

  return status;
}

int reset_device_info()
{
  qapi_Status_t res = QAPI_OK;

  if (NULL == device_info_hndl)
  {
    res = qapi_Device_Info_Init_v2(&device_info_hndl);
    if (QAPI_OK != res)
    {
      LOG_ERROR("[fota_client]: qapi_Device_Info_Init_v2 failed");
      return res;
    }
  }

  res = qapi_Device_Info_Reset_v2(device_info_hndl);
  if (QAPI_OK != res)
  {
    LOG_ERROR("[fota_client]: qapi_Device_Info_Reset_v2 failed");
  }

  if(device_info_hndl)
      qapi_Device_Info_Release_v2(device_info_hndl);

  return res;
}