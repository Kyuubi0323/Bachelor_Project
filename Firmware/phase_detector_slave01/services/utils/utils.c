/*
 * utils.c
 *
 *  Created on: 1 thg 9, 2020
 *      Author: E7470
 */
#include "sdk.h"
#include "log.h"
#include "utils.h"

int u_atoi(char* str) {
	int integer = 0;
	int index = 0;

	while ((str[index] != 0) && (str[index] != '\n')) {
		if ((str[index] >= '0') && (str[index] <= '9'))
			integer = (integer * 10) + (str[index] - 0x30);
		index++;
	}
	return integer;
}

double u_atof(char* str) {
	int integer = 0;
	int fraction = 0;
	int factor = 1;
	int index = 0;
	bool negative = false;
	double value = 0;

	while ((str[index] != '.') && (str[index] != 0)) {
		if ((str[index] >= '0') && (str[index] <= '9'))
			integer = (integer * 10) + (str[index] - 0x30);
		else if (str[index] == '-')
			negative = true;
		index++;
	}
	if (str[index] != 0)
		index++;

	while (str[index] != 0) {
		if ((str[index] >= '0') && (str[index] <= '9')) {
			fraction = (fraction * 10) + (str[index] - 0x30);
			factor *= 10;
		}
		index++;
		if (factor >= 1000000)
			break;
	}
	value = ((double)fraction) / factor;
	value += integer;
	if (negative)
		value = -value;
	return value;
}

uint32_t u_ip2addr(char* ip) {
	int index = 0;
	uint32_t value = 0;
	uint8_t temp[4] = {0};

	while ((*ip != '\0') && (index < 4)) {
		if (*ip == '.') {
			index++;
			ip++;
			continue;
		}

		if ((*ip <= '9') && (*ip >= '0'))
			temp[index] = (temp[index] * 10) + (*ip - '0');

		ip++;
	}
	if (index >= 3) {
		value = temp[0];
		value = (value << 8) | temp[1];
		value = (value << 8) | temp[2];
		value = (value << 8) | temp[3];
	}
	return value;
}

#define YEAR0 1900 /* the first year */
#define EPOCH_YR 1970 /* EPOCH = Jan 1 1970 00:00:00 */
#define SECS_DAY (24L * 60L * 60L)
#define LEAPYEAR(year) (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year) (LEAPYEAR(year) ? 366 : 365)
#define FIRSTSUNDAY(timp) (((timp)->tm_yday - (timp)->tm_wday + 420) % 7)
#define FIRSTDAYOF(timp) (((timp)->tm_wday - (timp)->tm_yday + 420) % 7)

const int _ytab[2][12] = {
		{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
		{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }};

struct tm* u_gmtime(const time_t* ts) {
	static struct tm br_time;
	struct tm *timep = &br_time;
	time_t time = *ts;
	unsigned long dayclock, dayno;
	int year = YEAR0;
	dayclock = (unsigned long)time % SECS_DAY;
	dayno = (unsigned long)time / SECS_DAY;
	timep->tm_sec = dayclock % 60;
	timep->tm_min = (dayclock % 3600) / 60;
	timep->tm_hour = dayclock / 3600;
	timep->tm_wday = (dayno + 4) % 7; /* day 0 was a thursday */
	{
		unsigned long zz = dayno / 1461; // 365+365+365+366 Breaks beyond 2100
		if (zz > 32) zz = 32; // fix 2101-2106
		year += zz * 4;
		dayno -= zz * 1461;
	}
	while (dayno >= YEARSIZE(year)) {
		dayno -= YEARSIZE(year);
		year++;
	}
	timep->tm_year = year - YEAR0;
	timep->tm_yday = dayno;
	timep->tm_mon = 0;
	while (dayno >= _ytab[LEAPYEAR(year)][timep->tm_mon]) {
		dayno -= _ytab[LEAPYEAR(year)][timep->tm_mon];
		timep->tm_mon++;
	}
	timep->tm_mday = dayno + 1;
	timep->tm_isdst = 0;
	return timep;
}

uint32_t u_mktime(struct tm* time) {
    unsigned long dayno = 0;
    unsigned long dayclock = time->tm_hour * 3600 + time->tm_min * 60 + time->tm_sec;
    int year = time->tm_year - 70; // EPOCH_YR - YEAR0
	int month = time->tm_mon - 1;
	int year_count;

    dayno = ((int)(year >> 2)) * 1461;
    dayno += (time->tm_mday - 1);
    year = time->tm_year + YEAR0;

    while (month >= 0) {
        dayno += _ytab[LEAPYEAR(year)][month];
        month--;
    }
    year_count = (year - 70) & 0x03; // year % 4
    while (year_count > 0) {
        dayno += YEARSIZE(year - 1);
        year_count--;
        year--;
    }
    return (dayno * SECS_DAY) + dayclock;
}

int split_string(char* source, char* dest, int start, int src_size, int dest_size, char delim) {
    int count = 0;
	umemset(dest, 0, dest_size);

	while ((start < src_size) && (count < dest_size)) {
		if (source[start] == delim) {
			start++;
			break;
		}
		dest[count] = source[start];
		start++;
		count++;
	}
	if (count >= dest_size)
		return (-1);
	return start;
}
