/**
  ******************************************************************************
  * @file    demo_system.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of system operation.
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
  * @brief  Read CPU used rate and free heap.
  * @param  void
  * @note   non-blocking
  * @retval void
  */
void sAPP_ReadSystemInfoDemo(void)
{
    UINT32 cpuUsedRate, heapFreeSize;
    sAPI_GetSystemInfo(&cpuUsedRate,&heapFreeSize);
    return;
}
   

