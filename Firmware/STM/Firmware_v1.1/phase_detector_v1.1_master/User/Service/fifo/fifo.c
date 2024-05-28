
#include "fifo.h"

static fifo_p fifo_proc = {
	FifoPutc,
	FifoGetc,
	FifoReset,
	FifoAvailable,
	FifoPuts,
	FifoGets,
};

static int FifoFull(fifo_t * obj)
{
	if (obj->count == obj->size)
		return 1;
	else
		return 0;
}

static int FifoEmpty(fifo_t * obj)
{
	if (obj->count == 0)
		return 1;
	else
		return 0;
}

static void FifoPut(fifo_t * obj, uint8_t data)
{
	obj->buf[obj->head] = data;
	obj->head = (obj->head + 1) % obj->size;
	obj->count++;
}

static void FifoGet(fifo_t * obj, uint8_t *data)
{
	*data = obj->buf[obj->tail];
	obj->tail = (obj->tail + 1) % obj->size;
	obj->count--;
}

fifo_t * FifoInit(uint8_t *buffer, int size)
{
	fifo_t * fifo = (fifo_t *) malloc(sizeof(fifo_t));
	if (fifo == NULL) return NULL;
	fifo->proc = &fifo_proc;
	fifo->buf = buffer;
	fifo->size = size;
	fifo->tail = fifo->head = fifo->count = 0;
	return fifo;
}
void FifoDeInit(fifo_t * fifo)
{
	if (fifo != NULL) free(fifo);
}
int FifoPutc(fifo_t * obj, uint8_t data)
{
	if (FifoFull(obj) == 1)
		return 0;
	FifoPut(obj, data);
	return 1;
}

int FifoGetc(fifo_t * obj, uint8_t *data)
{
	if (FifoEmpty(obj) == 1)
		return 0;
	FifoGet(obj, data);
	return 1;
}
int FifoPuts(fifo_t *fifo, uint8_t * data, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
		if (FifoPutc(fifo, data[i]) == 0) break;
	return i;
}
int FifoGets(fifo_t *fifo, uint8_t *data, int size)
{
	uint16_t bytes = 0;
	if (data == NULL) return 0;
	while (FifoAvailable(fifo))
	{
		bytes += FifoGetc(fifo, &data[bytes]);
		if (bytes > size - 1) break;
	}
	return bytes;
}
void FifoReset(fifo_t * obj)
{
	obj->count = obj->tail = obj->head = 0;
	memset(obj->buf, 0 , obj->size);
}
int FifoAvailable(fifo_t * obj)
{
	return obj->count;
}

