#ifndef __SWIIC_H__
#define __SWIIC_H__

typedef struct swiic_t swiic_t;

typedef struct swiic_p {
	int(*write)(swiic_t*, uint8_t, uint8_t, uint8_t);
	uint8_t(*read)(swiic_t*, uint8_t, uint8_t);
} swiic_p;

struct swiic_t {
	const swiic_p* proc;
	int error;
};

swiic_t* swiic_create(void);
void swiic_dispose(swiic_t* iic);
int swiic_writeByte(swiic_t* iic, uint8_t slaveAddr, uint8_t regsAddr, uint8_t data);
int swiic_readBytes(swiic_t* iic, uint8_t slaveAddr, uint8_t regsAddr, uint8_t* buffer, int size);
uint8_t swiic_readByte(swiic_t* iic, uint8_t slaveAddr, uint8_t regsAddr);

#endif
