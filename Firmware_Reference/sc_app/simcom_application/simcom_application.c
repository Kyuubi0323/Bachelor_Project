/**
  ******************************************************************************
  * @file    simcom_application.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source code for SIMCom OpenSDK application,static void Application(void * argv) is the app entry for OpenSDK application,customer should start application from this call.
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


typedef void (*app_t)(void * argv);
typedef struct
{
	app_t app_entry;

} appRegItem_t;
#define _appRegTable_attr_ __attribute__((unused, section(".appRegTable")))

/**
  * @brief  OpenSDK app entry.
  * @param  Pointer argv
  * @note   This is the app entry,like main(),all functions must start from here!!!!!!
  * @retval void
  */
static void Application(void * argv)
{

    /* Do not comment the initialization code below */
    unsigned long *apiTable = (unsigned long*)argv;
    if(apiTable != NULL)
    {
         get_sAPI(apiTable);
         sAPI_Debug("sc_Init successfull...");

    }
    /*End of initialization*/

    //sAPI_enableDUMP();
    sAPI_TaskSleep(5 * 200);
    sAPI_Debug("sAPI_Malloc:%p",sAPI_Malloc);
    sAPI_Debug("sAPI_Debug is OK...");
    sAPI_Debug("helloworld is finish...");
#ifdef SIMCOM_A7620_GREE
    sAPP_WatchDogTaskInit();
#endif
#ifdef SIMCOM_UI_DEMO
    sAPP_SimcomUIDemo();		//UI demo task for customer with CLI method for all demo running, customer need to define SIMCOM_UI_DEMO_TO_USB_AT_PORT or SIMCOM_UI_DEMO_TO_UART1_PORT(in simcom_api.h) to select hardware interface.
#endif

    sAPP_UartTask();
    sAPP_UrcTask();
#ifndef AT_COMMAND_SUPPORT
    sAPP_UsbVcomTask();
#endif
    sAPP_KeyTask();
#ifdef BT_SUPPORT
    sAPP_BleServerDemo();
#endif
    sAPP_HelloWorldDemo();
}

int printf(const char *format, ...)
{
    va_list ap;

    char *buffer = (char *)malloc(2048);
    if (!buffer) return -1;
    memset(buffer, 0, 2048);
    va_start(ap, format);
    vsnprintf(buffer, 2048, format, ap);
    va_end(ap);

    sAPI_Debug("%s", buffer);

    free(buffer);

    return 0;
}

appRegItem_t helloworldapp_entry _appRegTable_attr_ = {.app_entry = Application};
