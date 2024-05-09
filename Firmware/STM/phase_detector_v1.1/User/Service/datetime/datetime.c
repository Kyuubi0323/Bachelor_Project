#include "datetime.h"

const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d)
{
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i) // Add number of days for each month
    {
        days += daysInMonth[i - 1];
    }                        // of for-next loop for each month
    if (m > 2 && y % 4 == 0) // Deal with leap years
    {
        ++days;
    }                                         // of if-then a leap year
    if (((y % 100) == 0) && ((y % 400) != 0)) // if / by 100 but not by 400 then it is not a l.y.
    {
        --days;
    }                                        // of if-then special leap year
    return days + 365 * y + (y + 3) / 4 - 1; // Return computed value
}
static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s)
{
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

void epoch2dt(DateTime_t *this, uint32_t t)
{
    this->unixtime = t;
    if (t < SECS_1970_TO_2000)
        t = 0; // set to lowest possible date 2000-01-01
    else
        t -= SECS_1970_TO_2000; // bring to 2000 timestamp from 1970
    this->second = t % 60;
    t /= 60;
    this->minute = t % 60;
    t /= 60;
    this->hour = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (this->year = 0;; ++this->year)
    {
        leap = this->year % 4 == 0;
        if (days < (uint16_t)365 + leap)
            break;
        days -= 365 + leap;
    }
    for (this->month = 1;; ++this->month)
    {
        uint8_t daysPerMonth = daysInMonth[this->month - 1];
        if (leap && this->month == 2)
            ++daysPerMonth;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }
    this->day = days + 1;
    uint16_t temp = date2days(this->year, this->month, this->day);
    uint8_t dow = ((temp + 6) % 7);
    this->dayOfTheWeek = (dow == 0) ? 7 : dow; 
}
uint32_t dt2epoch(DateTime_t dt)
{
    uint16_t days = date2days(dt.year, dt.month, dt.day);
    uint32_t t = time2long(days, dt.hour, dt.minute, dt.second);
    t += SECS_1970_TO_2000;
    return t;
}
void dayOfWeek(DateTime_t *this)
{
    uint16_t temp = date2days(this->year, this->month, this->day);
    uint8_t dow = ((temp + 6) % 7);
    this->dayOfTheWeek = (dow == 0) ? 7 : dow; 
}