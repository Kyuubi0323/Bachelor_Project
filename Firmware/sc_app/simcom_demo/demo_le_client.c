/**
  ******************************************************************************
  * @file    demo_le_client.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of BLE operation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/

#ifdef BT_SUPPORT

#include "simcom_api.h"
#include "simcom_ble.h"
#include "simcom_common.h"
#include "simcom_os.h"
#include "simcom_list.h"

extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);

enum
{
    SC_LE_CLIENT_POWER_ON = 1,
    SC_LE_CLIENT_POWER_OFF,
    SC_LE_CLIENT_SCAN,
    SC_LE_CLIENT_SCAN_STOP,
    SC_LE_CLIENT_CONNECT,
    SC_LE_CLIENT_DISCONNECT,
    SC_LE_CLIENT_MTU_EXCHANGE,
    SC_LE_CLIENT_SEARCH_SERVICE,
    SC_LE_CLIENT_SEARCH_CHARACTERISTIC,
    SC_LE_CLIENT_SEARCH_DESCRIPTOR,
    SC_LE_CLIENT_READ_REQUEST,
    SC_LE_CLIENT_WRITE_REQUEST,
    SC_LE_CLIENT_WRITE_COMMAND,
    SC_LE_CLIENT_MAX = 99
};

typedef struct
{
    struct list_head ptr;
    int index;
    SC_BLE_ADDR_T address;
    int address_type;
} SC_BLE_REMOTE_SERVER_T;

typedef struct
{
    struct list_head ptr;
    int index;
    SC_BLE_REMOTE_SERVER_T *server;
    unsigned short start;
    unsigned short end;
    SC_UUID_COMMON_T uuid;
} SC_BLE_REMOTE_SERVICE_T;

typedef struct
{
    struct list_head ptr;
    int index;
    SC_BLE_REMOTE_SERVICE_T *service;
    unsigned short handle;
    unsigned char properties;
    unsigned short value_handle;
    SC_UUID_COMMON_T uuid;
} SC_BLE_REMOTE_CHARACTERISTIC_T;

typedef struct
{
    struct list_head ptr;
    int index;
    SC_BLE_REMOTE_CHARACTERISTIC_T *characteristic;
    unsigned short handle;
    SC_UUID_COMMON_T uuid;
} SC_BLE_REMOTE_DESCRIPTOR_T;

static int g_serverIndexPool = 0;
static int g_serviceIndexPool = 0;
static int g_characteristicIndexPool = 0;
static int g_descriptorIndexPool = 0;
static SC_BLE_REMOTE_SERVER_T *g_remoteServer = NULL;
static SC_BLE_REMOTE_SERVICE_T *g_remoteService = NULL;
static SC_BLE_REMOTE_CHARACTERISTIC_T *g_remoteCharacteristic = NULL;
static struct list_head g_remoteServerList = {.prev = &g_remoteServerList, .next = &g_remoteServerList};
static struct list_head g_remoteServiceList = {.prev = &g_remoteServiceList, .next = &g_remoteServiceList};
static struct list_head g_remoteCharacteristicList = {.prev = &g_remoteCharacteristicList, .next = &g_remoteCharacteristicList};
static struct list_head g_remoteDescriptorList = {.prev = &g_remoteDescriptorList, .next = &g_remoteDescriptorList};

/**
  * @brief  BLE client scan handler
  * @param  payload--pointer for BLE SCAN result
  * @note   
  * @retval void
  */
static void le_client_handle_scan(void *payload)
{
    SC_BLE_SCAN_EVENT_T *scan = (SC_BLE_SCAN_EVENT_T *)payload;
    char rspBuf[128];
    SC_BLE_REMOTE_SERVER_T *pos = NULL;

    list_for_each_entry(pos, &g_remoteServerList, SC_BLE_REMOTE_SERVER_T, ptr)
    {
        if (memcmp(pos->address.bytes, scan->address.bytes, 6) == 0)
        {
            return;
        }
    }

    pos = (SC_BLE_REMOTE_SERVER_T *)sAPI_Malloc(sizeof(SC_BLE_REMOTE_SERVER_T));
    if (pos == NULL)
    {
        PrintfResp("\r\nNo memory to create remote server buffer.\r\n");
        return;
    }

    g_serverIndexPool++;
    pos->index = g_serverIndexPool;
    memcpy(pos->address.bytes, scan->address.bytes, 6);
    pos->address_type = scan->address_type;

    list_add_tail(&pos->ptr, &g_remoteServerList);

    snprintf(rspBuf, sizeof(rspBuf),
        "\r\nscan result: index(%d), adv type(%d), address type(%d), mac address(%02X:%02X:%02X:%02X:%02X:%02X), rssi(%d)\r\n",
        pos->index, scan->adv_type, scan->address_type, scan->address.bytes[5], scan->address.bytes[4], scan->address.bytes[3],
        scan->address.bytes[2], scan->address.bytes[1], scan->address.bytes[0], scan->rssi);

    PrintfResp(rspBuf);
}

/**
  * @brief  BLE client connect handler
  * @param  payload--pointer for BLE connect result
  * @note   
  * @retval void
  */
static void le_client_handle_connect(void *payload)
{
    SC_BLE_CONNECT_EVENT_T *connect = (SC_BLE_CONNECT_EVENT_T *)payload;
    char rspBuf[64];

    snprintf(rspBuf, sizeof(rspBuf), "\r\nConnect to the device(%02X:%02X:%02X:%02X:%02X:%02X) success",
        connect->address.bytes[5], connect->address.bytes[4], connect->address.bytes[3],
        connect->address.bytes[2], connect->address.bytes[1], connect->address.bytes[0]);

    PrintfResp(rspBuf);
}

/**
  * @brief  BLE client disconnect handler
  * @param  payload--pointer for BLE disconnect result
  * @note   
  * @retval void
  */
static void le_client_handle_disconnect(void *payload)
{
    PrintfResp("\r\nDisconnect from the dest device success\r\n");

    {
        g_serviceIndexPool = 0;
        SC_BLE_REMOTE_SERVICE_T *pos, *next;
        list_for_each_entry_safe(pos, next, &g_remoteServiceList, SC_BLE_REMOTE_SERVICE_T, ptr)
        {
            sAPI_Free(pos);
        }
    }

    {
        g_characteristicIndexPool = 0;
        SC_BLE_REMOTE_CHARACTERISTIC_T *pos, *next;
        list_for_each_entry_safe(pos, next, &g_remoteCharacteristicList, SC_BLE_REMOTE_CHARACTERISTIC_T, ptr)
        {
            sAPI_Free(pos);
        }
    }

    {
        g_descriptorIndexPool = 0;
        SC_BLE_REMOTE_DESCRIPTOR_T *pos, *next;
        list_for_each_entry_safe(pos, next, &g_remoteDescriptorList, SC_BLE_REMOTE_DESCRIPTOR_T, ptr)
        {
            sAPI_Free(pos);
        }
    }
}

/**
  * @brief  BLE client mtu change handler
  * @param  payload--pointer for BLE mtu change result
  * @note   
  * @retval void
  */
static void le_client_handle_mtu_exchanged(void *payload)
{
    PrintfResp("\r\nMtu exchanged success.\r\n");
}

/**
  * @brief  BLE client read ready handler
  * @param  payload--pointer for BLE read ready result
  * @note   
  * @retval void
  */
static void le_client_handle_read_by_group_type(void *payload)
{
    SC_BLE_READ_BY_GROUP_TYPE_EVENT_T *rsp = (SC_BLE_READ_BY_GROUP_TYPE_EVENT_T *)payload;
    unsigned char *pdu = rsp->value;
    unsigned char each_size = pdu[0];

    if (((rsp->size - 1) % each_size) != 0)
    {
        PrintfResp("\r\nReceive a invalid response.\r\n");
        return;
    }

    int i;
    unsigned start = 0, end = 0;
    for (i = 0; i < (rsp->size - 1) / each_size; i++)
    {
        SC_UUID_COMMON_T uuid;

        start = ((unsigned short)pdu[2 + i * each_size] << 8) | pdu[1 + i * each_size];
        end = ((unsigned short)pdu[4 + i * each_size] << 8) | pdu[3 + i * each_size];

        switch (each_size - 4)
        {
            case LE_UUID_TYPE_16:
                uuid.uuid_16.type.type = LE_UUID_TYPE_16;
                uuid.uuid_16.value = (unsigned short)pdu[6 + i * each_size] << 8 | pdu[5 + i * each_size];
                break;

            case LE_UUID_TYPE_128:
                uuid.uuid_128.type.type = LE_UUID_TYPE_128;
                memcpy(&uuid.uuid_128.value, &pdu[5 + i * each_size], 16);
                break;

            default:
                PrintfResp("\r\nReceive a invalid response.\r\n");
                return;
        }

        SC_BLE_REMOTE_SERVICE_T *service = (SC_BLE_REMOTE_SERVICE_T *)sAPI_Malloc(sizeof(SC_BLE_REMOTE_SERVICE_T));
        if (service == NULL)
        {
            PrintfResp("\r\nNo memory to create service buffer.\r\n");
            return;
        }

        g_serviceIndexPool++;
        service->index = g_serviceIndexPool;
        service->server = g_remoteServer;
        service->start = start;
        service->end = end;
        memcpy(&service->uuid, &uuid, sizeof(uuid));

        list_add_tail(&service->ptr, &g_remoteServiceList);

        char rspBuf[128];

        snprintf(rspBuf, sizeof(rspBuf),
            "\r\nservice index(%d), start handle(%d), end handle(%d), uuid(0x%04X)",
            service->index, service->start, service->end, service->uuid.uuid_16.value);

        PrintfResp(rspBuf);
    }

    SC_UUID_16_T uuid;

    uuid.type.type = LE_UUID_TYPE_16;
    uuid.value = LE_ATT_UUID_PRIMARY;

    if (end != 0xffff)
    {
        sAPI_BleReadByGroupTypeRequest(end + 1, 0xffff, (SC_UUID_T *)&uuid);
    }
}

/**
  * @brief  BLE client read by type handler
  * @param  payload--pointer for BLE read by type result
  * @note   
  * @retval void
  */
static void le_client_handle_read_by_type(void *payload)
{
    SC_BLE_READ_BY_TYPE_EVENT_T *rsp = (SC_BLE_READ_BY_TYPE_EVENT_T *)payload;
    unsigned char *pdu = rsp->value;
    unsigned char each_size = pdu[0];

    if (((rsp->size - 1) % each_size) != 0)
    {
        PrintfResp("\r\nReceive a invalid response.\r\n");
        return;
    }

    int i;
    unsigned short handle, value_handle = 0;
    for (i = 0; i < (rsp->size - 1) / each_size; i++)
    {
        SC_UUID_COMMON_T uuid;
        handle = ((unsigned short)pdu[2 + i * each_size] << 8) | pdu[1 + i * each_size];
        unsigned char properties = pdu[3 + i * each_size];
        value_handle = ((unsigned short)pdu[5 + i * each_size] << 8) | pdu[4 + i * each_size];

        switch (each_size - 5)
        {
            case LE_UUID_TYPE_16:
                uuid.uuid_16.type.type = LE_UUID_TYPE_16;
                uuid.uuid_16.value = (unsigned short)pdu[7 + i * each_size] << 8 | pdu[6 + i * each_size];
                break;

            case LE_UUID_TYPE_128:
                uuid.uuid_128.type.type = LE_UUID_TYPE_128;
                memcpy(&uuid.uuid_128.value, &pdu[6 + i * each_size], 16);
                break;

            default:
                PrintfResp("\r\nReceive a invalid response.\r\n");
                return;
        }

        SC_BLE_REMOTE_CHARACTERISTIC_T *characteristic =
            (SC_BLE_REMOTE_CHARACTERISTIC_T *)sAPI_Malloc(sizeof(SC_BLE_REMOTE_CHARACTERISTIC_T));
        if (characteristic == NULL)
        {
            PrintfResp("\r\nNo memory to create characteristic buffer.\r\n");
            return;
        }

        g_characteristicIndexPool++;
        characteristic->index = g_characteristicIndexPool;
        characteristic->service = g_remoteService;
        characteristic->handle = handle;
        characteristic->properties = properties;
        characteristic->value_handle = value_handle;
        memcpy(&characteristic->uuid, &uuid, sizeof(uuid));

        list_add_tail(&characteristic->ptr, &g_remoteCharacteristicList);

        char rspBuf[128];

        snprintf(rspBuf, sizeof(rspBuf),
            "\r\ncharacteristic index(%d), handle(%d), properties(%d), value handle(%d), uuid(0x%04X)",
            characteristic->index, characteristic->handle, characteristic->properties,
            characteristic->value_handle, characteristic->uuid.uuid_16.value);

        PrintfResp(rspBuf);
    }

    if (value_handle < g_remoteService->end)
    {
        SC_UUID_16_T uuid = {.type.type = LE_UUID_TYPE_16, .value = LE_ATT_UUID_CHARC};
        if (sAPI_BleReadByTypeRequest(value_handle + 1, g_remoteService->end, (SC_UUID_T *)&uuid) != 0)
        {
            PrintfResp("\r\nCan't search of the characteristic.\r\n");
            return;
        }
    }
}

/**
  * @brief  BLE client find me handler
  * @param  payload--pointer for BLE find me result
  * @note   
  * @retval void
  */
static void le_client_handle_find_information(void *payload)
{
    SC_BLE_FIND_INFORMATION_EVENT_T *rsp = (SC_BLE_FIND_INFORMATION_EVENT_T *)payload;
    unsigned char *pdu = rsp->value;

    unsigned short handle = ((unsigned short)pdu[2] << 8) + pdu[1];

    SC_UUID_COMMON_T uuid;

    switch (pdu[0])
    {
        case 1:
            uuid.uuid_16.type.type = LE_UUID_TYPE_16;
            uuid.uuid_16.value = ((unsigned short)pdu[4] << 8) + pdu[3];

            if (uuid.uuid_16.value == LE_ATT_UUID_PRIMARY || uuid.uuid_16.value == LE_ATT_UUID_CHARC)
            {
                PrintfResp("\r\nis not a descriptor.\r\n");
                return;
            }

            break;
        case 2:
            uuid.uuid_128.type.type = LE_UUID_TYPE_128;
            memcpy(uuid.uuid_128.value, &pdu[3], LE_UUID_TYPE_128);
            break;
        default:
            PrintfResp("\r\nInvalid pdu.\r\n");
            return;
    }

    SC_BLE_REMOTE_DESCRIPTOR_T *descriptor = (SC_BLE_REMOTE_DESCRIPTOR_T *)sAPI_Malloc(sizeof(SC_BLE_REMOTE_DESCRIPTOR_T));
    if (descriptor == NULL)
    {
        PrintfResp("\r\nNo memory to create descriptor buffer.\r\n");
        return;
    }

    g_descriptorIndexPool++;
    descriptor->index = g_descriptorIndexPool;
    descriptor->characteristic = g_remoteCharacteristic;
    descriptor->handle = handle;
    memcpy(&descriptor->uuid, &uuid, sizeof(uuid));

    list_add_tail(&descriptor->ptr, &g_remoteDescriptorList);

    char rspBuf[64];

    snprintf(rspBuf, sizeof(rspBuf),
        "\r\ndescriptor index(%d), handle(%d), uuid(0x%04X)",
        descriptor->index, descriptor->handle, descriptor->uuid.uuid_16.value);

    PrintfResp(rspBuf);
}

/**
  * @brief  BLE client read handler
  * @param  payload--pointer for BLE read result
  * @note   
  * @retval void
  */
static void le_client_handle_read(void *payload)
{
    SC_BLE_READ_EVENT_T *rsp = (SC_BLE_READ_EVENT_T *)payload;
    char rspBuf[64];
    int size = 0;

    size = snprintf(rspBuf, sizeof(rspBuf), "\r\nRead response: ");

    int i;
    for(i = 0; i < rsp->size; i++)
    {
        size += snprintf(rspBuf + size, sizeof(rspBuf) - size, "%02X", rsp->value[i]);
    }

    size += snprintf(rspBuf + size, sizeof(rspBuf) - size, ".\r\n");

    PrintfResp(rspBuf);
}

/**
  * @brief  BLE client value indicate handler
  * @param  payload--pointer for BLE indicate result
  * @note   
  * @retval void
  */
static void le_client_handle_value_ind(void *payload)
{
    SC_BLE_HANDLE_VALUE_IND_EVENT_T *rsp = (SC_BLE_HANDLE_VALUE_IND_EVENT_T *)payload;
    char rspBuf[64];
    int size = 0;

    SC_BLE_REMOTE_CHARACTERISTIC_T *pos;
    list_for_each_entry(pos, &g_remoteCharacteristicList, SC_BLE_REMOTE_CHARACTERISTIC_T, ptr)
    {
        if (pos->value_handle == rsp->handle)
        {
            break;
        }
    }

    size = snprintf(rspBuf, sizeof(rspBuf), "\r\nIndication: index(%d), handle(%d), value(", pos->index, pos->handle);

    int i;
    for(i = 0; i < rsp->size; i++)
    {
        size += snprintf(rspBuf + size, sizeof(rspBuf) - size, "%02X", rsp->value[i]);
    }

    size += snprintf(rspBuf + size, sizeof(rspBuf) - size, ").\r\n");

    PrintfResp(rspBuf);
}

/**
  * @brief  BLE client value notify handler
  * @param  payload--pointer for BLE notify result
  * @note   
  * @retval void
  */
static void le_client_handle_value_ntf(void *payload)
{
    SC_BLE_HANDLE_VALUE_NTF_EVENT_T *rsp = (SC_BLE_HANDLE_VALUE_NTF_EVENT_T *)payload;
    char rspBuf[64];
    int size = 0;

    SC_BLE_REMOTE_CHARACTERISTIC_T *pos;
    list_for_each_entry(pos, &g_remoteCharacteristicList, SC_BLE_REMOTE_CHARACTERISTIC_T, ptr)
    {
        if (pos->value_handle == rsp->handle)
        {
            break;
        }
    }

    size = snprintf(rspBuf, sizeof(rspBuf), "\r\nNotication: index(%d), handle(%d), value(", pos->index, pos->handle);

    int i;
    for(i = 0; i < rsp->size; i++)
    {
        size += snprintf(rspBuf + size, sizeof(rspBuf) - size, "%02X", rsp->value[i]);
    }

    size += snprintf(rspBuf + size, sizeof(rspBuf) - size, ").\r\n");

    PrintfResp(rspBuf);
}

/**
  * @brief  BLE client response error handler
  * @param  payload--pointer for BLE response error result
  * @note   
  * @retval void
  */
static void le_client_handle_error_rsp(void *payload)
{
    SC_BLE_ERROR_RSP_EVENT_T *error_rsp = (SC_BLE_ERROR_RSP_EVENT_T *)payload;
    char rspBuf[128];

    snprintf(rspBuf, sizeof(rspBuf),
        "\r\nerror report: request(0x%02X), attribute handle(0x%04X), error code(0x%02X)\r\n",
        error_rsp->request, error_rsp->att_handle, error_rsp->code);

    PrintfResp(rspBuf);
}

/**
  * @brief  BLE client event handler group
  * @param  msg--event id
  * @note   
  * @retval void
  */
static int le_client_handle_event(SC_BLE_EVENT_MSG_T *msg)
{
    switch (msg->event_id)
    {
        case BLE_SCAN_EVENT:
            le_client_handle_scan(msg->payload);
            break;
        case BLE_CONNECT_EVENT:
            le_client_handle_connect(msg->payload);
            break;
        case BLE_DIS_CONNECT_EVENT:
            le_client_handle_disconnect(msg->payload);
            break;
        case BLE_CLIENT_MTU_EXCHANGED_EVENT:
            le_client_handle_mtu_exchanged(msg->payload);
            break;
        case BLE_CLIENT_READ_BY_GROUP_TYPE_RSP_EVENT:
            le_client_handle_read_by_group_type(msg->payload);
            break;
        case BLE_CLIENT_READ_BY_TYPE_RSP_EVENT:
            le_client_handle_read_by_type(msg->payload);
            break;
        case BLE_CLIENT_FIND_INFOMATION_RSP_EVENT:
            le_client_handle_find_information(msg->payload);
            break;
        case BLE_CLIENT_READ_RSP_EVENT:
            le_client_handle_read(msg->payload);
            break;
        case BLE_CLIENT_HANDLE_INDIATION_EVENT:
            le_client_handle_value_ind(msg->payload);
            break;
        case BLE_CLIENT_HANDLE_NOTIFY_EVENT:
            le_client_handle_value_ntf(msg->payload);
            break;
        case BLE_ERROR_RESPONSE_EVENT:
            le_client_handle_error_rsp(msg->payload);
            break;
        default:
            PrintfResp("not know event.");
    }

    return 0;
}

/**
  * @brief  BLE client demo
  * @param  void
  * @note   This demo will show how a BLE client power on\scan\connectread\write\notify\indicate with a BLE server.
  * @retval void
  */
void LEClientDemo(void)
{
    char flag = 1;
    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] = {
        "1. Power on",
        "2. Power off",
        "3. Start scan",
        "4. Stop scan",
        "5. Connect",
        "6. Disconnect",
        "7. Mtu Exchange",
        "8. Search Service",
        "9. Search Characteristic",
        "10. Search Descriptor",
        "11. Read Request",
        "12. Write Request",
        "13. Write Command",
        "99. back",
    };

    sMsgQRef messageQueue;

    sAPI_MsgQCreate(&messageQueue, (char *)"le client queue", sizeof(SIM_MSG_T), 4, SC_FIFO);

    while (flag)
    {
        PrintfResp(note);
        PrintfOptionMenu(options_list, sizeof(options_list) / sizeof(options_list[0]));

        SIM_MSG_T optionMsg;
        sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);
        if (SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("%s,msg_id is error!!", __func__);
            break;
        }

        unsigned char opt = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);

        switch (opt)
        {
            case SC_LE_CLIENT_POWER_ON:
            {
                SC_BLE_ADDR_T address;

                sAPI_BleCreateRandomAddress(&address);
                sAPI_BleSetAddress(&address);

                if (sAPI_BleOpen(messageQueue, 0) != 0)
                {
                    PrintfResp("\r\nPower on the device fail.\r\n");
                    break;
                }

                PrintfResp("\r\nPower on the device success.\r\n");
                break;
            }

            case SC_LE_CLIENT_POWER_OFF:
            {
                sAPI_BleClose();

                PrintfResp("\r\nPower off the device success.\r\n");
                break;
            }

            case SC_LE_CLIENT_SCAN:
            {
                sAPI_BleRegisterEventHandle(le_client_handle_event);
                if (sAPI_BleScan(LE_ACTIVE_SCAN, 0x1F40, 0x1F40, LE_ADDRESS_TYPE_PUBLIC) != 0)
                {
                    PrintfResp("\r\nstart scan fail.\r\n");
                    break;
                }

                PrintfResp("\r\nstart scan success.\r\n");
                break;
            }

            case SC_LE_CLIENT_SCAN_STOP:
            {
                if (sAPI_BleScanStop() != 0)
                {
                    PrintfResp("\r\nstop scan fail.\r\n");
                    break;
                }

                PrintfResp("\r\nstop scan success.\r\n");
                break;
            }

            case SC_LE_CLIENT_CONNECT:
            {
                PrintfResp("\r\nPlease input server index:\r\n");

                SIM_MSG_T optionMsg;
                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                int index = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                SC_BLE_REMOTE_SERVER_T *pos = NULL;
                list_for_each_entry(pos, &g_remoteServerList, SC_BLE_REMOTE_SERVER_T, ptr)
                {
                    if (pos->index == index)
                    {
                        break;
                    }
                }

                if (&pos->ptr == &g_remoteServerList)
                {
                    PrintfResp("\r\nInvalid index\r\n");
                    break;
                }

                g_remoteServer = pos;
                if (sAPI_BleConnect(&(pos->address), pos->address_type) != 0)
                {
                    PrintfResp("\r\nCan't connect to the dest.\r\n");
                    break;
                }
                PrintfResp("\r\nstart connect to the dest.\r\n");
                break;
            }

            case SC_LE_CLIENT_DISCONNECT:
            {
                if (sAPI_BleDisconnect() != 0)
                {
                    PrintfResp("\r\nCan't disconnect from the dest.\r\n");
                    break;
                }
                break;
            }

            case SC_LE_CLIENT_MTU_EXCHANGE:
            {
                if (sAPI_BleMtuRequest(185) != 0)
                {
                    PrintfResp("\r\nSend Mtu Request Fail.\r\n");
                    break;
                }
                break;
            }

            case SC_LE_CLIENT_SEARCH_SERVICE:
            {
                SC_UUID_16_T uuid = {.type.type = LE_UUID_TYPE_16, .value = LE_ATT_UUID_PRIMARY};
                if (sAPI_BleReadByGroupTypeRequest(0x0001, 0xffff, (SC_UUID_T *)&uuid) != 0)
                {
                    PrintfResp("\r\nCan't search of the service.\r\n");
                    break;
                }

                PrintfResp("\r\nStart search of the service.\r\n");
                break;
            }

            case SC_LE_CLIENT_SEARCH_CHARACTERISTIC:
            {
                PrintfResp("\r\nPlease input service index:\r\n");

                SIM_MSG_T optionMsg;
                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                int index = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                SC_BLE_REMOTE_SERVICE_T *pos = NULL;
                list_for_each_entry(pos, &g_remoteServiceList, SC_BLE_REMOTE_SERVICE_T, ptr)
                {
                    if (pos->index == index)
                    {
                        break;
                    }
                }

                if (&pos->ptr == &g_remoteServiceList)
                {
                    PrintfResp("\r\nInvalid index\r\n");
                    break;
                }

                g_remoteService = pos;
                SC_UUID_16_T uuid = {.type.type = LE_UUID_TYPE_16, .value = LE_ATT_UUID_CHARC};
                if (sAPI_BleReadByTypeRequest(pos->start, pos->end, (SC_UUID_T *)&uuid) != 0)
                {
                    PrintfResp("\r\nCan't search of the characteristic.\r\n");
                    break;
                }

                PrintfResp("\r\nStart search of the characteristic.\r\n");
                break;
            }

            case SC_LE_CLIENT_SEARCH_DESCRIPTOR:
            {
                PrintfResp("\r\nPlease input characteristic index:\r\n");

                SIM_MSG_T optionMsg;
                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                int index = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                SC_BLE_REMOTE_CHARACTERISTIC_T *pos = NULL;
                list_for_each_entry(pos, &g_remoteCharacteristicList, SC_BLE_REMOTE_CHARACTERISTIC_T, ptr)
                {
                    if (pos->index == index)
                    {
                        break;
                    }
                }

                if (&pos->ptr == &g_remoteCharacteristicList)
                {
                    PrintfResp("\r\nInvalid index\r\n");
                    break;
                }

                g_remoteCharacteristic = pos;
                if (sAPI_BleFindInformationRequest(pos->value_handle + 1, pos->value_handle + 1) != 0)
                {
                    PrintfResp("\r\nCan't search of the descriptor.\r\n");
                    break;
                }

                PrintfResp("\r\nStart search of the descriptor.\r\n");
                break;
            }

            case SC_LE_CLIENT_READ_REQUEST:
            {
                PrintfResp("\r\nPlease input characteristic index:\r\n");

                SIM_MSG_T optionMsg;
                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                int index = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                SC_BLE_REMOTE_CHARACTERISTIC_T *pos = NULL;
                list_for_each_entry(pos, &g_remoteCharacteristicList, SC_BLE_REMOTE_CHARACTERISTIC_T, ptr)
                {
                    if (pos->index == index)
                    {
                        break;
                    }
                }

                if (&pos->ptr == &g_remoteCharacteristicList)
                {
                    PrintfResp("\r\nInvalid index\r\n");
                    break;
                }

                if (sAPI_BleReadRequest(pos->value_handle) != 0)
                {
                    PrintfResp("\r\nSend read request fail.\r\n");
                    return;
                }

                PrintfResp("\r\nSend read request success.\r\n");
                break;
            }

            case SC_LE_CLIENT_WRITE_REQUEST:
            {
                PrintfResp("\r\nPlease input characteristic index:\r\n");

                SIM_MSG_T optionMsg;
                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                int index = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                SC_BLE_REMOTE_CHARACTERISTIC_T *pos = NULL;
                list_for_each_entry(pos, &g_remoteCharacteristicList, SC_BLE_REMOTE_CHARACTERISTIC_T, ptr)
                {
                    if (pos->index == index)
                    {
                        break;
                    }
                }

                if (&pos->ptr == &g_remoteCharacteristicList)
                {
                    PrintfResp("\r\nInvalid index\r\n");
                    break;
                }

                PrintfResp("\r\nPlease input write data:\r\n");

                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                if (sAPI_BleWriteRequest(pos->value_handle, optionMsg.arg3, strlen(optionMsg.arg3)) != 0)
                {
                    PrintfResp("\r\nSend read request fail.\r\n");
                    return;
                }

                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nSend read request success.\r\n");
                break;
            }

            case SC_LE_CLIENT_WRITE_COMMAND:
            {
                PrintfResp("\r\nPlease input characteristic index:\r\n");

                SIM_MSG_T optionMsg;
                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                int index = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                SC_BLE_REMOTE_CHARACTERISTIC_T *pos = NULL;
                list_for_each_entry(pos, &g_remoteCharacteristicList, SC_BLE_REMOTE_CHARACTERISTIC_T, ptr)
                {
                    if (pos->index == index)
                    {
                        break;
                    }
                }

                if (&pos->ptr == &g_remoteCharacteristicList)
                {
                    PrintfResp("\r\nInvalid index\r\n");
                    break;
                }

                PrintfResp("\r\nPlease input write data:\r\n");

                sAPI_MsgQRecv(simcomUI_msgq, &optionMsg, SC_SUSPEND);

                if (sAPI_BleWriteCommand(pos->value_handle, optionMsg.arg3, strlen(optionMsg.arg3)) != 0)
                {
                    PrintfResp("\r\nSend read request fail.\r\n");
                    return;
                }

                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nSend read request success.\r\n");
                break;
            }

            case SC_LE_CLIENT_MAX:
            {
                flag = 0;
                PrintfResp("\r\nReturn to the previous menu!\r\n");
                break;
            }

            default:
                PrintfResp("\r\nPlease select again:\r\n");
        }
    }

    sAPI_MsgQDelete(messageQueue);
}

#endif