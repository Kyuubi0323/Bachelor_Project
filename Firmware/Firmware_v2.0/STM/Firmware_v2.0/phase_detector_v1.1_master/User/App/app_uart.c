/*
 * app_uart.c
 *
 *  Created on: May 9, 2023
 *      Author: manht
 */
#include "main.h"
#include "log.h"
#include "gps_type.h"
#include "stimer.h"

extern offset[3];
static uint8_t Cal_CheckSumU8(uint8_t *buf, int len)
{
	uint8_t csum = 0;
	int i = 0;
	for (i = 0; i < len; i++) {
		csum ^= buf[i];
	}
	return csum;
}

void sendData(uint32_t timestamp, uint16_t l1, uint16_t l2, uint16_t l3, bool seq){
	uint8_t data[32] = {0};
	data[0] = 0xAA;
	data[1] = 0x01;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 11;
	*(uint32_t*)&data[5] = timestamp;
	*(uint16_t*)&data[9] = l1;
	*(uint16_t*)&data[11] = l2;
	*(uint16_t*)&data[13] = l3;
	data[15] = seq;
	data[16] = Cal_CheckSumU8(data, 16);
	data[17] = 0x55;
	HAL_UART_Transmit(&huart2, data, 18, 100);
}
void testSendData(void *param) {
	uint32_t timestamp = HAL_GetTick();
//	sendData(timestamp, 10, 16, 22, 1);
	startTimer(1000, testSendData, NULL);
	//sendData(timestamp, offset[0], offset[1], offset[2], 1);

}
void initAppUart(void) {
	startTimer(1000, testSendData, NULL);
}
