/**
  ******************************************************************************
  * @file    simcom_sms.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of SMS functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_SMS_H_
#define _SIMCOM_SMS_H_


#include "simcom_common.h"
#include "simcom_os.h"
#define SC_SMS_MAX_ADDRESS_LENGTH               40
#define SC_SMS_MAX_CI_MSG_PDU_SIZE             400
#define SC_SMS_MAX_MULTIPLE_MSG_LENGTH         820


typedef enum{
    SC_CI_SMS_PRIM_READ_MESAGE_REQ,
    SC_CI_SMS_PRIM_READ_MESAGE_CNF,
    SC_CI_SMS_PRIM_READ_PDU_MESSAGE_REQ,
    SC_CI_SMS_PRIM_READ_PDU_MESSAGE_CNF,
    SC_CI_SMS_PRIM_READ_TXT_MESSAGE_REQ,
    SC_CI_SMS_PRIM_READ_TXT_MESSAGE_CNF,//5
    SC_CI_SMS_PRIM_SET_PREFSTROGE_REQ,
    SC_CI_SMS_PRIM_SET_PREFSTROGE_CNF,
    SC_CI_SMS_PRIM_GET_PREFSTROGE_REQ,
    SC_CI_SMS_PRIM_GET_PREFSTROGE_CNF,
    SC_CI_SMS_PRIM_GET_SRCADDR_REQ,//10
    SC_CI_SMS_PRIM_GET_SRCADDR_CNF,
    SC_CI_SMS_PRIM_SEND_MSG_REQ,
    SC_CI_SMS_PRIM_SEND_MSG_CNF,
    SC_CI_SMS_PRIM_SEND_STROG_MSG_REQ,
    SC_CI_SMS_PRIM_SEND_STROG_MSG_CNF,//15
    SC_CI_SMS_PRIM_WRITE_MSG_REQ,
    SC_CI_SMS_PRIM_WRITE_MSG_CNF,
    SC_CI_SMS_PRIM_SET_SMSC_ADDR_REQ,
    SC_CI_SMS_PRIM_SET_SMSC_ADDR_CNF,
    SC_CI_SMS_PRIM_GET_SMSC_ADDR_REQ,//20
    SC_CI_SMS_PRIM_GET_SMSC_ADDR_CNF,
    SC_CI_SMS_PRIM_DEL_MSG_REQ,
    SC_CI_SMS_PRIM_DEL_MSG_CNF,
    SC_CI_SMS_PRIM_CONFIG_MSG_IND_REQ,
    SC_CI_SMS_PRIM_CONFIG_MSG_IND_CNF,
    SC_CI_SMS_PRIM_GET_MSG_IND_REQ,
    SC_CI_SMS_PRIM_GET_MSG_IND_CNF,
    SC_CI_SMS_PRIM_DEL_ONE_MSG_REQ,
    SC_CI_SMS_PRIM_DEL_ONE_MSG_CNF,
    SC_CI_SMS_PRIM_INVALID

}SC_CI_MSG_PRIMID;

/*API cmd type*/
typedef enum{
    SC_SMS_INVALID,
    SC_SMS_WRITE,
    SC_SMS_READ,
    SC_SMS_SND_CMGS,
    SC_SMS_SND_CMSS,
    SC_SMS_SND_CMGSEX,
    SC_SMS_DELALL,
    SC_SMS_DELONE,
    SC_SMS_TYPE_MAX
}SC_SMS_CMD_TYPE;



typedef enum{
    SC_SMS_READY_IND,
    SC_MSG_STATUS_REPORT_IND,//+CDS
    SC_MSG_DELIVER_REPORT_IND,//CMT CDS

}SC_SMS_EVENT_TYPE;


typedef enum{
    SC_SMS_SUCESS,
    SC_SMS_TIMEOUT,
    SC_SMS_PARAM_INVALID,
    SC_SMS_UNKNOWN_ERR,
    SC_SMS_OS_ERR,
    SC_SMS_OPERATE_NOTSUPPORT,
    SC_SMS_BUSY,
    SC_SMS_INVALID_INDEX,
    SC_SMS_MEMORY_FULL,
    SC_SMS_UNREADY,
    SC_SMS_FAIL

}SC_SMSReturnCode;

typedef enum{
    SC_SMS_FREE,
    SC_SMS_READY,
    SC_SMS_READ_CMD,
    SC_SMS_DEL_CMD
}SC_SMS_Status;


/*the response structure*/
typedef struct{
    int primId;
    int resultCode;
    INT8 *repStr;
}SC_SMS_RESP;




typedef UINT8 SCsmsFormatMode;


//CiMsgPrimReadMessageCnf
typedef struct
{
	UINT8     tsYear;
	UINT8     tsMonth;
	UINT8     tsDay;
	UINT8     tsHour;
	UINT8     tsMinute;
	UINT8     tsSecond;
	UINT8     tsTimezone;
	UINT8     tsZoneSign;
}SCsmsTimeStamp;

/* Struct used in readSmsDeliverPdu() */
typedef struct {
    UINT32 type;                            // @field DCS type
    UINT32 msgClass;                   // @field message class (Only for RIL_DCSTYPE_GENERAL and RIL_DCSTYPE_MSGCLASS)
    UINT32 alphabet;                   // @field DCS alphabet
    UINT32 indication;                 // @field indication (Only for RIL_DCSTYPE_MSGWAIT)
} SCsmsDcs;

typedef struct{
    UINT8 rc;
    UINT8 indexorref;
}SCsmsWriteSendCnf;

typedef struct {
    UINT16 resultcode;
    SCsmsFormatMode formatmode;

    UINT8  status;
    UINT16 len;
    INT8 smscBuf[SC_SMS_MAX_ADDRESS_LENGTH * 2 + 1];
    INT8 data[SC_SMS_MAX_ADDRESS_LENGTH];
}SCsmsPduReadCnf;

typedef enum{
    SMS_DELIVER,
    SMS_SUBMIT
}Smstype;

typedef struct{
  
    UINT8         Length;                   /* Address Length in digits */
    UINT8 Digits[ SC_SMS_MAX_ADDRESS_LENGTH];  /* Address Digits */
	
} SCsmsAddressInfo;


typedef struct{
    UINT16 resultcode;
    UINT8  status;	
    SCsmsFormatMode fmtmode;
    Smstype type;
    UINT8  srcAddr[ SC_SMS_MAX_ADDRESS_LENGTH + 1];
    UINT8 Pid;
    UINT8 fo;
    UINT8 Dsc;
    UINT8 vp;
    SCsmsTimeStamp   timeStamp;
    SCsmsDcs bDCS;
    UINT32 msglen;
    INT8 msgbuf[512];
    SCsmsAddressInfo srvCenterAddr;
    
}SCsmsPayload;


typedef enum{
    SC_SMS_STROGE_ME = 1,
    SC_SMS_STROGE_SM = 3,
    SC_SMS_STROGE_MAX
}SC_SMS_Stroge;

typedef struct{
    UINT8 mem1;
    UINT8 mem2;
    UINT8 mem3;
}SCsmsStroge;



typedef struct{
    UINT8 used;
    UINT8 tatal;
    UINT8 storage;
}SCsmsStrogeInfoT;
typedef SCsmsStrogeInfoT SCsmsStrogeInfo[3];







#endif






