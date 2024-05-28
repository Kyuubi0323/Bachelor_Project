#include "sdk.h"
#include "swiic.h"

#define I2C_CHANNEL     0

static swiic_p swiicProc = {
		swiic_writeByte,
		swiic_readByte
};
static swiic_t swiicObject = {&swiicProc, 0};

swiic_t* swiic_create() {
	return &swiicObject;
}

void swiic_dispose(swiic_t* iic) {
}

int swiic_writeByte(swiic_t* iic, uint8_t slaveAddr, uint8_t regsAddr, uint8_t data) {
    iic->error = (int)(sAPI_I2CWrite(I2C_CHANNEL, ((slaveAddr << 1) | 0x01), regsAddr, &data, 1));
	return iic->error;
}

uint8_t swiic_readByte(swiic_t* iic, uint8_t slaveAddr, uint8_t regsAddr) {
    uint8_t data = 0;
    iic->error = (int)(sAPI_I2CRead(I2C_CHANNEL, (slaveAddr << 1), regsAddr, &data, 1));
    return data;
}

int swiic_readBytes(swiic_t* iic, uint8_t slaveAddr, uint8_t regsAddr, uint8_t* buffer, int size) {
	iic->error = sAPI_I2CRead(I2C_CHANNEL, (slaveAddr << 1), regsAddr, buffer, size);
    return iic->error;
}
