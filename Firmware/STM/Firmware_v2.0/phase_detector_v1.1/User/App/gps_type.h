/*
 * gps_type.h
 *
 *  Created on: Mar 9, 2023
 *      Author: manht
 */

#ifndef APP_GPS_TYPE_H_
#define APP_GPS_TYPE_H_

#include "stdint.h"
#include "stdbool.h"

#define FRAME_SIZE	256

typedef struct nmeaData_t
{
	uint8_t buffer[FRAME_SIZE];
	int size;
} nmeaData_t;


typedef struct {
	uint32_t ts;
	uint16_t phaseA;
	uint16_t phaseB;
	uint16_t phaseC;
	char *seq;
	char *ret;
	int8_t offset;
} measureData_t;

extern measureData_t measureData[];
#endif /* APP_GPS_TYPE_H_ */
