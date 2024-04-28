/**
  ******************************************************************************
  * @file    demo_file_system.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of file system operation.
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


 typedef enum{
    SC_FS_DEMO_WRITEFILE         = 1,
    SC_FS_DEMO_FILESEEK         = 2,
    SC_FS_DEMO_READFILE        = 3,
    SC_FS_DEMO_DELETEFILE        = 4,
    SC_FS_DEMO_GETDISKSIZE        = 5,
    SC_FS_DEMO_MAKEDIR        = 6,
    SC_FS_DEMO_REMOVEDIR        = 7,
    SC_FS_DEMO_OPENDIR        = 8,
    SC_FS_DEMO_RXFILE        = 9,
    SC_FS_DEMO_TXFILE        = 10,
    SC_FS_DEMO_MAX          = 99
 }SC_FS_DEMO_TYPE;

 typedef enum{
     SC_DATA_READ,
     SC_DATA_WRITE
 }SC_RX_DATA_OP;

#define FALSE 0
#define TRUE 1

#define BUFF_LEN 100
#define MAX_OUTPUT_LEN 512
#define MAX_VALID_USER_NAME_LENGTH  255-7

#define RECV_LEN 50
#define FLASH_PAGE_SIZE (2048) //2k
#define FLASH_BLOCK_SIZE (FLASH_PAGE_SIZE * 64) //128k
#define READ_FILE_PACKET_SIZE FLASH_BLOCK_SIZE //128k
#define SIM_FILE_OUTPUT_SIZE 512
#define RECEIVE_FLAG_MASK_SYNC (0x01 << 0)
#define RECEIVE_FLAG_MASK_SUCCESS (0x01 << 1)
#define RECEIVE_FLAG_MASK_FAIL (0x01 << 2)
#define RECV_DATA_BUFFER_SIZE (1024 * 20)  // 20k
#define SC_RECV_FILE_TASK_PRIORITY (150)

char cur_path_in[MAX_VALID_USER_NAME_LENGTH] = "C:/";  // default path
static int gfile_len_total = 0;
static BOOL gbFileTransmitting = FALSE;
static char gpReceiveFileDes[MAX_VALID_USER_NAME_LENGTH + 1] = {0};
static UINT8 gFsRecvTaskStack[SC_DEFAULT_THREAD_STACKSIZE];
static sTaskRef gFsHandleTask = NULL;
static UINT8 gFsHandleTaskStack[SC_DEFAULT_THREAD_STACKSIZE];
static char *pRXDataBuffer = NULL;
static char *pDataR = NULL, *pDataW = NULL;
static unsigned int rxDataFreeSize = 0;
static sMutexRef gReceiveDataMutexRef = NULL;
static sFlagRef gReceiveDataPreparedFlagRef = NULL;
static BOOL bFail = FALSE;

extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);
extern SIM_MSG_T GetParamFromUart(void);

/**
  * @brief  File system received data read\write
  * @param  void
  * @note   
  * @retval void
  */
static int sReceiveDataRW(SC_RX_DATA_OP op, char *pData, int len)
{
    char *p = pData;
    int lessLen = len;
    int opLen = len;
    int tailSize = 0;

    sAPI_Debug( "FILE SYSTEM op: %d", op);

    if ((NULL == pRXDataBuffer) || (NULL == pData) || (len <= 0))
    {
        sAPI_Debug("FILE SYSTEM parameter invalid!! pRXDataBuffer: %p, pData: %p, len: %d", pRXDataBuffer,pData, len);
        return -1;
    }

    sAPI_MutexLock(gReceiveDataMutexRef, SC_SUSPEND);
    switch (op)
    {
        case SC_DATA_READ:
        {
            opLen = (len < (RECV_DATA_BUFFER_SIZE - rxDataFreeSize)) ? len :(RECV_DATA_BUFFER_SIZE - rxDataFreeSize);
            lessLen = opLen;
            rxDataFreeSize += opLen;
            sAPI_Debug("FILE SYSTEM test buff free size[%d]",rxDataFreeSize);

            if ((opLen == 0) && (pDataR == pDataW))
            {
                sAPI_Debug("FILE SYSTEM the buffer is empty!! opLen[%d]",opLen);
                opLen = 0;
                break;
            }

            if ((pDataR < pDataW)
                || ((RECV_DATA_BUFFER_SIZE == opLen) && (pDataR == pRXDataBuffer)))
            {
                memcpy(pData, pDataR, opLen);
                if (RECV_DATA_BUFFER_SIZE != opLen)
                {
                    pDataR += opLen;
                }
            }
            else if ((pDataR > pDataW)
                     || ((opLen == RECV_DATA_BUFFER_SIZE) && (pDataR != pRXDataBuffer)))
            {
                tailSize = pRXDataBuffer + RECV_DATA_BUFFER_SIZE - pDataR;
                memcpy(p, pDataR, tailSize);

                p += tailSize;
                lessLen -= tailSize;
                memcpy(p, pRXDataBuffer, lessLen);
                if (RECV_DATA_BUFFER_SIZE != opLen)
                {
                    pDataR = pRXDataBuffer + lessLen;
                }
            }
            else
            {
                opLen = -1;

                sAPI_Debug( "FILE SYSTEM buffer operation err!!");
                break;  // necessary
            }

            if (rxDataFreeSize < RECV_DATA_BUFFER_SIZE)
            {
                opLen += 1;
            }
        }
        break;

        case SC_DATA_WRITE:
        {
            /*free buff size less than this len*/
            if (rxDataFreeSize < len)
            {
                opLen = -1;

                sAPI_Debug( "FILE SYSTEM buffer is full!!");
                break;
            }
            else
            {
                rxDataFreeSize -= len; /*free buff size minus this length*/
            }

            tailSize = pRXDataBuffer + RECV_DATA_BUFFER_SIZE - pDataW;
            if (tailSize >= len)
            {
                memcpy(pDataW, pData, len);

                pDataW += len;
                if ((pRXDataBuffer + RECV_DATA_BUFFER_SIZE) == pDataW)
                {
                    pDataW = pRXDataBuffer;
                }
            }
            else
            {
                memcpy(pDataW, p, tailSize);

                p += tailSize;
                lessLen -= tailSize;
                memcpy(pRXDataBuffer, p, lessLen);
                pDataW = pRXDataBuffer + lessLen;
            }
            sAPI_Free(p);
        }
        break;

        default:
        {
            opLen = -1;
            sAPI_Debug( "FILE SYSTEM invalid operation!!");
        }
        break;
    }

    sAPI_Debug( "FILE SYSTEM cacked data: %d",(RECV_DATA_BUFFER_SIZE - rxDataFreeSize));

    if ((RECV_DATA_BUFFER_SIZE - rxDataFreeSize) > (1024))
    {
        // if buffer has had data more than 1K
        sAPI_Debug( "FILE SYSTEM SET flag!!!!");
        sAPI_FlagSet(gReceiveDataPreparedFlagRef, RECEIVE_FLAG_MASK_SYNC, SC_FLAG_OR);
    }
    sAPI_MutexUnLock(gReceiveDataMutexRef);

    return opLen;
}

/**
  * @brief  File system receiving process
  * @param  void
  * @note   This demo show how to process files sent to module
  * @retval void
  */
void sRecvProcesser(void)
{
    int single_len;
    unsigned int receivedLength = 0;

    int ret = -1;
    char *buff = NULL;
    SIM_MSG_T recv_msg ={0,0,0,NULL};

    PrintfResp("\r\nPlease input full data:\r\n");

    while(1)
    {
        recv_msg = GetParamFromUart();
        buff = (char *)sAPI_Malloc(RECV_LEN);
        memset(buff,0,RECV_LEN);

        memcpy(buff,recv_msg.arg3,recv_msg.arg2);
        sAPI_Free(recv_msg.arg3);

        single_len = recv_msg.arg2;
        receivedLength += single_len;

        sAPI_Debug("FILE SYSTEM single_recv_len[%d],total_recv_len[%d]",single_len,receivedLength);
        //sAPI_Debug("FILE SYSTEM single recv data[%s]",buff);


        if (!bFail)
        {
            ret = sReceiveDataRW(SC_DATA_WRITE, buff, single_len);
            if ((ret < 0) || (ret != single_len) || (receivedLength > gfile_len_total))
            {
                bFail = TRUE;
                sAPI_Debug( "FILE SYSTEM write data fail!!");
            }
        }

        if (receivedLength >= gfile_len_total)
        {
            if (receivedLength > gfile_len_total)
            {
                sAPI_Debug("FILE SYSTEM total_recv_len[%d] > gfile_len_total[%d]",receivedLength,gfile_len_total);
                bFail = TRUE;
            }

            if (!bFail)
            {
                sAPI_FlagSet(gReceiveDataPreparedFlagRef, RECEIVE_FLAG_MASK_SUCCESS, SC_FLAG_OR);
            }
            else
            {
                sAPI_FlagSet(gReceiveDataPreparedFlagRef, RECEIVE_FLAG_MASK_FAIL, SC_FLAG_OR);
            }
            return;
        }
    }
}

/**
  * @brief  File system handle process
  * @param  void
  * @note   This demo show how to process files sent to module
  * @retval void
  */
void sTask_FsHandeProcesser(void)
{
    unsigned int bufferLength = 0;
    unsigned int bufferLengthFree = 0;
    char *dataBuffer = NULL;
    char *p = NULL;
    unsigned int totalWriteLen = 0;
    SCFILE *handle = NULL;
    UINT32 flags = 0;
    BOOL bSuccessAndCleared = TRUE;
    int ret = -1;
    UINT32 actualWriteLen;
    char resp[50] ={0};

    while (1)
    {
        if (sAPI_FlagWait(gReceiveDataPreparedFlagRef,
                        RECEIVE_FLAG_MASK_FAIL | RECEIVE_FLAG_MASK_SYNC | RECEIVE_FLAG_MASK_SUCCESS,
                        SC_FLAG_OR_CLEAR, &flags, SC_SUSPEND) != SC_SUCCESS)
        {
        }
        sAPI_Debug( "FILE SYSTEM FLAG: %#x", flags);

        if (gbFileTransmitting != TRUE)
        {
            continue;
        }

        if (!bFail)
        {
            if (NULL == handle)
            {
                handle = sAPI_fopen(gpReceiveFileDes, "wb");
                if (handle == NULL)
                {
                    bFail = TRUE;
                    sAPI_Debug( "FS creat file fail!!");
                }
            }

            if (NULL == dataBuffer)
            {
                bufferLength = ((gfile_len_total < READ_FILE_PACKET_SIZE) ? gfile_len_total :READ_FILE_PACKET_SIZE);

                if ((dataBuffer = (char *)sAPI_Malloc(bufferLength)) == NULL)
                {
                    bFail = TRUE;
                    sAPI_Debug( "FILE SYSTEM creat buffer fail!!");
                }

                memset(dataBuffer, 0, bufferLength);
                p = dataBuffer;
                bufferLengthFree = bufferLength;
            }

            do
            {
                bSuccessAndCleared = TRUE;
                sAPI_Debug( "FILE SYSTEM test bufferLengthFree[%d]",bufferLengthFree);

                ret = sReceiveDataRW(SC_DATA_READ, p, bufferLengthFree);
                if (ret < 0)
                {
                    bFail = TRUE;
                    sAPI_Debug( "FILE SYSTEM read data err!!");
                }
                else if (ret > bufferLengthFree)
                {
                    // the cache has data not clear
                    ret -= 1;
                    if ((flags & RECEIVE_FLAG_MASK_SUCCESS) == RECEIVE_FLAG_MASK_SUCCESS)
                    {
                        bSuccessAndCleared = FALSE;
                    }
                }

                p += ret;
                bufferLengthFree -= ret;

                if ((ret >= 0)
                    && ((0 == bufferLengthFree)
                        || ((flags & RECEIVE_FLAG_MASK_SUCCESS) && (TRUE == bSuccessAndCleared)
                            && (bufferLengthFree < bufferLength))
                        || ((flags & RECEIVE_FLAG_MASK_FAIL) && (bufferLengthFree < bufferLength))))
                {
                    sAPI_Debug( "FILE SYSTEM write before");
                    actualWriteLen = sAPI_fwrite(dataBuffer, 1,(bufferLength - bufferLengthFree), handle);
                    if (actualWriteLen != (bufferLength - bufferLengthFree))
                    {
                        bFail = TRUE;
                        sAPI_Debug("FILE SYSTEM write file fail!! actual write len: %ld", actualWriteLen);
                    }
                    sAPI_Debug( "FILE SYSTEM write after");

                    totalWriteLen += actualWriteLen;

                    sAPI_Free(dataBuffer);
                    dataBuffer = NULL;

                    bufferLengthFree = bufferLength;
                    sAPI_Debug( "FILE SYSTEM plan_file_total = %d; totalWriteLen = %d.\n",gfile_len_total,totalWriteLen);
                }
            } while (FALSE == bSuccessAndCleared);
        }

        if ((flags & RECEIVE_FLAG_MASK_SUCCESS)|| (flags & RECEIVE_FLAG_MASK_FAIL))
        {
            if (totalWriteLen != gfile_len_total)
            {
                bFail = TRUE;
                sAPI_Debug( "FILE SYSTEM data count err!!");
            }

            if (bFail)
            {
                sAPI_Debug("FILE SYSTEM receive err,plan_file_total = %d; totalWriteLen = %d.\n",gfile_len_total,totalWriteLen);

                if (NULL != handle)
                {
                    sAPI_fclose(handle);
                    handle = NULL;
                    sAPI_remove((const char*)gpReceiveFileDes);
                }

                sprintf(resp,"FILE SYSTEM transfer file fail");
                sAPI_UartWrite(SC_UART,resp,strlen(resp));
            }
            else
            {
                sAPI_Debug("FILE SYSTEM recv succ, plan_file_total = %d; totalWriteLen = %d.\n",gfile_len_total,totalWriteLen);

                sAPI_fclose(handle);
                handle = NULL;

                sprintf(resp,"FILE SYSTEM transfer file succ");
                sAPI_UartWrite(SC_UART,resp,strlen(resp));
            }
            gfile_len_total = 0;
            totalWriteLen = 0;

            sAPI_Free(pRXDataBuffer);
            pRXDataBuffer = NULL;

            pDataR = NULL;
            pDataW = NULL;
            rxDataFreeSize = 0;
            gbFileTransmitting = FALSE;

            sAPI_FlagWait(gReceiveDataPreparedFlagRef,
                        RECEIVE_FLAG_MASK_FAIL | RECEIVE_FLAG_MASK_SYNC | RECEIVE_FLAG_MASK_SUCCESS,
                        SC_FLAG_OR_CLEAR,
                        &flags,
                        SC_NO_SUSPEND);
        }
    }
}

/**
  * @brief  File system demo operation
  * @param  void
  * @note   This demo will show how to operate file system on ASR1603 based module, such as write\read\seek ,etc
  * @retval void
  */
void FsDemo(void)
{
    SCFILE *file_hdl = NULL;
    SCDIR *dir_hdl = NULL;
    char file_name[MAX_VALID_USER_NAME_LENGTH] = {0};
    char dir_name[MAX_VALID_USER_NAME_LENGTH]  = {0};
    char tx_file_name[MAX_VALID_USER_NAME_LENGTH] = {0};
    char del_name[MAX_VALID_USER_NAME_LENGTH] = {0};
    //char new_name[MAX_VALID_USER_NAME_LENGTH]  = {0};

    char operation_path[MAX_VALID_USER_NAME_LENGTH]={0};
    char  pTemBuffer[MAX_OUTPUT_LEN*3];
    UINT32 ret = 0;
    char buff[MAX_OUTPUT_LEN*2] = {0};

    unsigned long buff_data_len = 0;
    UINT32 actul_write_len = 0;
    UINT32 actul_read_len = 0;
    INT64 total_size = 0;
    INT64 free_size = 0;
    INT64 used_size = 0;
    struct dirent *info_dir = NULL;
    struct dirent info_file = {0};
    char *pReadBuffer = NULL;
    int totalReadLen = 0;
    int needReadLen = 0;
    int prepareReadLen = 0;
    SIM_MSG_T optionMsg ={0,0,0,NULL};


    UINT32 opt = 0;
    char *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    char *options_list[] = {
        "1. Write file",
        "2. File seek",
        "3. Read file",
        "4. Delete file",
        "5. Get disk size", 
        "6. Make directory",
        "7. Remove directory",
        "8. List file",
        "9. Transfer file in",
        "10. Transfer file out",
        "99. Back",
    };

    memset(file_name,0,MAX_VALID_USER_NAME_LENGTH);
    memcpy(file_name, "c:/test_file.txt", strlen("c:/test_file.txt"));

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
        sAPI_Debug("opt %d",opt);
        sAPI_Free(optionMsg.arg3);

        switch(opt)
        {
            case SC_FS_DEMO_WRITEFILE:
            {
                file_hdl = sAPI_fopen(file_name, "wb");
                if(file_hdl == NULL)
                {
                    sAPI_Debug("sAPI_FsFileOpen err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file failed because open file failed\r\n");
                    goto err;
                }
                memset(buff,0,MAX_OUTPUT_LEN*2);
                memcpy(buff, "12345678", 8);
                buff_data_len = 8;
                actul_write_len = sAPI_fwrite(buff, buff_data_len,1,file_hdl);
                if(actul_write_len != buff_data_len)
                {
                    sAPI_Debug("sAPI_fwrite err write length: %d\r\n", actul_write_len);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file failed\r\n");
                    goto err;
                }

                ret = sAPI_fclose(file_hdl);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_fclose err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file failed because close file failed\r\nn");
                    goto err;
                }else{
                    file_hdl = NULL;
                }
                sprintf(pTemBuffer, "\r\nFILE SYSTEM Write file successful\r\n\r\nFilename is %s \r\n",file_name);

                ret = sAPI_stat(file_name,&info_file);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_FsStat make err,ERROR code: %d", ret);
                    strcat(pTemBuffer, "\r\nFILE SYSTEM stat fail\r\n");
                    goto err;
                }

                sAPI_Debug("sAPI_FsStat succ [%s]-[%d]-[%d]",info_file.name,info_file.size,info_file.type);
                strcat(pTemBuffer, "\r\nFILE SYSTEM stat SUCC\r\n");

                break;
            }
            case SC_FS_DEMO_FILESEEK:
            {
                buff_data_len = 0;

                file_hdl = sAPI_fopen(file_name, "rb");
                if(file_hdl == NULL)
                {
                    sAPI_Debug("sAPI_fopen err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed because open file failed\r\n");
                    goto err;
                }

                ret = sAPI_fseek(file_hdl, 3, FS_SEEK_BEGIN);
                if(ret != 0)
                {
                     sAPI_Debug("sAPI_FsFileSeek fail! ERROR code: %d",ret);
                     sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed!\r\n");
                     goto err;
                }
                ret = sAPI_ftell(file_hdl);
                if(ret < 0)
                {
                     sAPI_Debug("sAPI_ftell fail! ERROR code: %d",ret);
                     sprintf(pTemBuffer, "\r\nFILE SYSTEM File tell failed!\r\n");
                     goto err;
                }
                sAPI_Debug("sAPI_FsFileTell file position:  %d",ret);

                buff_data_len = sAPI_fsize(file_hdl);
                sAPI_Debug("sAPI_fsize buff_data_len: %d",  buff_data_len);

                memset(buff, 0, MAX_OUTPUT_LEN*2);
                actul_read_len = sAPI_fread(buff, buff_data_len,1,file_hdl);
                if(actul_read_len <= 0)
                {
                    sAPI_Debug("sAPI_fread err,data: %s, len: %d", buff, actul_read_len);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed\r\n");
                    goto err;
                }
                sAPI_Debug("read data: %s, len: %d", buff, actul_read_len);

                ret = sAPI_fclose(file_hdl);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_fclose err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek failed\r\n");
                    goto err;
                }else{
                    file_hdl = NULL;
                }
                sprintf(pTemBuffer, "\r\nFILE SYSTEM File seek successful\r\n\r\nFilename is %s,data read after offset is %s\r\n",file_name,buff);

                break;
            }
            case SC_FS_DEMO_READFILE:
            {
                buff_data_len = 0;
                file_hdl = sAPI_fopen(file_name, "rb");
                if(file_hdl == NULL)
                {
                    sAPI_Debug("sAPI_fopen err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }

                buff_data_len = sAPI_fsize(file_hdl);
                sAPI_Debug("sAPI_fsize buff_data_len: %d",  buff_data_len);

                memset(buff, 0, MAX_OUTPUT_LEN*2);
                actul_read_len = sAPI_fread(buff,buff_data_len, 1, file_hdl);
                if(actul_read_len <= 0)
                {
                    sAPI_Debug("sAPI_FsFileRead err,data: %s, len: %d", buff, actul_read_len);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }

                sAPI_Debug("read data: %s, len: %d", buff, actul_read_len);

                ret = sAPI_fclose(file_hdl);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_fclose err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }else{
                    file_hdl = NULL;
                }

                sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file successful\r\n\r\nFilename is %s,read data is %s\r\n",file_name,buff);


            #if 0
                /*test rename api*/
                memset(new_name,0,MAX_VALID_USER_NAME_LENGTH);
                memcpy(new_name, "c:/newname.txt", strlen("c:/newname.txt"));
                memset(pTemBuffer,0,sizeof(pTemBuffer));

                ret = sAPI_rename(file_name,new_name);
                if(ret != 0){
                    sAPI_Debug("[FS] sAPI_rename err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Rename file failed\r\n");
                    goto err;
                } 
                sAPI_Debug("[FS] sAPI_rename succ");
            #endif

                break;
            }
            case SC_FS_DEMO_DELETEFILE:
            {
                memset(del_name,0,MAX_VALID_USER_NAME_LENGTH);

                PrintfResp("\r\nPlease input path:\r\n");
                optionMsg = GetParamFromUart();
                strcpy(del_name,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);
                
                ret = sAPI_remove(del_name);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_FsFileDelete err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Delete file failed\r\n");
                    goto err;
                }

                sprintf(pTemBuffer, "\r\nFILE SYSTEM Delete file successful\r\n");
                break;
            }
            case SC_FS_DEMO_GETDISKSIZE:
            {
                total_size = sAPI_GetSize(cur_path_in);
                free_size = sAPI_GetFreeSize(cur_path_in);
                used_size = sAPI_GetUsedSize(cur_path_in);

                sAPI_Debug("total_size= %lld, free_size= %lld,used_size = %lld", total_size, free_size,used_size);
                sprintf(pTemBuffer, "\r\nFILE SYSTEM Total_size= %lld, Free_size= %lld,used_size = %lld\r\n",total_size, free_size,used_size);
                break;
            }
            case SC_FS_DEMO_MAKEDIR:
            {
                memset(dir_name,0,MAX_VALID_USER_NAME_LENGTH);
                memcpy(dir_name, "c:/dir1", strlen("c:/dir1"));

                sAPI_Debug("sAPI_FsDirMake:dir[%s]",dir_name);

                ret = sAPI_mkdir(dir_name,0);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_FsDirProcessor make err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Make dir failed\r\n");
                    goto err;
                }

                sprintf(pTemBuffer, "\r\nFILE SYSTEM Make dir SUCC\r\n");

                ret = sAPI_stat(dir_name,&info_file);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_FsStat make err,ERROR code: %d", ret);
                    strcat(pTemBuffer, "\r\nFILE SYSTEM stat fail\r\n");
                    goto err;
                }
                sAPI_Debug("sAPI_FsStat succ [%s]-[%ld]-[%d]",info_file.name,info_file.size,info_file.type);
                strcat(pTemBuffer, "\r\nFILE SYSTEM stat SUCC\r\n");

                break;
            }
            case SC_FS_DEMO_REMOVEDIR:
            {
                memset(dir_name,0,MAX_VALID_USER_NAME_LENGTH);
                memcpy(dir_name, "c:/dir1", strlen("c:/dir1"));

                ret = sAPI_remove(dir_name);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_FsDirRemove del err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM REMOVE dir failed\r\n");
                    goto err;
                }
                sprintf(pTemBuffer, "\r\nFILE SYSTEM REMOVE dir SUCC\r\n");

                break;
            }
            case SC_FS_DEMO_OPENDIR:
            {
                memset(operation_path,0,MAX_VALID_USER_NAME_LENGTH);

                PrintfResp("\r\nPlease input path:\r\n");
                optionMsg = GetParamFromUart();
                strcpy(operation_path,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                sAPI_Debug("sAPI_FsDirProcessor:operation_path[%s]",operation_path);
                dir_hdl = sAPI_opendir(operation_path);
                if(dir_hdl == NULL)
                {
                    sAPI_Debug("sAPI_FsDirOpen OPEN err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Open dir failed\r\n");
                    goto err;
                }
                sAPI_Debug("\r\nFILE SYSTEM Open dir SUCC\r\n");

                while((info_dir = sAPI_readdir(dir_hdl)) != NULL)
                {
                    sprintf(pTemBuffer, "\r\n[name]-[filesize]-[type]\r\n");
                    sprintf(buff, "[%s]-[%ld]-[%d]\r\n",info_dir->name,info_dir->size,info_dir->type);
                    strcat(pTemBuffer,buff);
                    sAPI_UartWrite(SC_UART,pTemBuffer,strlen(pTemBuffer));
                    memset(pTemBuffer,0,sizeof(pTemBuffer));
                }

                ret = sAPI_closedir(dir_hdl);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_FsDirClose make err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Close dir Failed\r\n");
                    goto err;
                }
                sAPI_Debug("sAPI_FsDirClose succ");
                sprintf(pTemBuffer, "\r\nFILE SYSTEM List File END\r\n");

                break;
            }
            case SC_FS_DEMO_RXFILE:
            {
                memset(pTemBuffer,0,sizeof(pTemBuffer));
                memset(gpReceiveFileDes,0,MAX_VALID_USER_NAME_LENGTH);

                PrintfResp("\r\nPlease input full filename:\r\n");
                optionMsg = GetParamFromUart();
                strcpy(gpReceiveFileDes,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                if(strstr(gpReceiveFileDes,"\r\n")){
                    sAPI_Debug("FILE SYSTEM File name should not have a carriage return feed");
                    sprintf(pTemBuffer, "\r\n rx file fail - filename should not have carriage return newlines\r\n");
                    goto err;
                }

                PrintfResp("\r\nPlease input data length:\r\n");
                optionMsg = GetParamFromUart();
                gfile_len_total = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);
                sAPI_enableDUMP();

                free_size = sAPI_GetFreeSize(cur_path_in);
                if(gfile_len_total >= free_size){
                    sAPI_Debug("FILE SYSTEM  free size[%lld] is not enough for filesize[%d]",free_size,gfile_len_total);
                    sprintf(pTemBuffer, "\r\n rx file size is too large\r\n");
                    goto err;
                }

                if(gfile_len_total < 1){
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM file size should not be zero\r\n");
                    goto err;
                }
                if ((pRXDataBuffer = (char *)sAPI_Malloc(RECV_DATA_BUFFER_SIZE)) == NULL)
                {
                    sAPI_Debug("FILE SYSTEM malloc fail!!");
                    sprintf(pTemBuffer, "\r\n rx file fail -malloc fail\r\n");
                    goto err;
                }
                sAPI_Debug("FILE SYSTEM malloc pRXDataBuffer[%p]",pRXDataBuffer);

                pDataR = pRXDataBuffer;
                pDataW = pRXDataBuffer;
                rxDataFreeSize = RECV_DATA_BUFFER_SIZE;
                gbFileTransmitting = TRUE;
                bFail = FALSE;

                if (NULL == gReceiveDataMutexRef){
                    if ((sAPI_MutexCreate(&gReceiveDataMutexRef, SC_FIFO)) != SC_SUCCESS)
                    {
                        sprintf(pTemBuffer, "\r\n rx file fail -sAPI_MutexCreate fail\r\n");
                        goto err;
                    }
                }

                if (NULL == gReceiveDataPreparedFlagRef)
                {
                    if (sAPI_FlagCreate(&gReceiveDataPreparedFlagRef) != SC_SUCCESS){
                        sAPI_MutexDelete(gReceiveDataMutexRef);
                        sprintf(pTemBuffer, "\r\n rx file fail -sAPI_FlagCreate1 fail\r\n");
                        goto err;
                    }
                }

                if (NULL == gFsHandleTask)
                {
                    if(sAPI_TaskCreate(&gFsHandleTask, gFsHandleTaskStack, SC_DEFAULT_THREAD_STACKSIZE, SC_RECV_FILE_TASK_PRIORITY, (char*)"fsHandleProcesser", sTask_FsHandeProcesser,(void *)0)) //init the demo task
                    {
                        sAPI_MutexDelete(gReceiveDataMutexRef);
                        sAPI_FlagDelete(gReceiveDataPreparedFlagRef); 
                        sprintf(pTemBuffer, "\r\n rx file fail -sAPI_TaskCreate2 fail\r\n");
                        goto err;
                    }
                }
                sAPI_Debug("FILE SYSTEM Successful preparation!!");

                sRecvProcesser();

                break;
            }
            case SC_FS_DEMO_TXFILE:
            {
                memset(tx_file_name,0,MAX_VALID_USER_NAME_LENGTH);

                PrintfResp("\r\nPlease input full filename:\r\n");
                optionMsg = GetParamFromUart();
                strcpy(tx_file_name,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                memset(pTemBuffer,0,sizeof(pTemBuffer));
                ret = sAPI_stat(tx_file_name,&info_file);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_FsStat make err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM file not exist\r\n");
                    goto err;
                }
                needReadLen = info_file.size;

                file_hdl = sAPI_fopen(tx_file_name, "rb");
                if (NULL == file_hdl)
                {
                    sAPI_Debug("sAPI_fopen make err,ERROR code: %d", ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM open file fail\r\n");
                    goto err;

                }
                totalReadLen = 0;
                while(1)
                {
                    if ((needReadLen - totalReadLen) < SIM_FILE_OUTPUT_SIZE)
                    {
                        prepareReadLen = needReadLen - totalReadLen;
                    }
                    else
                    {
                        prepareReadLen = SIM_FILE_OUTPUT_SIZE;
                    }

                    if (NULL == pReadBuffer){
                        pReadBuffer = (char *)sAPI_Malloc(SIM_FILE_OUTPUT_SIZE+1);
                        if (NULL == pReadBuffer)
                        {
                            sAPI_Debug("sAPI_Malloc err,ERROR code: %d", ret);
                            sprintf(pTemBuffer, "\r\nFILE SYSTEM MALLOC fail\r\n");
                            goto err;
                        }
                    }
                    else{
                        sAPI_Debug("pReadBuffer is not null");
                    }
    
                    memset(pReadBuffer, 0, sizeof(pReadBuffer));
                    actul_read_len = sAPI_fread(pReadBuffer, 1, prepareReadLen, file_hdl);
                    totalReadLen += actul_read_len;

                    memset(pTemBuffer,0,sizeof(pTemBuffer));
                    memset(buff, 0, sizeof(buff));
                    if (actul_read_len > 0)
                    {
                        sprintf(pTemBuffer, "read file size: %d\r\n", actul_read_len);
                        sprintf(buff, "read file data:%s\r\n",pReadBuffer);
                        strcat(pTemBuffer,buff);
                        sAPI_UartWrite(SC_UART,pTemBuffer,strlen(pTemBuffer));
                        sAPI_Free(pReadBuffer);
                        pReadBuffer = NULL;

                        if (totalReadLen >= needReadLen)
                        {
                            sprintf(pTemBuffer, "read file END\r\n");
                            sAPI_UartWrite(SC_UART,pTemBuffer,strlen(pTemBuffer));
                            memset(pTemBuffer,0,sizeof(pTemBuffer));
                            break;
                        }
                    }
                    else
                    {
                        sAPI_Debug("sAPI_fread err,actual read: %d", actul_read_len);
                        sprintf(pTemBuffer, "\r\nFILE SYSTEM READ fail\r\n");
                        sAPI_Free(pReadBuffer);
                        pReadBuffer = NULL;
                        goto err;
                    }
                }

                sAPI_fclose(file_hdl);
                if(ret != 0)
                {
                    sAPI_Debug("sAPI_fclose err,err_code[%d]",ret);
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM close file failed\r\n");
                    goto err;
                }else{
                    file_hdl = NULL;
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
            default:
            {
                memset(pTemBuffer,0,sizeof(pTemBuffer));
                break;
            }
        }

err:
        if(file_hdl != NULL)
        {
            ret = sAPI_fclose(file_hdl);
            if(ret != 0)
            {
                sAPI_Debug("sAPI_FsFileClose err");
                sprintf(pTemBuffer, "\r\nFILE SYSTEM close file failed\r\n");
            }else{
                file_hdl = NULL;
            }
        }

        if(strlen(pTemBuffer) > 0)
        {
            sAPI_Debug("sAPI_Fs test_result [%s]", pTemBuffer);
            sAPI_UartWrite(SC_UART,pTemBuffer,strlen(pTemBuffer));
        }
    }

}

