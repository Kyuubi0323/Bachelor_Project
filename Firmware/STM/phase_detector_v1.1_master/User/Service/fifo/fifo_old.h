/*
 * fifo.h
 *
 *  Created on: Nov 29, 2021
 *      Author: manht
 */
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _FIFO_H_
#define _FIFO_H_

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */

/* Inclusion of system and local header files goes here */
#include "main.h"
/* ==================================================================== */
/* ============================ constants ============================= */
/* ==================================================================== */

/* #define and enum statements go here */
typedef struct
{
	uint8_t *buf;  			// luu dia chi cua mang cBuff can dung
	uint16_t size;          // kich thuoc mang cbuf
	uint16_t tail;          // phan tu doc
	uint16_t head;          // phan tu viet
	uint16_t count;         // bien dem so phan tu da ghi
	uint8_t flagRevEn;
} fifo_t;

/* ==================================================================== */
/* ========================== public data ============================= */
/* ==================================================================== */

/* Definition of public (external) data types go here */





/* ==================================================================== */
/* ======================= public functions =========================== */
/* ==================================================================== */

/* Function prototypes for public (external) functions go here */
void CBUFFER_Init(fifo_t *cBuf, uint8_t *address, uint16_t size);
uint8_t CBUFFER_Putc(fifo_t *cBuf, uint8_t data);
uint8_t CBUFFER_Getc(fifo_t *cBuf, uint8_t *data);
void CBUFFER_Reset(fifo_t *cBuf);
uint16_t CBUFFER_Available(fifo_t *cBuf);
uint16_t CBUFFER_PutStr(fifo_t *cBuf, uint8_t *data, uint16_t size);
uint16_t CBUFFER_GetBytes(fifo_t *cBuf, uint8_t *data, uint16_t size);

#endif /* _FIFO_H_ */
