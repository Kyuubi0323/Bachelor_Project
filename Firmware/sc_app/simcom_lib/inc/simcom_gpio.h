/**
  ******************************************************************************
  * @file    simcom_gpio.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of GPIO function.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */



#ifndef __SIMCOM_GPIO_H__
#define __SIMCOM_GPIO_H__
typedef void (*GPIOCallback)(void);


#ifdef SIMCOM_A7678_V1_02
#include "simcom_A7670C_FASL_R2_gpio.h"

#elif defined(SIMCOM_A7670C_LASL_WHXA)
#include "simcom_A7670C_LASL_WHXA_gpio.h"

#elif defined(SIMCOM_A7670C_MASL_YCKJ)
#include "simcom_A7670C_MASL_YCKJ_gpio.h"

#elif defined(SIMCOM_A7670C_LASL_WS)
#include "simcom_A7670C_LASL_WS_gpio.h"

#elif defined(SIMCOM_A7670SA_LASE_CZJ)
#include "simcom_A7670SA_LASE_CZJ_gpio.h"

#elif defined(SIMCOM_DA7600C1)
#include "SIMCOM_DA7600C1_gpio.h"

#elif defined(SIMCOM_A7678_V1_02_MMI)
#include "simcom_A7670C_FASL_R2_MMI_gpio.h"

#elif defined(SIMCOM_A7630_V2_01)
#include "simcom_A7630C_gpio.h"

#elif defined(SIMCOM_A7630_V3_01)
#include "simcom_A7630C_LAAL_gpio.h"

#elif defined(SIMCOM_A7630_V3_01_MMI)
#include "simcom_A7630C_LAAL_MMI_gpio.h"

#elif defined(SIMCOM_A7630_LXT)
#include "simcom_A7630C_LXT_gpio.h"

#elif defined(SIMCOM_A7630C_ST)
#include "simcom_A7630C_ST_gpio.h"

#elif defined(SIMCOM_A7680C_V1_01)
#include "simcom_A7680C_V1_01_gpio.h"

#elif defined(SIMCOM_A7680C_V2_01)
#include "simcom_A7680C_V2_01_gpio.h"

#elif defined(SIMCOM_A7600C1_V3_01)
#include "simcom_A7600C1_gpio.h"

#elif defined(SIMCOM_A7600C1_V3_01_OL)
#include "simcom_A7600C1_OL_gpio.h"

#elif defined(SIMCOM_A7600C1_V4_01)
#include "simcom_A7600C1_V4_01_gpio.h"

#elif defined(SIMCOM_A7682E_V1_02)
#include "simcom_A7682E_gpio.h"

#elif defined(SIMCOM_A7676E_V1_01)
#include "simcom_A7676E_V1_01_gpio.h"

#else

#error no_selected_module

#endif


typedef enum
{
	SC_GPIORC_FALSE = 0,
	SC_GPIORC_TRUE = 1,
	SC_GPIORC_LOW = 0,
	SC_GPIORC_HIGH = 1,

	SC_GPIORC_OK = 0,
    SC_GPIORC_INVALID_PORT_HANDLE = -100,
    SC_GPIORC_NOT_OUTPUT_PORT,
    SC_GPIORC_NO_TIMER,
    SC_GPIORC_NO_FREE_HANDLE,
    SC_GPIORC_AMOUNT_OUT_OF_RANGE,
    SC_GPIORC_INCORRECT_PORT_SIZE,
    SC_GPIORC_PORT_NOT_ON_ONE_REG,
    SC_GPIORC_INVALID_PIN_NUM,
    SC_GPIORC_PIN_USED_IN_PORT,
    SC_GPIORC_PIN_NOT_FREE,
    SC_GPIORC_PIN_NOT_LOCKED,
    SC_GPIORC_NULL_POINTER,
    SC_GPIORC_PULLED_AND_OUTPUT,
	SC_GPIORC_INCORRECT_PORT_TYPE,
	SC_GPIORC_INCORRECT_DEBOUNCE,
    SC_GPIORC_INCORRECT_TRANSITION_TYPE,
	SC_GPIORC_INCORRECT_DIRECTION,
	SC_GPIORC_INCORRECT_PULL,
	SC_GPIORC_INCORRECT_INIT_VALUE,
	SC_GPIORC_WRITE_TO_INPUT,
	SC_GPIORC_INVALID_MULTI_FUNCTION
}SC_GPIOReturnCode;

typedef enum
{
    SC_GPIO_IN_PIN = 0,
    SC_GPIO_OUT_PIN = 1
}SC_GPIOPinDirection;

typedef enum
{
    SC_GPIO_PULL_DISABLE = 0,
    SC_GPIO_PULLUP_ENABLE,
    SC_GPIO_PULLDN_ENABLE
}SC_GPIOPullUpDown;

typedef enum
{
    SC_GPIO_NO_EDGE = 0,
    SC_GPIO_RISE_EDGE,
    SC_GPIO_FALL_EDGE,
    SC_GPIO_TWO_EDGE,
}SC_GPIOTransitionType;


typedef struct
{
	SC_GPIOPinDirection pinDir;
	UINT32			initLv;
	SC_GPIOPullUpDown 	pinPull;
	SC_GPIOTransitionType pinEd;
	GPIOCallback isr;
	GPIOCallback wu;
} SC_GPIOConfiguration;





#endif

