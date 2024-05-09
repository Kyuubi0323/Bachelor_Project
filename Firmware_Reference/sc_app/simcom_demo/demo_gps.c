/**
  ******************************************************************************
  * @file    demo_gps.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of gps operation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/
#ifdef FEATURE_SIMCOM_GPS
#include "simcom_api.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define NMEA_STRING_LEN (256)

typedef enum{
    SC_GNSS_DEMO_AP_FLASH_STATUS        = 1,
    SC_GNSS_DEMO_POWER_STATUS           = 2,
    SC_GNSS_DEMO_OUTPUT_NMEA_PORT       = 3,
    SC_GNSS_DEMO_START_MODE             = 4,
    SC_GNSS_DEMO_BAUD_RATE              = 5,
    SC_GNSS_DEMO_MODE                   = 6,
    SC_GNSS_DEMO_NMEA_RATE              = 7,
    SC_GNSS_DEMO_NMEA_SENTENCE          = 8,
    SC_GNSS_DEMO_GPS_INFO               = 9,
    SC_GNSS_DEMO_GNSS_INFO              = 10,
    SC_GNSS_DEMO_SEND_CMD               = 11,
    SC_GNSS_DEMO_AGPS                   = 12,
    SC_GNSS_DEMO_BACK                   = 99
}SC_GNSS_DEMO_TYPE;


typedef enum{
    SC_GNSS_DEMO_POWER_ON           = 1,
    SC_GNSS_DEMO_POWER_OFF          = 2,
    SC_GNSS_DEMO_POWER_STATUS_GET   = 3,
    SC_GNSS_DEMO_POWER_BACK         = 99
}SC_GNSS_DEMO_POWER_STATUS_API;

typedef enum{
    SC_GNSS_DEMO_AP_FLASH_ON           = 1,
    SC_GNSS_DEMO_AP_FLASH_OFF          = 2,
    SC_GNSS_DEMO_AP_FLASH_STATUS_GET   = 3,
    SC_GNSS_DEMO_AP_FLASH_BACK         = 99
}SC_GNSS_DEMO_AP_FLASH_STATUS_API;

typedef enum{
    SC_GNSS_DEMO_START_GET_NMEA_DATA_BY_PORT    = 1,
    SC_GNSS_DEMO_STOP_GET_NMEA_DATA_BY_PORT     = 2,
    SC_GNSS_DEMO_START_GET_NMEA_DATA_BY_URC     = 3,
    SC_GNSS_DEMO_STOP_GET_NMEA_DATA_BY_URC      = 4,
    SC_GNSS_DEMO_GET_NMEA_DATA_BACK     = 99
}SC_GNSS_DEMO_GET_NMEA_DATA_API;

typedef enum{
    SC_GNSS_DEMO_START_HOT      = 1,
    SC_GNSS_DEMO_START_WARM     = 2,
    SC_GNSS_DEMO_START_COLD     = 3,
    SC_GNSS_DEMO_START_BACK     = 99
}SC_GNSS_DEMO_START_MODE_API;

typedef enum{
    //SC_GNSS_DEMO_BAUD_RATE_4800      = 1,
    SC_GNSS_DEMO_BAUD_RATE_9600      = 1,
    //SC_GNSS_DEMO_BAUD_RATE_19200     = 3,
    //SC_GNSS_DEMO_BAUD_RATE_38400     = 4,
    //SC_GNSS_DEMO_BAUD_RATE_57600     = 5,
    SC_GNSS_DEMO_BAUD_RATE_115200    = 2,
    SC_GNSS_DEMO_BAUD_RATE_230400    = 3,
    SC_GNSS_DEMO_BAUD_RATE_GET       = 4,
    SC_GNSS_DEMO_BAUD_RATE_BACK      = 99
}SC_GNSS_DEMO_BAUD_RATE_API;


typedef enum{
#ifdef FEATURE_SIMCOM_GPS_OVERSEAS
    SC_GNSS_DEMO_MODE_GPS_L1_SBAS_QZSS                  = 1,
    SC_GNSS_DEMO_MODE_BDS                               = 2,
    SC_GNSS_DEMO_MODE_GPS_GLONASS_GALILEO_SBAS_QZSS     = 3,
    SC_GNSS_DEMO_MODE_GPS_BDS_GALILEO_SBAS_QZSS         = 4,
    SC_GNSS_DEMO_MODE_GET              = 5,
#else
    SC_GNSS_DEMO_MODE_GPS_BDS_QZSS     = 1,
    SC_GNSS_DEMO_MODE_BDS              = 2,
    SC_GNSS_DEMO_MODE_GPS_QZSS         = 3,
    //SC_GNSS_DEMO_MODE_GLONASS          = 4,
    //SC_GNSS_DEMO_MODE_GPS_GLONASS      = 5,
    //SC_GNSS_DEMO_MODE_BDS_GLONASS      = 6,
    //SC_GNSS_DEMO_MODE_GPS_BDS_GLONASS  = 7,
    SC_GNSS_DEMO_MODE_GET              = 4,
#endif
    SC_GNSS_DEMO_MODE_BACK             = 99
}SC_GNSS_DEMO_MODE_API;

typedef enum{
    SC_GNSS_DEMO_NMEA_RATE_1Hz              = 1,
    SC_GNSS_DEMO_NMEA_RATE_2Hz              = 2,
    SC_GNSS_DEMO_NMEA_RATE_5Hz              = 3,
    SC_GNSS_DEMO_NMEA_RATE_10Hz             = 4,
    SC_GNSS_DEMO_NMEA_RATE_GET              = 5,
    SC_GNSS_DEMO_NMEA_RATE_BACK             = 99
}SC_GNSS_DEMO_NMEA_RATE_API;

typedef enum{
    SC_GNSS_DEMO_NMEA_SENTENCE_INPUT         = 1,
    SC_GNSS_DEMO_NMEA_SENTENCE_GET           = 2,
    SC_GNSS_DEMO_NMEA_SENTENCE_BACK          = 99
}SC_GNSS_DEMO_NMEA_SENTENCE_API;

typedef enum
{
    SC_GNSS_DEMO_RETURN_CODE_OK                =  0,
    SC_GNSS_DEMO_RETURN_CODE_ERROR             = -1,

    SC_GNSS_DEMO_CODE_SOCKRT_ERROR             = 101,
    SC_GNSS_DEMO_CODE_GET_SERVER_ERROR         = 102,
    SC_GNSS_DEMO_CODE_CONNECT_ERROR            = 103,
    SC_GNSS_DEMO_CODE_WRITE_SOCKET_ERROR       = 104,
    SC_GNSS_DEMO_CODE_READ_SOCKET_ERROR        = 105

}SC_Gnss_DEMO_Return_Code;


extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);

/**
  * @brief  Get GNSS power state
  * @param  void
  * @note   
  * @retval 0: GNSS is power off  1:GNSS is power on
  */
BOOL powerOn(void)
{
    SC_Gnss_Power_Status pwrStatus;
    pwrStatus = sAPI_GnssPowerStatusGet();
    if(SC_GNSS_POWER_ON == pwrStatus)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  Demo for GNSS.
  * @param  void
  * @note   Responses are captured through attention SC_URC_INTERNAL_GNSS_MASK in cus_urc.c
  * @retval void
  */
void GNSSDemo(void)
{
    SIM_MSG_T optionMsg ={0,0,0,NULL};
    UINT32 opt = 0;
    UINT32 ret = 0;
    UINT8 i;
    UINT8 outputBuf[256],arrBuf[32];
    UINT8 *arr=NULL;
    INT8 nmeaString[NMEA_STRING_LEN];
    SC_Gnss_Power_Status pwrStatus;
    SC_Gnss_Ap_Flash_Status apFlashStatus;
    SC_Gnss_Baud_Rate baudRate;
    SC_Gnss_Mode gnssMode;
    SC_Gnss_Nmea_Rate nmeaRate;
    INT32 pGreg = 0;
    UINT8 cpin = 0;
    SC_simcard_err_e simRet = SC_SIM_RTEURN_UNKNOW;

    INT8 *note = "\r\nPlease select an option to test from the items listed below, demo just for GNSS.\r\n";
    INT8 *options_list[] = {
       "1. GNSS ap_flash status",
       "2. GNSS power status",
       "3. Get NMEA data",
       "4. GNSS start mode",
       "5. GNSS baud rate",
       "6. GNSS mode",
       "7. GNSS nmea rate",
       "8. GNSS nmea sentence",
       "9. GPS information",
       "10. GNSS information",
       "11. Send command to GNSS",
       "12. AGPS",
       "99. back",
    }; 

   INT8 *ap_flash_list[] = {
       "1. ap_flash on",
       "2. ap_flash off",
       "3. get ap_flash status",
       "99. back",
    };

   INT8 *power_list[] = {
       "1. power on",
       "2. power off",
       "3. get power status",
       "99. back",
    };

   INT8 *get_nmea_data_list[] = {
       "1. start get NMEA data by port",
       "2. stop get NMEA data by port",
       "3. start get NMEA data by URC",
       "4. stop get NMEA data by URC",
       "99. back",
    };

    INT8 *start_mode_list[] = {
       "1. hot start",
       "2. warm start",
       "3. cold start",
       "99. back",
    };

    INT8 *baud_rate_list[] = {
       "1. set 9600",
       "2. set 115200",
       "3. set 230400",
       "4. get baud rate",
       "99. back",
    };

   INT8 *mode_list[] = {
#ifdef FEATURE_SIMCOM_GPS_OVERSEAS
      "1. set GPS+SBAS+QZSS mode",
      "2. set BDS mode",
      "3. set GPS+GLONASS+GALILEO+SBAS+QZSS mode",
      "\r\n"
      "4. set GPS+BDS+GALILEO+SBAS+QZSS mode",
      "5. get mode",
#else
      "1. set GPS+BDS+QZSS mode",
      "2. set BDS mode",
      "3. set GPS+QZSS mode",
      "4. get mode",
#endif
      "99. back",
   };

    INT8 *nmea_rate_list[] = {
        "1. set 1Hz",
        "2. set 2Hz",
        "3. set 5Hz",
        "4. set 10Hz",
        "5. get nmea rate",
        "99. back",
    };

    INT8 *nmea_sentence_list[] = {
        "1. input mask(0-255)",
        "2. get nmea sentence",
        "99. back",
    };

    
    while(1)
       {
           PrintfResp(note);
reSelect:
           PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
           sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
           if(SRV_UART != optionMsg.msg_id)
           {
               sAPI_Debug("%s,msg_id is error!!",__func__);
               break;
           }

           opt = atoi(optionMsg.arg3);
           sAPI_Free(optionMsg.arg3);

           switch(opt)
           {
                case SC_GNSS_DEMO_AP_FLASH_STATUS:
                {
                   if(SRV_UART != optionMsg.msg_id)
                   {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                   }
reApFlashStatus:
                   PrintfOptionMenu(ap_flash_list,sizeof(ap_flash_list)/sizeof(ap_flash_list[0]));
                   sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                   if(SRV_UART != optionMsg.msg_id)
                   {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                   }
                   opt = atoi(optionMsg.arg3);
                   sAPI_Free(optionMsg.arg3);
                   switch (opt)
                   {
                        case SC_GNSS_DEMO_AP_FLASH_ON:
                        {
                            ret=sAPI_GnssApFlashSet(SC_GNSS_AP_FLASH_ON);
                            PrintfResp("\r\nset ap_flash on!\r\n");
                            if (SC_GNSS_RETURN_CODE_ERROR == ret)
                            {
                                sAPI_Debug("%s: AP_FLASH turn on error!",__func__);
                            }
                            break;
                        }
                        case SC_GNSS_DEMO_AP_FLASH_OFF:
                        {
                            ret=sAPI_GnssApFlashSet(SC_GNSS_AP_FLASH_OFF);
                            PrintfResp("\r\nset ap_flash off!\r\n");
                            if (SC_GNSS_RETURN_CODE_ERROR == ret)
                            {
                                sAPI_Debug("%s: AP_FLASH turn off error!",__func__);
                            }
                            break;
                        }
                        case SC_GNSS_DEMO_AP_FLASH_STATUS_GET:
                        {
                            apFlashStatus = sAPI_GnssApFlashGet();
                            switch(apFlashStatus)
                            {
                                case SC_GNSS_AP_FLASH_ON:
                                    PrintfResp("\r\nap_flash on!!\r\n");
                                    break;
                                case SC_GNSS_AP_FLASH_OFF:
                                    PrintfResp("\r\nap_flash off!!\r\n");
                                    break;
                                default:
                                    PrintfResp("\r\nError!\r\n");
                                    break;
                            }
                        }
                        case SC_GNSS_DEMO_POWER_BACK:
                        {
                            PrintfResp("\r\nReturn to the previous menu!\r\n");
                            goto reSelect;
                        }
                        default:
                            PrintfResp("\r\ninput error!\r\n");
                            break;
                   }
                   goto reApFlashStatus;
                   break;
                }

                case SC_GNSS_DEMO_POWER_STATUS:
                {
                   if(SRV_UART != optionMsg.msg_id)
                   {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                   }
rePowerStatus:
                   PrintfOptionMenu(power_list,sizeof(power_list)/sizeof(power_list[0]));
                   sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                   if(SRV_UART != optionMsg.msg_id)
                   {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                   }
                   opt = atoi(optionMsg.arg3);
                   sAPI_Free(optionMsg.arg3);
                   switch (opt)
                   {
                        case SC_GNSS_DEMO_POWER_ON:
                        {
                            ret=sAPI_GnssPowerStatusSet(SC_GNSS_POWER_ON);
                            PrintfResp("\r\nset power on!\r\n");
                            if (SC_GNSS_RETURN_CODE_ERROR == ret)
                            {
                                sAPI_Debug("%s: power on error!",__func__);
                            }
                            break;
                        }
                        case SC_GNSS_DEMO_POWER_OFF:
                        {
                            ret=sAPI_GnssPowerStatusSet(SC_GNSS_POWER_OFF);
                            PrintfResp("\r\nset power off!\r\n");
                            if (SC_GNSS_RETURN_CODE_ERROR == ret)
                            {
                                sAPI_Debug("%s: power off error!",__func__);
                            }
                            break;
                        }
                        case SC_GNSS_DEMO_POWER_STATUS_GET:
                        {
                            pwrStatus = sAPI_GnssPowerStatusGet();
                            switch(pwrStatus)
                            {
                                case SC_GNSS_POWER_ON:
                                    PrintfResp("\r\npower on!!\r\n");
                                    break;
                                case SC_GNSS_POWER_OFF:
                                    PrintfResp("\r\npower off!!\r\n");
                                    break;
                                default:
                                    PrintfResp("\r\nError!\r\n");
                                    break;
                            }
                            break;
                        }
                        case SC_GNSS_DEMO_POWER_BACK:
                        {
                            PrintfResp("\r\nReturn to the previous menu!\r\n");
                            goto reSelect;
                        }
                        default:
                            PrintfResp("\r\ninput error!\r\n");
                            break;
                   }
                   goto rePowerStatus;
                   break;
                }

                case SC_GNSS_DEMO_OUTPUT_NMEA_PORT:
                {
                   if(SRV_UART != optionMsg.msg_id)
                   {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                   }
reOutputNmeaPort:
                   PrintfOptionMenu(get_nmea_data_list,sizeof(get_nmea_data_list)/sizeof(get_nmea_data_list[0]));
                   sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                   if(SRV_UART != optionMsg.msg_id)
                   {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                   }
                   opt = atoi(optionMsg.arg3);
                   sAPI_Free(optionMsg.arg3);

                   if (powerOn())
                   {
                       switch (opt)
                       {
                          case SC_GNSS_DEMO_START_GET_NMEA_DATA_BY_PORT:
                          {
                              ret=sAPI_GnssNmeaDataGet(SC_GNSS_START_OUTPUT_NMEA_DATA,SC_GNSS_NMEA_DATA_GET_BY_PORT);
                              PrintfResp("\r\nstart get NMEA data by NMEA port!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: output to nema port error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_STOP_GET_NMEA_DATA_BY_PORT:
                          {
                              ret=sAPI_GnssNmeaDataGet(SC_GNSS_STOP_OUTPUT_NMEA_DATA,SC_GNSS_NMEA_DATA_GET_BY_PORT);
                              PrintfResp("\r\nstop get NMEA data by nema port!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: output to nema port error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_START_GET_NMEA_DATA_BY_URC:
                          {
                              ret=sAPI_GnssNmeaDataGet(SC_GNSS_START_OUTPUT_NMEA_DATA,SC_GNSS_NMEA_DATA_GET_BY_URC);
                              PrintfResp("\r\nstart get NMEA data by URC!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: URC report error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_STOP_GET_NMEA_DATA_BY_URC:
                          {
                              ret=sAPI_GnssNmeaDataGet(SC_GNSS_STOP_OUTPUT_NMEA_DATA,SC_GNSS_NMEA_DATA_GET_BY_URC);
                              PrintfResp("\r\nstop get NMEA data by URC!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: URC report error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_GET_NMEA_DATA_BACK:
                          {
                              PrintfResp("\r\nReturn to the previous menu!\r\n");
                              goto reSelect;
                          }
                          default:
                              PrintfResp("\r\ninput error!\r\n");
                              break;
                       }
                       goto reOutputNmeaPort;
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                        goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_START_MODE:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
reStartMode:
                    PrintfOptionMenu(start_mode_list,sizeof(start_mode_list)/sizeof(start_mode_list[0]));
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    opt = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if (powerOn())
                    {
                        switch (opt)
                        {
                            case SC_GNSS_DEMO_START_HOT:
                            {
                                ret=sAPI_GnssStartMode(SC_GNSS_START_HOT);
                                PrintfResp("\r\nhot start!\r\n");
                                if (SC_GNSS_RETURN_CODE_ERROR == ret)
                                {
                                    sAPI_Debug("%s: hot start error!",__func__);
                                }
                                break;
                            }
                            case SC_GNSS_DEMO_START_WARM:
                            {
                                ret=sAPI_GnssStartMode(SC_GNSS_START_WARM);
                                PrintfResp("\r\nwarm start!\r\n");
                                if (SC_GNSS_RETURN_CODE_ERROR == ret)
                                {
                                    sAPI_Debug("%s: warm start error!",__func__);
                                }
                                break;
                            }
                            case SC_GNSS_DEMO_START_COLD:
                            {
                                ret=sAPI_GnssStartMode(SC_GNSS_START_COLD);
                                PrintfResp("\r\ncold start!\r\n");
                                if (SC_GNSS_RETURN_CODE_ERROR == ret)
                                {
                                    sAPI_Debug("%s: cold start error!",__func__);
                                }
                                break;
                            }
                            case SC_GNSS_DEMO_START_BACK:
                            {
                              PrintfResp("\r\nReturn to the previous menu!\r\n");
                              goto reSelect;
                            }
                            default:
                              PrintfResp("\r\ninput error!\r\n");
                              break;
                        }
                        goto reStartMode;
                    }
                    else
                    {
                      PrintfResp("\r\nplease set power on!\r\n");
                      goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_BAUD_RATE:
                {
                     if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
reBaudRate:
                    PrintfOptionMenu(baud_rate_list,sizeof(baud_rate_list)/sizeof(baud_rate_list[0]));
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    opt = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if (powerOn())
                    {
                        switch (opt)
                        {
                            case SC_GNSS_DEMO_BAUD_RATE_9600:
                            {
                              ret=sAPI_GnssBaudRateSet(SC_GNSS_BAUD_RATE_9600);
                              PrintfResp("\r\nset baud rate is 9600!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: baud rate set fail!",__func__);
                              }
                              break;
                            }
                            case SC_GNSS_DEMO_BAUD_RATE_115200:
                            {
                              ret=sAPI_GnssBaudRateSet(SC_GNSS_BAUD_RATE_115200);
                              PrintfResp("\r\nset baud rate is 115200!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: baud rate set fail!",__func__);
                              }
                              break;
                            }
                            case SC_GNSS_DEMO_BAUD_RATE_230400:
                            {
                              ret=sAPI_GnssBaudRateSet(SC_GNSS_BAUD_RATE_230400);
                              PrintfResp("\r\nset baud rate is 230400!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: baud rate set fail!",__func__);
                              }
                              break;
                            }
                            case SC_GNSS_DEMO_BAUD_RATE_GET:
                            {
                              baudRate=sAPI_GnssBaudRateGet();
                              switch(baudRate)
                              {
                                  case SC_GNSS_BAUD_RATE_9600:
                                      PrintfResp("\r\nbaud rate is 9600!\r\n");
                                      break;
                                  case SC_GNSS_BAUD_RATE_115200:
                                      PrintfResp("\r\nbaud rate is 115200!\r\n");
                                      break;
                                  case SC_GNSS_BAUD_RATE_230400:
                                      PrintfResp("\r\nbaud rate is 230400!\r\n");
                                      break;
                                  default:                                
                                      PrintfResp("\r\ncannot get baud rate!\r\n");
                                      break;
                              }
                              break;
                            }
                            case SC_GNSS_DEMO_BAUD_RATE_BACK:
                            {
                              PrintfResp("\r\nReturn to the previous menu!\r\n");
                              goto reSelect;
                            }
                            default:
                              PrintfResp("\r\ninput error!\r\n");
                              break;
                        }
                        goto reBaudRate;
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                        goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_MODE:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
reMode:
                    PrintfOptionMenu(mode_list,sizeof(mode_list)/sizeof(mode_list[0]));
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    opt = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if (powerOn())
                    {
                        switch (opt)
                        {
#ifdef FEATURE_SIMCOM_GPS_OVERSEAS
                          case SC_GNSS_DEMO_MODE_GPS_L1_SBAS_QZSS:
                          {
                              ret=sAPI_GnssModeSet(SC_GNSS_MODE_GPS_L1_SBAS_QZSS);
                              PrintfResp("\r\nset GPS+SBAS+QZSS mode!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set mode error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_MODE_BDS:
                          {
                              ret=sAPI_GnssModeSet(SC_GNSS_MODE_BDS);
                              PrintfResp("\r\nset BDS mode!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set mode error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_MODE_GPS_GLONASS_GALILEO_SBAS_QZSS:
                          {
                              ret=sAPI_GnssModeSet(SC_GNSS_MODE_GPS_GLONASS_GALILEO_SBAS_QZSS);
                              PrintfResp("\r\nset GPS+GLONASS+GALILEO+SBAS+QZSS mode!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set mode error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_MODE_GPS_BDS_GALILEO_SBAS_QZSS:
                          {
                              ret=sAPI_GnssModeSet(SC_GNSS_MODE_GPS_BDS_GALILEO_SBAS_QZSS);
                              PrintfResp("\r\nset GPS+BDS+GALILEO+SBAS+QZSS mode!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set mode error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_MODE_GET:
                          {
                              gnssMode=sAPI_GnssModeGet();
                              switch (gnssMode)
                              {
                                  case SC_GNSS_MODE_GPS_L1_SBAS_QZSS:
                                      PrintfResp("\r\n GPS+SBAS+QZSS mode!\r\n");
                                      break;
                                  case SC_GNSS_MODE_BDS:
                                      PrintfResp("\r\nBDS mode!\r\n");
                                      break;
                                  case SC_GNSS_DEMO_MODE_GPS_GLONASS_GALILEO_SBAS_QZSS:
                                      PrintfResp("\r\nGPS+GLONASS+GALILEO+SBAS+QZSS mode!\r\n");
                                      break;
                                  case SC_GNSS_MODE_GPS_BDS_GALILEO_SBAS_QZSS:
                                      PrintfResp("\r\nGPS+BDS+GALILEO+SBAS+QZSS mode!\r\n");
                                      break;
                                  default:
                                      PrintfResp("\r\ncannot get mode!\r\n");
                                      break;
                              }
                              break;
                          }
#else
                          case SC_GNSS_DEMO_MODE_GPS_BDS_QZSS:
                          {
                              ret=sAPI_GnssModeSet(SC_GNSS_MODE_GPS_BDS_QZSS);
                              PrintfResp("\r\nset GPS+BDS+QZSS mode!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set mode error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_MODE_BDS:
                          {
                              ret=sAPI_GnssModeSet(SC_GNSS_MODE_BDS);
                              PrintfResp("\r\nset BDS mode!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set mode error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_MODE_GPS_QZSS:
                          {
                              ret=sAPI_GnssModeSet(SC_GNSS_MODE_GPS_QZSS);
                              PrintfResp("\r\nset GPS+QZSS mode!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set mode error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_MODE_GET:
                          {
                              gnssMode=sAPI_GnssModeGet();
                              switch (gnssMode)
                              {
                                  case SC_GNSS_MODE_GPS_BDS_QZSS:
                                      PrintfResp("\r\nGPS+BDS+QZSS mode!\r\n");
                                      break;
                                  case SC_GNSS_MODE_BDS:
                                      PrintfResp("\r\nBDS mode!\r\n");
                                      break;
                                  case SC_GNSS_MODE_GPS_QZSS:
                                      PrintfResp("\r\nGPS+QZSS mode!\r\n");
                                      break;
                                  default:
                                      PrintfResp("\r\ncannot get mode!\r\n");
                                      break;
                              }
                              break;
                          }
#endif
                          case SC_GNSS_DEMO_MODE_BACK:
                          {
                              PrintfResp("\r\nReturn to the previous menu!\r\n");
                              goto reSelect;
                          }
                          default:
                              PrintfResp("\r\ninput error!\r\n");
                              break;
                        }
                        goto reMode;
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                        goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_NMEA_RATE:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
reNmeaRate:
                    PrintfOptionMenu(nmea_rate_list,sizeof(nmea_rate_list)/sizeof(nmea_rate_list[0]));
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    opt = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if (powerOn())
                    {
                        switch (opt)
                        {
                          case SC_GNSS_DEMO_NMEA_RATE_1Hz:
                          {
                              ret=sAPI_GnssNmeaRateSet(SC_GNSS_NMEA_UPDATE_RATE_1HZ);
                              PrintfResp("\r\nset nmea rate to 1Hz!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set nmea rate error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_NMEA_RATE_2Hz:
                          {
                              ret=sAPI_GnssNmeaRateSet(SC_GNSS_NMEA_UPDATE_RATE_2HZ);
                              PrintfResp("\r\nset nmea rate to 2Hz!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set nmea rate error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_NMEA_RATE_5Hz:
                          {
                              ret=sAPI_GnssNmeaRateSet(SC_GNSS_NMEA_UPDATE_RATE_5HZ);
                              PrintfResp("\r\nset nmea rate to 5Hz!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set nmea rate error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_NMEA_RATE_10Hz:
                          {
                              ret=sAPI_GnssNmeaRateSet(SC_GNSS_NMEA_UPDATE_RATE_10HZ);
                              PrintfResp("\r\nset nmea rate to 10Hz!\r\n");
                              if (SC_GNSS_RETURN_CODE_ERROR == ret)
                              {
                                  sAPI_Debug("%s: set nmea rate error!",__func__);
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_NMEA_RATE_GET:
                          {
                              nmeaRate=sAPI_GnssNmeaRateGet();
                              switch(nmeaRate)
                              {
                                  case SC_GNSS_NMEA_UPDATE_RATE_1HZ:
                                      PrintfResp("\r\nnmea rate is 1Hz!\r\n");
                                      break;
                                  case SC_GNSS_NMEA_UPDATE_RATE_2HZ:
                                      PrintfResp("\r\nnmea rate is 2Hz!\r\n");
                                      break;
                                  case SC_GNSS_NMEA_UPDATE_RATE_5HZ:
                                      PrintfResp("\r\nnmea rate is 5Hz!\r\n");
                                      break;
                                  case SC_GNSS_NMEA_UPDATE_RATE_10HZ:
                                      PrintfResp("\r\nnmea rate is 10Hz!\r\n");
                                      break;
                                  default:
                                      PrintfResp("\r\ncannot get nmea rate!\r\n");
                                      break;
                              }
                              break;
                          }
                          case SC_GNSS_DEMO_NMEA_RATE_BACK:
                          {
                              PrintfResp("\r\nReturn to the previous menu!\r\n");
                              goto reSelect;
                          }
                          default:
                              PrintfResp("\r\ninput error!\r\n");
                              break;
                        }
                        goto reNmeaRate;
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                        goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_NMEA_SENTENCE:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
reNmeaSentence:
                    PrintfOptionMenu(nmea_sentence_list,sizeof(nmea_sentence_list)/sizeof(nmea_sentence_list[0]));
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    opt = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(powerOn())
                    {
                        switch (opt)
                        {
                          case SC_GNSS_DEMO_NMEA_SENTENCE_INPUT:
                          {
                               PrintfResp("\r\nbit0-GGA,default is 1\r\n");
                               PrintfResp("bit1-GLL,default is 1\r\n");
                               PrintfResp("bit2-GSA,default is 1\r\n");
                               PrintfResp("bit3-GSV,default is 1\r\n");
                               PrintfResp("bit4-RMC,default is 1\r\n");
                               PrintfResp("bit5-VTG,default is 1\r\n");
                               PrintfResp("bit6-ZDA,default is 0\r\n");
                               PrintfResp("bit7-GST,default is 0\r\n");
                               PrintfResp("default mask is 63(00111111)\r\n");
                               PrintfResp("please input mask to enable\\disable nmea sentence: \r\n");

                               sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                                if(SRV_UART != optionMsg.msg_id)
                                {
                                   sAPI_Debug("%s,msg_id is error!!",__func__);
                                   break;
                                }
                                opt = atoi(optionMsg.arg3);
                                sAPI_Free(optionMsg.arg3);
                                ret=sAPI_GnssNmeaSentenceSet(opt);
                                if (SC_GNSS_RETURN_CODE_ERROR == ret)
                                {
                                  sAPI_Debug("%s: set nmea sentence error!",__func__);
                                }
                                break;
                          }
                          case SC_GNSS_DEMO_NMEA_SENTENCE_GET:
                          {
                              PrintfResp("\r\nbit0-GGA,   bit1-GLL,   bit2-GSA,  bit3-GSV,  \r\n");
                              PrintfResp("bit4-RMC,   bit5-VTG,   bit6-ZDA,   bit7-GST\r\n\r\n");
                              memset(outputBuf,0,256);
                              arr=sAPI_GnssNmeaSentenceGet();
                              for (i = 0; i < 8; i++)
                              {
                                  memset(arrBuf,0,32);
                                  snprintf(arrBuf,32,"bit[%d] is %d\r\n",i,arr[i]);
                                  strcat((char *)outputBuf,arrBuf);
                              }
                              PrintfResp(outputBuf);
                              break;
                          }
                          case SC_GNSS_DEMO_NMEA_SENTENCE_BACK:
                          {
                              PrintfResp("\r\nReturn to the previous menu!\r\n");
                              goto reSelect;
                          }
                          default:
                              PrintfResp("\r\ninput error!\r\n");
                              break;
                        }
                        goto reNmeaSentence;
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                        goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_GPS_INFO:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    PrintfResp("\r\nplease input the reporting cycle of GPS information, the range is 0-255, 0 means stop reporting!\r\n");
reGPSINFO:
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    opt = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(powerOn())
                    {
                        ret=sAPI_GpsInfoGet(opt);
                        if (SC_GNSS_RETURN_CODE_ERROR == ret)
                        {
                          sAPI_Debug("%s: set the reporting cycle of GPS information error!",__func__);
                        }
                        if (opt == 0)
                        {
                            goto reSelect;
                        }
                        goto reGPSINFO;
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                    }
                    goto reSelect;
                }

                case SC_GNSS_DEMO_GNSS_INFO:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    PrintfResp("\r\nplease input the reporting cycle of GNSS information, the range is 0-255, 0 means stop reporting!\r\n");
reGNSSINFO:
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    opt = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(powerOn())
                    {
                        ret=sAPI_GnssInfoGet(opt);
                        if (SC_GNSS_RETURN_CODE_ERROR == ret)
                        {
                          sAPI_Debug("%s: set the reporting cycle of GNSS information error!",__func__);
                        }
                        if (opt == 0)
                        {
                            goto reSelect;
                        }
                        goto reGNSSINFO;
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                    }
                    goto reSelect;
                }

                 case SC_GNSS_DEMO_SEND_CMD:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
reSendCmd:
                    PrintfResp("\r\nplease input the string of the NMEA format directly, like this: $CFGMSG,0,1,0\r\n");
                    PrintfResp("\r\nreturn to upper menu after inputting '99'\r\n");
                    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
                    if(SRV_UART != optionMsg.msg_id)
                    {
                       sAPI_Debug("%s,msg_id is error!!",__func__);
                       break;
                    }
                    memset(nmeaString, 0, NMEA_STRING_LEN);
                    strcpy(nmeaString, optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    if(powerOn())
                    {
                        if (!memcmp(nmeaString, "99",2))
                        {
                            goto reSelect;
                        }
                        else
                        {
                            ret=sAPI_SendCmd2Gnss(nmeaString);
                            if (SC_GNSS_RETURN_CODE_ERROR == ret)
                            {
                              sAPI_Debug("%s: send command to GNSS error!",__func__);
                            }
                            goto reSendCmd;
                        }
                    }
                    else
                    {
                        PrintfResp("\r\nplease set power on!\r\n");
                        goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_AGPS:
                {
                    if(SRV_UART != optionMsg.msg_id)
                    {
                        sAPI_Debug("%s,msg_id is error!!",__func__);
                        break;
                    }
                    if(powerOn())
                    {
                        simRet = sAPI_SimcardPinGet(&cpin);

                        if(simRet == SC_SIM_RETURN_SUCCESS)
                        {
                            sAPI_Debug("[AGPS] simcard state:%d!(0:READY 1:PIN 2:PUK 3:BLK 4:REMV 5:CRASH 6:NOINSRT 7:UNKN)",cpin);
                            if(cpin == 0)
                            {
                                sAPI_Debug("[AGPS] simcard is ready!");
                                while(1)   //get network status
                                {
                                    sAPI_NetworkGetCgreg(&pGreg);
                                    if(1 != pGreg)
                                    {
                                        sAPI_Debug("[AGPS] NETWORK STATUS IS [%d]",pGreg);
                                        sAPI_TaskSleep(200*3);
                                    }
                                    else
                                    {
                                        sAPI_Debug("[AGPS] NETWORK STATUS IS NORMAL");
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                sAPI_Debug("[AGPS] simcard is not ready!!");
                                break;
                            }
                        }
                        else
                        {
                            sAPI_Debug("[AGPS] Get Pin State Falied!");
                            break;
                        }

                         ret = (SC_Gnss_DEMO_Return_Code)sAPI_GnssAgpsSeviceOpen();
                         switch (ret)
                         {
                             case SC_GNSS_DEMO_RETURN_CODE_OK:
                                 PrintfResp("\r\nAGPS OK!\r\n");
                                 break;
                             case SC_GNSS_DEMO_CODE_SOCKRT_ERROR:
                                 PrintfResp("\r\n101: open socket unsuccessfully!\r\n");
                                 break;
                             case SC_GNSS_DEMO_CODE_GET_SERVER_ERROR:
                                 PrintfResp("\r\n102: get the AGNSS server unsuccessfully!\r\n");
                                 break;
                             case SC_GNSS_DEMO_CODE_CONNECT_ERROR:
                                 PrintfResp("\r\n103: connect to AGNSS server unsuccessfully!\r\n");
                                 break;
                             case SC_GNSS_DEMO_CODE_WRITE_SOCKET_ERROR:
                                 PrintfResp("\r\n104: write information to socket unsuccessfully!\r\n");
                                 break;
                             case SC_GNSS_DEMO_CODE_READ_SOCKET_ERROR:
                                 PrintfResp("\r\n105: read AGPS data from socket unsuccessfully!\r\n");
                                 break;
                             default:
                                PrintfResp("\r\nAGPS error!\r\n");
                                break;

                         }
                         goto reSelect;
                    }
                    else
                    {
                         PrintfResp("\r\nplease set power on!\r\n");
                         goto reSelect;
                    }
                }

                case SC_GNSS_DEMO_BACK:
                {
                    sAPI_Debug("Return to the previous menu!");
                    PrintfResp("\r\nReturn to the previous menu!\r\n");
                    return;
                }

                default:
                    PrintfResp("\r\nPlease select again:\r\n");
                    goto reSelect;
                    break;
           }

    }

}
#endif
