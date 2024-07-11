/*
 * app_uart.c
 *
 *  Created on: May 9, 2023
 *      Author: manht
 */

#include "log.h"
#include "main.h"
#include "fifo.h"
#include "gps_type.h"
#include <math.h>
#include <string.h>

#define OFFSET	6
extern uint8_t 	refDataBuffer[];
extern int 		refDataIdx;
extern uint8_t 	endRefData;
extern uint8_t 	beginRefData;
extern uint8_t 	refData ;

measureData_t refDataPhase = {0};
extern measureData_t measureData[];
extern int count;
extern void sendData(uint32_t timestamp, uint16_t l1, uint16_t l2, uint16_t l3, char* ret);
void appUartInit(void) {
	HAL_UART_Receive_IT(&huart2, &refData, 1);
}

//New
void setLedstatus(int led0, int led1, int led2)
{
	HAL_GPIO_WritePin(GPIOB, LED0_Pin, led0);
	HAL_GPIO_WritePin(GPIOB, LED1_Pin, led1);
	HAL_GPIO_WritePin(GPIOB, LED0_Pin, led2);
}

void ledDisplay(char *ret)
{
	if(strcmp(ret, "A") == 0)
		setLedstatus(0, 1, 1);
	else if(strcmp(ret, "B") == 0)
		setLedstatus(1, 0, 1);
	else
		setLedstatus(1, 1, 0);
}

//void chasingLedInit(void)
//{
//	setLedstatus(0, 1, 1);
//	HAL_Delay(200);
//	setLedstatus(1, 0, 1);
//	HAL_Delay(200);
//	setLedstatus(1, 1, 0);
//	HAL_Delay(200);
//	setLedstatus(0, 1, 1);
//	HAL_Delay(100);
//	setLedstatus(1, 0, 1);
//	HAL_Delay(100);
//	setLedstatus(1, 1, 0);
//	HAL_Delay(100);
//	setLedstatus(0, 1, 1);
//	HAL_Delay(50);
//	setLedstatus(1, 0, 1);
//	HAL_Delay(50);
//	setLedstatus(1, 1, 0);
//	HAL_Delay(50);
//}

static uint8_t Cal_CheckSumU8(uint8_t *buf, int len)
{
	uint8_t csum = 0;
	int i = 0;
	for (i = 0; i < len; i++) {
		csum ^= buf[i];
	}
	return csum;
}

void sendData(uint32_t timestamp, uint16_t l1, uint16_t l2, uint16_t l3, char* ret){
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
	data[15] = *ret;
	data[16] = Cal_CheckSumU8(data, 16);
	data[17] = 0x55;
	HAL_UART_Transmit(&huart2, data, 18, 50);
}
static void parseRefData(void) {
	int idx = 0, i = 0;
	uint8_t pos = 0;
	uint8_t temp[32] = {0};
	for (i = 0; i < refDataIdx; i++) {
		if (refDataBuffer[i] == ',') {
			memcpy(temp, &refDataBuffer[idx], i - idx);
			if (pos == 1)
				refDataPhase.ts = atoi((char*)temp);
			else if (pos == 2)
				refDataPhase.phaseA = atoi((char*)temp);
			else if (pos == 3)
				refDataPhase.phaseB = atoi((char*)temp);
			else if (pos == 4)
				refDataPhase.phaseC = atoi((char*)temp);
			else if (pos == 5) {
				if (!strcmp((char*)temp, "ABC"))
					refDataPhase.seq = "ABC";
				else if (!strcmp((char*)temp, "ACB"))
					refDataPhase.seq = "ACB";
			}
			memset(temp, 0, sizeof(temp));
			idx = i+1;
			pos ++;
		}
	}
	printf("REF data: [%ld] %d %d %d - %s\r\n", refDataPhase.ts,
			refDataPhase.phaseA, refDataPhase.phaseB, refDataPhase.phaseC, refDataPhase.seq);
}
int processSimData(void) {
	//char Phase = {0};
	//char *phs = {0};
	if (endRefData) {
		endRefData = 0;
		parseRefData();

		for (int i = 0; i < count ;i++)
			if (measureData[i].ts == refDataPhase.ts) {
				measureData[i].offset =(measureData[i].phaseA - refDataPhase.phaseA);

				uint8_t absPhaseB = abs(measureData[i].phaseA - refDataPhase.phaseB);
				uint8_t absPhaseC = abs(measureData[i].phaseA - refDataPhase.phaseC);
				uint8_t absXphaseB = abs(measureData[i].phaseA + 200 - refDataPhase.phaseB);
				uint8_t absXphaseC = abs(measureData[i].phaseA + 200 - refDataPhase.phaseC);
				uint8_t absXphaseA = abs(measureData[i].phaseA - refDataPhase.phaseA);
				if (measureData[i].offset > 0)
				{
					measureData[i].ret = (measureData[i].offset < absPhaseB) ?
							((measureData[i].offset < absPhaseC) ? "A" : "C") : ((absPhaseB < absPhaseC) ? "B" : "C");
				}
				else
				{
					measureData[i].ret = (absXphaseA < absXphaseB) ?
							((absXphaseA < absXphaseC) ? "A" : "C") : ((absXphaseB < absXphaseC) ? "B" : "C");
				}
				printf("%ld, %d, %d, %d, %s \r\n", refDataPhase.ts,
						refDataPhase.phaseA, measureData[i].phaseA, measureData[i].offset, measureData[i].ret);
				//ledDisplay(measureData[i].ret);
				sendData(measureData[i].ts, measureData[i].phaseA, measureData[i].phaseB,
						measureData[i].phaseC, measureData[i].ret);
			}
		return 1;
	}
	return 0;
}

