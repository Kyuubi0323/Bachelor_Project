/**
  ******************************************************************************
  * @file    cus_key.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of power key thread.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "simcom_common.h"
#include "simcom_debug.h"
#include "simcom_api.h"
#include "string.h"

/**
  * @brief  Callback for power key when there is edge trigger event.
  * @param  void
  * @note   Please do not run delay\sleep or blocking API inside this callback.
  * @retval void
  */
void PowerKeyIntCBFunc(void)
{
    ONKEY_STATUS keyStatus;

	/* Get power key status */
    keyStatus = sAPI_GetPowerKeyStatus();

    sAPI_Debug("%s keyStatus:%d", __func__, keyStatus);
}

/**
  * @brief  Register power key operation feedback.
  * @param  void
  * @note   
  * @retval void
  */
void sAPP_KeyTask(void)
{
    /* Power key edge trigger call back function register */
    sAPI_PowerKeyRegisterCallback(PowerKeyIntCBFunc);

    sAPI_Debug("%s run !",__func__);
}


