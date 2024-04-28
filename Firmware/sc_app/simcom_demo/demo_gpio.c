/**
  ******************************************************************************
  * @file    demo_gpio.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of gpio operation.
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
#include "stdio.h"
#include "string.h"
#include "simcom_system.h"


#ifndef NULL
#define NULL ((void *)0)
#endif
extern SIM_MSG_T GetParamFromUart(void);
extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);
//#define GPIO_INT_WAKEUP_TEST


enum SC_GPIO_SWITCH
{
    SC_GPIO_SET_DIRECTION = 1,
    SC_GPIO_GET_DIRECTION,
    SC_GPIO_SET_LEVEL,
    SC_GPIO_GET_LEVEL,
    SC_GPIO_SET_INTERRUPT,
    SC_GPIO_WAKEUP_ENABLE,
    SC_GPIO_CONFIG,
    SC_INIT_NET_LIGHT,
    SC_GPIO_AUTO_INPUT_TEST,
    SC_GPIO_TURN_ON_TEST,
    SC_GPIO_TURN_OFF_TEST,
    SC_GPIO_BACK = 99
};

unsigned int All_GpioNUM[] = {
    0,1,2,3,4,
    5,6,7,8,9,
    10,11,12,13,14,
    15,16,17,18,19,
    20,21,22,23,24
};

/**
  * @brief  GPIO interrupt handle
  * @param  void
  * @note   
  * @retval void
  */
void GPIO_IntHandler(void)
{
#ifdef GPIO_INT_WAKEUP_TEST
    static unsigned int i = 0;

    if(i%2 == 0)
        sAPI_GpioSetValue(SC_MODULE_GPIO_10, 1);
    else
        sAPI_GpioSetValue(SC_MODULE_GPIO_10, 0);

    i++;
#endif
}

/**
  * @brief  GPIO to wake up system,interrupt handle
  * @param  void
  * @note   
  * @retval void
  */
void GPIO_WakeupHandler(void)
{
#ifdef GPIO_INT_WAKEUP_TEST
    static unsigned int i = 0;

    if(i%2 == 0)
        sAPI_GpioSetValue(SC_MODULE_GPIO_9, 1);
    else
        sAPI_GpioSetValue(SC_MODULE_GPIO_9, 0);

    i++;
#endif
    /* If you want to leave sleep after wake up, please disable system sleep ! */
    sAPI_SystemSleepSet(SC_SYSTEM_SLEEP_DISABLE);
}

/**
  * @brief  GPIO demo code.
  * @param  This demo will show how to set direction\level\read\write\interrupt on GPIOs.
  * @note   
  * @retval void
  */
void GpioDemo(void)
{
    SIM_MSG_T optionMsg ={0,0,0,NULL};
    UINT32 opt = 0;
    SC_GPIOReturnCode ret;
    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] = {
        "1. GPIO set direction",
        "2. GPIO get direction",
        "3. GPIO set level",
        "4. GPIO get level",
        "5. GPIO set interrupt",
        "6. GPIO wakeup",
        "7. GPIO parameters config",
        "8. Init NET Light",
        "99. back",
    };
    unsigned int gpio_num;
    unsigned int direction;
    unsigned int gpio_level = 0;
#ifdef GPIO_INT_WAKEUP_TEST
    SC_GPIOConfiguration pinConfig;
    pinConfig.initLv = 0;
    pinConfig.isr =  NULL;
    pinConfig.pinDir = SC_GPIO_IN_PIN;
    pinConfig.pinEd = SC_GPIO_NO_EDGE;
    pinConfig.wu = GPIO_WakeupHandler;
    pinConfig.pinPull = SC_GPIO_PULLUP_ENABLE;//pull_up

    ret = sAPI_GpioConfig(SC_MODULE_GPIO_10, pinConfig);
    if(ret == SC_GPIORC_OK)
    {
        PrintfResp("\r\nConfig GPIO successed !\r\n");
    }
    else
    {
        sAPI_Debug("\r\nConfig GPIO failed ret =%d!\r\n",ret);
    }

    ret = sAPI_GpioConfig(SC_MODULE_GPIO_9, pinConfig);
    if(ret == SC_GPIORC_OK)
    {
        PrintfResp("\r\nConfig GPIO successed !\r\n");
    }
    else
    {
        sAPI_Debug("\r\nConfig GPIO failed ret= %d!\r\n",ret);
    }
#endif

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
            case SC_GPIO_SET_DIRECTION:

                {
                    PrintfResp("\r\nPlease input gpio number.\r\n");
                    optionMsg = GetParamFromUart();
                    gpio_num = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(gpio_num >= SC_MODULE_GPIO_MAX)
                    {
                        PrintfResp("\r\nincorrect SC MODULE GPIO NUMBER.\r\n");
                        break;
                    }

                    PrintfResp("\r\nPlease input direction.     0:input    1:output.\r\n");
                    optionMsg = GetParamFromUart();
                    direction = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    ret = sAPI_GpioSetDirection(gpio_num,direction);
                    if(ret != SC_GPIORC_OK)
                        sAPI_Debug("sAPI_setGpioDirection:    failed.");
                    else
                        sAPI_Debug("sAPI_setGpioDirection:    success.");

                    PrintfResp("\r\noperation successful!\r\n");

                    break;
                }
            case SC_GPIO_GET_DIRECTION:
            {
                    PrintfResp("\r\nPlease input gpio number.\r\n");
                    optionMsg = GetParamFromUart();
                    gpio_num = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(gpio_num >= SC_MODULE_GPIO_MAX)
                    {
                        PrintfResp("\r\nincorrect SC MODULE GPIO NUMBER.\r\n");
                        break;
                    }

                    direction = sAPI_GpioGetDirection(gpio_num);

                    char tmp[40];

                    sprintf(tmp,"\r\nthe direction of gpio_%d is %d \r\n",gpio_num,direction);

                    PrintfResp(tmp);
                    PrintfResp("\r\noperation successful!\r\n");

                    break;
            }
            case SC_GPIO_SET_LEVEL:

                {
                    PrintfResp("\r\nPlease input gpio number.\r\n");
                    optionMsg = GetParamFromUart();
                    gpio_num = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(gpio_num >= SC_MODULE_GPIO_MAX)
                    {
                        PrintfResp("\r\nincorrect SC MODULE GPIO NUMBER.\r\n");
                        break;
                    }

                    PrintfResp("\r\nPlease input gpio level.    0:low level    1:high level.\r\n");
                    optionMsg = GetParamFromUart();
                    gpio_level = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    ret = sAPI_GpioSetValue(gpio_num,gpio_level);
                    if(ret != SC_GPIORC_OK)
                        sAPI_Debug("sAPI_GpioSetValue:    failed.");
                    else
                        sAPI_Debug("sAPI_GpioSetValue:    success.");

                    PrintfResp("\r\noperation successful!\r\n");

                    break;
                }

            case SC_GPIO_GET_LEVEL:

                {
                    PrintfResp("\r\nPlease input gpio number.\r\n");
                    optionMsg = GetParamFromUart();
                    gpio_num = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(gpio_num >= SC_MODULE_GPIO_MAX)
                    {
                        PrintfResp("\r\nincorrect SC MODULE GPIO NUMBER.\r\n");
                        break;
                    }

                    ret = sAPI_GpioGetValue(gpio_num);

                    char tmp[40];

                    sprintf(tmp,"\r\nthe level of gpio_%d is %d \r\n",gpio_num,ret);

                    PrintfResp(tmp);
                    PrintfResp("\r\noperation successful!\r\n");

                    break;
                }
            case SC_GPIO_SET_INTERRUPT:
            {
                    PrintfResp("\r\nPlease input gpio number.\r\n");
                    optionMsg = GetParamFromUart();
                    gpio_num = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(gpio_num >= SC_MODULE_GPIO_MAX)
                    {
                        PrintfResp("\r\nincorrect SC MODULE GPIO NUMBER.\r\n");
                        break;
                    }
                    ret = sAPI_GpioSetDirection(gpio_num,0);
                    if(ret != SC_GPIORC_OK)
                    {
                        sAPI_Debug("sAPI_setGpioDirection:    failed.");
                        break;
                    }
                    ret = sAPI_GpioConfigInterrupt(gpio_num,SC_GPIO_TWO_EDGE,GPIO_IntHandler);
                    if(ret != SC_GPIORC_OK)
                    {
                        sAPI_Debug("sAPI_GpioConfigInterrupt:    failed.");
                        break;
                    }

                    PrintfResp("\r\noperation successful!\r\n");
                    break;
            }

            case SC_GPIO_WAKEUP_ENABLE:
            {
                    PrintfResp("\r\nPlease input wake up gpio number.\r\n");
                    optionMsg = GetParamFromUart();
                    gpio_num = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(gpio_num >= SC_MODULE_GPIO_MAX)
                    {
                        PrintfResp("\r\nincorrect SC MODULE GPIO NUMBER.\r\n");
                        break;
                    }

                    ret = sAPI_GpioWakeupEnable(gpio_num, SC_GPIO_FALL_EDGE);
                    if(ret != SC_GPIORC_OK)
                    {
                        PrintfResp("\r\nGpio set wake up failed !\r\n");
                        break;
                    }

                    PrintfResp("\r\nGpio set wake up successful !\r\n");
                    break;
            }
            case SC_GPIO_CONFIG:
            {
                    PrintfResp("\r\nPlease input all parameters, delimited by comma.\r\n");
                    PrintfResp("\r\nSyntax: GPIO number,direction,init level,pull type,edge type\r\n");
                    PrintfResp("\r\nExample: 0,0,1,1,3\r\n");
                    optionMsg = GetParamFromUart();
                    char input_paras[40] = {0};
                    memcpy(input_paras,optionMsg.arg3,optionMsg.arg2);
                    sAPI_Free(optionMsg.arg3);

                    SC_GPIOConfiguration pinconfig;
                    char *p_delim;
                    p_delim = strtok(input_paras,",");
                    if(p_delim)
                        gpio_num = atoi(p_delim);
                    else
                    {
                        PrintfResp("\r\nFomat Error.\r\n");
                        break;
                    }

                    p_delim = strtok(NULL,",");
                    if(p_delim)
                        pinconfig.pinDir = atoi(p_delim);
                    else
                    {
                        PrintfResp("\r\nFomat Error.\r\n");
                        break;
                    }

                    p_delim = strtok(NULL,",");
                    if(p_delim)
                        pinconfig.initLv = atoi(p_delim);
                    else
                    {
                        PrintfResp("\r\nFomat Error.\r\n");
                        break;
                    }

                    p_delim = strtok(NULL,",");
                    if(p_delim)
                        pinconfig.pinPull = atoi(p_delim);
                    else
                    {
                        PrintfResp("\r\nFomat Error.\r\n");
                        break;
                    }

                    p_delim = strtok(NULL,",");
                    if(p_delim)
                        pinconfig.pinEd = atoi(p_delim);
                    else
                    {
                        PrintfResp("\r\nFomat Error.\r\n");
                        break;
                    }

                    if(pinconfig.pinEd == SC_GPIO_RISE_EDGE || pinconfig.pinEd == SC_GPIO_FALL_EDGE || pinconfig.pinEd == SC_GPIO_TWO_EDGE)
                        pinconfig.isr = GPIO_IntHandler;
                    else
                        pinconfig.isr = NULL;

                    pinconfig.wu = GPIO_WakeupHandler;

                    ret = sAPI_GpioConfig(gpio_num,pinconfig);
                    if(ret == SC_GPIORC_OK)
                        PrintfResp("\r\nConfig OK!\r\n");
                    else
                    {
                        char tmp[40];
                        sprintf(tmp,"\r\nConfig Error, Error code is %d\r\n",ret);
                        PrintfResp(tmp);
                    }
                    break;
            }
            case SC_INIT_NET_LIGHT:

                    sAPI_InitNetLight(1);

                break;

            case SC_GPIO_TURN_ON_TEST:
            {
                PrintfResp("\r\nturn all gpio on.\r\n");
                int i;

                for(i = 0; i< sizeof(All_GpioNUM)/sizeof(int); i++)
                {
                    ret = sAPI_GpioSetDirection(All_GpioNUM[i],SC_GPIO_OUT_PIN);
                    if(ret != SC_GPIORC_OK)
                    {
                        char tmp[55] = {0};
                        sprintf(tmp,"[auto]%d set params failed. ret -- %d\r\n",All_GpioNUM[i],ret);
                        PrintfResp(tmp);
                    }

                    ret = sAPI_GpioSetValue(All_GpioNUM[i],1);
                    if(ret != SC_GPIORC_OK)
                    {
                        char tmp[55] = {0};
                        sprintf(tmp,"[auto]%d set params failed. ret -- %d\r\n",All_GpioNUM[i],ret);
                        PrintfResp(tmp);
                    }
                }
                break;
            }

            case SC_GPIO_TURN_OFF_TEST:
            {
                PrintfResp("\r\nturn all gpio off.\r\n");
                int i;

                for(i = 0; i< sizeof(All_GpioNUM)/sizeof(int); i++)
                {
                    ret = sAPI_GpioSetDirection(All_GpioNUM[i],SC_GPIO_OUT_PIN);
                    if(ret != SC_GPIORC_OK)
                    {
                        char tmp[55] = {0};
                        sprintf(tmp,"[auto]%d set params failed. ret -- %d\r\n",All_GpioNUM[i],ret);
                        PrintfResp(tmp);
                    }
                    ret = sAPI_GpioSetValue(All_GpioNUM[i],0);
                    if(ret != SC_GPIORC_OK)
                    {
                        char tmp[55] = {0};
                        sprintf(tmp,"[auto]%d set params failed. ret -- %d\r\n",All_GpioNUM[i],ret);
                        PrintfResp(tmp);
                    }
                }
                break;
            }

            case SC_GPIO_BACK:
               return;

        }

    }
}




#ifdef SIMCOM_A7620_GREE

//watchdog demo
//asume SC_MODULE_GPIO_6 is used to control extrenal reset circuit.

sTaskRef watchdogTaskRef;
char watchdogTaskStack[1024];
static int level = 0;

/**
  * @brief  Feed external watch dog by toogle GPIO6
  * @param  argv
  * @note   
  * @retval void
  */
void sAPP_CusFeedDog(void * argv)
{
    while(1)
    {
        level = !level;
        sAPI_GpioSetDirection(SC_MODULE_GPIO_6, SC_GPIO_OUT_PIN);
        sAPI_GpioSetValue(SC_MODULE_GPIO_6, level);
        sAPI_TaskSleep(10*200);//10s
    }
}

/**
  * @brief  Watch dog task initial
  * @param  void
  * @note   
  * @retval void
  */
void sAPP_WatchDogTaskInit(void)
{
    if(SC_SUCCESS != sAPI_TaskCreate(&watchdogTaskRef, watchdogTaskStack, 1024, 250, "watchdog", sAPP_CusFeedDog, NULL))
        sAPI_Debug("Create timer failed\r\n");
}

#endif





