/**
  ******************************************************************************
  * @file    simcom_file_system.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of simcom file system.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef SIMCOM_FILE_SYSTEM_H
#define SIMCOM_FILE_SYSTEM_H

#include "simcom_os.h"


#include "lfs.h"
#include "lfs_api.h"
#include "lfs_cache.h"


/****************************************************************************************************************************************************

General rules for naming (both directories and files):

    * The name is a ascii string.
    
    * The length of actual fully qualified names of files(C:/) can not exceed 255.
    
    * Directory and file names can not include the following characters:    \  :  *  ?  “  <  >  |  ,  ;
    
    * Between directory name and file/directory name, use character “/” as list separator, so it can not appear in directory name or file name.

****************************************************************************************************************************************************/

/*Seek from start of file*/
#define FS_SEEK_BEGIN	0

/*Seek from current position*/
#define FS_SEEK_CURREN	1

/*Seek from end of file*/
#define FS_SEEK_END		2


typedef struct dirent
{
    unsigned char type;
    unsigned long size;
    char name[255];
}dirent_t;

typedef struct
{
    void *disk;
    lfs_file_t file;
} SCFILE;

typedef struct
{
    void *disk;
    lfs_dir_t dir;
    struct lfs_info info;
} SCDIR;

typedef enum{
    TYPE_REG        = 0,/*regular file*/
    TYPE_DIR        = 1,/*sub_directory*/
}file_type;

#ifdef FEATURE_SIMCOM_FS_OLD
typedef unsigned short FILE_ID;


typedef enum{
    SC_LOC_INVALID = 0,
    SC_LOC_FLASH,
    SC_LOC_RELATED
}SCfileLocation;

typedef struct {
    char path[255];
    char name[255];
}SCfileNameInfo;

typedef struct {
    FILE_ID flash;
    SCfileLocation loc;
}SCfileHandle;

typedef enum
{
    SC_FS_ERROR = 0,
    SC_FS_OK,
    SC_FS_FILE_PATH_INVALID,
    SC_FS_FILE_PATH_HEAD_INVALID,
    SC_FS_FILE_NAME_INVALID,
    SC_FS_PARAMETER_EMPTY, // 5
    SC_FS_PARAMETER_INVALID,
    SC_FS_NOT_SUPPORT_NON_ASCII,
    SC_FS_MAX
} SC_FS_ERR_CODE;

typedef SC_FS_ERR_CODE SCfsReturnCode;

#endif


#endif


