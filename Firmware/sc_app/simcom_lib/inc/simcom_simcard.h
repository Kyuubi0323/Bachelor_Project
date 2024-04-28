/**
  ******************************************************************************
  * @file    simcom_simcard.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of sim card management APIs.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef SIMCOM_SIMCARD_H
#define SIMCOM_SIMCARD_H
#include "simcom_os.h"

typedef enum {
    SC_SIM_RETURN_SUCCESS,
    SC_SIM_RETURN_FAIL,
    SC_SIM_RTEURN_UNKNOW
}SC_simcard_err_e;

typedef enum{
    SC_SIMCARD_INVALID,
    SC_SIMCARD_SWITCHCARD,
    SC_SIMCARD_HOTPLUG,
    SC_SIMCARD_URCIND,
    SC_SIMCARD_TYPE_MAX
}SC_SIMCARD_CMD_TYPE;

typedef enum{
    SC_HOTSWAP_INVALID,
    SC_HOTSWAP_QUERY_STATE,
    SC_HOTSWAP_QUERY_LEVEL,
    SC_HOTSWAP_SET_SWITCH,
    SC_HOTSWAP_SET_LEVEL,
    SC_HOTSWAP_TYPE_MAX
}SC_HotSwapCmdType_e;

typedef struct
{
  SC_HotSwapCmdType_e option;
  int SetParam;
}HotSwapMsg_st;

typedef struct
{
  UINT32 status;
  UINT32 mask;
}UrcIndMsg_st;

typedef struct
{
  unsigned char isenable;      /*1:hotplug is enabled 0: hotplug is disabled */
  unsigned char trigger_mode;  /*1:high level trigger 0:Low level trigger*/
}HotSwapFileType_st;

typedef struct
{
  char spn[64];/*Service provider name from SIM*/
  char mcc[4];/*Mobile Country Code*/
  char mnc[4];/*Mobile Network Code*/
}Hplmn_st;

typedef SC_simcard_err_e SC_SIM_RETURN_ERRCODE;

#endif

