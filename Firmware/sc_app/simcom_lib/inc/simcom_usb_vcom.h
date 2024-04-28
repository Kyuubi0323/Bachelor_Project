/**
  ******************************************************************************
  * @file    simcom_usb_vcom.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of USB VCOM(USB AT port).
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef SIMCOMUSBVCOM_H
#define SIMCOMUSBVCOM_H


/****************************************************************************
    Define enum
*****************************************************************************/
typedef enum  /* The status of Tx */
{
    USB_TX_SUCCEED  = 0,
    USB_TX_FAIL     = -1
}Usb_Tx_Status;

typedef enum  /* The status of USB Vbus */
{
    USB_VBUS_LOW      = 0,
    USB_VBUS_HIGH     = 1,
}SC_Usb_Vbus_Status;

/*****************************************************************************
 * FUNCTION
 *  sAPI_UsbVcomWrite
 *
 * DESCRIPTION
 *  Send data to usb Tx
 *
 * PARAMETERS
 *  *data & length
 *
 * RETURNS
 *  Usb_Tx_Status
 *
 *  USB_TX_SUCCEED  = 0          Sent successfully
 *  USB_TX_FAIL     = -1         fail in send
 *
 * NOTE
 *
 * int sAPI_UsbVcomWrite(unsigned char* data, unsigned long length);
*****************************************************************************/




typedef void (*SC_Usb_Callback)(void *para);
typedef void (*SC_Usb_CallbackEX)(int len, void *reserve);


#endif


