/**
  ******************************************************************************
  * @file    simcom_i2c.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of IIC peripherals operation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_I2C_H__
#define __SIMCOM_I2C_H__


typedef enum
{
    SC_I2C_RC_OK,
    SC_I2C_RC_NOT_OK,
    SC_I2C_RC_INVALID_DATA_SIZE,
    SC_I2C_RC_INVALID_DATA_PTR,
    SC_I2C_RC_TOO_MANY_REGISTERS,
    SC_I2C_RC_TIMEOUT_ERROR,                               // 5
    SC_I2C_RC_CHIP_BUSY,                                   // 6
    SC_I2C_RC_INVALID_GENERAL_CALL_SLAVE_ADDRESS,          // 7
	SC_I2C_RC_UNREGISTER_ERR,                              // 8
	SC_I2C_RC_MESSAGE_QUEUE_IS_FULL,                       // 9
    SC_I2C_ISR_UNEXPECTED_INTERRUPT,                       // 0xA
	SC_I2C_ISR_BUS_ERROR,                                  // 0xB
	SC_I2C_ISR_BUS_BUSY,                                   // 0xC
	SC_I2C_ISR_EARLY_BUS_BUSY,                             // 0xD
	SC_I2C_ISR_CALL_BACK_FUNCTION_ERR,                     // 0xE
	SC_I2C_ISR_ARBITRATION_LOSS,                           // 0xF
	SC_I2C_PARAMS_ERROR,
	SC_I2C_RC_ILLEGAL_USE_OF_API
}SC_I2C_ReturnCode;

typedef enum
{
    SC_I2C_CHANNEL0,
    SC_I2C_CHANNEL1,
    SC_I2C_CHANNEL2,
    SC_I2C_CHANNEL3,
}SC_I2C_CHANNEL;

typedef enum
{
    SC_I2C_STANDARD_MODE = 0,	/*100Kbps*/
    SC_I2C_FAST_MODE     = 1,	/*400Kbps*/
    SC_I2C_HS_MODE       = 2,	/*3.4 Mbps slave/3.3 Mbps master,standard mode when not doing a high speed transfer*/
    SC_I2C_HS_MODE_FAST  = 3,	/*3.4 Mbps slave/3.3 Mbps master,fast mode when not doing a high speed transfer*/
}SC_I2C_CLOCK;

typedef struct
{
    SC_I2C_CHANNEL i2c_channel;
    SC_I2C_CLOCK i2c_clock;
}SC_I2C_DEV;

#endif
