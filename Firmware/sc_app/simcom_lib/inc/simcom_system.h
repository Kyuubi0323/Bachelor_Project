/**
  ******************************************************************************
  * @file    simcom_system.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of system APIs.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef _SIMCOM_SYSTEM_H_
#define _SIMCOM_SYSTEM_H_

 
#define CUS_VERSION       "NULL"

typedef struct
{
	unsigned char               DeycrptNumber[16];
	unsigned char               UserData[16];
	unsigned char               EncryptNumber[16];
	unsigned long               UserKey[26];
	unsigned long               SubKey[26];
	unsigned char               ImeiEncrypt;
} IMEIExtendedDataS;

typedef struct
{
    char *manufacture_namestr;
    char *Module_modelstr;
    char *Revision;
    char *GCAP;
    char *cgmr;
    char *internal_verstr;
    char *version_tagstr;
    char *SDK_Version;
}SIMComVersion;

typedef struct
{
    char sdkversion[50];
}SDKVersion;

typedef struct
{
    char rfversion1[50];
    char rfversion2[50];
}RFVersion;

typedef struct
{
    char cusversion[50];
}CUSVersion;

typedef enum
{
    SC_SYSTEM_SLEEP_DISABLE     =   0,
    SC_SYSTEM_SLEEP_ENABLE      =   1
}SC_SYSTEM_SLEEP_FLAG;

typedef struct
{
  SC_SYSTEM_SLEEP_FLAG sleep_flag;
  unsigned char time;
}SC_SleepEx_str;
#endif
