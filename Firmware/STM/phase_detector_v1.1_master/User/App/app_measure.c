/*
 * gnss.c
 *
 *  Created on: Mar 9, 2023
 *      Author: manht
 */
#include "main.h"
#include "log.h"
#include "gps_type.h"
#include "lwgps.h"
#include "datetime.h"


#include "string.h"
#include "stimer.h"

#define SLAVE 	0

#ifndef NMEA_DEBUG
#define NMEA_DEBUG	1
#endif

#define SAMPLE_CNT 10

uint8_t gps_rx = 0;

uint8_t RxBuf[FRAME_SIZE] = {0};

__IO int fix_gps = 0;
__IO int startSample = 0;
int start_timer = 0;
__IO int start_phaseA = 0;
uint16_t phaseA_time[256] = {0};
uint16_t phaseB_time[256] = {0};
uint16_t phaseC_time[256] = {0};

int phaseA_cnt = 0;
int phaseB_cnt = 0;
int phaseC_cnt = 0;
__IO uint32_t tick = 0;

uint16_t offset[3] = {0};
static uint32_t lastTick[3] = {0};
lwgps_t nmea;

measureData_t measureData = {0};

bool phaseA_connect = 0;

extern bool MQTT_Connected;
extern bool endNmea;
extern nmeaData_t EvtData;

extern void sendData(uint32_t timestamp, uint16_t l1, uint16_t l2, uint16_t l3, bool seq);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPS_PPS_Pin) {
		if (!fix_gps)
			fix_gps = true;
		if (startSample && !start_timer) {
			tick = 0;
			lastTick[0] = 0;
			lastTick[1] = 0;
			lastTick[2] = 0;
			HAL_TIM_Base_Start_IT(&htim4);
			start_timer = 1;
			start_phaseA = 0;
		}
	}
	else if (GPIO_Pin == L1_Pin) {
		phaseA_connect = 1;
		HAL_TIM_Base_Start_IT(&htim3);
		if (!start_phaseA)
			start_phaseA = 1;
		if (start_timer == 1) {
			phaseA_time[phaseA_cnt++] = (uint16_t)(tick - lastTick[0]);
			lastTick[0] = tick;
		}
	}
	else if (GPIO_Pin == L2_Pin) {
		if (start_timer == 1 && start_phaseA) {
			phaseB_time[phaseB_cnt++] = (uint16_t)(tick - lastTick[1]);
			lastTick[1] = tick;
		}
	}
	else if (GPIO_Pin == L3_Pin) {
		if (start_timer == 1 && start_phaseA) {
			phaseC_time[phaseC_cnt++] = (uint16_t)(tick - lastTick[2]);
			lastTick[2] = tick;
		}
	}
}

static bool UartReInit(uint32_t baud) {
	bool ret = 0;
	HAL_UART_AbortReceive(&huart1);
	HAL_UART_DeInit(&huart1);
	huart1.Init.BaudRate = baud;
	if (HAL_UART_Init(&huart1) == HAL_OK) {
		ret = 1;
		HAL_UART_Receive_IT(&huart1, &gps_rx, 1);;
	}
	return ret;
}
uint32_t findBaudRate(void) {
	uint32_t baud[] = { 9600, 115200, 19200};
	uint32_t baudrate = 0;
	for (int i = 0; i < (sizeof(baud)/sizeof(baud[0])); i++) {
		if (!UartReInit(baud[i])) continue;
		uint32_t start = HAL_GetTick();
		while (HAL_GetTick() - start < 5000) {
			if (endNmea) {
				endNmea = false;
				baudrate = baud[i];
				return baudrate;
			}
		}
		Logi("Try Baudrate = %ld timeout", baud[i]);
	}
	return baudrate;
}
void processMeasure(void) {
	static uint8_t fixStatus = 0;
	if (phaseA_cnt >= 15 || phaseB_cnt >= 15 || phaseC_cnt >= 15) {
		  phaseA_cnt = 0;
		  phaseB_cnt = 0;
		  phaseC_cnt = 0;
		  HAL_TIM_Base_Stop_IT(&htim4);
		  start_timer = false;
		  offset[0] = phaseA_time[0];
		  offset[1] = phaseB_time[0];
		  offset[2] = phaseC_time[0];
		  memset(phaseA_time, 0, sizeof(phaseA_time));
		  memset(phaseB_time, 0, sizeof(phaseB_time));
		  memset(phaseC_time, 0, sizeof(phaseC_time));
	}

	if (endNmea) {
		endNmea = false;
#if (NMEA_DEBUG)
		printf("%s\n", EvtData.buffer);
#endif
		lwgps_process(&nmea, EvtData.buffer, EvtData.size);
		if (nmea.is_valid != fixStatus) {
			fixStatus = nmea.is_valid;
			if (nmea.is_valid) {
				startSample = true;
				Logi("GNSS fix, start sample");
			}
			else if (!nmea.is_valid) {
				startSample = false;
				Logi("GNSS no fix, stop sample");
			}
		}
		if (startSample) {
			DateTime_t dt = {0};
			dt.year = nmea.year;
			dt.month = nmea.month;
			dt.day = nmea.date;
			dt.hour = nmea.hours;
			dt.minute = nmea.minutes;
			dt.second = nmea.seconds;
			measureData.ts = dt2epoch(dt);
			measureData.phaseA = offset[0];
			if (offset[1] > offset[2]) {
				measureData.phaseB = offset[2];
				measureData.phaseC = offset[1];
				measureData.seq = 1;
			}
			else {
				measureData.phaseB = offset[1];
				measureData.phaseC = offset[2];
				measureData.seq = 0;
			}
			Logi("Sample data on phase: [%ld] %d %d %d - %s", dt2epoch(dt),
					offset[0], offset[1], offset[2], measureData.seq ? "ACB" : "ABC");
//			char msg[128] = {0};
//			sprintf(msg,"%ld, %d, %d, %d, %s", measureData.ts, measureData.phaseA,
//					measureData.phaseB, measureData.phaseC, measureData.seq);
			//Send to SIM here
			sendData(measureData.ts, measureData.phaseA, measureData.phaseB,
					measureData.phaseC, measureData.seq);
			memset(&measureData, 0, sizeof(measureData));
		}
	}
}
void gnssInit(void){
	HAL_GPIO_WritePin(GPS_RST_GPIO_Port, GPS_RST_Pin, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPS_RST_GPIO_Port, GPS_RST_Pin, 1);
	HAL_Delay(900);
	lwgps_init(&nmea);
	uint32_t baud = findBaudRate();
	if (baud == 0)
		return;
	if (baud != 9600) {
		Logi("current baudrate = %ld -> switch to 9600", baud);
		uint8_t *cmd = (uint8_t*)"$PMTK251,9600*17\r\n";
		HAL_UART_Transmit(&huart1, cmd, strlen((char*)cmd), 1000);
		UartReInit(9600);
	}
	else Logi("current baudrate = %ld", baud);
	HAL_Delay(1000);
	uint8_t *cmd = (uint8_t*)"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
	HAL_UART_Transmit(&huart1, cmd, strlen((char*)cmd), 1000);
}
void appMeasureInit(void) {
	gnssInit();
}

