/**
  ******************************************************************************
  * @file    simcom_mqtts_client.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of MQTTS client function.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef SIMCOM_MQTTS_CLIENT_H
#define SIMCOM_MQTTS_CLIENT_H

#include "simcom_os.h"

//MQTT return code
typedef enum
{
    SC_MQTT_RESULT_SUCCESS = 0,   
    SC_MQTT_RESULT_FAIL,
    SC_MQTT_RESULT_BAD_UTF8_STR,
    SC_MQTT_RESULT_SOCK_CONN_FAIL,
    SC_MQTT_RESULT_SOCK_CREATE_FAIL,
    SC_MQTT_RESULT_SOCK_CLOSE_FAIL, //5
    SC_MQTT_RESULT_RCV_FAIL,
    SC_MQTT_RESULT_NETWORK_OPEN_FAIL,
    SC_MQTT_RESULT_NETWORK_CLOSE_FAIL,
    SC_MQTT_RESULT_NETWORK_NO_OPEN,
    SC_MQTT_RESULT_CLINET_INDEX_ERR, //10
    SC_MQTT_RESULT_NO_CONNECTION,
    SC_MQTT_RESULT_INVALID_PARAMETER,    
    SC_MQTT_RESULT_OPERATION_NOT_SUPPORT,
    SC_MQTT_RESULT_BUSY,
    SC_MQTT_RESULT_REQ_CONNECTION_FAIL, //15
    SC_MQTT_RESULT_SOCK_SENDING_FAIL,
    SC_MQTT_RESULT_TIMOUT,
    SC_MQTT_RESULT_TOPIC_EMPTY,
    SC_MQTT_RESULT_CLIENT_IN_USE,
    SC_MQTT_RESULT_CLIENT_NOT_ACCQ, //20
    SC_MQTT_RESULT_CLIENT_NOT_REL,
    SC_MQTT_RESULT_EXCEED_MAX_VAL,
    SC_MQTT_RESULT_NETWORK_HAVE_OPENED,
    SC_MQTT_RESULT_PACKET_FAIL,
    SC_MQTT_RESULT_DNS_ERROR,
    SC_MQTT_RESULT_SOCK_CLOSE,// 26  socket closed by server
    SC_MQTT_RESULT_UNACCEPTED_PROTOCOL_VER, //27 - connection refused: unaccepted protocol version
    SC_MQTT_RESULT_ID_REJECTED, //28  - connection refused: identifier rejected
    SC_MQTT_RESULT_SER_UNVAILBLE, //29  - connection refused: server unavailable
    SC_MQTT_RESULT_BAD_USRNAME_PWD, //30 - connection refused: bad user name or password
    SC_MQTT_RESULT_NOT_AUTHORIZED, // 31 - connection refused: not authorized
    SC_MQTT_RESULT_SSL_HANDSHAKE_ERR, //32 for ssl
    SC_MQTT_RESULT_NOT_SET_CERTS,//33 for ssl
    SC_MQTT_RESULT_OPEN_SESSION_ERR,//34 for ssl
    SC_MQTT_RESULT_DISCONN_FAIL,
    SC_MQTT_RESULT_MAX
}SCmqttResultType;

//MQTT operation code
typedef enum {
    SC_MQTT_OP_SET = 0,
    SC_MQTT_OP_GET,
}SCmqttOperationType;


/**************************************************************************************************************************/
/*************************************************parameters defination********************************************************/
typedef SCmqttResultType  SCmqttReturnCode;

typedef void (*mqtt_connlost_cb)(int client_index, int cause);

/*********************************************************
* discription: 
*       this structure prepared for message sending 
* 
* SC_STATUS simMsgSend( OSMsgQRef   msgQRef, SIM_MSG_T   *msgPtr )
*                                                           ^
*
* SC_STATUS simMsgRecv( OSMsgQRef   msgQRef, SIM_MSG_T   *recvMsg, UINT32      timeout)
*                                                           ^
*
*********************************************************/
typedef struct{
    int client_index;
    unsigned int topic_len;
    char *topic_P;
    unsigned int payload_len;
    char *payload_P;
}SCmqttData;



/******************************************************************************************************************************/
/*************************************************API defination***************************************************************/












#endif
