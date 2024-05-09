/*
 * fifo.h
 *
 *  Created on: Nov 29, 2021
 *      Author: manht
 */

#ifndef _FIFO_H_
#define _FIFO_H_

#include "stdint.h"
#include <stdbool.h>
#include "stdlib.h"
#include "string.h"
typedef struct fifo_t fifo_t;
typedef struct fifo_p {
	int (*fifoPutC)(fifo_t *fifo, uint8_t data);
	int (*fifoGetC)(fifo_t *fifo, uint8_t *data);
	void (*fifoReset)(fifo_t *fifo);
	int (*fifoAvailable)(fifo_t *fifo);
	int (*fifoPuts)(fifo_t *fifo, uint8_t *data, int size);
	int (*fifoGets)(fifo_t *fifo, uint8_t *data, int size);
} fifo_p;

struct fifo_t{
	const fifo_p * proc;
	uint8_t *buf;
	int size;
	int tail;
	int head;
	int count;
};

fifo_t * FifoInit(uint8_t *buffer, int size);
int FifoPutc(fifo_t * obj, uint8_t data);
int FifoGetc(fifo_t * obj, uint8_t *data);
void FifoReset(fifo_t * obj);
int FifoAvailable(fifo_t * obj);
int FifoGets(fifo_t *fifo, uint8_t *data, int size);
int FifoPuts(fifo_t *fifo, uint8_t * data, int size);
void FifoDeInit(fifo_t * fifo);
#endif /* _FIFO_H_ */
