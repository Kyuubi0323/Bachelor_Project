/**
  ******************************************************************************
  * @file    simcom_call.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of module voice call.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_CALL_H_
#define _SIMCOM_CALL_H_


#include "simcom_common.h"
#include "simcom_os.h"

#define SC_CALL_MAX_NUMBER_LENGTH   40

typedef enum{
    SC_CALL_SUCESS,
    SC_CALL_FAIL
}SC_CALLReturnCode;

typedef enum{
    SC_CALL_INVALID,
    SC_CALL_DIAL,
    SC_CALL_ANSWER,
    SC_CALL_END,
    SC_AUTO_ANSWER,
    SC_CALL_TYPE_MAX
}SC_CALL_CMD_TYPE;

#endif







