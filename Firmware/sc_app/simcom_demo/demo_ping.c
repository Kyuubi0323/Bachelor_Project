/**
  ******************************************************************************
  * @file    demo_ping.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of ping operation.
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

/**
  * @brief  ping operation callback
  * @param  size---packet of response.
  * @param  statistics
  * @note   
  * @retval void
  */
void pingResult(int size,char *statistics)
{
    if(statistics != NULL)
        sAPI_Debug("size[%d] statistics[%s]",size,statistics);
}

/**
  * @brief  PING an IP or Domain name with API
  * @param  void
  * @note   
  * @retval void
  */
void simcom_ping_test(void)
{
    SCPingResultCode ret ;

    SCping ctx;
    memset(&ctx,0,sizeof(ctx));
    ctx.destination = "www.baidu.com";
    ctx.count = 20;
    ctx.interval = 10;
    ctx.resultcb = pingResult;

    
    ret = sAPI_Ping(&ctx);

    sAPI_Debug("simcom_ping_test ret[%d]",ret);
}

