/**
  ******************************************************************************
  * @file    simcom_ping.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of ping function.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_PING_H_
#define _SIMCOM_PING_H_

//callback
typedef void (*pingResultCallback)(UINT32 size,INT8 *statistics);

typedef enum{
    SC_PING_SUCESSED,
    SC_PING_DOMAIN_UNKNOW,
    SC_PING_PRAM_INVALID,
    SC_PING_OPERATION_BUSY,
    SC_PING_SEND_ERR,
    SC_PING_RECV_ERR,
    SC_PING_FAIL
    
}SCPingResultCode;


typedef struct{
    INT8 *destination;
    UINT32  interval; //Specify the interval between sending and receiving messages
    UINT32 count;     //Stop after sending count ECHO_REQUEST packets. With deadline option, ping waits for count ECHO_REPLY packets, until the timeout expires.
    pingResultCallback resultcb;/*callback fun, for print result*/
}SCping;



#endif


