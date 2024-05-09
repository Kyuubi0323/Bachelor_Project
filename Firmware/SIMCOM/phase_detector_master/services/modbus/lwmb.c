#include <stdio.h>
#include "lwmb.h"
#include "log.h"
#define MAX_CB  32

#define MB_READ_COILS_BIT   (1 << 0)
#define MB_READ_DISCRETE_INPUT_BIT  (1 << 1)
#define MB_READ_REGISTERS_BIT   (1 << 2)

#define MB_ERR_CRC_BIT  (1 << 4)
#define MB_ERR_EXCEPTION_BIT    (1 << 5)


static mb_t object = {0};

static mb_p mb_proc = {
    mb_parse,
    mb_serialize,
};

static callback_t callback_list[MAX_CB] ={0};

static int callback_index = 0;

static sFlagRef mb_flag = NULL;


mb_send mb_send_fnt = NULL;


UINT16 word(UINT8 H, UINT8 L)
{
	bytesFields W;
	W.u8[0] = L;
	W.u8[1] = H;
	return W.u16[0];
}

UINT16 calcCRC16(UINT8 *Buffer, UINT32 length)
{
	unsigned int temp, temp2, flag;
	temp = 0xFFFF;
	for (unsigned char i = 0; i < length; i++)
	{
		temp = temp ^ Buffer[i];
		for (unsigned char j = 1; j <= 8; j++)
		{
			flag = temp & 0x0001;
			temp >>= 1;
			if (flag)
				temp ^= 0xA001;
		}
	}
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;
	return temp;
}
mb_t *mb_init(void)
{
    mb_t *obj = NULL;
    if (sAPI_FlagCreate(&mb_flag) != SC_SUCCESS) {
        loge("create modbus flag notifiy failure");
        return NULL;
    }
    obj = &object;
    obj->proc = &mb_proc;
    return obj;
}
int mb_parse(mb_t *mb, UINT8 *buffer, int size)
{
    int success = -1;
    do {
        UINT16 u16MsgCRC = (buffer[size - 2] << 8) | buffer[size - 1];
        UINT16 u16CalCRC = calcCRC16(buffer, size - 2);
        if (u16CalCRC != u16MsgCRC) {
            if (mb_flag != NULL)
                sAPI_FlagSet(mb_flag, MB_ERR_CRC_BIT, SC_FLAG_OR);
            break;      //Wrong CRC
        }

        if (mb->id != buffer[0])
            break;      //RX SlaveID != TX SlaveID
        if (buffer[1] - mb->fnc == 128) {
            //Process exception
            //Call exception callback
            logi("Exception %.2x", buffer[1]);
            if (mb_flag != NULL)
                sAPI_FlagSet(mb_flag, MB_ERR_EXCEPTION_BIT, SC_FLAG_OR);
            break;
        }
        else if (buffer[1] != mb->fnc)
            break;
        // logi("modbus fnc %d", mb->fnc);
        if (mb->fnc == MB_FC_READ_COILS) {
            mb->bytesNo = buffer[2];
            memcpy((UINT8*)mb->payload, &buffer[3], mb->bytesNo);
            if (mb_flag != NULL)
                sAPI_FlagSet(mb_flag, MB_READ_COILS_BIT, SC_FLAG_OR);
        }
        else if (mb->fnc == MB_FC_READ_REGISTERS) {
            mb->bytesNo = buffer[2];
            memcpy((UINT8*)mb->payload, &buffer[3], mb->bytesNo);
            if (mb_flag != NULL)
                sAPI_FlagSet(mb_flag, MB_READ_REGISTERS_BIT, SC_FLAG_OR);
        }
        success = 0;
    } while(0);
    return success;
}



mb_err_t mb_readcoils(mb_t *obj, UINT8 id, UINT16 address, UINT16 numberCoils, UINT16 *data, INT16 *coils) {
    mb_err_t err = ERR_OK_QUERY;
    obj->id = id;
    obj->fnc = MB_FC_READ_COILS;
    obj->addr = address;
    obj->coilsNo = numberCoils;
    UINT8 msg[MAX_MB_MSG] = {0};
    msg[0] = id;
    msg[1] = MB_FC_READ_COILS;
    msg[2] = address >> 8;
    msg[3] = address & 0xFF;
    msg[4] = numberCoils >> 8;
    msg[5] = numberCoils & 0xFF; 
    UINT16 crc16 = calcCRC16(msg, 6);
    msg[6] = crc16 >> 8;
    msg[7] = crc16 & 0xFF; 
    if (mb_send_fnt != NULL)
        mb_send_fnt(msg, 8);
    UINT32 flag = 0;
    SC_STATUS status = sAPI_FlagWait(mb_flag, MB_READ_COILS_BIT | MB_ERR_CRC_BIT | MB_ERR_EXCEPTION_BIT, SC_FLAG_OR_CLEAR, &flag, 400);
    if (status == SC_TIMEOUT) {
        err = ERR_TIME_OUT;
        loge("read coils -> timeout");
    }
    else if (status == SC_SUCCESS)  {
        if (flag == MB_ERR_CRC_BIT) {
            loge("read coils -> crc error");
            err = ERR_BAD_CRC;
        }
        else if (flag == MB_ERR_EXCEPTION_BIT) {
            loge("read coils -> exception");
            err = ERR_EXCEPTION;
        }
        else if (flag == MB_READ_COILS_BIT) {
            INT16 bytes = obj->bytesNo;
            // sAPI_UartPrintf("Data %d\r\n", bytes);
            memcpy((UINT8*)data, (UINT8*)obj->payload, bytes);
            *coils = bytes;
        }

    }
    return err;
}
mb_err_t mb_readholdingregisters(mb_t *obj, UINT8 id, UINT16 address, UINT16 number_register, UINT16 *data, INT16 *size) {
    mb_err_t err = ERR_OK_QUERY;
    obj->id = id;
    obj->fnc = MB_FC_READ_REGISTERS;
    obj->addr = address;
    obj->coilsNo = number_register;
    UINT8 msg[MAX_MB_MSG] = {0};
    msg[0] = id;
    msg[1] = MB_FC_READ_REGISTERS;
    msg[2] = address >> 8;
    msg[3] = address & 0xFF;
    msg[4] = number_register >> 8;
    msg[5] = number_register & 0xFF; 
    UINT16 crc16 = calcCRC16(msg, 6);
    msg[6] = crc16 >> 8;
    msg[7] = crc16 & 0xFF; 
    if (mb_send_fnt != NULL)
        mb_send_fnt(msg, 8);
    UINT32 flag = 0;
    SC_STATUS status = sAPI_FlagWait(mb_flag, MB_READ_REGISTERS_BIT | MB_ERR_CRC_BIT | MB_ERR_EXCEPTION_BIT, SC_FLAG_OR_CLEAR, &flag, 400);
    if (status == SC_TIMEOUT) {
        err = ERR_TIME_OUT;
        loge("read registers -> timeout");
    }
    else if (status == SC_SUCCESS)  {
        if (flag == MB_ERR_CRC_BIT) {
            loge("read registers -> crc error");
            err = ERR_BAD_CRC;
        }
        else if (flag == MB_ERR_EXCEPTION_BIT) {
            loge("read registers -> exception");
            err = ERR_EXCEPTION;
        }
        else if (flag == MB_READ_REGISTERS_BIT) {
            INT16 bytes = obj->bytesNo / 2;
            *size = bytes;
            UINT8 u8byte, i;
            for (i = 0; i < bytes; i++) {
                data[i] = word(obj->payload[u8byte], obj->payload[u8byte + 1]);
                u8byte += 2;
            }
        }

    }
    return err;
}
int mb_serialize(mb_t *obj, UINT16 type, int len, UINT8 *payload, UINT8 *data, int *size)
{
    return 1;
}
int mb_register_callback(UINT8 fnc, void (*callback)(mb_t *, void *))
{
    int success = -1;
    do {
        if (callback_index > MAX_CB) 
            break;
        callback_list[callback_index].fnc = fnc;
        callback_list[callback_index].callback = callback;
        callback_index ++;
    } while(0);
    return success;
}
void mb_register_send_event(mb_t *mb, mb_send send_fnt) {
    mb_send_fnt = send_fnt;
}
