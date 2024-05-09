#include "sdk.h"
#include "flash.h"

int flash_read(uint32_t address, uint8_t* buffer, int size) {
    return sAPI_ReadFlash(address, (char*)buffer, size);
}

int flash_write(uint32_t address, uint8_t* buffer, int size) {
    return sAPI_WriteFlash(address, (char*)buffer, size);
}

int flash_erase(uint32_t block, int size) {
    return sAPI_EraseFlashSector(block & 0x1, size);
}