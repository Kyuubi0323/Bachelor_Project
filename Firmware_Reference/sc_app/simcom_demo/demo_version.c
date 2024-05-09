/**
  ******************************************************************************
  * @file    demo_version.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of FW version management.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "simcom_api.h"
#include "simcom_system.h"

/**
  * @brief  Read IMEI from module.
  * @param  void
  * @note   
  * @retval 0:succesful  1: error
  */
unsigned int sc_getimei(void)
{
    int ret = 0;
    char imei_value[16];
    ret = sAPI_SysGetImei(imei_value);
    sAPI_Debug("imei_value : %s", imei_value);
    return ret;
}

/**
  * @brief  Read some version information from module.
  * @param  void
  * @note   
  * @retval void
  */
void sc_getversioninfo(void)
{
    SIMComVersion simcominfo;
    sAPI_SysGetVersion (&simcominfo);
    sAPI_Debug("manufacture_namestr: %s ; Module_modelstr: %s ; Revision: %s ; GCAP: %s ;", 
        simcominfo.manufacture_namestr, simcominfo.Module_modelstr, simcominfo.Revision, simcominfo.GCAP);
    sAPI_Debug("cgmr: %s ; internal_verstr: %s ; version_tagstr: %s ; SDK_Version: %s ;", 
        simcominfo.cgmr, simcominfo.internal_verstr, simcominfo.version_tagstr, simcominfo.SDK_Version);
    return;
}

/**
  * @brief  Read cust defined version information from module.
  * @param  void
  * @note   For customerizied version
  * @retval void
  */
void sc_getcusversion(void)
{
    CUSVersion CUSinfo;
    sAPI_SysGetCusVersion (&CUSinfo);
    sAPI_Debug("cusversion: %s",CUSinfo.cusversion);
    return;
}

/**
  * @brief  Read RF version from module.
  * @param  void
  * @note   
  * @retval void
  */
void sc_getrfversion(void)
{
    RFVersion RFinfo;
    sAPI_SysGetRFVersion (&RFinfo);
    sAPI_Debug("rfversion1: %s ; rfversion2:%s ", RFinfo.rfversion1, RFinfo.rfversion1);
    return;
}

/**
  * @brief  Demo version task creation.
  * @param  void
  * @note   
  * @retval void
  */
void sAPP_VersionDemo(void)
{
    if(0 != sc_getimei())
    {
        sAPI_Debug("get IMEI err");
    }
    
    sc_getversioninfo();

    sc_getcusversion();

    sc_getrfversion();

    return;
}

