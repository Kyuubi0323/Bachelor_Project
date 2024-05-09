/**
  ******************************************************************************
  * @file    demo_fota.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of fota function,  this is used to update core FW.
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
#include "simcom_fota.h"
#include "stdio.h"
#include "simcom_file_system.h"
#include "simcom_uart.h"
extern SIM_MSG_T GetParamFromUart(void);
extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);


enum SC_FOTA_SWITCH
{
    SC_FTP_FOTA = 1,
    SC_HTTP_FOTA,
    SC_LOCAL_FOTA,
    SC_FOTA_BACK = 99
};

/**
  * @brief  Fota operation callback to indicated the downloading finishing percentage.
  * @param  void
  * @note   isok will be used to indicate the download result.
  * @retval void
  */
int fotacb(int isok)
{
    if(isok == 100)
    {
        PrintfResp("fota download is successful, please reset to update.\r\n");
    }
    else
    {
        PrintfResp("fota download failed, try again\r\n");
    }
    return 0;

}

/**
  * @brief  Fota demo
  * @param  void
  * @note   suppor HTTP and FTP\local update. For local update need to send AT+FSPRESET=system_patch.bin to change director
  * @retval void
  */
void FotaDemo(void)
{
    SIM_MSG_T optionMsg ={0,0,0,NULL};
    UINT32 opt = 0;
    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] = {
        "1. FTP fota",
        "2. HTTP fota",
        "3. LOCAL fota",
        "99. back",
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

        switch(opt)
        {
            case SC_FTP_FOTA:

            {

                    struct SC_FotaApiParam param = {0};
                    strcpy(param.host , "183.230.174.137:6081/fota_pakage/system_patch.bin");
                    strcpy(param.username , "guocheng.lu");
                    strcpy(param.password , "123456");
                    param.mode = 0;
                    param.sc_fota_cb = fotacb;
                    int ret = sAPI_FotaServiceBegin((void *)&param);
                    if(ret == -1)
                        PrintfResp("\r\nparam error\r\n");
                    else if(ret == -2)
                        PrintfResp("\r\nmalloc mem error\r\n");
                    else if(ret == -3)
                        PrintfResp("\r\nnet error\r\n");
                    break;
            }

            case SC_HTTP_FOTA:
            {

                struct SC_FotaApiParam param = {0};
                strcpy(param.host , "47.108.134.22/system_patch.bin");
                strcpy(param.username , "111");
                strcpy(param.password , "222");
                param.mode = 1;
                param.sc_fota_cb = fotacb;
                int ret = sAPI_FotaServiceBegin((void *)&param);
                if(ret == -1)
                    PrintfResp("\r\nparam error\r\n");
                else if(ret == -2)
                    PrintfResp("\r\nmalloc mem error\r\n");
                else if(ret == -3)
                    PrintfResp("\r\nnet error\r\n");
                break;
            }

            case SC_LOCAL_FOTA:
            {
                SCFILE *file_hdl = NULL;
                char file_name[260] = {0};
                char  pTemBuffer[512*5];
                UINT32 ret = 0;
                char buff[1024] = {0};
                unsigned long buff_data_len = 0;
                UINT32 actul_read_len = 0;
                int receive_len = 0;
                memset(file_name,0,260);
                memcpy(file_name, "c:/system_patch.bin", strlen("c:/system_patch.bin"));

                /* send: AT+FSPRESET=system_patch.bin change directory */
                PrintfResp("\r\nAT command send: AT+FSPRESET=system_patch.bin\r\n");
                char respStr[20];
                ret = sAPI_SendATCMDWaitResp(10, "AT+FSPRESET=system_patch.bin\r", 150, "OK", 0, "ERROR", respStr, sizeof(respStr));
                PrintfResp(respStr);
                PrintfResp("\r\n");
                if(ret) break;
                /* send: AT+FSPRESET=system_patch.bin change directory */

                file_hdl = sAPI_fopen(file_name, "rb");
                if(file_hdl == NULL)
                {
                    sAPI_Debug("sAPI_fopen err");
                    sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                    goto err;
                }

                buff_data_len = sAPI_fsize(file_hdl);
                sAPI_Debug("sAPI_fsize buff_data_len: %d",  buff_data_len);

                while(receive_len < buff_data_len)
                {
                    memset(buff, 0, 1024);
                    actul_read_len = sAPI_fread(buff,1024, 1, file_hdl);
                    if(actul_read_len <= 0)
                    {
                        sAPI_Debug("sAPI_FsFileRead err,data: %s, len: %d", buff, actul_read_len);
                        sprintf(pTemBuffer, "\r\nFILE SYSTEM Read file failed\r\n");
                        goto err;
                    }
                    sAPI_Debug("read data: [%s], len: %d", buff, actul_read_len);

                   int ret = sAPI_FotaImageWrite(buff, 1024,buff_data_len);

                   receive_len += actul_read_len;
                   sAPI_TaskSleep(10);
                }
                if(ret == 0)
                {
                    if(sAPI_FotaImageVerify(buff_data_len) != 0)
                    goto err;
                }
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

                break;
            }

            case SC_FOTA_BACK:

                    return;

        }

    }
}

