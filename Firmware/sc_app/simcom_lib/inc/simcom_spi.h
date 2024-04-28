/**
  ******************************************************************************
  * @file    simcom_spi.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of SPI peripherals.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_SPI_H__
#define __SIMCOM_SPI_H__


typedef enum
{
    SC_SPI_RC_OK=0,
    SC_SPI_RC_ERROR=-1,
}SC_SPI_ReturnCode;

typedef struct
{
    unsigned char index;   //spi interface index, default 0 avaliable
    int clock;  //spi clock,refer to SC_SPI_CLOCK
    int mode;   //spi mode,refer to SC_SPI_MODE
    int csMode; //cs mode,refer to CS_MODE. When set to SSP_MODE,cs will be controled by SPI logic automatically. When set to 1, cs will be GPIO controled by customer.
}SC_SPI_DEV;

typedef enum
{
    SPI_CLOCK_6MHz = 0,
    SPI_CLOCK_13MHz,
    SPI_CLOCK_26MHz,
    SPI_CLOCK_52MHz,
    SPI_CLOCK_3250KHz,
    SPI_CLOCK_1625KHz,
    SPI_CLOCK_812KHz,
}SC_SPI_CLOCK;

typedef enum
{
    SPI_MODE_PH0_PO0 = 0,
    SPI_MODE_PH0_PO1,
    SPI_MODE_PH1_PO0,
    SPI_MODE_PH1_PO1,
}SC_SPI_MODE;

typedef enum
{
    GPIO_MODE = 0,
    SSP_MODE,
    CUS_CS_MODE,
}CS_MODE;

#endif

