/**
  ******************************************************************************
  * @file    demo_spi.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of SPI operation.
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
#include "simcom_debug.h"
#include "simcom_os.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define NEW_SPI_API
#define MAX_SPI_BUFFER_DATA_LEN 300
#define SPI_TX_RX_DATA_LED 4

extern SIM_MSG_T GetParamFromUart(void);
extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8 *options_list[], int array_size);
extern void PrintfResp(INT8 *format);

typedef enum
{
    READ_ID = 1,
    READ_WRITE_STATUS,
    BACK = 99,
} SPI_OPS;

typedef enum
{
    SPI_FLASH_INIT = 1,
    SPI_FLASH_READ_ID = 2,
    SPI_FLASH_ERASE,
    SPI_FLASH_READ,
    SPI_FLASH_WRITE,
    SPI_FLASH_BACK = 99,
} SPI_FLASH;

 /**
  * @brief  This demo show that how to use spi api by operating "XM25QU128B" flash IC.
  * @param  void
  * @note   
  * @retval void
  */
void SpiDemo(void)
{
    SIM_MSG_T optionMsg = {0, 0, 0, NULL};
    UINT32 opt = 0;
    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] =
    {
        "1. spi flash read id",
        "2. spi flash read/write status reg",
        "99. back",
    };
    int ret = 0;
    UINT8 disBuff[128] = {0};
#ifdef NEW_SPI_API
    /**************** spi init ****************/
    SC_SPI_DEV spiDev;
    /*set spi clock*/
    spiDev.clock = SPI_CLOCK_26MHz;
    /* set spi mode */
    spiDev.mode = SPI_MODE_PH0_PO0;
    /*
    set cs mode
    GPIO_MODE:cs remains low level during data stream transmission.
    SSP_MODE:cs remains low level during Byte transmission.
     */
    spiDev.csMode = GPIO_MODE;
    /* Set SPI channel, default is 0 */
    spiDev.index = 0;
    sAPI_SpiConfigInitEx(&spiDev);
#else
    sAPI_SPIConfigInit(SPI_CLOCK_6MHz, SPI_MODE_PH0_PO0);
#endif
    while (1)
    {
        PrintfResp(note);
        PrintfOptionMenu(options_list, sizeof(options_list) / sizeof(options_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);
        if (SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("%s,msg_id is error!!", __func__);
            break;
        }

        sAPI_Debug("arg3 = [%s]", optionMsg.arg3);
        opt = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);

        switch (opt)
        {
#ifdef NEW_SPI_API
            case READ_ID:
            {

                char SendData[4] = {0x9F, 0, 0, 0};
                char RevData[4] = {0};

                ret = sAPI_SpiReadBytesEx(&spiDev, SendData, 1, RevData, 3);
                if (ret != 0)
                {
                    sprintf(disBuff, "\r\nspi0 read bytes fail\r\n");
                }
                else
                {
                    sprintf(disBuff, "\r\nspi0 read device ID:%x\r\n", (RevData[0] << 16) | (RevData[1] << 8) | RevData[2]);
                }

                PrintfResp(disBuff);
                break;
            }
            case READ_WRITE_STATUS:
            {
                /* read status register*/
                char writeData[2] = {0x01};
                char readCmd = 0x05;
                char status = 0;

                ret = sAPI_SpiReadBytesEx(&spiDev, &readCmd, 1, &status, 1);
                if (ret != 0)
                    sAPI_Debug("ReadBytes fail\r\n");
                sAPI_Debug("read bytes seccess 0x%x\r\n", status);

                writeData[1] = status | 0x02;
                ret = sAPI_SpiWriteBytesEx(&spiDev, writeData, 2);
                if (ret != 0)
                    sAPI_Debug("ReadBytes fail\r\n");
                sAPI_Debug("write bytes seccess 0x%x\r\n", writeData[1]);

                break;
            }
#else
            case READ_ID:
            {
                char SendData[4] = {0x9F, 0, 0, 0};
                char RevData[4] = {0};

                ret = sAPI_ExtSpiReadBytes(SendData, 1, RevData, 3);
                if (ret != 0)
                    sAPI_Debug("ReadBytes fail\r\n");
                sAPI_Debug("spi0 read device ID:%x\r\n", (RevData[0] << 16) | (RevData[1] << 8) | RevData[2]);
                break;
            }
            case READ_WRITE_STATUS:
            {
                break;
            }
#endif
            case BACK:
                return;
        }
    }

}

#define START_ADDR  0x10000
 /**
  * @brief  This demo show that how to use spi api by operating nor flash IC.
  * @param  void
  * @note   
  * @retval void
  */
void SpiNorDemo(void)
{
    SIM_MSG_T optionMsg = {0, 0, 0, NULL};
    UINT32 opt = 0;
    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] =
    {
        "1. spi flash init",
        "2. spi flash read id",
        "3. spi flash erase",
        "4. spi flash read",
        "5. spi flash write",
        "99. back",
    };
    int ret = 0;
    UINT8 disBuff[1024] = {0};

    while (1)
    {
        PrintfResp(note);
        PrintfOptionMenu(options_list, sizeof(options_list) / sizeof(options_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);
        if (SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("%s,msg_id is error!!", __func__);
            break;
        }

        sAPI_Debug("arg3 = [%s]", optionMsg.arg3);
        opt = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);

        switch (opt)
        {
            case SPI_FLASH_INIT:
            {
                unsigned char Buffer[30] = {0};
                sAPI_Debug("sAPI_ExtNorFlashInit");
                sAPI_ExtNorFlashInit();
                sAPI_Debug("sAPI_ExtNorFlashInit...");

#if 0   /*Show the nor flash which is supported*/
                ret = sAPI_ExtNorFlashShowSupportList(Buffer, 30);
                if (!ret)
                {
                    PrintfResp(Buffer);
                }
#endif
                sprintf(disBuff, "\r\nspi nor flash init finished...\r\n");
                PrintfResp(disBuff);
                break;
            }
            case SPI_FLASH_READ_ID:
            {
                unsigned int id = 0;;
                sAPI_ExtNorFlashReadID((unsigned char *)&id);

                sprintf(disBuff, "\r\nsAPI_ExtNorFlashReadID : 0x%x", id);
                PrintfResp(disBuff);
                break;
            }
            case SPI_FLASH_ERASE:
            {
                ret = sAPI_ExtNorFlashSectorErase(START_ADDR, 0x40000);
                if (ret)
                {
                    memset(disBuff, 0, 1024);
                    sprintf(disBuff, "\r\nerase fail\r\n");
                    PrintfResp(disBuff);
                }
                break;
            }
            case SPI_FLASH_READ:
            {
                int i;
                char Buffer[8192] = {0};
                memset(Buffer, 0, 8192);
                unsigned char *ptrDisBuff = disBuff;
                memset(disBuff, 0, 1024);

                ret = sAPI_ExtNorFlashRead(START_ADDR, 8192, (uint32_t)Buffer);
                if (ret != 0)
                    sAPI_Debug("read fail\r\n");

                for (i = 0; i < 0x100; i++)
                {
                    sprintf(ptrDisBuff, "%02x  ", Buffer[i]);
                    ptrDisBuff += 4;
                }
                PrintfResp(disBuff);
                break;
            }
            case SPI_FLASH_WRITE:
            {
                int i;
                char Buffer[8192] = {0};
                memset(Buffer, 0X99, 8192);
                ret = sAPI_ExtNorFlashWrite(START_ADDR, 8192, (uint32_t)Buffer);
                if (ret != 0)
                    sAPI_Debug("write fail\r\n");
                break;
            }

            case 88:
            {
                sAPI_ExtNorFlashShowSupportList(NULL, 0);
                break;
            }

            case SPI_FLASH_BACK:
                return;
        }
    }
}


