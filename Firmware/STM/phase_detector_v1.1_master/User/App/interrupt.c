/*
 * interrupt.c
 *
 *  Created on: Jan 11, 2023
 *      Author: manht
 */
#include <fifo.h>
#include "main.h"
#include "gps_type.h"
#include "string.h"


extern uint8_t gps_rx;
extern __IO uint32_t tick;
extern bool phaseA_connect;

bool begin = false;
bool endNmea = false;
int idx = 0;
nmeaData_t EvtData = {0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM4)
  {
    tick++;
  }
  if (htim->Instance == TIM3) {
	  phaseA_connect = false;
	  HAL_TIM_Base_Stop_IT(&htim3);
  }
  /* USER CODE END Callback 1 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t lastData = 0;
	if (huart == &huart1){
		if (gps_rx == '$' && !begin) {
			begin = true;
			idx = 0;
			lastData = 0;
			memset(&EvtData, 0 , sizeof(nmeaData_t));
		}
		else if (gps_rx == 10 && lastData == 13 && begin) {
			begin = false;
			endNmea = true;
			EvtData.size = idx;
		}
		if (begin) {
			EvtData.buffer[idx++] = gps_rx;
			lastData = gps_rx;
		}
		HAL_UART_Receive_IT(&huart1, &gps_rx, 1);
	}
}

