/**
  ******************************************************************************
  * @file    demo_simcard.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of SIM card demo operation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "simcom_simcard.h"
#include "simcom_os.h"
#include "simcom_debug.h"
#include "simcom_api.h"
#include "simcom_uart.h"


sMsgQRef g_simcard_demo_msgQ;
extern sMsgQRef simcomUI_msgq;

#define SIMCARD_URC_RECIVE_TIME_OUT 3000

typedef enum{
    SC_SIMCARD_DEMO_SWITCHCARD            = 1,
    SC_SIMCARD_DEMO_HOTPLUG               = 2,
    SC_SIMCARD_DEMO_SETPIN                = 3,
    SC_SIMCARD_DEMO_GETPIN                = 4,
    SC_SIMCARD_DEMO_GETICCID              = 5,
    SC_SIMCARD_DEMO_GETIMSI               = 6,
    SC_SIMCARD_DEMO_GETHPLMN              = 7,
    SC_SIMCARD_DEMO_SETCLCK               = 8,
    SC_SIMCARD_DEMO_GETNUM                = 9,
    SC_SIMCARD_DEMO_MAX                   = 99
}SC_SIMCARD_DEMO_TYPE;

/**
  * @brief  SIM card switch card 
  * @param  void
  * @note   
  * @retval void
  */
SC_simcard_err_e simcardSwitchdemo()
{
  sAPI_Debug("[SIMSwitchDemo] ENTER %s...\r\n",__func__);
  SC_simcard_err_e ret = SC_SIM_RETURN_SUCCESS;
  INT8 rsp_buff[100] = {0};
  SIM_MSG_T optionMsg ={0,0,0,NULL};
  SIM_MSG_T msg;
  __attribute__((__unused__)) SC_STATUS status;
  UINT8 simcardSwitch;

  memset(&msg,0,sizeof(msg));
  INT8 *note = "\r\nPlease select:\r\n 0:SIM1\r\n 1:SIM2\r\n";
  PrintfResp(note);
  sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
  if(SRV_UART != optionMsg.msg_id)
  {
      sAPI_Debug("msg_id is error!!\r\n");
  }
  sAPI_Debug("[SIMSwitchDemo] arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
  sAPI_UartWriteString(SC_UART, optionMsg.arg3);
  simcardSwitch = atoi(optionMsg.arg3);
  sAPI_Free(optionMsg.arg3);
  #if 0 /* change to intra msgQ@20211111*/
  ret = sAPI_SimcardSwitchMsg(simcardSwitch, g_simcard_demo_msgQ);
  if(ret == SC_SIM_RETURN_SUCCESS)
  {
    status = sAPI_MsgQRecv(g_simcard_demo_msgQ,&msg,SIMCARD_URC_RECIVE_TIME_OUT);
    sprintf(rsp_buff,"\r\sAPI_SimcardSwitchMsg:\r\n\tresultCode:%d     (0:success  1:fail)\r\n",msg.arg2);
    sAPI_UartWriteString(SC_UART, (UINT8*)rsp_buff);
  }
  #else
  ret = sAPI_SimcardSwitchMsg(simcardSwitch, NULL);
  sprintf(rsp_buff,"\r\sAPI_SimcardSwitchMsg:\r\n\tresultCode:%d     (0:success  1:fail)\r\n",ret);
  sAPI_UartWriteString(SC_UART, (UINT8*)rsp_buff);
  #endif
  if(msg.arg3 != NULL)
  {
    sAPI_Free(msg.arg3);
  }
  return ret;
}

/**
  * @brief  SIM card hot swap
  * @param  void
  * @note   Please select 8 pin SIM card holder and connect SIM_DET to card holder related pin.
  * @retval void
  */
SC_simcard_err_e SimcardHotSwapDemo()
{
  sAPI_Debug("[HotSwapDemo] ENTER %s...\r\n",__func__);
  SC_simcard_err_e ret = SC_SIM_RETURN_FAIL;
  INT8 rsp_buff[100] = {0};
  SIM_MSG_T optionMsg ={0,0,0,NULL};
  SIM_MSG_T msg;
  __attribute__((__unused__)) SC_STATUS status;
  UINT32 opt = 0;
  int param = 0;


  INT8 *note = "\r\nPlease select an option to test from the items listed below:\r\n";
  INT8 *options_list[] = {
    "1. Query function state",
    "2. Query detection level",
    "3. Set function switch",
    "4. Set detection level",
    "99. Back"
  };

  while(1)
  {
    PrintfResp(note);
    PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
    sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
    if(SRV_UART != optionMsg.msg_id)
    {
        sAPI_Debug("[HotSwapDemo] msg_id is error!!\r\n");
    }
    sAPI_Debug("[HotSwapDemo] OPTION arg2,arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
    sAPI_UartWriteString(SC_UART, optionMsg.arg3);
    opt = atoi(optionMsg.arg3);
    sAPI_Free(optionMsg.arg3);
    ret = SC_SIM_RETURN_FAIL;

    switch(opt)
    {
      case SC_HOTSWAP_QUERY_STATE:
      {
        sAPI_Debug("[HotSwapDemo] Query function state\r\n");
        ret = sAPI_SimcardHotSwapMsg((SC_HotSwapCmdType_e)opt,0,g_simcard_demo_msgQ);
        break;
      }
      case SC_HOTSWAP_QUERY_LEVEL:
      {
        sAPI_Debug("[HotSwapDemo] Query detection level");
        ret = sAPI_SimcardHotSwapMsg((SC_HotSwapCmdType_e)opt,0,g_simcard_demo_msgQ);
        break;
      }
      case SC_HOTSWAP_SET_SWITCH:
      {
        sAPI_Debug("[HotSwapDemo] Set function switch");
        INT8 *func_list[] = {
                "0. Off",
                "1. On",
        };
        PrintfResp(note);
        PrintfOptionMenu(func_list,sizeof(func_list)/sizeof(func_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("[HotSwapDemo] msg_id is error!!\r\n");
        }
        sAPI_Debug("[HotSwapDemo] FUNC arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
        sAPI_UartWriteString(SC_UART, optionMsg.arg3);
        param = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);
        if((param == 0) ||(param ==1))
        {
          ret = sAPI_SimcardHotSwapMsg((SC_HotSwapCmdType_e)opt,param,g_simcard_demo_msgQ);
        }
        else
        {
          sAPI_UartWriteString(SC_UART, "Invalid Parameter");
        }
        break;
      }
      case SC_HOTSWAP_SET_LEVEL:
      {
        sAPI_Debug("[HotSwapDemo] Set detection level");
        INT8 *level_list[] = {
                "0. Low",
                "1. High",
        };
        PrintfResp(note);
        PrintfOptionMenu(level_list,sizeof(level_list)/sizeof(level_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("[HotSwapDemo] msg_id is error!!\r\n");
        }
        sAPI_Debug("[HotSwapDemo] LEVEL arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
        sAPI_UartWriteString(SC_UART, optionMsg.arg3);
        param = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);
        if((param == 0) ||(param == 1))
        {
          ret = sAPI_SimcardHotSwapMsg((SC_HotSwapCmdType_e)opt,param,g_simcard_demo_msgQ);
        }
        else
        {
          sAPI_UartWriteString(SC_UART, "Invalid Parameter");
        }
        break;
      }
      case 99:
      {
        sAPI_Debug("[HotSwapDemo] back");
        return;
      }
      default:
      {
        break;
      }
    }

    if(SC_SIM_RETURN_SUCCESS == ret)
    {
      sAPI_Debug("[HotSwapDemo] ret :SC_SIM_RETURN_SUCCESS\r\n");
      memset(&msg,0,sizeof(msg));
      status = sAPI_MsgQRecv(g_simcard_demo_msgQ,&msg,SIMCARD_URC_RECIVE_TIME_OUT);
      sAPI_Debug("[HotSwapDemo] msg.arg2:%d\r\n",msg.arg2);
      switch(opt)
      {
        case SC_HOTSWAP_QUERY_STATE:
        {
          sprintf(rsp_buff,"\r\tHot Swap State:%d\t(0:off  1:on)\r\n",msg.arg2);
          break;
        }
        case SC_HOTSWAP_QUERY_LEVEL:
        {
          sprintf(rsp_buff,"\r\tHot Swap Detection Level:%d\t(0:low  1:high)\r\n",msg.arg2);
          break;
        }
        case SC_HOTSWAP_SET_SWITCH:
        {
          sprintf(rsp_buff,"\r\tHot Swap Set Function Switch Succeed\r\n");
          break;
        }
        case SC_HOTSWAP_SET_LEVEL:
        {
          sprintf(rsp_buff,"\r\tHot Swap Set Detection Level Succeed\r\n");
          break;
        }
        default:
        {
          break;
        }
      }
      sAPI_UartWriteString(SC_UART, (UINT8*)rsp_buff);
    }
  }

  return ret;
}

/**
  * @brief  Set CPIN
  * @param  void
  * @note   
  * @retval void
  */
SC_simcard_err_e SimcardSetPinDemo()
{
  SC_simcard_err_e ret = SC_SIM_RETURN_SUCCESS;
  INT8 rsp_buff[100] = {0};
  SIM_MSG_T optionMsg ={0,0,0,NULL};
  SIM_MSG_T msg;
  __attribute__((__unused__)) SC_STATUS status;
  UINT8 opt;
  UINT8 oldPasswd[16] = {0};
  UINT8 newPasswd[16] = {0};
  UINT8 oldPasswdTemp[16] = {0};

  INT8 *note = "\r\nPlease select:\r\n 0:No Need New PIN\r\n 1:Need New PIN\r\n";
  PrintfResp(note);
  sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
  if(SRV_UART != optionMsg.msg_id)
  {
    sAPI_Debug("msg_id is error!!\r\n");
  }
  sAPI_Debug("--SimcardDemo--, arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
  sAPI_UartWriteString(SC_UART, optionMsg.arg3);
  opt = atoi(optionMsg.arg3);
  sAPI_Free(optionMsg.arg3);

  switch(opt)
  {
    case 0:
    {
      sAPI_Debug("[PinsetDemo] No Need New PIN");
      INT8 *scan_pin ="\r\nPlease enter the PIN:\r\n";
      PrintfResp(scan_pin);
      sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
      if(SRV_UART != optionMsg.msg_id)
      {
        sAPI_Debug("[PinsetDemo] msg_id is error!!\r\n");
      }
      sAPI_Debug("[PinsetDemo] pin arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
      if(optionMsg.arg2 >= 4)
      {
        memcpy(oldPasswdTemp,optionMsg.arg3,optionMsg.arg2);
      }
      else
      {
        sAPI_Debug("[PinsetDemo] old pin length too short!!\r\n");
        sAPI_UartWriteString(SC_UART, "\r\n PIN too short\r\n");
        ret = SC_SIM_RETURN_FAIL;
        break;
      }
      if((oldPasswdTemp[optionMsg.arg2-1] == '\n') && (oldPasswdTemp[optionMsg.arg2-2] == '\r'))    /*delet CR LF*/
      {
        sAPI_Debug("[PinsetDemo] delet CR LF\r\n");
        memcpy(oldPasswd,oldPasswdTemp,optionMsg.arg2-2);
      }
      else
        memcpy(oldPasswd,oldPasswdTemp,optionMsg.arg2);

      sAPI_Debug("[PinsetDemo] oldPasswd:%s,len:%d\r\n",oldPasswd,strlen(oldPasswd));

      sAPI_UartWriteString(SC_UART, oldPasswd);
      sAPI_Free(optionMsg.arg3);
      ret = sAPI_SimcardPinSet(oldPasswd,newPasswd,opt);
      if(ret != SC_SIM_RETURN_SUCCESS)
      {
        sAPI_UartWriteString(SC_UART, "\r\n PIN Set Faild!\r\n");
      }
      else
      {
        sAPI_UartWriteString(SC_UART, "\r\n PIN Set Succeed!\r\n");
      }
      break;
    }
    case 1:
    {
      sAPI_Debug("[PinsetDemo] Need New PIN");
      INT8 *scan_oldpin ="\r\nPlease enter the old PIN:\r\n";
      PrintfResp(scan_oldpin);
      sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
      if(SRV_UART != optionMsg.msg_id)
      {
        sAPI_Debug("[PinsetDemo] msg_id is error!!\r\n");
      }
      sAPI_Debug("[PinsetDemo] oldpin arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
      memcpy(oldPasswd,optionMsg.arg3,strlen(optionMsg.arg3)-2);/*cut out CR LF*/
      sAPI_UartWriteString(SC_UART, oldPasswd);
      sAPI_Free(optionMsg.arg3);

      INT8 *scan_newpin ="\r\nPlease enter the new PIN:\r\n";
      PrintfResp(scan_newpin);
      sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
      if(SRV_UART != optionMsg.msg_id)
      {
        sAPI_Debug("[PinsetDemo] msg_id is error!!\r\n");
      }
      sAPI_Debug("[PinsetDemo] newpin arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
      memcpy(newPasswd,optionMsg.arg3,strlen(optionMsg.arg3)-2);/*cut out CR LF*/
      sAPI_UartWriteString(SC_UART, newPasswd);
      sAPI_Free(optionMsg.arg3);

      ret = sAPI_SimcardPinSet(oldPasswd,newPasswd,opt);
      if(ret != SC_SIM_RETURN_SUCCESS)
      {
        sAPI_UartWriteString(SC_UART, "\r\n PIN Set Faild!\r\n");
      }
      else
      {
        sAPI_UartWriteString(SC_UART, "\r\n PIN Set Succeed!\r\n");
      }
      break;
    }
    default:
    break;
  }
  return ret;
}

/**
  * @brief  Get PIN state
  * @param  void
  * @note   
  * @retval void
  */
SC_simcard_err_e SimcardGetPinDemo()
{
  SC_simcard_err_e ret = SC_SIM_RTEURN_UNKNOW;
  INT8 rsp_buff[100] = {0};
  int resp_buff_len = 120;
  UINT8 cpin = 0;

  ret = sAPI_SimcardPinGet(&cpin);
  if(ret == SC_SIM_RETURN_SUCCESS)
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet pin state:%d\t(0:READY 1:PIN 2:PUK 3:BLK 4:REMV 5:CRASH 6:NOINSRT 7:UNKN)\r\n",cpin);
  }
  else
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet Pin State Falied!\r\n");
  }
  PrintfResp(rsp_buff);
  return ret;
}

/**
  * @brief  Get IMSI
  * @param  void
  * @note   
  * @retval void
  */
SC_simcard_err_e SimcardGetImsiDemo()
{
  SC_simcard_err_e ret = SC_SIM_RTEURN_UNKNOW;
  INT8 rsp_buff[100] = {0};
  int resp_buff_len = 120;
  char imsi[32] = {0};

  ret = sAPI_SysGetImsi(&imsi[0]);
  if(ret == SC_SIM_RETURN_SUCCESS)
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet IMSI:%s\r\n",&imsi[0]);
  }
  else
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet IMSI Faild!!\r\n");
  }
  PrintfResp(rsp_buff);
  return ret;
}

/**
  * @brief  Get ICCID
  * @param  void
  * @note   
  * @retval void
  */
SC_simcard_err_e SimcardGetIccidDemo()
{
  SC_simcard_err_e ret = SC_SIM_RTEURN_UNKNOW;
  INT8 rsp_buff[100] = {0};
  int resp_buff_len = 120;
  char iccid[32] = {0};

  ret = sAPI_SysGetIccid(&iccid[0]);
  if(ret == SC_SIM_RETURN_SUCCESS)
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet ICCID:%s\r\n",&iccid[0]);
  }
  else
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet ICCID Faild!!\r\n");
  }
  PrintfResp(rsp_buff);
  return ret;
}

/**
  * @brief  Get HPLMN
  * @param  void
  * @note   
  * @retval void
  */
SC_simcard_err_e SimcardGetHplmnDemo()
{
  SC_simcard_err_e ret = SC_SIM_RTEURN_UNKNOW;
  INT8 rsp_buff[100] = {0};
  int resp_buff_len = 120;
  Hplmn_st hplmn ={0};

  ret = sAPI_SysGetHplmn(&hplmn);
  if(ret == SC_SIM_RETURN_SUCCESS)
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nSPN:%s\r\nHPLMN:%s-%s",hplmn.spn,hplmn.mcc,hplmn.mnc);
  }
  else
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet HPLMN Faild!!\r\n");
  }
  PrintfResp(rsp_buff);
  return ret;
}

SC_simcard_err_e SimcardSetClckDemo()
{
  SC_simcard_err_e ret = SC_SIM_RETURN_SUCCESS;
  INT8 rsp_buff[100] = {0};
  SIM_MSG_T optionMsg ={0,0,0,NULL};
  UINT8 Passwd[16] = {0};
  UINT8 PasswdTemp[16] = {0};
  SIM_MSG_T msg;
  UINT8 opt;
  __attribute__((__unused__)) SC_STATUS status;
  INT8 *note = "\r\nPlease select:\r\n 1:set simcard lock\r\n 2:set simcard unlock\r\n";
  PrintfResp(note);
  sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
  
  if(SRV_UART != optionMsg.msg_id)
  {
    sAPI_Debug("msg_id is error!!\r\n");
  }
  sAPI_Debug("--SimcardDemo--, arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
  sAPI_UartWriteString(SC_UART, optionMsg.arg3);
  opt = atoi(optionMsg.arg3);
  sAPI_Free(optionMsg.arg3);

    switch(opt)
    {
      case 1:
      {
        sAPI_Debug("[SimcardSetClckDemo] Set lock");
        INT8 *scan_pin ="\r\nPlease enter the PIN:\r\n";
        PrintfResp(scan_pin);
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
          sAPI_Debug("[SimcardSetClckDemo] msg_id is error!!\r\n");
        }
        sAPI_Debug("[SimcardSetClckDemo] pin arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
        if(optionMsg.arg2 >= 4)
        {
          memcpy(PasswdTemp,optionMsg.arg3,optionMsg.arg2);
          sAPI_Debug("PasswdTemp=%s",PasswdTemp);
        }
        else
        {
           sAPI_Debug("[SimcardSetClckDemo] old pin length too short!!\r\n");
           sAPI_UartWriteString(SC_UART, "\r\n PIN too short\r\n");
           ret = SC_SIM_RETURN_FAIL;
           break;
        }

        if((PasswdTemp[optionMsg.arg2-1] == '\n') && (PasswdTemp[optionMsg.arg2-2] == '\r'))    /*delet CR LF*/
        {
          sAPI_Debug("[SimcardSetClckDemo] delet CR LF\r\n");
          memcpy(Passwd,PasswdTemp,optionMsg.arg2-2);
        }
        else
          memcpy(Passwd,PasswdTemp,optionMsg.arg2);
        
        sAPI_Debug("[SimcardSetClckDemo] Passwd:%s,len:%d\r\n",Passwd,strlen(Passwd));
        sAPI_UartWriteString(SC_UART, Passwd);
        sAPI_Free(optionMsg.arg3);
        
        ret = sAPI_SimLockSet(1,Passwd);
        if(ret == SC_SIM_RETURN_SUCCESS)
        {
          sAPI_UartWriteString(SC_UART, "\r\n\set  simcard lock sucess \r\n");
        }
        else
        {
          sAPI_UartWriteString(SC_UART, "\r\n\tSet failed\r\n");
        }
        return ret;
      }
      case 2:
      {
        sAPI_Debug("[SimcardSetClckDemo] Set lock");
        INT8 *scan_pin ="\r\nPlease enter the PIN:\r\n";
        PrintfResp(scan_pin);
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
          sAPI_Debug("[SimcardSetClckDemo] msg_id is error!!\r\n");
        }
        sAPI_Debug("[SimcardSetClckDemo] pin arg3 = [%d, %s]",optionMsg.arg2, optionMsg.arg3);
        if(optionMsg.arg2 >= 4)
        {
          memcpy(PasswdTemp,optionMsg.arg3,optionMsg.arg2);
        }
        else
        {
           sAPI_Debug("[SimcardSetClckDemo] old pin length too short!!\r\n");
           sAPI_UartWriteString(SC_UART, "\r\n PIN too short\r\n");
           ret = SC_SIM_RETURN_FAIL;
           break;
        }

        if((PasswdTemp[optionMsg.arg2-1] == '\n') && (PasswdTemp[optionMsg.arg2-2] == '\r'))    /*delet CR LF*/
        {
          sAPI_Debug("[SimcardSetClckDemo] delet CR LF\r\n");
          memcpy(Passwd,PasswdTemp,optionMsg.arg2-2);
        }
        else
        memcpy(Passwd,PasswdTemp,optionMsg.arg2);
        
        sAPI_Debug("[SimcardSetClckDemo] Passwd:%s,len:%d\r\n",Passwd,strlen(Passwd));
        sAPI_UartWriteString(SC_UART, Passwd);
        sAPI_Free(optionMsg.arg3);
        ret = sAPI_SimLockSet(0,Passwd);
        if(ret == SC_SIM_RETURN_SUCCESS)
        {
          sAPI_UartWriteString(SC_UART, "\r\n\set  simcard unlock sucess \r\n");
        }
        else
        {
          sAPI_UartWriteString(SC_UART, "\r\n\tSet failed\r\n");
        }
        return ret;
      }
    }
}


SC_simcard_err_e SimcardGetNumDemo()
{
  SC_simcard_err_e ret = SC_SIM_RTEURN_UNKNOW;
  INT8 rsp_buff[100] = {0};
  int resp_buff_len = 120;
  char num[50] = {0};

  ret = sAPI_SysGetSubscriber(&num[0]);
  if(ret == SC_SIM_RETURN_SUCCESS)
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet Num:%s\r\n",&num[0]);
  }
  else
  {
    snprintf(rsp_buff,resp_buff_len,"\r\nGet Num Faild!!\r\n");
  }
  PrintfResp(rsp_buff);
  return ret;
}



/**
  * @brief  SIM card operation demo
  * @param  void
  * @note   
  * @retval void
  */

void SimcardDemo(void)
{
    SC_simcard_err_e ret;
    int resp_buff_len = 120;
    __attribute__((__unused__)) SC_STATUS status;
    SIM_MSG_T msg;
    SIM_MSG_T optionMsg ={0,0,0,NULL};
    INT8 rsp_buff[resp_buff_len];
    UINT32 opt = 0;

    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    INT8 *options_list[] = {
        "1. SwitchCard",
        "2. HotPlug",
        "3. PinSet",
        "4. PinGet",
        "5. GetICCID",
        "6. GetIMSI",
        "7. GetHPLMN",
        "8. SimLockSet",
        "9. GetNum",
        "99. Back"
    };

    status = sAPI_MsgQCreate(&g_simcard_demo_msgQ, "g_simcard_demo_msgQ", sizeof(SIM_MSG_T), 4, SC_FIFO);
    if(status != SC_SUCCESS)
    {
      sAPI_Debug("ERROR: message queue creat err!\n");
    }

    while(1)
    {
        memset(rsp_buff, 0, resp_buff_len);
        PrintfResp(note);
        PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("%s,msg_id is error!!",__func__);
            break;
        }

        sAPI_Debug("--SIMCARDDemo--, arg3 = [%s]",optionMsg.arg3);
        opt = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);

        switch(opt)
        {
            case SC_SIMCARD_DEMO_SWITCHCARD:
            {
                ret = simcardSwitchdemo();
                break;
            }
            case SC_SIMCARD_DEMO_HOTPLUG:
            {
                ret = SimcardHotSwapDemo();
                break;
            }
            case SC_SIMCARD_DEMO_SETPIN:
            {
                ret = SimcardSetPinDemo();
                break;
            }
            case SC_SIMCARD_DEMO_GETPIN:
            {
                ret = SimcardGetPinDemo();
                break;
            }
            case SC_SIMCARD_DEMO_GETICCID:
            {
                ret = SimcardGetIccidDemo();
                break;
            }
            case SC_SIMCARD_DEMO_GETIMSI:
            {
                ret = SimcardGetImsiDemo();
                break;
            }
            case SC_SIMCARD_DEMO_GETHPLMN:
            {
                ret = SimcardGetHplmnDemo();
                break;
            }
            case SC_SIMCARD_DEMO_SETCLCK:
            {
                ret = SimcardSetClckDemo();
                break;
            }
            case SC_SIMCARD_DEMO_GETNUM:
            {
                ret = SimcardGetNumDemo();
                break;
            }
            case SC_SIMCARD_DEMO_MAX:
            {
                sAPI_Debug("[SIMCARD] Return to the previous menu!");
                PrintfResp("\r\nReturn to the previous menu!\r\n");
                sAPI_MsgQDelete(g_simcard_demo_msgQ);
                return;
            }
            default :
                break;

        }

    }

}

