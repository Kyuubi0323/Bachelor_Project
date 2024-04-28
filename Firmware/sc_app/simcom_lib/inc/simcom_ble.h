/**
  ******************************************************************************
  * @file    simcom_ble.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of BLE peripheral.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_BLE_H__
#define __SIMCOM_BLE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "simcom_os.h"

//adv type
#define LE_ADV_TYPE_IND                 0x00
#define LE_ADV_TYPE_DIRECT_IND          0x01
#define LE_ADV_TYPE_SCAN_IND            0x02
#define LE_ADV_TYPE_NONCONN_IND         0x03
#define LE_ADV_TYPE_DIRECT_IND_LOW_DUTY 0x04

//address type
#define LE_ADDRESS_TYPE_PUBLIC          0x00
#define LE_ADDRESS_TYPE_RANDOM          0x01

//scan type
#define LE_PASSIVE_SCAN                 0x00
#define LE_ACTIVE_SCAN                  0x01

//uuid type
#define LE_UUID_TYPE_16     (2)
#define LE_UUID_TYPE_32     (4)
#define LE_UUID_TYPE_128    (16)

//ATT character property
#define LE_ATT_CHARC_PROP_READ          0x02
#define LE_ATT_CHARC_PROP_WWP           0x04
#define LE_ATT_CHARC_PROP_WRITE         0x08
#define LE_ATT_CHARC_PROP_NOTIFY        0x10
#define LE_ATT_CHARC_PROP_INDICATE      0x20

//ATT PM
#define LE_ATT_PM_READABLE              0x0001
#define LE_ATT_PM_WRITEABLE             0x0002
#define LE_ATT_PM_AUTHENT_REQUIRED      0x0004
#define LE_ATT_PM_AUTHORIZE_REQUIRED    0x0008
#define LE_ATT_PM_ENCRYPTION_REQUIRED   0x0010
#define LE_ATT_PM_AUTHENT_MITM_REQUERED 0x0020

//ATT UUID
#define LE_ATT_UUID_PRIMARY             0x2800
#define LE_ATT_UUID_CHARC               0x2803

#define LE_CCC_BIT_INDICATED            0x02
#define LE_CCC_BIT_NOTIFIED             0x01

#define LE_ATT_UUID_CLIENT_CHARC_CONFIG 0x2902

#define LE_ATT_UUID_IMMEDIATE_ALERT     0x1802
#define LE_ATT_UUID_DEVICE_INFO         0x180A
#define LE_ATT_UUID_MESH_PRO_SERVICE    0x1827
#define LE_ATT_UUID_MESH_PROXY_SERVICE  0x1828

#define LE_ATT_UUID_ALERT_LEVEL         0x2A06
#define LE_ATT_UUID_MANUFACTURER_NAME   0x2A29
#define LE_ATT_UUID_MESH_PRO_DATA_IN    0x2ADB
#define LE_ATT_UUID_MESH_PRO_DATA_OUT   0x2ADC
#define LE_ATT_UUID_MESH_PROXY_DATA_IN  0x2ADD
#define LE_ATT_UUID_MESH_PROXY_DATA_OUT 0x2ADE

typedef enum
{
    SC_BLE_RETURNCODE_OK,
    SC_BLE_RETURNCODE_NOT_KNOW_ERROR,
    SC_BLE_RETURNCODE_ALERT,
    SC_BLE_RETURNCODE_PARAM_ERROR,
    SC_BLE_RETURNCODE_OPEN_FAIL,
    SC_BLE_RETURNCODE_MSGQ_ERROR,
} SC_BLE_RETURNCODE_T;

typedef enum
{
    BLE_ADV_DATA_TYPE_FLAG = 0x01,
    BLE_ADV_DATA_TYPE_COMPLETE_SERVICE_LIST = 0x03,
    BLE_ADV_DATA_TYPE_SHORT_NAME = 0x08,
    BLE_ADV_DATA_TYPE_COMPLETE_NAME = 0x09,
    BLE_ADV_DATA_TYPE_APPEARANCE = 0x19,
    BLE_ADV_DATA_TYPE_SPECIFIC_DATA = 0xff,
} SC_BLE_ADV_DATATYPE_T;

typedef enum
{
    BLE_SCAN_EVENT,
    BLE_CONNECT_EVENT,
    BLE_DIS_CONNECT_EVENT,
    BLE_INDICATE_EVENT,
    BLE_ERROR_RESPONSE_EVENT,
    BLE_MTU_EXCHANGED_EVENT,
    BLE_CLIENT_MTU_EXCHANGED_EVENT,
    BLE_CLIENT_READ_BY_GROUP_TYPE_RSP_EVENT,
    BLE_CLIENT_READ_BY_TYPE_RSP_EVENT,
    BLE_CLIENT_FIND_INFOMATION_RSP_EVENT,
    BLE_CLIENT_READ_RSP_EVENT,
    BLE_CLIENT_HANDLE_NOTIFY_EVENT,
    BLE_CLIENT_HANDLE_INDIATION_EVENT
} SC_BLE_EVENT_TYPE_T;

typedef struct
{
    unsigned char bytes[6];
} SC_BLE_ADDR_T;

typedef struct
{
    unsigned short int interval_min;
    unsigned short int interval_max;
    unsigned char advertising_type; // LE_ADV_TYPE_XXX
    unsigned char own_address_type;
    unsigned char peer_address_type;
    SC_BLE_ADDR_T peer_address;
    unsigned char filter; /* 0x00: process scan and connection request from all devices
                             0x01: process connection request from all devices
                                   and scan request only from White List
                             0x02: process scan request from all devices
                                   and conneciton request only from White List
                             0x03: process scan and connection reques only from in the White List
                          */
} SC_BLE_ADV_PARAM_T;

typedef struct
{
    unsigned char type;
} SC_UUID_T;

typedef struct
{
    SC_UUID_T type;
    unsigned short value;
} SC_UUID_16_T;

typedef struct
{
    SC_UUID_T type;
    unsigned char value[16];
} SC_UUID_128_T;

typedef struct
{
    union
    {
        SC_UUID_16_T uuid_16;
        SC_UUID_128_T uuid_128;
    };
} SC_UUID_COMMON_T;


typedef struct
{
    SC_UUID_T *uuid;
    void *value;
    int size;
    unsigned short handle;
    unsigned char permission;
    int (*write_cb)(void *arg);
    int (*read_cb)(void *arg);
} SC_BLE_SERVICE_T;

typedef struct
{
    unsigned short int event_type;
    unsigned short int event_id;
    int payload_length;
    void *payload;
} SC_BLE_EVENT_MSG_T;

#pragma pack(1)
typedef struct
{
    unsigned char properties;
    unsigned short handle;
    unsigned short uuid;
} SC_BLE_CHARACTERISTIC_16_T;

typedef struct
{
    unsigned char properties;
    unsigned short handle;
    unsigned char uuid[16];
} SC_BLE_CHARACTERISTIC_128_T;
#pragma pack()

typedef struct
{
    unsigned short handle;
    unsigned short length;
    unsigned short offset;
    unsigned char *data;
} SC_BLE_RW_T;

typedef struct
{
    unsigned char adv_type;         //广播类型
    unsigned char address_type;     //设备的地址类型
    SC_BLE_ADDR_T address;          //设备的mac地址
    unsigned char length;           //广播包长度
    unsigned char data[31];         //广播包长度
    char rssi;                      //信号强度
} SC_BLE_SCAN_EVENT_T;

typedef struct
{
    SC_BLE_ADDR_T address;
    unsigned char address_type;
    int acl_handle;
    int role;
} SC_BLE_CONNECT_EVENT_T;

typedef struct
{
    int mut;
    int acl_handle;
} SC_BLE_MTU_EXCHANGE_EVENT_T;

typedef struct
{
    unsigned char size;
    unsigned char value[255];
} SC_BLE_READ_BY_GROUP_TYPE_EVENT_T;

typedef struct
{
    unsigned char size;
    unsigned char value[255];
} SC_BLE_READ_BY_TYPE_EVENT_T;

typedef struct
{
    unsigned char size;
    unsigned char value[255];
} SC_BLE_FIND_INFORMATION_EVENT_T;

typedef struct
{
    unsigned char size;
    unsigned char value[255];
} SC_BLE_READ_EVENT_T;

typedef struct
{
    unsigned short acl_handle;
    unsigned short handle;
    unsigned char value[255];
    int size;
} SC_BLE_HANDLE_VALUE_IND_EVENT_T;

typedef struct
{
    unsigned short acl_handle;
    unsigned short handle;
    unsigned char value[255];
    int size;
} SC_BLE_HANDLE_VALUE_NTF_EVENT_T;

typedef struct
{
    unsigned char request;
    unsigned short att_handle;
    unsigned char code;
} SC_BLE_ERROR_RSP_EVENT_T;

typedef int (*SC_BLE_EVENT_HANDLE_T)(SC_BLE_EVENT_MSG_T *msg);

/*
 * 原型:      sAPI_BleOpen
 * 功能:  打开ble。其它的一切操作应该在打开ble之后。
 *
 * 参数:
 *      msgQ    消息队列。flag=1，把打开ble的结果将通过它传递给外部。传递的类型为SC_BLE_RETURNCODE_T。
 *      flag    异步标志。flag=1，函数执行结果通过消息队列传递，flag=0，函数将阻塞直到获得打开ble的结果，执行结果通过返回值通知。
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleOpen(sMsgQRef msgQ, int flag);

/*
 * 原型:      sAPI_BleClose
 * 功能:  关闭ble。
 *
 * 参数:
 *
 * 返回值
 */
void sAPI_BleClose(void);

/*
 * 原型:      sAPI_BleRegisterEventHandle
 * 功能:  注册ble事件处理函数。必须在在进行广播之前设置。
 *
 * 参数:
 *      handle  ble事件处理函数。具体的事件类型请查阅SC_BLE_EVENT_TYPE_T。
 *
 * 返回值：
 */
void sAPI_BleRegisterEventHandle(SC_BLE_EVENT_HANDLE_T handle);

/*
 * 原型:      sAPI_BleCreateRandomAddress
 * 功能:  创建一个随机的mac地址。
 *
 * 参数:
 *      address ble地址类型。
 *
 * 返回值：
 */
void sAPI_BleCreateRandomAddress(SC_BLE_ADDR_T *address);

/*
 * 原型:      sAPI_BleSetAddress
 * 功能:  设置ble广播地址。应该在进行广播之前设置。
 *
 * 参数:
 *      address ble地址类型。
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleSetAddress(SC_BLE_ADDR_T *address);

/*
 * 原型:      sAPI_BleCreateAdvData
 * 功能:  根据输入的数据输出ble广播数据包。
 *
 * 参数:
 *      dataType    存入的数据类型。具体的数据类型请查阅SC_BLE_ADV_DATATYPE_T。
 *      advData     输出广播的数据包的地址。总长度应该不超过31bytes。
 *      advData     advData的大小。值应该大于length+2。
 *      data        存入的数据首地址。
 *      length      存入的数据总长度
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleCreateAdvData(int dataType, void *advData, int advDataSize, const void *data, int length);

/*
 * 原型:      sAPI_BleSetAdvData
 * 功能:  设置广播的数据包。应该在进行广播之前设置。
 *
 * 参数:
 *      advData     广播的数据包首地址。总长度应该不超过31bytes。
 *      length      数据包长度
 *
 * 返回值： 成功返回广播包的长度，失败返回-1。
 */
int sAPI_BleSetAdvData(const void *advData, int length);

/*
 * 原型:      sAPI_BleSetAdvParam
 * 功能:  设置广播的参数。应该在进行广播之前设置。
 *
 * 参数:
 *      param     广播的参数。
 *
 * 返回值： 成功返回广播包的长度，失败返回-1。
 */
int sAPI_BleSetAdvParam(SC_BLE_ADV_PARAM_T *param);

/*
 * 原型:      sAPI_BleRegisterService
 * 功能:  注册ble服务。应该在进行广播之前设置。
 *
 * 参数:
 *      service 服务存放的首地址。
 *      length  service的个数。
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleRegisterService(SC_BLE_SERVICE_T *service, int length);

/*
 * 原型:      sAPI_BleUnregisterService
 * 功能:  取消已经注册的ble服务。
 *
 * 参数:
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleUnregisterService(void);

/*
 * 原型:      sAPI_BleEnableAdv
 * 功能:  使能ble广播。
 *
 * 参数:
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleEnableAdv(void);

/*
 * 原型:      sAPI_BleDisableAdv
 * 功能:  关闭ble广播。
 *
 * 参数:
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleDisableAdv(void);

/*
 * 原型:      sAPI_BleScan
 * 功能:  扫描周围的le设备，扫描的结果通过回调handler通知。
 *
 * 参数:
 *      type                扫描采用的方式，有主动扫描(LE_ACTIVE_SCAN)和被动扫描(LE_PASSIVE_SCAN)两种
 *      interval            扫描间隔，每隔多长时间进行一次扫描，范围：0x0004-0x4000，单位：0.625msec
 *      window              扫描窗口，每次扫描的时间，范围：0x0004-0x4000，单位：0.625msec
 *      own_address_type    扫描的地址类型，有公共地址(LE_ADDRESS_TYPE_PUBLIC)和随机地址(LE_ADDRESS_TYPE_RANDOM)两种。
 *      handler             扫描的回调，将接收到扫描的结果
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleScan(unsigned char type,
                 unsigned short int interval,
                 unsigned short int window,
                 unsigned char own_address_type);

/*
 * 原型:      sAPI_BleScanStop
 * 功能:  停止扫描。
 *
 * 参数:
 *      none
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleScanStop(void);

/*
 * 原型:      sAPI_BleConnect
 * 功能:  连接到目标设备。
 *
 * 参数:
 *      addr            目标设备mac地址
 *      type            目标设置地址类型
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleConnect(SC_BLE_ADDR_T *addr, int type);

/*
 * 原型:      sAPI_BleDisconnect
 * 功能:  断开所有ble连接。
 *
 * 参数:
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleDisconnect(void);

/*
 * 原型:      sAPI_BleIndicate
 * 功能:  发送一条indicate。
 *
 * 参数:
 *      att_handle      属性的句柄，在连接成功之后从characteristic中获得。
 *      data            发送的数据
 *      size            数据的长度
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleIndicate(unsigned short att_handle, const void *data, int size);

/*
 * 原型:      sAPI_BleNotify
 * 功能:  发送一条notify。
 *
 * 参数:
 *      att_handle      属性的句柄，在连接成功之后从characteristic中获得。
 *      data            发送的数据
 *      size            数据的长度
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleNotify(unsigned short att_handle, const void *data, int size);

/*
 * 原型:      sAPI_BleMtuRequest
 * 功能:  发起一个mtu请求
 *
 * 参数:
 *      mtu_size         协商的mtu size。现在仅支持185.
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleMtuRequest(unsigned short mtu_size);

/*
 * 原型:      sAPI_BleFindInformationRequest
 * 功能:  发起一个find information请求
 *
 * 参数:
 *      start         开始的属性attribute handle。
 *      end           结束的attribute handle。
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleFindInformationRequest(unsigned short start, unsigned short end);

/*
 * 原型:      sAPI_BleReadByGroupTypeRequest
 * 功能:  发起一个read by group type请求。
 *
 * 参数:
 *      start                   开始的属性attribute handle。
 *      end                     结束的attribute handle。
 *      attribute_group_type    查找的group类型。
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleReadByGroupTypeRequest(unsigned short start, unsigned short end, SC_UUID_T *attribute_group_type);

/*
 * 原型:      sAPI_BleReadByTypeRequest
 * 功能:  发起一个read by type请求。
 *
 * 参数:
 *      start                   开始的属性attribute handle。
 *      end                     结束的attribute handle。
 *      attribute_type          查找的attribute类型。
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleReadByTypeRequest(unsigned short start, unsigned short end, SC_UUID_T *attribute_type);

/*
 * 原型:      sAPI_BleReadRequest
 * 功能:  发起一个read请求。
 *
 * 参数:
 *      attribute_handle          attribute handle。
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleReadRequest(unsigned short attribute_handle);

/*
 * 原型:      sAPI_BleWriteRequest
 * 功能:  发起一个write请求。
 *
 * 参数:
 *      attribute_handle          attribute handle。
 *      data                      待写入的数据
 *      size                      待写入的数据长度
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleWriteRequest(unsigned short attribute_handle, const void *data, unsigned short size);

/*
 * 原型:      sAPI_BleWriteRequest
 * 功能:  发起一个write命令。
 *
 * 参数:
 *      attribute_handle          attribute handle。
 *      data                      待写入的数据
 *      size                      待写入的数据长度
 *
 * 返回值： 0是成功，其它的均为失败。
 */
int sAPI_BleWriteCommand(unsigned short attribute_handle, const void *data, unsigned short size);

#ifdef __cplusplus
}
#endif

#endif
