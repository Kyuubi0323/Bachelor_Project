#ifndef __FLASH_H__
#define __FLASH_H__

int flash_read(uint32_t address, uint8_t* buffer, int size);
int flash_write(uint32_t address, uint8_t* buffer, int size);
int flash_erase(uint32_t block, int size); // block 0 or 1

#endif // __FLASH_H__