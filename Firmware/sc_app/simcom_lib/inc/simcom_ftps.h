/**
  ******************************************************************************
  * @file    simcom_ftps.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of FTPS function.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef SIMCOMFTPS_H
#define SIMCOMFTPS_H
#include "simcom_os.h"
/*
    SIMCOM_FTPS_ERROR_CODE
*/
typedef enum {
    SC_FTPS_RESULT_OK,// 0
    SC_FTPS_RESULT_SSL_ALERT,// 1
    SC_FTPS_RESULT_UNKNOWN_ERROR, // 2
    SC_FTPS_RESULT_ERROR_BUSY,    // 3
    SC_FTPS_RESULT_SERVER_CLOSED,  // 4
    SC_FTPS_RESULT_ERROR_TIMEOUT,// 5
    SC_FTPS_RESULT_ERROR_TRANSFER_FAILED,   // 6  /*receive/send socket data failed*/ 
    SC_FTPS_RESULT_ERROR_MEMORY_ERROR,// 7
    SC_FTPS_RESULT_ERROR_INVALID_PARAM,  // 8
    SC_FTPS_RESULT_ERROR_REJ_BY_SERVER,  // 9
    SC_FTPS_RESULT_NETWORK_ERROR,  //10
    SC_FTPS_RESULT_STATE_ERROR,// 11
    SC_FTPS_RESULT_FAILED_TO_PARSE_SERVER_NAME, // 12
    SC_FTPS_RESULT_FAILED_TO_CREATE_SOCKET, // 13
    SC_FTPS_RESULT_FAILED_TO_CONNECT_SOCKET, //14
    SC_FTPS_RESULT_FAILED_TO_CLOSE_SOCKET, // 15
    SC_FTPS_RESULT_FAILED_TO_PEER_CLOSED,// 16
    SC_FTPS_RESULT_ERROR_FILE_ERROR,// 17
    SC_FTPS_DO_NOTHING,
    SC_FTPS_RESULT_ERR_SESSION_NULL,
    SC_FTPS_RESULT_ERR_CLOSE_SSL,
    SC_FTPS_RESULT_SERVER_CONN_TIMEOUT_ERROR = 421,
    SC_END_OF_MFTP_RESULT
}SC_FTPS_ERROR_CODE;
/*
    STRUCT DEFINE
*/
typedef struct{
    char host[256]; /*xx.xx.xx.xx:port, or URL*/
    char username[256]; /*note: qijun require 256, need turn*/
    char password[256];
    INT32 port;
    UINT8 serverType; /*0: FTP client, 1:FTPS SSL client 2:FTPS TLS client 3:implicit FTPS client*/
}SCftpsLoginMsg;
/*
    PDP ACTIVE TYPE
*/
typedef enum {
    SC_FTPS_USB,
    SC_FTPS_UART
}SC_PDP_ACTIVE_TYPE;
/*
    FILE LOCATION
*/
typedef enum{
    SC_FTPS_FILE_FLASH = 1,
    SC_FTPS_FILE_SDCARD,
    SC_FTPS_FILE_EXT_FLASH_FS
}SC_FTPS_FILE_LOCATION;
/*
    DATA SOCKET IP TYPE
*/
typedef enum{
    SC_PORT_RESPONSE_BY_SERVER,
    SC_SAME_AS_CONTROLSOCKET_IP
}SC_FTPS_DATASOCKET_IP_TYPE;

/*
    API OPERATION TYPE
*/    
typedef enum{
    SC_FTPS_INIT,
    SC_FTPS_DEINIT,
    SC_FTPS_LOGIN,
    SC_FTPS_LOGOUT,
    SC_FTPS_DOWNLOADFILE,
    SC_FTPS_DOWNLOADTOBUFFER,
    SC_FTPS_UPLOADFILE,
    SC_FTPS_DELETEFILE,
    SC_FTPS_CREATDIR,
    SC_FTPS_DELETEDIR,
    SC_FTPS_CHANGEDIR,
    SC_FTPS_GETDIR,
    SC_FTPS_LIST,
    SC_FTPS_SIZE,
    SC_FTPS_SETTYPE,
    SC_FTPS_GETTYPE,
    SC_FTPS_SSLCONFIG
}SC_FTPS_API_TYPE;

/*
    DATA FLAG
*/  
typedef enum{
    SC_DATA_COMPLETE,
    SC_DATA_RESUME
}SC_FTPS_DATA_FLAG;

typedef struct{
    SC_FTPS_DATA_FLAG flag;
    char *data;
    UINT32 len;
}SCapiFtpsData;

typedef SC_FTPS_ERROR_CODE SC_FTPS_RETURNCODE;

#endif


