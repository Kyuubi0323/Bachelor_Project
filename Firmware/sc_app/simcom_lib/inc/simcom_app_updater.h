/**
  ******************************************************************************
  * @file    simcom_app_updater.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of app update.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_APP_UPDATER_H__
#define __SIMCOM_APP_UPDATER_H__

typedef struct{
    int hasPackage;
    unsigned int binSize;
    unsigned int crcValue;
}SCAppPackageInfo;


#endif
