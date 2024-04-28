/**
  ******************************************************************************
  * @file    simcom_A7630C_ST_gpio.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of A7630C ST GPIO configuration, please refer to related HD description.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_A7670C_LAAL_ST_GPIO_H__
#define __SIMCOM_A7670C_LAAL_ST_GPIO_H__

typedef enum
{
    SC_MODULE_GPIO_0 = 0,//PIN1
    SC_MODULE_GPIO_1 = 1,//PIN2
    SC_MODULE_GPIO_2 = 2,//PIN3
    SC_MODULE_GPIO_3 = 3,//PIN4
    SC_MODULE_GPIO_4 = 4,
    SC_MODULE_GPIO_5 = 5,
    SC_MODULE_GPIO_6 = 6,
    SC_MODULE_GPIO_7 = 7,
    SC_MODULE_GPIO_8 = 8,
    SC_MODULE_GPIO_9 = 9,
    SC_MODULE_GPIO_10 = 10,
    SC_MODULE_GPIO_12 = 12,
    SC_MODULE_GPIO_13 = 13,
    SC_MODULE_GPIO_14 = 14,
    SC_MODULE_GPIO_15 = 15,
    SC_MODULE_GPIO_16 = 16,
    SC_MODULE_GPIO_17 = 17,
    SC_MODULE_GPIO_19 = 19,
    SC_MODULE_GPIO_20 = 20,
    SC_MODULE_GPIO_25 = 25,
    SC_MODULE_GPIO_26 = 26,
    SC_MODULE_GPIO_27 = 27,
    SC_MODULE_GPIO_28 = 28,
    SC_MODULE_GPIO_29 = 29,
    SC_MODULE_GPIO_30 = 30,
    SC_MODULE_GPIO_35 = 35,//PIN39
    SC_MODULE_GPIO_36 = 36,//PIN40
    SC_MODULE_GPIO_37 = 37,//PIN56
    SC_MODULE_GPIO_38 = 38,//PIN34
    SC_MODULE_GPIO_39 = 39,//PIN33
    SC_MODULE_GPIO_MAX

}SC_Module_GPIONumbers;
#endif
