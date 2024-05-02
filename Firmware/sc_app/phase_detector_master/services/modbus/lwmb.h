#ifndef _LWMB_H_
#define _LWMB_H_

#include <stdlib.h>
#include <string.h>
#include "stdint.h"
#include "simcom_api.h"

#define MAX_MB_MSG  256

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define lowByte(w) ((w)&0xff)
#define highByte(w) ((w) >> 8

typedef struct mb_t mb_t;
typedef struct mb_p
{
    int (*parse)(mb_t *prot, UINT8 *buffer, int size);
    int (*serialize)(mb_t *obj, UINT16 type, int len, UINT8 *payload, UINT8 *data, int *size);
} mb_p;

typedef union
{
	UINT8 u8[4];
	UINT16 u16[2];
	UINT32 u32;
} bytesFields;

typedef enum mb_fnc
{
	MB_FC_READ_COILS = 1,				/*!< FCT=1 -> read coils or digital outputs */
	MB_FC_READ_DISCRETE_INPUT = 2,		/*!< FCT=2 -> read digital inputs */
	MB_FC_READ_REGISTERS = 3,			/*!< FCT=3 -> read registers or analog outputs */
	MB_FC_READ_INPUT_REGISTER = 4,		/*!< FCT=4 -> read analog inputs */
	MB_FC_WRITE_COIL = 5,				/*!< FCT=5 -> write single coil or output */
	MB_FC_WRITE_REGISTER = 6,			/*!< FCT=6 -> write single register */
	MB_FC_WRITE_MULTIPLE_COILS = 15,	/*!< FCT=15 -> write multiple coils or outputs */
	MB_FC_WRITE_MULTIPLE_REGISTERS = 16 /*!< FCT=16 -> write multiple registers */
} mb_fnc_t;

typedef enum mb_err
{
    ERR_OK_QUERY = 0,
	ERR_NOT_MASTER = -1,
	ERR_POLLING = -2,
	ERR_BUFF_OVERFLOW = -3,
	ERR_BAD_CRC = -4,
	ERR_EXCEPTION = -5,
	ERR_BAD_SIZE = -6,
	ERR_BAD_ADDRESS = -7,
	ERR_TIME_OUT = -8,
	ERR_BAD_SLAVE_ID = -9,
} mb_err_t;


struct mb_t {
	const mb_p *proc;
    UINT8 id;
    mb_fnc_t fnc;
    UINT16 crc16;
    UINT16 addr;
    UINT16 coilsNo;
    UINT8 bytesNo;
    UINT8 payload[MAX_MB_MSG];
    void *user_data;
};
typedef struct callback_t {
    void (*callback)(mb_t *, void *);
    mb_fnc_t fnc;
} callback_t;

typedef int(*mb_send)(UINT8 *data, UINT32 size);

int mb_register_callback(UINT8 fnc, void (*callback)(mb_t *, void *));
mb_t *mb_init(void);
int mb_parse(mb_t *prot, UINT8 *buffer, int size);
int mb_serialize(mb_t *obj, UINT16 type, int len, UINT8 *payload, UINT8 *data, int *size);
mb_err_t mb_readcoils(mb_t *obj, UINT8 id, UINT16 address, UINT16 numberCoils, UINT16 *data, INT16 *coils);
void mb_register_send_event(mb_t *mb, mb_send send_fnt)  ;
#endif
