/**
  ******************************************************************************
  * @file    simcom_network.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of network management related APIs.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_NETWORK_H_
#define _SIMCOM_NETWORK_H_

#include "simcom_os.h"
#define APIHandle 0XFF


#define SC_GET_CSQ_MASK                 (0x01<<SC_GET_CSQ)
#define SC_GET_CREG_MASK                (0x01<<SC_GET_CREG)
#define SC_GET_CGREG_MASK               (0x01<<SC_GET_CGREG)
#define SC_GET_CPSI_MASK                (0x01<<SC_GET_CPSI)
#define SC_GET_CNMP_MASK                (0x01<<SC_GET_CNMP)
#define SC_GET_COPS_MASK                (0x01<<SC_GET_COPS)
#define SC_GET_CGDCONT_MASK             (0x01<<SC_GET_CGDCONT)
#define SC_GET_CGACT_MASK               (0x01<<SC_GET_CGACT)
#define SC_GET_CGATT_MASK               (0x01<<SC_GET_CGATT)
#define SC_GET_CGPADDR_MASK             (0x01<<SC_GET_CGPADDR)
#define SC_GET_CPIN_MASK                (0x01<<SC_GET_CPIN)
#define SC_GET_CFUN_MASK                (0x01<<SC_GET_CFUN)
#define SC_SET_CFUN_MASK                (0x01<<SC_SET_CFUN)
#define SC_GET_IMSI_MASK                (0x01<<SC_GET_IMSI)
#define SC_GET_HPLMN_MASK               (0x01<<SC_GET_HPLMN)
#define SC_GET_CNETCI_MASK              (0x01<<SC_GET_CNETCI)
#define SC_SET_CPIN_MASK                (0x01<<SC_SET_CPIN)
#define SC_SET_CLCK_MASK                (0x01<<SC_SET_CLCK)



/**  \brief Encoded bit error rate (BER) indication */
/** \remarks Common Data Section */
typedef UINT8 SCciMmEncodedBER; /**< Reported as an index between 0-7, inclusive  */

/** \brief  Signal quality indications: information structure */
typedef struct SCciMmSigNormalQualityInfo_struct {
    UINT8       Rssi; 
    UINT8       Ber;
} SCciMmSigNormalQualityInfo;

typedef struct{
    char networkmode[40];
    char Mnc_Mcc[20];
    int LAC;
    int CellID;
    char GSMBandStr[20];
    char LTEBandStr[20];
    int TAC;
    int Rsrp;
    int Rsrq;
    int Rssi;
    int RXLEV;
    int TA;
    int SINR;
    int dlEuArfcn;
    int subframeAssignment;
    int systemFrameNumber;
    int pCellID;
}SCcpsiParm;


typedef struct{
    char Mnc_Mcc[20];
    int CellID;
    int TAC;
    int Rsrp;
    int Rsrq;
    int RXSIGLEVEL;
}SCcnetciParm;

typedef struct{
    UINT8 cid;
    UINT8 iptype;/*IPV4 == 1   ipv6 == 2   ipv4v6 == 3  if iptype == 0,inactive IP*/
    char ipv4addr[16];
    char ipv6addr[64];
}SCcgpaddrParm;


enum SCnetworkid
{
    SC_GET_CSQ = 0,
    SC_GET_CREG,
    SC_GET_CGREG,
    SC_GET_CPSI,
    SC_GET_CNMP,
    SC_GET_COPS,
    SC_GET_CGDCONT,
    SC_GET_CGACT,
    SC_GET_CGATT,
    SC_GET_CGPADDR,
    SC_GET_CPIN,
    SC_GET_CFUN,
    SC_SET_CFUN,
    SC_GET_IMSI,
    SC_GET_HPLMN,
    SC_GET_CNETCI,
    SC_SET_CPIN,
    SC_SET_CLCK
};
enum SCnetworkSF
{
    SC_NET_SUCCESS = 0,
    SC_NET_FAIL
};

enum SCsimcardstate
{
	SC_SIM_READY = 0,
	SC_SIM_PIN,
	SC_SIM_PUK,
	SC_SIM_BLOCKED,
	SC_SIM_REMOVED,
	SC_SIM_CRASH,
	SC_SIM_NOINSRETED,
	SC_SIM_UNKNOW
};

typedef struct{
    UINT8 cid;
    UINT8 iptype;/*IPV4 = 1   ipv6 = 2   ipv4v6 = 3  if iptype = 0,inactive*/
    char ApnStr[40];
}SCApnParm;

typedef struct{
    UINT8 cid;
    UINT8 authtype;
    char user[50];
    char passwd[50];
}SCCGAUTHParm;

typedef struct{
    UINT8 cid;
    UINT8 isActived;/*0:deactivated   1:activated*/
    UINT8 isdefine;/*0:Nodefine   1:defined*/
}SCAPNact;

typedef struct{
	unsigned long long Rx_Bytes;
	unsigned long long Tx_Bytes;
	unsigned long TX_Packets;
    unsigned long RX_Packets;
	unsigned long Rx_Dropped_Packets;
    unsigned long Tx_Dropped_Packets;
}SCstatisticsData;

#endif
