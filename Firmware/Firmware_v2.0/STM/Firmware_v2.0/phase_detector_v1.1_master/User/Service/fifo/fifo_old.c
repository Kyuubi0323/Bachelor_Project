/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */

/* Inclusion of system and local header files goes here */
#include <fifo_old.h>
#include <stdbool.h>
#include <math.h>

/* ==================================================================== */
/* ============================ constants ============================= */
/* ==================================================================== */

/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */

/* Global variables definitions go here */
//cicularBuf_t cBuffer;


/* ==================================================================== */
/* ========================== private data ============================ */
/* ==================================================================== */

/* Definition of private datatypes go here */


/* ==================================================================== */
/* ====================== private functions =========================== */
/* ==================================================================== */

/* Function prototypes for private (static) functions go here */
static uint8_t CBUFFER_Full(fifo_t *cBuf);
static uint8_t CBUFFER_Empty(fifo_t *cBuf);
static void CBUFFER_Put(fifo_t *cBuf, uint8_t data);
static void CBUFFER_Get(fifo_t *cBuf, uint8_t *data);

/* ==================================================================== */
/* ===================== All functions by section ===================== */
/* ==================================================================== */

/* Functions definitions go here, organised into sections */

void CBUFFER_Init(fifo_t *cBuf, uint8_t *address, uint16_t size)
{
	cBuf->buf  = address;
	cBuf->size = size;
	cBuf->tail = 0;
	cBuf->head = 0;
	cBuf->count = 0;
	cBuf->flagRevEn = false;
}

uint8_t CBUFFER_Putc(fifo_t *cBuf, uint8_t data)
{
	if(CBUFFER_Full(cBuf) == 1) return 0;
	CBUFFER_Put(cBuf, data);
	return 1;
}

uint8_t CBUFFER_Getc(fifo_t *cBuf, uint8_t *data)
{
	if(CBUFFER_Empty(cBuf) == 1) return 0;
	CBUFFER_Get(cBuf, data);
	return 1;
}

uint16_t CBUFFER_PutStr(fifo_t *cBuf, uint8_t *data, uint16_t size){
	uint16_t i = 0;
	uint16_t bytes = 0;
	if (data == NULL) return 0;
	for (i = 0; i < size; i++)
		bytes += CBUFFER_Putc(cBuf, data[i]);
	return bytes;
}

uint16_t CBUFFER_GetBytes(fifo_t *cBuf, uint8_t *data, uint16_t size){
	uint16_t bytes = 0;
	if (data == NULL) return 0;
	while (CBUFFER_Available(cBuf))
	{
		bytes += CBUFFER_Getc(cBuf, &data[bytes]);
		if (bytes > size - 1) break;
	}
	return bytes;
}
void CBUFFER_Reset(fifo_t *cBuf)
{
	cBuf->count = cBuf->tail = cBuf->head = 0;
}
uint16_t CBUFFER_Available(fifo_t *cBuf){
	return cBuf->count;
}
/* ==================================================================== */
/* ====================== private functions =========================== */
/* ==================================================================== */
static uint8_t CBUFFER_Full(fifo_t *cBuf)
{
	if(cBuf->count == cBuf->size) return 1;
	else return 0;
}

static uint8_t CBUFFER_Empty(fifo_t *cBuf)
{
	if(cBuf->count == 0) return 1;
	else return 0;
}

static void CBUFFER_Put(fifo_t *cBuf, uint8_t data)
{
	cBuf->buf[cBuf->head] = data;
	cBuf->head = (cBuf->head+1) % cBuf->size;
	cBuf->count++;
}

static void CBUFFER_Get(fifo_t *cBuf, uint8_t *data)
{
	*data = cBuf->buf[cBuf->tail];
	cBuf->tail = (cBuf->tail+1) % cBuf->size;
	cBuf->count--;
}



