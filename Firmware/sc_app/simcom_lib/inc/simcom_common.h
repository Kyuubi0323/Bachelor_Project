/**
  ******************************************************************************
  * @file    simcom_common.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of common definition.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIM_COMMON_H_
#define _SIM_COMMON_H_

#include "simcom_os.h"


/* mask definition for msg.arg1 in cus_urc.c*/
#define SC_MODULE_NONE               (0)
#define SC_URC_PB_MASK               (0x01 << 0)
#define SC_URC_SMS_MASK              (0x01 << 1)
#define SC_URC_SIM_MASK              (0x01 << 2)
#define SC_URC_NETSTATUE_MASK        (0x01 << 3)
#define SC_URC_INTERNAL_AT_RESP_MASK (0x01 << 4)
#define SC_URC_WIFI_SCAN_MASK		 (0x01 << 5)
#define SC_URC_GNSS_MASK	    	 (0x01 << 6)
#define SC_URC_CALL_MASK	    	 (0x01 << 7)
#define SC_URC_AUDIO_MASK            (0x01 << 8)
#define SC_URC_TTS_MASK              (0x01 << 9)
#define SC_URC_SIM2_MASK             (0x01 << 10)

#define SC_MODULE_ALL              (0XFFFFFFFF)

/* mask definition for msg.arg2 */
enum sim_urc_code
{
    /*URC_PB_MASK*/
    SC_URC_PBDOWN          = 0,

    /*URC_SMS_MASK*/
    SC_URC_SMSDWON,
    SC_URC_SMSFULL,
    SC_URC_STATUS_REPORT,
    SC_URC_NEW_MSG_IND,
    SC_URC_FLASH_MSG,

    /*URC_CPIN_MASK*/
    SC_URC_CPIN_READY,
    SC_URC_CPIN_REMOVED,
    SC_URC_SIM_IS_LOCK,
    SC_URC_SIM_IS_BLOCKD,
    SC_URC_SIM_IS_CRASH,

    /*URC_NETSTATUE_MASE*/
    SC_URC_NETACTED,
    SC_URC_NETDIS,
    SC_URC_PDP_ACTIVE,
    SC_URC_PDP_DEACT,
    /*URC_WIFI_SCAN*/
	SC_URC_WIFI_SCAN,

    /*URC_GNSS_MASK*/
    SC_URC_GPS_INFO,
    SC_URC_GNSS_INFO,
    SC_URC_NMEA_DATA,

    /*URC_CALL_MASK*/
    SC_URC_RING_IND,
    SC_URC_DTMF_IND,

    /*URC_AUDIO_MASK*/
    SC_URC_AUDIO_START,
    SC_URC_AUDIO_STOP,
    SC_URC_CREC_START,
    SC_URC_CREC_STOP,
    SC_URC_CREC_FIFULL,

    /*URC_TTS_MASK*/
    SC_URC_TTS_IND,
};

enum appChannelType{
    SC_SRV_NONE = 50,
    SC_SRV_FTPS,
    SC_SRV_HTTPS,
    SC_SRV_TCPIP,
    SC_SRV_MQTT,
    SC_SRV_HTP,
    SC_SRV_NTP,
    SC_SRV_SSL,
    SC_SRV_CCH,
    SC_SRV_FOTA,
    SC_SRV_FS,
    SC_SRV_LBS,
    SC_SRV_MAX
};


enum sim_msg_id
{
    SIM_MSG_INIT = 0,
    SRV_CREATER,
    SRV_SELF,
    SRV_NET_CONTROL,
    SRV_URC,
    SRV_UART,
    SRV_UART3,
	SRV_USB_VCOM,
    SRV_SMS,
    SIM_MSG_MAX,
};

#define MAKE_MSG_ID_BY_SRV(srv) (UINT32)(SRV_CREATER & ((UINT32)srv << 8))
#define GET_SRV_FROM_MSG_ID(id) (UINT8)(0xff & ((UINT32)id >> 8))
#define IS_SRV_CREATER(id) (SRV_CREATER == (SRV_CREATER & id))




#endif

