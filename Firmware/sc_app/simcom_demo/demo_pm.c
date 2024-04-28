/**
  ******************************************************************************
  * @file    demo_pm.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of power system management.
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
#include "simcom_gpio.h"
#include "simcom_pm.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

extern sMsgQRef simcomUI_msgq;
extern SIM_MSG_T GetParamFromUart(void);

/**
  * @brief  Power management demo 
  * @param  void
  * @note   
  * @retval void
  */
void PMUDemo(void)
{
    SC_STATUS status = SC_SUCCESS;
    SIM_MSG_T optionMsg ={0,0,0,NULL};
    int opt,ret;
    t_rtc timeval;
    char buff[100] = {0};

    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] = {
        "1. detect power key status",
        "2. onoff power key shutdown",
        "3. power up&down event",
        "4. VBAT&ADC0 voltage",
        "5. set VDD_AUX",
        "6. poweroff",
        "7. reset",
        "99. back",
    };

    while(1)
    {
        PrintfResp(note);
        PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("%s,msg_id is error!!",__func__);
            break;
        }

        sAPI_Debug("arg3 = [%s]",optionMsg.arg3);
        opt = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);

        switch(opt)
        {
			case 1:
			{
				int onoff = -1;

				onoff = sAPI_GetPowerKeyStatus();
				sprintf(buff,"\r\n PowerKeyStaus  = %d \r\n", onoff);
                PrintfResp(buff);
			}
				break;

			case 2:
			{
				ONKEY_STATUS onoff = -1;
				int ret;

				PrintfResp("\r\n select : \r\n");
				PrintfResp("\r\n  0: disable \r\n");
				PrintfResp("\r\n  1: open \r\n");
				PrintfResp("\r\nany: just return current status \r\n");

				optionMsg = GetParamFromUart();
				onoff = atoi(optionMsg.arg3);
				sAPI_Free(optionMsg.arg3);

				ret = sAPI_SetPowerKeyOffFunc(onoff, 50);   //50 x 100ms: about 5S
				sprintf(buff,"\r\n PowerKeyOffFunc %d \r\n", ret);
                PrintfResp(buff);
			}
				break;

            case 3:
            {
                sprintf(buff,"powerup = %d\r\n",sAPI_GetPowerUpEvent());
                PrintfResp(buff);

                sprintf(buff,"powerdown = %d\r\n",sAPI_GetPowerDownEvent());
                PrintfResp(buff);

                break;
            }

            case 4:
            {
                sprintf(buff,"vbat = %d\r\n",sAPI_ReadVbat());
                PrintfResp(buff);
                sprintf(buff,"ADC0 = %d\r\n",sAPI_ReadAdc(0));
                PrintfResp(buff);

                break;
            }

            case 5:
            {
                unsigned int voltage = 0;
                PrintfResp("\r\nPlease input voltage.\r\n");
                optionMsg = GetParamFromUart();
                voltage = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                if(voltage > 3000 || voltage < 2500)
                {
                    PrintfResp("\r\nPlease input correct voltage.\r\n");
                    break;
                }

                if(sAPI_SetVddAux(voltage) == 0)
                {
                    sprintf(buff,"set VDD_AUX %dmv success!\r\n",voltage);
                    PrintfResp(buff);
                }

                break;
            }

            case 6:
                sAPI_SysPowerOff();
                break;

            case 7:
                sAPI_SysReset();
                break;


			case 99:
				return;

			default:
				break;
		}
    }
}



