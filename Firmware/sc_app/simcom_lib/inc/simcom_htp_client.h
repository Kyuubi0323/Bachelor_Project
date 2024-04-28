/**
  ******************************************************************************
  * @file    simcom_htp_client.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of HTP function,it can be used to synchronous time and date from HTTP.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef  SIMCOM_HTP_CLIENT_H
#define  SIMCOM_HTP_CLIENT_H


#include "simcom_os.h"





typedef enum
{
  SC_HTP_OK = 0,
  SC_HTP_ERROR,
  SC_HTP_UNKNOWN_ERROR, 
  SC_HTP_INVALID_PARAM,  
  SC_HTP_BAD_DATETIME_GOT,  
  SC_HTP_NETWORK_ERROR,  
  SC_HTP_END
}SChtpResultType;

typedef SChtpResultType SChtpReturnCode;

typedef enum {
    SC_HTP_OP_SET = 0,
    SC_HTP_OP_GET,
}SChtpOperationType;








#endif

