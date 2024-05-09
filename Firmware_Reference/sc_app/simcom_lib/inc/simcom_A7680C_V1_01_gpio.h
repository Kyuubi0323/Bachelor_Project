/**
  ******************************************************************************
  * @file    simcom_A7680C_V1_01_gpio.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of A7680C V1 01 GPIO configuration, please refer to related HD description.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_A7680C_V1_01_GPIO_H__
#define __SIMCOM_A7680C_V1_01_GPIO_H__

typedef enum
{
    SC_MODULE_GPIO_0 = 0,
	SC_MODULE_GPIO_1 = 1,
    SC_MODULE_GPIO_2 = 2,
    SC_MODULE_GPIO_3 = 3,
    SC_MODULE_GPIO_4 = 4,
    SC_MODULE_GPIO_5 = 5,
    SC_MODULE_GPIO_6 = 6, //The default is UART3_TXD, and it needs to be reconfigured with sAPI_setPinFunction when it is used as gpio.
    SC_MODULE_GPIO_7 = 7, //The default is UART3_RXD, and it needs to be reconfigured with sAPI_setPinFunction when it is used as gpio.
    SC_MODULE_GPIO_8 = 8,
    SC_MODULE_GPIO_9 = 9,

    SC_MODULE_GPIO_MAX

}SC_Module_GPIONumbers;
#endif
