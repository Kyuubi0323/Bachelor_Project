/**
  ******************************************************************************
  * @file    simcom_rtc.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of RTC peripherals.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_RTC_H_
#define _SIMCOM_RTC_H_

typedef struct rtc_time {
  int tm_sec;	//seconds [0,59]
  int tm_min;	//minutes [0,59]
  int tm_hour;  //hour [0,23]
  int tm_mday;  //day of month [1,31]
  int tm_mon;   //month of year [1,12]
  int tm_year; 	// since 1970
  int tm_wday; 	// day of week [0,6] (sunday = 0)
}t_rtc;

#endif

