/**
  ******************************************************************************
  * @file    demo_ble.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of BLE operation,this demo will show how the BLE works with server and client mode.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/
#include "simcom_api.h"
#include "simcom_uart.h"
#include "simcom_os.h"

#ifdef BT_SUPPORT

#define qDebug(fmt, ...) sAPI_Debug("[%s,%d] "fmt, __func__, __LINE__, ##__VA_ARGS__)
//debug(fmt, ##__VA_ARGS__)     enhance
//sAPI_Debug("[%s,%d] "fmt, __func__, __LINE__, ##__VA_ARGS__)      USB

/*  function declaration  */
static int ble_write_test_cb(void *param);
static int ble_read_test_cb(void *param);

static SC_UUID_16_T PERIMARY_SERVICE =
{
    .type.type = LE_UUID_TYPE_16,
    .value = LE_ATT_UUID_PRIMARY
};

static SC_UUID_16_T CHARACTERISTIC =
{
    .type.type = LE_UUID_TYPE_16,
    .value = LE_ATT_UUID_CHARC
};

static SC_UUID_16_T CLIENT_CHARC_CONFIG =
{
    .type.type = LE_UUID_TYPE_16,
    .value = LE_ATT_UUID_CLIENT_CHARC_CONFIG
};

#define LE_DECLARE_PRIMARY_SERVICE(_UUID_, _UUID_TYPE_)  \
    {.uuid = (SC_UUID_T *)&PERIMARY_SERVICE, \
     .value = &(_UUID_.value), \
     .size = _UUID_TYPE_, \
     .permission = LE_ATT_PM_READABLE}

#define LE_DECLARE_CHARACTERISTIC(_CHARACTERISTIC_, _UUID_, _PERMISSION_, _WCB_, _RCB_, _VALUE_, _VALUE_SIZE_) \
    {.uuid = (SC_UUID_T *)&CHARACTERISTIC, \
     .value = &_CHARACTERISTIC_, \
     .size = sizeof(_CHARACTERISTIC_), \
     .permission = LE_ATT_PM_READABLE}, \
    {.uuid = (SC_UUID_T *)&_UUID_, \
     .value = (void *)_VALUE_, \
     .size = _VALUE_SIZE_, \
     .permission = _PERMISSION_, \
     .write_cb = _WCB_, \
     .read_cb = _RCB_}

#define LE_DECLARE_CLINET_CHRAC_CONFIG(_VALUE_, _WCB_) \
    {.uuid = (SC_UUID_T *)&CLIENT_CHARC_CONFIG, \
     .value = &_VALUE_, \
     .size = sizeof(_VALUE_), \
     .permission = LE_ATT_PM_READABLE | LE_ATT_PM_WRITEABLE, \
     .write_cb = _WCB_}

static unsigned short descriptorData = 0;

static SC_UUID_16_T uuid_immediate =
{
    .type.type = LE_UUID_TYPE_16,
    .value = LE_ATT_UUID_IMMEDIATE_ALERT
};

static SC_UUID_16_T uuid_alert =
{
    .type.type = LE_UUID_TYPE_16,
    .value = LE_ATT_UUID_ALERT_LEVEL
};

static SC_BLE_CHARACTERISTIC_16_T characteristic =
{
    .uuid = LE_ATT_UUID_ALERT_LEVEL,
    .properties = LE_ATT_CHARC_PROP_READ | LE_ATT_CHARC_PROP_WWP | LE_ATT_CHARC_PROP_INDICATE
};

static SC_BLE_SERVICE_T immediate_attrs_16[] =
{
    LE_DECLARE_PRIMARY_SERVICE(uuid_immediate, LE_UUID_TYPE_16),
    LE_DECLARE_CHARACTERISTIC(characteristic, uuid_alert, LE_ATT_PM_READABLE | LE_ATT_PM_WRITEABLE, ble_write_test_cb, ble_read_test_cb, 0, 0),
    LE_DECLARE_CLINET_CHRAC_CONFIG(descriptorData, NULL),
};

static SC_UUID_128_T uuid_uart =
{
    .type.type = LE_UUID_TYPE_128,
    .value = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
              0x93, 0xf3, 0xa3, 0xb5, 0x01, 0x00, 0x40, 0x6e}
};

static SC_UUID_128_T uuid_tx =
{
    .type.type = LE_UUID_TYPE_128,
    .value = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
              0x93, 0xf3, 0xa3, 0xb5, 0x03, 0x00, 0x40, 0x6e}
};

static SC_BLE_CHARACTERISTIC_128_T characteristic_tx =
{
    .uuid = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
             0x93, 0xf3, 0xa3, 0xb5, 0x03, 0x00, 0x40, 0x6e},
    .properties = LE_ATT_CHARC_PROP_NOTIFY
};

static SC_UUID_128_T uuid_rx =
{
    .type.type = LE_UUID_TYPE_128,
    .value = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
              0x93, 0xf3, 0xa3, 0xb5, 0x02, 0x00, 0x40, 0x6e}
};

static SC_BLE_CHARACTERISTIC_128_T characteristic_rx =
{
    .uuid = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
             0x93, 0xf3, 0xa3, 0xb5, 0x02, 0x00, 0x40, 0x6e},
    .properties = LE_ATT_CHARC_PROP_WWP | LE_ATT_CHARC_PROP_WRITE
};

static SC_BLE_SERVICE_T immediate_attrs_128[] =
{
    LE_DECLARE_PRIMARY_SERVICE(uuid_uart, LE_UUID_TYPE_128),
    LE_DECLARE_CHARACTERISTIC(characteristic_tx, uuid_tx, LE_ATT_PM_READABLE | LE_ATT_PM_WRITEABLE, NULL, NULL, 0, 0),
    LE_DECLARE_CLINET_CHRAC_CONFIG(descriptorData, NULL),
    LE_DECLARE_CHARACTERISTIC(characteristic_rx, uuid_rx, LE_ATT_PM_READABLE | LE_ATT_PM_WRITEABLE, ble_write_test_cb, NULL, 0, 0),
};

/**
  * @brief  debug message print to UART1 with printf method.
  * @param  Pointer *format.
  * @note   Max string length is 256 bytes
  * @retval void
  */
static void debug(const char *format, ...)
{
    char buffer[256];
    int size = 0;

    va_list ap;
    va_start(ap, format);
    size = vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);

    sAPI_UartWrite(SC_UART, (UINT8 *)buffer, size);
}

/**
  * @brief  Callback for BLE write event.
  * @param  Pointer *param.
  * @note   
  * @retval void
  */
static int ble_write_test_cb(void *param)
{
    SC_BLE_RW_T *rw = (SC_BLE_RW_T *)param;
    int i = 0;

    for (i = 0; i < rw->length; i++)
    {
        qDebug("%d(%02x)", rw->data[i], rw->data[i]);
    }

    return 0;
}

/**
  * @brief  Callback for BLE read event.
  * @param  Pointer *param.
  * @note   !!!!!no free
  * @retval void
  */
static int ble_read_test_cb(void *param)
{
    SC_BLE_RW_T *rw = (SC_BLE_RW_T *)param;
    const char *data = "simcom";
    int size = strlen(data);

    rw->data = (unsigned char *)sAPI_Malloc(size);
    if (rw->data == NULL)
    {
        qDebug("no memory to create read buffer.");
        return 0;
    }

    memcpy(rw->data, data, size);

    return size;
}

/**
  * @brief  BLE connect handle.
  * @param  Pointer *msg.
  * @note   inline API
  * @retval void
  */
static inline void ble_handle_connect(void *msg)
{
    SC_BLE_CONNECT_EVENT_T *connect = (SC_BLE_CONNECT_EVENT_T *)msg;

    if (connect == NULL) return;

    qDebug("role = %d, handle = %d, address = %02x%02x%02x%02x%02x%02x",
        connect->role, connect->acl_handle, connect->address.bytes[0], connect->address.bytes[1],
        connect->address.bytes[2], connect->address.bytes[3], connect->address.bytes[4], connect->address.bytes[5]);
}

/**
  * @brief  BLE disconnect handle.
  * @param  Pointer *msg.
  * @note   inline API
  * @retval void
  */
static inline void ble_handle_disconnect(void *msg)
{
    int *handle = (int *)msg;

    qDebug("handle = %d", *handle);
}

/**
  * @brief  BLE mtu exchange handle.
  * @param  Pointer *msg.
  * @note   inline API
  * @retval void
  */
static void ble_handle_mtu_exchange(void *msg)
{
    SC_BLE_MTU_EXCHANGE_EVENT_T *mtu = (SC_BLE_MTU_EXCHANGE_EVENT_T *)msg;

    qDebug("mtu exchaneg message %d %d.", mtu->mut, mtu->acl_handle);
}

/**
  * @brief  BLE indicate handle.
  * @param  Pointer *msg.
  * @note   inline API
  * @retval void
  */
static inline void ble_handle_indicate(void *msg)
{
    qDebug("indicate confirm response");
}

/**
  * @brief  BLE event handle for CONNECT\DISCONNECT\MTU_EXCHANGE\INDICATE
  * @param  Pointer *msg.
  * @note   return fixed 0
  * @retval 0
  */
static int ble_handle_event(SC_BLE_EVENT_MSG_T *msg)
{
    switch (msg->event_id)
    {
        case BLE_CONNECT_EVENT:
            ble_handle_connect(msg->payload);
            break;
        case BLE_DIS_CONNECT_EVENT:
            ble_handle_disconnect(msg->payload);
            break;
        case BLE_MTU_EXCHANGED_EVENT:
            ble_handle_mtu_exchange(msg->payload);
            break;
        case BLE_INDICATE_EVENT:
            ble_handle_indicate(msg->payload);
            break;
        default: qDebug("not know event type.");
    }

    return 0;
}

/**
  * @brief  Set BLE address randomly.
  * @param  void
  * @note   Refer to sAPI_BleSetAddress.
  * @retval 1:set succesfully  0:set failed
  */
static BOOL ble_set_address(void)
{
    SC_BLE_ADDR_T address;

    sAPI_BleCreateRandomAddress(&address);
    qDebug("address(%02x:%02x:%02x:%02x:%02x:%02x)",
        address.bytes[5], address.bytes[4], address.bytes[3],
        address.bytes[2], address.bytes[1], address.bytes[0]);

    return (sAPI_BleSetAddress(&address) == 0);
}

/**
  * @brief  Set BLE adv data packet.
  * @param  void
  * @note   
  * @retval 1: set succesfully  0: set failed.
  */
static BOOL ble_set_adv_data(void)
{
    char advData[31];
    int size = 0;

    unsigned char flags = (1 << 1);
    size += sAPI_BleCreateAdvData(BLE_ADV_DATA_TYPE_FLAG, &advData[size], sizeof(advData) - size, (char *)&flags, sizeof(flags));

    const char *deviceName = "qBle";
    size += sAPI_BleCreateAdvData(BLE_ADV_DATA_TYPE_COMPLETE_NAME, &advData[size], sizeof(advData) - size, deviceName, strlen(deviceName));

    unsigned short appearance = 0;
    size += sAPI_BleCreateAdvData(BLE_ADV_DATA_TYPE_APPEARANCE, &advData[size], sizeof(advData) - size, (char *)&appearance, sizeof(appearance));

    const char *company = "simcom";
    size += sAPI_BleCreateAdvData(BLE_ADV_DATA_TYPE_SPECIFIC_DATA, &advData[size], sizeof(advData) - size, company, strlen(company));

    return (sAPI_BleSetAdvData(advData, size) == 0);
}

/**
  * @brief  Set BLE adv configuration.
  * @param  void
  * @note   
  * @retval 1: set succesfully  0: set failed.
  */
static BOOL ble_set_adv_param(void)
{
    SC_BLE_ADV_PARAM_T param;

    memset(&param, 0, sizeof(SC_BLE_ADV_PARAM_T));

    param.interval_min = 0x80;
    param.interval_max = 0x800;
    param.advertising_type = LE_ADV_TYPE_IND;
    param.own_address_type = LE_ADDRESS_TYPE_RANDOM;

    return (sAPI_BleSetAdvParam(&param) == 0);
}
/**
  * @brief  Handller for BLE clientscan
  * @param  argv
  * @note   
  * @retval void
  */
static void ble_client_handle_scan(SC_BLE_SCAN_EVENT_T *scan)
{
    unsigned char index = 0;
    int total_size = (int)scan->length;

    qDebug("address type %d, addrss %02x%02x%02x%02x%02x%02x, rssi = %d",
            scan->address_type, scan->address.bytes[0], scan->address.bytes[1], scan->address.bytes[2], scan->address.bytes[3],
            scan->address.bytes[4], scan->address.bytes[5], scan->rssi);

    while(total_size > 0 && scan->adv_type == LE_ADV_TYPE_IND)
    {
        int length = scan->data[index];
        int type = scan->data[index + 1];

        switch(type)
        {
            case BLE_ADV_DATA_TYPE_SHORT_NAME:
            case BLE_ADV_DATA_TYPE_COMPLETE_NAME:
            {
                char temp[31];
                memset(temp, 0, 31);
                memcpy(temp, &scan->data[index + 2], length - 1);
                qDebug("handle name %s", temp);
                break;
            }
            default:
                break;
        }
        index += (length + 1);
        total_size -= (length + 1);
    }
}
/**
  * @brief  BLE server set 16 bit UUID
  * @param  argv
  * @note   
  * @retval void
  */
static void ble_demo_server_uuid_16(void)
{
    sMsgQRef msgRef = NULL;

    if (sAPI_MsgQCreate(&msgRef, (char *)"ble server msgQ", sizeof(SIM_MSG_T), 4, SC_FIFO) != 0)
    {
        qDebug("create msgQ fail.");
        return;
    }

    qDebug("prepare open ble.");

    if (sAPI_BleOpen(msgRef, 0) != 0)
    {
        qDebug("open ble fail.");
        return;
    }

    qDebug("open ble success.");

    if (ble_set_address() != TRUE)
    {
        qDebug("set address fail.");
        return;
    }

    if (ble_set_adv_data() != TRUE)
    {
        qDebug("set adv data fail.");
        return;
    }

    if (ble_set_adv_param() != TRUE)
    {
        qDebug("set adv param fail.");
        return;
    }

    if (sAPI_BleRegisterService(immediate_attrs_16, sizeof(immediate_attrs_16) / sizeof(SC_BLE_SERVICE_T)) != 0)
    {
        qDebug("register service fail.");
        return;
    }

    qDebug("register service success.");

    sAPI_BleRegisterEventHandle(ble_handle_event);

    if (sAPI_BleEnableAdv() != 0)
    {
        qDebug("open adv fail.");
        return;
    }

    qDebug("ble server adv success.");
}
/**
  * @brief  BLE server set 128 bit UUID
  * @param  argv
  * @note   
  * @retval void
  */
static void ble_demo_server_uuid_128(void)
{
    sMsgQRef msgRef = NULL;

    if (sAPI_MsgQCreate(&msgRef, (char *)"ble server msgQ", sizeof(SIM_MSG_T), 4, SC_FIFO) != 0)
    {
        qDebug("create msgQ fail.");
        return;
    }

    qDebug("prepare open ble.");

    if (sAPI_BleOpen(msgRef, 0) != 0)
    {
        qDebug("open ble fail.");
        return;
    }

    qDebug("open ble success.");

    if (ble_set_address() != TRUE)
    {
        qDebug("set address fail.");
        return;
    }

    if (ble_set_adv_data() != TRUE)
    {
        qDebug("set adv data fail.");
        return;
    }

    if (ble_set_adv_param() != TRUE)
    {
        qDebug("set adv param fail.");
        return;
    }

    if (sAPI_BleRegisterService(immediate_attrs_128, sizeof(immediate_attrs_128) / sizeof(SC_BLE_SERVICE_T)) != 0)
    {
        qDebug("register service fail.");
        return;
    }

    qDebug("register service success.");

    sAPI_BleRegisterEventHandle(ble_handle_event);

    if (sAPI_BleEnableAdv() != 0)
    {
        qDebug("open adv fail.");
        return;
    }

    qDebug("ble server adv success.");
}

/**
  * @brief  BLE client scan
  * @param  argv
  * @note   
  * @retval void
  */
static void ble_demo_client_scan(void)
{
    qDebug("Task runs successfully");

    sAPI_TaskSleep(100);

    sMsgQRef msgRef = NULL;

    if (sAPI_MsgQCreate(&msgRef, (char *)"ble server msgQ", sizeof(SIM_MSG_T), 4, SC_FIFO) != 0)
    {
        qDebug("create msgQ fail.");
        return;
    }

    qDebug("prepare open ble.");

    if (sAPI_BleOpen(msgRef, 0) != 0)
    {
        qDebug("open ble fail.");
        return;
    }

    qDebug("open ble success.");

    if (sAPI_BleScan(LE_ACTIVE_SCAN, 0x1F40, 0x1F40, LE_ADDRESS_TYPE_RANDOM) != 0)
    {
        qDebug("scan fail.");
    }
}

/**
  * @brief  BLE server demo task processor
  * @param  argv
  * @note   
  * @retval void
  */
static void sTask_BleServerProcesser(void *argv)
{
    sAPI_TaskSleep(100);

    typedef enum
    {
        BLE_DEMO_TYPE_BLE_SERVER_UUID_16,
        BLE_DEMO_TYPE_BLE_SERVER_UUID_128,
        BLE_DEMO_TYPE_BLE_CLIENT_SCAN,
    } SC_BLE_DEMO_TYPE_T;

    SC_BLE_DEMO_TYPE_T demoType = BLE_DEMO_TYPE_BLE_SERVER_UUID_128;

    switch (demoType)
    {
        case BLE_DEMO_TYPE_BLE_SERVER_UUID_16:
            ble_demo_server_uuid_16();
            break;
        case BLE_DEMO_TYPE_BLE_SERVER_UUID_128:
            ble_demo_server_uuid_128();
            break;
        case BLE_DEMO_TYPE_BLE_CLIENT_SCAN:
            ble_demo_client_scan();
            break;
        default:
            qDebug("error type.");
    }
}

/**
  * @brief  BLE server demo tasj initial
  * @param  void
  * @note   
  * @retval void
  */
void sAPP_BleServerDemo(void)
{
    sTaskRef bleServerTask;
    void *bleServerStack = sAPI_Malloc(8096);
    if (bleServerStack == NULL)
    {
        qDebug("no memory to create ble server stack.");
        return;
    }

    if (SC_SUCCESS != sAPI_TaskCreate(&bleServerTask, bleServerStack, 8096, 150, "ble Server Processer",
        sTask_BleServerProcesser, 0))
    {
        qDebug("create ble server task fail.");
    }

    qDebug("create ble server task success.");
}

#endif
