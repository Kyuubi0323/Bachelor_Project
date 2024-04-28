/**
  ******************************************************************************
  * @file    simcom_wifi.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of wifi scan feature, only support wifi RX to scan nearby wifi hotspot mac address\rssi ,etc.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef __SIMCOM_WIFI_H__
#define __SIMCOM_WIFI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define SC_WIFI_INFO_NUMBERS_MAX (30)

enum
{
    SC_WIFI_EVENT_SCAN_STOP,
    SC_WIFI_EVENT_SCAN_INFO,
};

typedef struct
{
    unsigned char mac_addr[6];
    unsigned char padding[2];
    int rssi;
    unsigned int channel_number;
} SC_WIFI_INFO_T;

typedef struct
{
    unsigned char type;
    union
    {
        SC_WIFI_INFO_T info;
    };
} SC_WIFI_EVENT_T;

typedef struct SC_WIFI_SCANPARAM_S
{
    unsigned char round_number;     //from 1 to 3. default is 3.
    unsigned char max_bssid_number; //from 4 to 10。 default is 5
    unsigned char timeout;         // uint:second。 default is 25.
    unsigned char priority;         //the prioprity of PS and wifi. 0 is PS, 1 is wifi. default is 0.
} SC_WIFI_SCANPARAM_T;

/*
 * 原型:      sAPI_WifiSetHandler
 * 功能:  设置WIFI处理函数回调。WIFI的所有操作执行结束后，将通过回调函数通知外部业务。
 *
 * 参数:
 *      handler:    回调函数原型。
 *
 * 返回值：null。
 */
void sAPI_WifiSetHandler(void (*handler)(const SC_WIFI_EVENT_T *param));

/*
 * 原型:      sAPI_WifiSetScanParam();
 * 功能:  设置扫描参数。
 *
 * 参数:
 *      param: wifi扫描可配置参数。
 *
 * 返回值：null。
 */
void sAPI_WifiSetScanParam(const SC_WIFI_SCANPARAM_T *param);

/*
 * 原型:      sAPI_WifiScanStart
 * 功能:  开启WIFI扫描功能。
 *
 * 参数:
 *      null。
 *
 * 返回值：null。
 */
void sAPI_WifiScanStart(void);

/*
 * 原型:      sAPI_WifiScanStop
 * 功能:  停止WIFI扫描功能。
 *
 * 参数:
 *      null。
 *
 * 返回值：null。
 */
void sAPI_WifiScanStop(void);

#ifdef __cplusplus
}
#endif

#endif
