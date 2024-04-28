/**
  ******************************************************************************
  * @file    demo_file_system_old.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of file system operation,old style.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/
#include "simcom_api.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#ifdef FEATURE_SIMCOM_FS_OLD
 typedef enum{
    SC_FS_DEMO_WRITEFILE         = 1,
    SC_FS_DEMO_FILESEEK         = 2,
    SC_FS_DEMO_READFILE        = 3,
    SC_FS_DEMO_DELETEFILE        = 4,
    SC_FS_DEMO_GETDISKSIZE        = 5,
    SC_FS_DEMO_MAKEDIR        = 6,
    SC_FS_DEMO_REMOVEDIR        = 7,
    SC_FS_DEMO_OPENDIR        = 8,
    SC_FS_DEMO_MAX          = 99
 }SC_FS_DEMO_TYPE;

#define BUFF_LEN 100
#define MAX_OUTPUT_LEN 512
#define MAX_VALID_USER_NAME_LENGTH  255-7
static char cur_path[MAX_VALID_USER_NAME_LENGTH] = "C:/";  // default is flash

extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);
extern SIM_MSG_T GetParamFromUart(void);

void Fs2Demo(void)
{
    SCfileHandle file_hdl = {0};
    char pTemBuffer[MAX_OUTPUT_LEN*5];
    char *file_list = NULL;
    char *dir_list = NULL;
    UINT32 ret = 0;
    char buff[BUFF_LEN] = {0};
    unsigned long buff_data_len = 0;
    UINT32 actul_write_len = 0;
    UINT32 actul_read_len = 0;
    INT64 total_size = 0;
    INT64 free_size = 0;
    SCfileNameInfo file_name = {0};
    SCfileNameInfo dir_name = {0};
    SCfileNameInfo copy_name = {0};

    char dir[MAX_VALID_USER_NAME_LENGTH]  = {0};
    char file[MAX_VALID_USER_NAME_LENGTH]  = {0};
    //char copy[MAX_VALID_USER_NAME_LENGTH]  = {0};

    //strncpy(file_name.path, "c:/", strlen("c:/"));
    //strncpy(file_name.name, "test_file.txt", strlen("test_file.txt"));
    //strncpy(dir_name.path, "c:/", strlen("c:/"));
    //strncpy(dir_name.name, "dir1", strlen("dir1"));

    strncpy(dir, "c:/dir1", strlen("c:/dir1"));
    strncpy(file, "c:/test_file.txt", strlen("c:/test_file.txt"));

    SIM_MSG_T optionMsg ={0,0,0,NULL};
    UINT32 opt = 0;
    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] = {
        "1. Write file",
        "2. File seek",
        "3. Read file",
        "4. Delete file",
        "5. Get disk size",
        "6. Make directory",
        "7. Remove directory",
        "8. List file",
        "99. Back",
    };

    while(1)
    {
        PrintfResp(note);
        PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("%s,msg_id is error!!",__func__);
            break;
        }

        sAPI_Debug("arg3 = [%s]",optionMsg.arg3);
        opt = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);

        sAPI_FsNameSeparate(&file_name,file);
        sAPI_FsNameSeparate(&dir_name,dir);

        switch(opt)
        {
            case SC_FS_DEMO_WRITEFILE:
            {
                sAPI_FsGetDiskSize(file_name.path, &total_size, &free_size);
                sAPI_Debug("total_size= %lld, free_size= %lld", total_size, free_size);
                sAPI_Debug("fullname[%s%s]",file_name.path,file_name.name);

                ret = sAPI_FsIsFileNameValid(&file_name);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsIsFileNameValid err");
                    sprintf(pTemBuffer, "\r\nWrite file failed\r\n");
                    goto err;
                }

                file_hdl = sAPI_FsFileOpen(&file_name, "wb");
                if(file_hdl.loc == SC_LOC_INVALID)
                {
                    sAPI_Debug("sAPI_FsFileOpen err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file failed because open file fail\r\n");
                    goto err;
                }
                memcpy(buff, "12345678", 8);
                buff_data_len = 8;
                actul_write_len = sAPI_FsFileWrite(buff, buff_data_len, file_hdl);
                if(actul_write_len != buff_data_len)
                {
                    sAPI_Debug("sAPI_FsFileWrite errwrite length: %d\r\n", actul_write_len);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file failed\r\n");
                    goto err;
                }

                ret = sAPI_FsFileClose(file_hdl);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsFileClose err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file failed\r\nn");
                    goto err;
                }
                else
                {
                    file_hdl.loc = SC_LOC_INVALID; /*Note: Avoid closing files repeatedly */
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file successful\r\n\r\nFilename is %s \r\n",file);
                }
                break;
            }
            case SC_FS_DEMO_FILESEEK:
            {
                buff_data_len = 0;
                ret = sAPI_FsFindFileAndGetSize(&file_name, &buff_data_len);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsFindFileAndGetSize fail ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed\r\n");
                    goto err;
                }
                file_hdl = sAPI_FsFileOpen(&file_name, "rb");
                if(file_hdl.loc == SC_LOC_INVALID)
                {
                    sAPI_Debug("sAPI_FsFileOpen err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed\r\n");
                    goto err;
                }

                ret = sAPI_FsFileSeek(file_hdl, 3, FS_SEEK_BEGIN);
                if(ret != SC_FS_OK)
                {
                     sAPI_Debug("sAPI_FsFileSeek fail! ERROR code: %d",ret);
                     sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed!\r\n");
                     goto err;
                }

                ret = sAPI_FsFileTell(file_hdl);
                if(ret < 0)
                {
                     sAPI_Debug("sAPI_FsFileTell fail! ERROR code: %d",ret);
                     sprintf(pTemBuffer, "\r\nFILE SYSTEM File tell failed!\r\n");
                     goto err;
                }
                sAPI_Debug("sAPI_FsFileTell file position:  %d",ret);

                memset(buff, 0, BUFF_LEN);
                actul_read_len = sAPI_FsFileRead(buff, buff_data_len, file_hdl);
                if(actul_read_len == 0)
                {
                    sAPI_Debug("sAPI_FsFileRead err,data: %s, len: %d", buff, actul_read_len);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed\r\n");
                    goto err;
                }
                else
                {
                    sAPI_Debug("read data: %s, len: %d", buff, actul_read_len);
                }
                ret = sAPI_FsFileClose(file_hdl);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsFileClose err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed\r\n");
                    goto err;
                }
                else
                {
                    file_hdl.loc = SC_LOC_INVALID;
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek successful\r\n\r\nFilename is %s,data read after offset is %s\r\n",file,buff);
                }
                break;
            }
            case SC_FS_DEMO_READFILE:
            {
                buff_data_len = 0;
                ret = sAPI_FsFindFileAndGetSize(&file_name, &buff_data_len);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsFindFileAndGetSize fail ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }
                file_hdl = sAPI_FsFileOpen(&file_name, "rb");
                if(file_hdl.loc == SC_LOC_INVALID)
                {
                    sAPI_Debug("sAPI_FsFileOpen err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }
                memset(buff, 0, BUFF_LEN);
                actul_read_len = sAPI_FsFileRead(buff, buff_data_len, file_hdl);
                if(actul_read_len == 0)
                {
                    sAPI_Debug("sAPI_FsFileRead err,data: %s, len: %d", buff, actul_read_len);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }
                else
                {
                    sAPI_Debug("read data: %s, len: %d", buff, actul_read_len);
                }
                ret = sAPI_FsFileClose(file_hdl);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsFileClose err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }
                else
                {
                    file_hdl.loc = SC_LOC_INVALID;
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file successful\r\n\r\nFilename is %s,read data is %s\r\n",file,buff);
                }

                /*test rename api*/
#if 0
                strncpy(copy, "c:/copy.txt", strlen("c:/copy.txt"));
                sAPI_FsNameSeparate(&copy_name,copy);

                ret = sAPI_FsFileRename(&file_name,&copy_name);
                if(ret != SC_FS_OK){
                    sAPI_Debug("[FS] sAPI_FsFileRename err,ERROR code: %d", ret);
                    memset(pTemBuffer,0,sizeof(pTemBuffer));
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Rename file failed\r\n");
                    goto err;
                }
                strcat(pTemBuffer, "\r\nFILE SYSTEM Rename file succ\r\n");
#endif
                break;
            }

            case SC_FS_DEMO_DELETEFILE:
            {
                ret = sAPI_FsFileDelete(&file_name);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsFileDelete err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Delete file failed\r\n");
                    goto err;
                }
                else
                {
                     sAPI_Debug("sAPI_FsFileDelete successed");
                     sprintf(pTemBuffer, "\r\nFILE SYSTEM Delete file successful\r\n");
                }

                /*test api:sAPI_FsFileSave*/
                //sAPI_FsFileSave(1,"c:/save1.txt","55555555",8);
                break;
            }
            case SC_FS_DEMO_GETDISKSIZE:
            {
                sAPI_FsGetDiskSize("c:/", &total_size, &free_size);
                sAPI_Debug("total_size= %lld, free_size= %lld", total_size, free_size);
                sprintf(pTemBuffer, "\r\nFILE SYSTEM Total_size= %lld, Free_size= %lld\r\n",total_size, free_size);

                break;
            }

            case SC_FS_DEMO_MAKEDIR:
            {
                sAPI_Debug("sAPI_FsDirMake:dir[%s%s]",dir_name.path,dir_name.name);

                ret = sAPI_FsDirProcessor(0,&dir_name);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsDirProcessor make err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Make dir failed\r\n");
                    goto err;
                }

                sprintf(pTemBuffer, "\r\nFILE SYSTEM Make dir SUCC\r\n");

                break;
            }
            case SC_FS_DEMO_REMOVEDIR:
            {
                ret = sAPI_FsIsPathExist(dir);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsIsPathExist err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM dir[%s] not exist\r\n",dir);
                    goto err;
                }
                sAPI_Debug("sAPI_FsIsPathExist dir[%s] exists",dir);

                ret = sAPI_FsDirProcessor(1,&dir_name);
                if(ret != SC_FS_OK)
                {
                    sAPI_Debug("sAPI_FsDirProcessor make err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM remove dir failed\r\n");
                    goto err;
                }

                sprintf(pTemBuffer, "\r\nFILE SYSTEM remove dir SUCC\r\n");

                break;
            }

            case SC_FS_DEMO_OPENDIR:
            {
                file_list = (char *)sAPI_Malloc(MAX_VALID_USER_NAME_LENGTH*2);
                dir_list = (char *)sAPI_Malloc(MAX_VALID_USER_NAME_LENGTH*2);
                if ((NULL == file_list)||(NULL == dir_list)) {
                    sAPI_Debug("[FS] list err");
                    goto err;
                }

                memset(file_list, 0, MAX_VALID_USER_NAME_LENGTH*2);
                memset(dir_list, 0, MAX_VALID_USER_NAME_LENGTH*2);
                memset(pTemBuffer,0,sizeof(pTemBuffer));

                ret = sAPI_FsFileTraverse(cur_path,file_list,dir_list);
                if(ret != SC_FS_OK){
                    sAPI_Debug("[FS] sAPI_FsFileList err��ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM List file failed\r\n");

                    sAPI_Free(file_list);
                    sAPI_Free(dir_list);
                    goto err;
                }
                else{
                    sAPI_Debug("[FS] sAPI_FsFileList successed");
                    sAPI_Debug("[FS]  dir_list_len: %d  file_list_len %d", strlen(dir_list),strlen(file_list));

                    sprintf(pTemBuffer, "\r\nFILE SYSTEM List file successful\r\n");
                    if(strlen(dir_list) > 0){
                        strcat(pTemBuffer, "+FSLS: SUBDIRECTORIES:\r\n");
                        strcat(pTemBuffer, dir_list);
                    }
                    else{
                        strcat(pTemBuffer, "+FSLS: SUBDIRECTORIES:No subdirectories\r\n");
                    }

                    if(strlen(file_list) > 0){
                        strcat(pTemBuffer, "+FSLS: FILES:\r\n");
                        strcat(pTemBuffer, file_list);
                    }
                    else{
                        strcat(pTemBuffer, "+FSLS: FILES:No files\r\n");
                    }

                    sAPI_Free(file_list);
                    sAPI_Free(dir_list);
                }

                break;
            }
            case SC_FS_DEMO_MAX:
            {
                sAPI_Debug("Return to the previous menu!");
                PrintfResp("\r\nReturn to the previous menu!\r\n");
                memset(pTemBuffer,0,sizeof(pTemBuffer));

                return;

            }
            default :
                break;
        }

err:
        if(file_hdl.loc != SC_LOC_INVALID)
        {
            ret = sAPI_FsFileClose(file_hdl);
            if(ret != SC_FS_OK)
            {
                sAPI_Debug("sAPI_FsFileClose fail ERROR code: %d",ret);
                sprintf(pTemBuffer, "\r\nFILE SYSTEM Close File failed\r\n");
            }
            else
            {
                 file_hdl.loc = SC_LOC_INVALID;
            }
        }

        sAPI_Debug("sAPI_Fs test_result [%s]", pTemBuffer);
        sAPI_UartWrite(SC_UART,pTemBuffer,strlen(pTemBuffer));
        memset(pTemBuffer,0,sizeof(pTemBuffer));
    }

}

#endif
