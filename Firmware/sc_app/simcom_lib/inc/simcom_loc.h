/**
  ******************************************************************************
  * @file    simcom_loc.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of LBS function.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef SIMCOM_LBS_H
#define SIMCOM_LBS_H

#include "simcom_os.h"


typedef enum {

  SC_LBS_SUCCESS,
  SC_LBS_FAIL, 
  SC_LBS_INVALID_PARAMETER,
  SC_LBS_SIMCARD_NOT_READY,
  SC_LBS_RESULT_NETWORK_ERROR,
  SC_LBS_GET_LOC_FAIL,
  SC_LBS_ERROR_END
}SC_lbs_err_e;


typedef enum{
    SC_LBS_GET_LONLAT           = 1,
    SC_LBS_GET_DETAILADDRESS    = 2,
    SC_LBS_GET_ERRNO            = 3,
    SC_LBS_GET_LONLATTIME       = 4,
    SC_LBS_TEST                 = 5,
    SC_LBS_DEMO_MAX             = 99
}SC_LBS_DEMO_TYPE;

typedef struct
{   
    unsigned char 	ver[6];                     	//<version -6 byte
    unsigned long   u32SequId;                		//<sequence_id - 4 byte
    unsigned char   u8ErrorCode;              		//<ret - 1 byte
    unsigned long   u32Lng;                  		//<lng - 4 byte
    unsigned long   u32Lat;                  		//<lat - 4 byte
    unsigned short  u16Acc;                   		//<acc - 2 byte
    unsigned char   u8LocAddress[150];          	//<addr - SIMCOM_LBS_DETAILADDR_LEN
    unsigned long   u32AddrLen;                     //<addrlen - 4 byte
    unsigned char   u8DateAndTime[40];         		//<dataandtime - SIMCOM_LBS_DATAANDTIME_LEN
    unsigned long   u32QueryAmount;                 //<amout - 4 byte
}SC_lbs_info_t;



typedef  SC_lbs_err_e SC_LBS_RETURNCODE;


#endif




