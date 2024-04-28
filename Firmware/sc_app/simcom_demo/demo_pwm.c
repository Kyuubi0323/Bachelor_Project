/**
  ******************************************************************************
  * @file    demo_pwm.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of pwm operation.
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

//PWM_ReturnCode sAPI_PWMConfig(SC_PWM_DEVICE_NUM pwm_num,
                                //    PWM_SWITCH on_off,
                                //    unsigned char frq_div,
                                //    unsigned short high_level_cnt,
                                //    unsigned short total_cnt);


/*---------------------------theory calculation formula-----------------------------*/
/*                          period = frq_div*total_cnt/32000       (s)              */
/*                          frequency = 32000/(frq_div*total_cnt)  (Hz)             */
/*                          duty = high_level_cnt/total_cnt                         */
/*                                                                                  */
/*                                                                                  */


/**
  * @brief  PWM operation demo
  * @param  void
  * @note   
  * @retval void
  */
void demo_pwm(void)
{
    sAPI_Debug("%s, enter",__func__);

    /**************** pwm configuration method 1 ****************/
    /*In this way, you can choose to configure the clock source.*/
    /************************************************************/
    /************************************************************/
    SC_PWM_CONFIG pwmDev;

    /* set pwm channel */
    pwmDev.pwm_channel = SC_PWM_DEVICE_1;
    /*set pwm switch*/
    pwmDev.pwm_switch = PWM_ON;
    /*set pwm clock*/
    pwmDev.pwm_clock = PWM_CLK_SRC_13M;
    /*set pwm clock frequency divisor*/
    pwmDev.frq_div = 64;
    /*set pwm high_level_cnt*/
    pwmDev.high_level_cnt = 7;
    /*set pwm total_cnt*/
    pwmDev.total_cnt = 14;

    if(PWM_RC_OK != sAPI_PWMConfigEx(&pwmDev)) //PWM1: 16.1 KHz in test and 14.5 KHz in theory value
    {
        sAPI_Debug("%s,PWM config init fail!",__func__);
    }
    else
    {
        sAPI_Debug("%s,PWM config init successful!",__func__);
    }
    /**************** pwm configuration method 2 ****************/
    /*In this way, you can only set the clock source to 32K as the default*/
    /************************************************************/
    /************************************************************/
    //sAPI_PWMConfig(SC_PWM_DEVICE_1,PWM_ON,2,5,10); //PWM1: 1.49 KHz in test and 1.6 KHz in theory value
    sAPI_PWMConfig(SC_PWM_DEVICE_2,PWM_ON,64,7,14); //PWM2: 34.15 Hz in test and 35.71 Hz in theory value
    return;
}

