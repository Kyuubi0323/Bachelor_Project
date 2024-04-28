/**
  ******************************************************************************
  * @file    simcom_https.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of HTTPS function.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef SIMCOM_HTTPS_H
#define SIMCOM_HTTPS_H

#include "simcom_os.h"

typedef struct
{
    INT32 status_code;  		/*process result for request msg*/
    INT32 method;      			/*for sAPI_HttpAction*/
    INT32 action_content_len;   /*the recv lenth of sAPI_HttpAction and sAPI_HttpPostfile*/
    UINT8 *data;     			/*the data trans from API*/
    INT32 dataLen;   			/*the datalen of data*/
} SChttpApiTrans;

typedef enum
{
    SC_HTTPS_SUCCESS,
    SC_HTTPS_FAIL,
    SC_HTTPS_SERVICE_NOT_AVAILABLE,
    SC_HTTPS_INVALID_PARAMETER,
    SC_HTTPS_FILE_NOT_EXIST,
    SC_HTTPS_WRITE_FILE_FAIL,
    SC_HTTPS_READ_FILE_FAIL,
    SC_HTTPS_DNS_PARSE_FAIL,
    SC_HTTPS_CONNECT_FAIL,
    SC_HTTPS_HANDSHAKE_FAILED,
    SC_HTTPS_SSL_CLOSE_FAILED,
    SC_HTTPS_TRANSFER_ERROR,     //http receive data or send fail.
    SC_HTTPS_ERROR_END
} SC_HTTP_RETURNCODE;
#endif
