#ifndef _DATE_TIME_H_
#define _DATE_TIME_H_
#include <stdint.h>

#define SECS_1970_TO_2000   946684800

typedef struct DateTime_t
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dayOfTheWeek;
    uint32_t unixtime;
} DateTime_t;
uint32_t dt2epoch(DateTime_t dt);
void epoch2dt(DateTime_t *this, uint32_t t);
#endif
