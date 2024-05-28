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

#define SLAVE 	1

#ifndef NMEA_DEBUG
#define NMEA_DEBUG	0
#endif

#define SAMPLE_CNT 10

uint8_t gps_rx = 0;

uint8_t RxBuf[FRAME_SIZE] = {0};

__IO int fix_gps = 0;
__IO int startSample = 0;
int start_timer = 0;
__IO int start_phaseA = 0;
uint16_t phaseA_time[256] = {0};

int phaseA_cnt = 0;

__IO uint32_t tick = 0;

uint16_t offset[3] = {0};
static uint32_t lastTick[3] = {0};
lwgps_t nmea;

measureData_t measureData[SAMPLE_CNT] = {0};
int count = 0;

extern bool MQTT_Connected;
extern bool endNmea;
extern nmeaData_t EvtData;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPS_PPS_Pin) {
		if (!fix_gps)
			fix_gps = true;
		if (startSample && !start_timer) {
			tick = 0;
			lastTick[0] = 0;
			HAL_TIM_Base_Start_IT(&htim4);
			start_timer = 1;
		}
	}
	else if (GPIO_Pin == L1_Pin) {
		if (start_timer == 1) {
			phaseA_time[phaseA_cnt++] = (uint16_t)(tick - lastTick[0]);
			lastTick[0] = tick;
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
	if (phaseA_cnt >= 5) {
//		  for (int i = 0; i < phaseA_cnt; i++)
//			  printf("%d ", phaseA_time[i]);
//		  printf("\r\n ");
		  phaseA_cnt = 0;
		  HAL_TIM_Base_Stop_IT(&htim4);
		  start_timer = false;
		  offset[0] = phaseA_time[0];
		  memset(phaseA_time, 0, sizeof(phaseA_time));
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
				measureData[count].ts = dt2epoch(dt);
				measureData[count].phaseA = offset[0];
//				printf("SAM data: [%ld] %d \r\n", dt2epoch(dt), offset[0]);
		}
		if (count < SAMPLE_CNT - 1) count++;
		else {
			count = 0;
			memset(&measureData[0], 0, sizeof(measureData));
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

