/**
  ******************************************************************************
  * @file    simcom_uart.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of UART peripherals.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef SIMCOMUART_H
#define SIMCOMUART_H


/****************************************************************************
    Define enum  
*****************************************************************************/
typedef enum
{
    SC_UART_RETURN_CODE_OK = 0,
    SC_UART_RETURN_CODE_ERROR = -1
}SC_Uart_Return_Code;
    
typedef enum
{
    SC_UART = 1,  //Full Function UART
    SC_UART2 = 2,  //Debug UART
    SC_UART3 = 3,  //UART3
    SC_UART_MAX
}SC_Uart_Port_Number;

typedef enum
{
    SC_UART_READY_READ = 1,
    SC_UART_READ_DONE ,
    SC_UART_READY_WRITE,
    SC_UART_EVENT_MAX
}SC_Uart_Event;

typedef enum  /* The status of Rx */
{
    SC_UART_RX_IDEL = 0,
    SC_UART_RX_BUSY  = 1,
    SC_UART_RX_STATUS_MAX = -1
}SC_Uart_Rx_Status;

typedef enum
{
    SC_UART_OPEN,
    SC_UART_CLOSE,
}SC_Uart_Control;

typedef enum   /* All the UART Baud Rate that the UART Package supplay */
{
    SC_UART_BAUD_300       = 300,
    SC_UART_BAUD_600       = 600,
    SC_UART_BAUD_1200      = 1200,
    SC_UART_BAUD_2400      = 2400,
    SC_UART_BAUD_3600      = 3600,
    SC_UART_BAUD_4800      = 4800,
    SC_UART_BAUD_9600      = 9600,
    SC_UART_BAUD_19200     = 19200,
    SC_UART_BAUD_38400     = 38400,
    SC_UART_BAUD_57600     = 57600,
    SC_UART_BAUD_115200    = 115200,
    SC_UART_BAUD_230400    = 230400,
    SC_UART_BAUD_460800    = 460800,
    SC_UART_BAUD_921600    = 921600,
    SC_UART_BAUD_1842000   = 1842000,
    SC_UART_BAUD_3686400   = 3686400,
    SC_UART_MAX_NUM_BAUD
}SC_UART_BaudRates;

typedef enum  /* The Word Len of the UART Frame Format  */
{
    SC_UART_WORD_LEN_5,                /* set Word Lengto to 5 Bits         */
    SC_UART_WORD_LEN_6,                /* set Word Lengto to 6 Bits         */
    SC_UART_WORD_LEN_7,                /* set Word Lengto to 7 Bits         */
    SC_UART_WORD_LEN_8                 /* set Word Lengto to 8 Bits         */
}SC_UART_WordLen;

typedef enum   /* The Stop Bits of the UART Frame Format */
{
    SC_UART_ONE_STOP_BIT,
    SC_UART_ONE_HALF_OR_TWO_STOP_BITS
}SC_UART_StopBits;

typedef enum  /* The Parity Bits of the UART Frame Format */
{
    SC_UART_NO_PARITY_BITS,
    SC_UART_EVEN_PARITY_SELECT,
    SC_UART_ODD_PARITY_SELECT
}SC_UART_ParityTBits;

typedef enum
{
    SC_UART_RS485_DE_LOW_LEVEL   = 0,
    SC_UART_RS485_DE_HIGH_LEVEL
}SC_UartRs485DeActiveLevel;

/****************************************************************************
    Define struct 
*****************************************************************************/
typedef struct SCuartConfiguration
{
    SC_UART_BaudRates BaudRate;  /* the baudrate of the uart(default - 115200)*/
    SC_UART_WordLen DataBits;   /* 5, 6, 7, or 8 number of data bits in the UART data frame (default - 8). */
    SC_UART_StopBits StopBits;  /* 1, 1.5 or 2 stop bits in the UART data frame (default - 1).   */
    SC_UART_ParityTBits ParityBit; /* Even, Odd or no-parity bit type in the UART data frame (default - Non). */
}SCuartConfiguration;

typedef void (*SC_Uart_Callback)(SC_Uart_Port_Number port, void *para);
typedef void (*SC_Uart_CallbackEX)(SC_Uart_Port_Number port, int len, void *reserve);

#endif


