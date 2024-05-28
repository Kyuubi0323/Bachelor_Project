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

#define OFFSET	6
extern uint8_t 	refDataBuffer[];
extern int 		refDataIdx;
extern uint8_t 	endRefData;
extern uint8_t 	beginRefData;
extern uint8_t 	refData ;

measureData_t refDataPhase = {0};
extern measureData_t measureData[];
extern int count;
void appUartInit(void) {
	HAL_UART_Receive_IT(&huart2, &refData, 1);
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
	if (endRefData) {
		endRefData = 0;
		parseRefData();
		for (int i = 0; i < count ;i++)
			if (measureData[i].ts == refDataPhase.ts) {
//				if (measureData[i].phaseA < refDataPhase.phaseA)
//					measureData[i].phaseA = measureData[i].phaseA + 200;
//				uint16_t offset = abs (measureData[i].phaseA - refDataPhase.phaseA - OFFSET) ;
//				int offset =  (measureData[i].phaseA - refDataPhase.phaseA);
				printf("%ld, %d, %d, %s \r\n", refDataPhase.ts,
						refDataPhase.phaseA, measureData[i].phaseA, (measureData->seq) ? "ACB" : "ABC");
			}
		return 1;
	}
	return 0;
}
