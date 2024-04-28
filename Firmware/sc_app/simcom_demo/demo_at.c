/**
  ******************************************************************************
  * @file    demo_at.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of AT cmd demo,this demo will show how to send AT cmd to the core,to receive response and URC please refer to cus_urc.c on SC_URC_INTERNAL_AT_RESP_MASK.
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
#include "string.h"

/**
  * @brief  Demo for AT cmd comunication.
  * @param  void
  * @note   Responses are captured through attention SC_URC_INTERNAL_AT_RESP_MASK in cus_urc.c
  * @retval void
  */
void sAPP_AtSendDemo(void)
{
    /*OpenSDK Bool type not defined*/

    /*Responses are captured through attention SC_URC_INTERNAL_AT_RESP_MASK */
    if(sAPI_AtSend("AT+CSQ\r\n",strlen("AT+CSQ\r\n"))!= 1/*TRUE*/)
        sAPI_Debug("send at fail");
}






