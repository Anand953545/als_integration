#include "fota_dam_swapping.h"
#include "stringl.h"
#include "aware_log.h"

#include "qapi_fs.h"
#include "app_utils_misc.h"
#include "aware_utils.h"
#include "app_context.h"
#include "aware_app.h"
#include "data_mgr_device_info.h"
#include "aware_app_state.h"

#define OLD_FOTA_FILE_PATH  "/datatx/fota_image/aware_dam_app_v2.bin"
#define NEW_FOTA_FILE_PATH  "/datatx/aware_client_v1.bin"

#define OLD_BACKUP_FILE_PATH "/datatx/aware_client_v1.bin"
#define NEW_BACKUP_FILE_PATH "/datatx/backup_image/aware_client_v1.bin"

#define FOTA_DIR_PATH      "/datatx/fota_image"
#define BACKUP_DIR_PATH    "/datatx/backup_image"

#define S_IFDIR  0040000 /**< Directory */
#define S_IFMT   0170000
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)

int is_dir_exists(const char *path);

/*-------------------------------------------------------------------------*/
/**
  @brief Checks whether the directory is present or not
  @param path path of the file present
  @return 0 on SUCCESS or 1 on FAILURE
 */
/*--------------------------------------------------------------------------*/
int is_dir_exists(const char *path)
{
    int result, ret_val = 0;
    struct qapi_FS_Stat_Type_s sbuf;

    memset (&sbuf, 0, sizeof (sbuf));

    result = qapi_FS_Stat(path, &sbuf);

    if (result != 0)
      goto end;

    if (S_ISDIR (sbuf.st_Mode)==1)
      ret_val = 1;

end:
      return ret_val;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Setup dam ota to update directory
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int setup_dam_ota_update_dir(void)
{
    int ret=0;
    int status=QAPI_OK;

    ret = is_dir_exists(FOTA_DIR_PATH);
    if(ret == 0)
    {
        status = qapi_FS_Mk_Dir(FOTA_DIR_PATH, QAPI_FS_S_IRUSR_E|QAPI_FS_S_IWUSR_E|QAPI_FS_S_IXUSR_E);
        // // if(status!=QAPI_OK)
        // // {
        // //     LOG_INFO("[Fota Thread]: fota dir not created");        
        // // }
        // else{
        //     LOG_INFO("[Fota Thread]: fota dir created");
        // }
    } else {
        LOG_INFO("[Fota Thread]: fota dir exist"); 
    }
    
    ret = is_dir_exists(BACKUP_DIR_PATH);
    if(ret == 0)
    {
        status=qapi_FS_Mk_Dir(BACKUP_DIR_PATH,QAPI_FS_S_IRUSR_E|QAPI_FS_S_IWUSR_E|QAPI_FS_S_IXUSR_E);
        // if(status!=QAPI_OK)
        // {
        //     LOG_INFO("[Fota Thread]: backup dir not created");        
        // }
        // else{
        //     LOG_INFO("[Fota Thread]: backup dir created");
        // }
    } else {
        LOG_INFO("[Fota Thread]: backup dir exist");
    }
    
    status = qapi_FS_Unlink(NEW_BACKUP_FILE_PATH);
    if(status==QAPI_OK)
    {
        LOG_INFO("[Fota Thread]: Backup file remove");
    }

    status = qapi_FS_Unlink(OLD_FOTA_FILE_PATH);
    if(status!=QAPI_OK)
    {
        LOG_INFO("[Fota Thread]: Fota file remove");
    }

    return status;
}

/*-------------------------------------------------------------------------*/
/**
  @brief Swap dam files
  @return SUCCESS or FAILURE
 */
/*--------------------------------------------------------------------------*/
int swap_dam_file(void)
{
    qapi_Status_t status = QAPI_OK;
    set_fota_state(FOTA_APPLYING);
    
    status = qapi_FS_Rename(OLD_BACKUP_FILE_PATH,NEW_BACKUP_FILE_PATH);
    if(status!=QAPI_OK)
        return status;

    LOG_INFO("[Fota Thread]: Aware existing DAM backed up in /moduleid/datatx/backup directory");        
    
    status = qapi_FS_Rename(OLD_FOTA_FILE_PATH,NEW_FOTA_FILE_PATH);
    if(status!=QAPI_OK)
        return status;

    LOG_INFO("[Fota Thread]: Aware new DAM copied to /datatx directory");
    
    LOG_INFO("[Fota Thread]: Resetting the device to apply the changes");

    status = data_mgr_reset_device_info();
    if(status != QAPI_OK)
        return status;

    return status;
}