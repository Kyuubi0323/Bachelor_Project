/**
  ******************************************************************************
  * @file    simcom_api.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of all APIs from SIMCom OpenSDK peripherals components.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_API_H_
#define _SIMCOM_API_H_

#include "simcom_os.h"
#include "simcom_debug.h"
#include "simcom_network.h"
#include "simcom_simcard.h"
#include "simcom_sms.h"
#include "simcom_uart.h"
#include "simcom_gpio.h"
#include "simcom_pm.h"
#include "simcom_i2c.h"
#include "simcom_spi.h"
#include "simcom_htp_client.h"
#include "simcom_ntp_client.h"
#include "simcom_tcpip.h"
#include "simcom_ftps.h"
#include "simcom_mqtts_client.h"
#include "simcom_https.h"
#include "simcom_ssl.h"
#include "simcom_system.h"
#include "simcom_common.h"
#include "simcom_file_system.h"
#include "simcom_audio.h"
#include "simcom_loc.h"
#include "simcom_app_download.h"
#include "simcom_ping.h"
#include "simcom_wifi.h"
#include "simcom_call.h"
#include "simcom_gps.h"
#include "stdio.h"
#include "stdarg.h"
#include "simcom_pwm.h"
#include "simcom_app_updater.h"
#include "simcom_lcd.h"
#include "sockets.h"
#include "simcom_tts_api.h"
#include "simcom_ble.h"
#include "simcom_rtc.h"
#include "simcom_pm.h"
#include "simcom_usb_vcom.h"
#include "simcom_cam.h"
#include "simcom_dspgain.h"

#define SIMCOM_UI_DEMO

/*For running UI demo, please choose UART1 or USB AT port for cmd CLI interface, only one could be selected*/

//#define SIMCOM_UI_DEMO_TO_USB_AT_PORT
#define SIMCOM_UI_DEMO_TO_UART1_PORT

extern void sAPP_SslTaskDemo(void);
extern void sAPP_SimCardTaskDemo(void);
extern void sAPP_UrcTask(void);
extern void sAPP_UartTask(void);
extern void sAPP_AppUpdaterDemo(void);
extern void sAPP_WatchDogTimer(void);
extern void read_write_critical_chunk_demo(void);
extern void sAPP_VersionDemo(void);
extern void sAPP_SimcomUIDemo(void);
extern void UsbVcomCBFunc(void *para);
extern void UsbVcomCBFuncEx(int len, void *para);
extern void sAPP_KeyTask(void);
extern void sAPP_HelloWorldDemo(void);
extern void sAPP_SpiOperationDemo(void);

#if 0
typedef char *(*_strcpy)(char *dest, const char *src);
typedef UINT32 (*_strlen)(const char *str);
typedef char *(*_strncpy)(char *dest, const char *src,UINT32 n);
typedef char *(*_strcat)(char *dest, const char *src);
typedef char *(*_strncat)(char *dest, const char *src, UINT32 n);
typedef int (*_strcmp)(const char *str1, const char *str2);
typedef int (*_strncmp)(const char *str1, const char *str2, UINT32 n);
typedef int (*_sprintf)(char *str, const char *format, ...);
typedef int (*_snprintf)(char * str, UINT32 size, const char * format, ... );
typedef int (*_sscanf)(const char *str, const char *format, ...);
typedef int (*_atoi)(const char *str);
typedef void *(*_memcpy)(void *str1, const void *str2, UINT32 n);
typedef void *(*_memchr)(const void *str, int c, UINT32 n);
typedef int (*_memcmp)(const void *str1, const void *str2, UINT32 n);
typedef void *(*_memset)(void *str, int c, UINT32 n);
#endif

typedef void (*_sAPI_Debug)(const char *format, ...);
typedef void (*_sAPI_Printf)(char* log);

/*simcom_os*/
typedef SC_STATUS (*_sAPI_TaskCreate)(sTaskRef *taskRef, void* stackPtr, UINT32 stackSize, UINT8 priority, char *taskName, void (*taskStart)(void*), void* argv);
typedef SC_STATUS (*_sAPI_TaskDelete)(sTaskRef taskRef);
typedef SC_STATUS (*_sAPI_TaskSuspend)(sTaskRef taskRef);
typedef SC_STATUS (*_sAPI_TaskResume)(sTaskRef taskRef);
typedef SC_STATUS (*_sAPI_TaskSleep)(UINT32 ticks);
typedef SC_STATUS (*_sAPI_TaskGetCurrentRef)(sTaskRef *taskRef);
typedef SC_STATUS (*_sAPI_TaskTerminate)(sTaskRef taskRef);
typedef SC_STATUS (*_sAPI_MsgQCreate)(sMsgQRef *msgQRef, char *queueName, UINT32 maxSize, UINT32 maxNumber, UINT8 waitingMode);
typedef SC_STATUS (*_sAPI_MsgQDelete)(sMsgQRef msgQRef);
typedef SC_STATUS (*_sAPI_MsgQSend)(sMsgQRef msgQRef, SIM_MSG_T *msgPtr);
typedef SC_STATUS (*_sAPI_MsgQSendSuspend)(sMsgQRef msgQRef, SIM_MSG_T *msgPtr,UINT32 timeout);
typedef SC_STATUS (*_sAPI_MsgQRecv)(sMsgQRef msgQRef,SIM_MSG_T *recvMsg, UINT32 timeout);
typedef SC_STATUS (*_sAPI_MsgQPoll)(sMsgQRef msgQRef,UINT32* msgCount);
typedef SC_STATUS (*_sAPI_SemaphoreCreate)(sSemaRef *semaRef, UINT32 initialCount, UINT8 waitingMode);
typedef SC_STATUS (*_sAPI_SemaphoreDelete)(sSemaRef semaRef);
typedef SC_STATUS (*_sAPI_SemaphoreAcquire)(sSemaRef semaRef, UINT32 timeout);
typedef SC_STATUS (*_sAPI_SemaphoreRelease)(sSemaRef semaRef);
typedef SC_STATUS (*_sAPI_SemaphorePoll)(sSemaRef semaRef, UINT32 *count);
typedef SC_STATUS (*_sAPI_MutexCreate)(sMutexRef *mutexRef,UINT8 waitingMode);
typedef SC_STATUS (*_sAPI_MutexDelete)(sMutexRef mutexRef);
typedef SC_STATUS (*_sAPI_MutexLock)(sMutexRef mutexRef,UINT32 timeout);
typedef SC_STATUS (*_sAPI_MutexUnLock)(sMutexRef mutexRef);
typedef SC_STATUS (*_sAPI_FlagCreate)(sFlagRef *flagRef);
typedef SC_STATUS (*_sAPI_FlagDelete)(sFlagRef flagRef);
typedef SC_STATUS (*_sAPI_FlagSet)(sFlagRef flagRef, UINT32 mask, UINT32 operation);
typedef SC_STATUS (*_sAPI_FlagWait)(sFlagRef flagRef, UINT32 mask, UINT32 operation, UINT32* flags, UINT32 timeout);
typedef SC_STATUS (*_sAPI_TimerCreate)(sTimerRef *timerRef);
typedef SC_STATUS (*_sAPI_TimerStart)(sTimerRef timerRef, UINT32 initialTime, UINT32 rescheduleTime, void (*callBackRoutine)(UINT32), UINT32 timerArgc);
typedef SC_STATUS (*_sAPI_TimerStop)(sTimerRef timerRef);
typedef SC_STATUS (*_sAPI_TimerDelete)(sTimerRef timerRef);
typedef SC_STATUS (*_sAPI_TimerGetStatus)(sTimerRef timerRef,sTimerStatus* timerStatus);
typedef UINT32 (*_sAPI_GetTicks)(void);
typedef INT32 (*_sAPI_Gettimeofday)(sTimeval *tv,void* dummy);
typedef INT32 (*_sAPI_GettimeofdaySyncRtc)(sTimeval *tv,void* dummy);
typedef unsigned long (*_sAPI_Time)(unsigned long *t);
typedef void (*_sAPI_DelayUs)(unsigned long t);
typedef SC_STATUS (*_sAPI_ContextLock)(void);
typedef SC_STATUS (*_sAPI_ContextUnlock)(void);
typedef void* (*_sAPI_Realloc)(void *ptr, size_t size);
typedef void *(*_sAPI_Calloc)(size_t nmemb, size_t size);
typedef unsigned long (*_sAPI_DisableInterrupts)(void);
typedef unsigned long (*_sAPI_EnableInterrupts)(void);
typedef void (*_sAPI_RestoreInterrupts)(unsigned long ir);
typedef SC_STATUS (*_sAPI_GetTaskInfo)(sTaskRef taskRef,sTaskInfo *task_info);

/*simcom_network*/
typedef void (*_sAPI_NetworkInit)(void);
typedef unsigned int (*_sAPI_NetworkGetCsq)(UINT8 *pCsq);
typedef unsigned int (*_sAPI_NetworkGetCreg)(int* pReg);
typedef unsigned int (*_sAPI_NetworkGetCpsi)(SCcpsiParm *pStr);
typedef unsigned int (*_sAPI_NetworkGetCgreg)(int* pGreg);
typedef unsigned int (*_sAPI_NetworkGetCnmp)(int *pCnmp);
typedef unsigned int (*_sAPI_NetworkSetCnmp)(int CnmpValue);
typedef unsigned int (*_sAPI_NetworkGetCops)(char* pCops);
typedef unsigned int (*_sAPI_NetworkSetCops)(int modeVal,int formatVal,char *networkOperator,int accTchVal);
typedef unsigned int (*_sAPI_NetworkGetCgdcont)(SCApnParm* pCgdcont);
typedef unsigned int (*_sAPI_NetworkSetCgdcont)(int primCid,char *type,char *APNstr);
typedef unsigned int (*_sAPI_NetworkGetCGAUTH)(SCCGAUTHParm *pCgauth,int cid);
typedef unsigned int (*_sAPI_NetworkGetCgauth)(SCCGAUTHParm *pCgauth,int cid);
typedef unsigned int (*_sAPI_NetworkSetCGAUTH)(SCCGAUTHParm *pCgauth,BOOL delflag);
typedef unsigned int (*_sAPI_NetworkSetCgauth)(SCCGAUTHParm *pCgauth,BOOL delflag);
typedef unsigned int (*_sAPI_NetworkGetCgact)(SCAPNact* pCgact);
typedef unsigned int (*_sAPI_NetworkSetCgact)(int pdpState,int cid);
typedef unsigned int (*_sAPI_NetworkGetCgatt)(int* pCgatt);
typedef unsigned int (*_sAPI_NetworkSetCgatt)(int CgattValue);
typedef unsigned int (*_sAPI_NetworkSetCfun)(int CfunValue);
typedef unsigned int (*_sAPI_NetworkGetCfun)(UINT8 *pCfun);
typedef unsigned int (*_sAPI_NetworkSetCtzu)(int CtzuValue);
typedef unsigned int (*_sAPI_NetworkGetCgpaddr)(int Cid,SCcgpaddrParm *Pstr);
typedef unsigned int (*_sAPI_NetworkGetCnetci)(SCcnetciParm *pStr);
/*simcom_simcard*/
typedef SC_simcard_err_e (*_sAPI_SimPinGet)(UINT8 *gcpin);
typedef SC_simcard_err_e (*_sAPI_SimcardPinGet)(UINT8 *gcpin);
typedef SC_simcard_err_e (*_sAPI_SysGetIccid)(char *IccidValue);
typedef SC_simcard_err_e (*_sAPI_SimcardSwitchMsg)(UINT8 status, sMsgQRef msgQ);
typedef SC_simcard_err_e (*_sAPI_SimcardHotSwapMsg)(SC_HotSwapCmdType_e opt, UINT8 param, sMsgQRef msgQ);
typedef SC_simcard_err_e (*_sAPI_SimPinSet)(char * oldpin, char * newpin,int newflg);
typedef SC_simcard_err_e (*_sAPI_SimcardPinSet)(char * oldpin, char * newpin,int newflg);
typedef SC_simcard_err_e (*_sAPI_SysGetImsi)(char *ImsiValue);
typedef SC_simcard_err_e (*_sAPI_SysGetimsi)(char *ImsiValue);
typedef SC_simcard_err_e (*_sAPI_SysGetHplmn)(char *HplmnValue);
typedef SC_simcard_err_e (*_sAPI_SimLockSet)(int mode, char *pin);
typedef SC_simcard_err_e (*_sAPI_SysGetSubscriber)(char *SubscriberValue);


/*simcom_sms*/
typedef SC_SMSReturnCode (*_sAPI_SmsWriteMsg)(SCsmsFormatMode fmatmode,UINT8* sms,UINT16 smslen,UINT8* srcAddr,UINT8 stat,sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsSendMsg)(SCsmsFormatMode fmatmode,UINT8* sms,UINT16 smslen,UINT8* addr,sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsSendStrogeMsg)(INT32 msgIndex,UINT8* addr,sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsSendStorageMsg)(INT32 msgIndex,UINT8* addr,sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsSendLongMsg)(UINT8* sms,UINT16 smslen,UINT8* addr,sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsReadMsg)(SCsmsFormatMode fmatmode,INT32 msgIndex,sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsDelAllMsg)(sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsDelOneMsg)(INT32 msgIndex,sMsgQRef msgQ);
typedef SC_SMSReturnCode (*_sAPI_SmsSetNewMsgInd)(INT32 fmatmode, INT32 mode, INT32 mt, INT32 bm, INT32 ds, INT32 bfr);

/*simcom_uart*/
typedef int (*_sAPI_UartWrite)(SC_Uart_Port_Number port,UINT8 *data, UINT32 length);
typedef int (*_sAPI_UartRead)(SC_Uart_Port_Number port, unsigned char *data, int len);
typedef int (*_sAPI_UartWriteString)(SC_Uart_Port_Number port,UINT8 *data);
typedef SC_Uart_Return_Code (*_sAPI_UartSetConfig)(SC_Uart_Port_Number port, const SCuartConfiguration *config);
typedef SC_Uart_Return_Code (*_sAPI_UartGetConfig)(SC_Uart_Port_Number port, SCuartConfiguration *config);
typedef SC_Uart_Return_Code (*_sAPI_UartPrintf)(const char *fmt, ...);
typedef SC_Uart_Return_Code (*_sAPI_SendATCMDWaitResp)(int sATPInd,char *in_str, int timeout, char *ok_fmt, int ok_flag,char *err_fmt, char *out_str, int resplen);
typedef SC_Uart_Rx_Status (*_sAPI_UartRxStatus)(SC_Uart_Port_Number port, int timeout);
typedef SC_Uart_Return_Code (*_sAPI_UartControl)(SC_Uart_Port_Number port, SC_Uart_Control ctl);
typedef int (*_sAPI_UartRefRegister)(SC_Uart_Port_Number port, sMsgQRef ref);
typedef SC_Uart_Return_Code (*_sAPI_UartRegisterCallback)(SC_Uart_Port_Number port, SC_Uart_Callback cb);
typedef SC_Uart_Return_Code (*_sAPI_UartRegisterCallbackEX)(SC_Uart_Port_Number port, SC_Uart_CallbackEX cb, void *reserve);
typedef SC_GPIOReturnCode (*_sAPI_UartRs485DePinAssign)(SC_Uart_Port_Number port, SC_Module_GPIONumbers GpinNum);
typedef SC_GPIOReturnCode (*_sAPI_UartRs485DePinAssignEx)(SC_Uart_Port_Number port, SC_Module_GPIONumbers GpinNum, SC_UartRs485DeActiveLevel DeActiveLevel);


/*simcom_GNSS*/
typedef SC_Gnss_Return_Code (*_sAPI_GnssPowerStatusSet)(SC_Gnss_Power_Status power);
typedef SC_Gnss_Power_Status (*_sAPI_GnssPowerStatusGet)(void);
typedef SC_Gnss_Return_Code (*_sAPI_GnssApFlashSet)(SC_Gnss_Ap_Flash_Status ctl);
typedef SC_Gnss_Ap_Flash_Status (*_sAPI_GnssApFlashGet)(void);
typedef SC_Gnss_Return_Code (*_sAPI_GnssNmeaDataGet)(SC_Gnss_Output_Control ctl, SC_Gnss_Nmea_Data_Get mode);
typedef SC_Gnss_Return_Code (*_sAPI_GnssStartMode)(SC_Gnss_Start_Mode mode);
typedef SC_Gnss_Return_Code (*_sAPI_GnssBaudRateSet)(SC_Gnss_Baud_Rate baudrate);
typedef SC_Gnss_Baud_Rate (*_sAPI_GnssBaudRateGet)(void);
typedef SC_Gnss_Return_Code (*_sAPI_GnssModeSet)(SC_Gnss_Mode mode);
typedef SC_Gnss_Mode (*_sAPI_GnssModeGet)(void);
typedef SC_Gnss_Return_Code (*_sAPI_GnssNmeaRateSet)(SC_Gnss_Nmea_Rate rate);
typedef SC_Gnss_Nmea_Rate (*_sAPI_GnssNmeaRateGet)(void);
typedef SC_Gnss_Return_Code (*_sAPI_GnssNmeaSentenceSet)(unsigned short mask);
typedef UINT8* (*_sAPI_GnssNmeaSentenceGet)(void);
typedef SC_Gnss_Return_Code (*_sAPI_GPSInfoGet)(UINT8 period);
typedef SC_Gnss_Return_Code (*_sAPI_GNSSInfoGet)(UINT8 period);
typedef SC_Gnss_Return_Code (*_sAPI_SendCmd2GNSS)(char *string);
typedef SC_Gnss_Return_Code (*_sAPI_AGPS)(void);
typedef SC_Gnss_Return_Code (*_sAPI_GpsInfoGet)(UINT8 period);
typedef SC_Gnss_Return_Code (*_sAPI_GnssInfoGet)(UINT8 period);
typedef SC_Gnss_Return_Code (*_sAPI_SendCmd2Gnss)(char *string);
typedef SC_Gnss_Return_Code (*_sAPI_GnssAgpsSeviceOpen)(void);

/*API: Sleep/Wake up*/
typedef int (*_sAPI_SystemSleepSet)(SC_SYSTEM_SLEEP_FLAG flag);
typedef SC_SYSTEM_SLEEP_FLAG (*_sAPI_SystemSleepGet)(void);
typedef int (*_sAPI_SystemAlarmClock2Wakeup)(unsigned long time);
typedef int (*_sAPI_SystemSleepExSet)(SC_SYSTEM_SLEEP_FLAG flag, unsigned char time);
typedef SC_SleepEx_str (*_sAPI_SystemSleepExGet)(void);


/*simcom_gpio*/
typedef SC_GPIOReturnCode (*_sAPI_GpioSetValue)(unsigned int gpio,unsigned int value);
typedef SC_GPIOReturnCode (*_sAPI_GpioGetValue)(unsigned int gpio);
typedef SC_GPIOReturnCode (*_sAPI_GpioConfig)(unsigned int gpio, SC_GPIOConfiguration GpioConfig);
typedef SC_GPIOReturnCode (*_sAPI_GpioConfigInterrupt)(unsigned int gpio,SC_GPIOTransitionType type,GPIOCallback handler);
typedef SC_GPIOReturnCode (*_sAPI_GpioWakeupEnable)(unsigned int gpio, SC_GPIOTransitionType type);
typedef SC_GPIOReturnCode (*_sAPI_setPinFunction)(unsigned int gpio, unsigned int mf);
typedef SC_GPIOReturnCode (*_sAPI_setGpioDirection)(unsigned int gpio, unsigned int direction);//old type
typedef SC_GPIOReturnCode (*_sAPI_GpioSetDirection)(unsigned int gpio, unsigned int direction);
typedef SC_GPIOReturnCode (*_sAPI_GpioGetDirection)(unsigned int gpio);
typedef SC_GPIOReturnCode (*_sAPI_SimDetectPinStatus)(void);



/*simcom_pmu*/
typedef unsigned int (*_sAPI_ReadAdc)(int channel);
typedef unsigned int (*_sAPI_ReadVbat)(void);
typedef void (*_sAPI_SysPowerOff)(void);
typedef void (*_sAPI_SysReset)(void);
typedef int (*_sAPI_SetVddAux)(unsigned int voltage);
typedef POWER_UP_REASON (*_sAPI_GetPowerUpEvent)(void);
typedef POWER_DOWN_REASON (*_sAPI_GetPowerDownEvent)(void);
typedef ONKEY_STATUS (*_sAPI_GetPowerKeyStatus)(void);
typedef UINT8 (*_sAPI_SetPowerKeyOffFunc)(UINT8 onoff, UINT32 powerKeyOffTime); //powerKeyOffTime: unit about 100ms
typedef void (*_sAPI_PowerKeyRegisterCallback)(PowerKeyIntCallback cb);


/*simcom_spi*/
typedef void (*_sAPI_ExtNorFlashInit)(void);
typedef SC_SPI_ReturnCode (*_sAPI_ExtNorFlashShowSupportList)(void *outBuff,int outBuffLen);
typedef SC_SPI_ReturnCode (*_sAPI_ExtNorFlashBlock64kErase)(int offset, int size);
typedef SC_SPI_ReturnCode (*_sAPI_ExtNorFlashSectorErase)(int offset, int size);
typedef SC_SPI_ReturnCode (*_sAPI_ExtNorFlashRead)(unsigned int FlashOffset, int Size, unsigned int Buffer);
typedef SC_SPI_ReturnCode (*_sAPI_ExtNorFlashWrite)(unsigned int Address,int Size, unsigned int Buffer);
typedef SC_SPI_ReturnCode (*_sAPI_ExtNorFlashReadID)(unsigned char *id);
typedef void (*_sAPI_ExtNorFlashCs)(char value);

typedef SC_SPI_ReturnCode (*_sAPI_SPIReadBytes)(unsigned int *SendData,unsigned int *RevData,int Size);
typedef SC_SPI_ReturnCode (*_sAPI_ExtSpiReadBytes)(unsigned char *sendBuff,unsigned int sendSize, unsigned char *revBuff,unsigned int revSize);
typedef SC_SPI_ReturnCode (*_sAPI_SPIWriteBytes)(unsigned int *SendData,int Size);
typedef SC_SPI_ReturnCode (*_sAPI_SPIConfigInit)(int ssp_clk, int ssp_mode);

typedef SC_SPI_ReturnCode (*_sAPI_SpiConfigInitEx)(SC_SPI_DEV *handle);
typedef SC_SPI_ReturnCode (*_sAPI_SpiReadBytesEx)(SC_SPI_DEV *handle,uint8_t *sendBuff,uint32_t sendSize, uint8_t *revBuff,uint32_t revSize);
typedef SC_SPI_ReturnCode (*_sAPI_SpiWriteBytesEx)(SC_SPI_DEV *handle,uint8_t *sendBuff,uint32_t sendSize);

/*simcom_i2c*/
typedef SC_I2C_ReturnCode (*_sAPI_I2CRead)(int i2cChannel,UINT8 slaveAddress,UINT8 regAddress,UINT8 *receiveDataBuffer,UINT16 datasize);
typedef SC_I2C_ReturnCode (*_sAPI_I2CWrite)(int i2cChannel,UINT8 slaveAddress,UINT8 regAddress,UINT8 *data,UINT16 datasize);
typedef SC_I2C_ReturnCode (*_sAPI_I2CReadEx)(int i2cChannel,UINT8 slaveAddress,UINT8 regAddress,UINT8 *receiveDataBuffer,UINT16 datasize);
typedef SC_I2C_ReturnCode (*_sAPI_I2CWriteEx)(int i2cChannel,UINT8 slaveAddress,UINT8 regAddress,UINT8 *data,UINT16 datasize);
typedef SC_I2C_ReturnCode (*_sAPI_I2CConfigInit)(SC_I2C_DEV *handle);


/*simcom_filesystem*/
typedef SCFILE *(*_sAPI_fopen)(const char *fname, const char *mode);
typedef int (*_sAPI_fclose)(SCFILE *fp);
typedef int (*_sAPI_fread)(void *buffer, size_t size, size_t num, SCFILE *fp);
typedef int (*_sAPI_fwrite)(const void *buffer, size_t size, size_t num, SCFILE *fp);
typedef int (*_sAPI_fseek)(SCFILE *fp, long offset, int whence);
typedef long (*_sAPI_ftell)(SCFILE *fp);
typedef void (*_sAPI_frewind)(SCFILE *fp);
typedef int (*_sAPI_fsize)(SCFILE *fp);
typedef int (*_sAPI_fsync)(SCFILE *fp);
typedef int (*_sAPI_mkdir)(const char *path, unsigned int mode);
typedef SCDIR *(*_sAPI_opendir)(const char *path);
typedef int (*_sAPI_closedir)(SCDIR *dirp);
typedef struct dirent *(*_sAPI_readdir)(SCDIR *dirp);
typedef int (*_sAPI_seekdir)(SCDIR *dirp, unsigned long offset);
typedef int (*_sAPI_telldir)(SCDIR *dirp);
typedef int (*_sAPI_remove)(const char *fname);
typedef int (*_sAPI_rename)(const char *oldpath, const char *newpath);
typedef int (*_sAPI_access)(const char *path, int mode);
typedef int (*_sAPI_stat)(const char *path, struct dirent *info);
typedef long long (*_sAPI_GetSize)(char *disc);
typedef long long (*_sAPI_GetFreeSize)(char *disc);
typedef long long (*_sAPI_GetUsedSize)(char *disc);
typedef int (*_sAPI_ftruncate)(SCFILE *fp, size_t length);
typedef long long (*_sAPI_GetFsSize)(char *disc);
typedef long long (*_sAPI_GetFsFreeSize)(char *disc);

#ifdef FEATURE_SIMCOM_FS_OLD
/*Compatible with the old APIs*/
typedef SCfsReturnCode (*_sAPI_FsIsFileNameValid)(const SCfileNameInfo *pName_info);
typedef SCfsReturnCode (*_sAPI_FsDirProcessor)(int operation_num, const SCfileNameInfo *pName_info);
typedef SCfileHandle (*_sAPI_FsFileOpen)(const SCfileNameInfo *pName_info, const char *pMode);
typedef unsigned int (*_sAPI_FsFileRead)(char *buf, unsigned int len, SCfileHandle handle);
typedef SCfsReturnCode (*_sAPI_FsFileSeek)(SCfileHandle handle, long offset, int whereFrom);
typedef unsigned int (*_sAPI_FsFileWrite)(const char *buf, unsigned int len, SCfileHandle handle);
typedef SCfsReturnCode (*_sAPI_FsFileClose)(SCfileHandle handle);
typedef SCfsReturnCode (*_sAPI_FsFileRename)(const     SCfileNameInfo *pFileOld, const SCfileNameInfo *pFileNew);
typedef SCfsReturnCode (*_sAPI_FsFindFileAndGetSize)(const SCfileNameInfo * pName_info, unsigned long * size);
typedef SCfsReturnCode (*_sAPI_FsFileDelete)(const SCfileNameInfo *pName_info);
typedef SCfsReturnCode (*_sAPI_FsGetDiskSize)(const char *pPath_in, INT64 *total_size, INT64 *free_size);
typedef BOOL (*_sAPI_FsIsPathExist)(const char *pPath_in);
typedef unsigned int (*_sAPI_FsFileSave)(BOOL covering, const char *filename, const char *data, unsigned int length);
typedef SCfsReturnCode (*_sAPI_FsFileTraverse)(const char *pPath_in,char *fileList,char *dirlist);
typedef SCfsReturnCode (*_sAPI_FsNameSeparate)(SCfileNameInfo *pName, char *pFull);
typedef SCfsReturnCode (*_sAPI_FsSync)(SCfileHandle handle);
typedef int (*_sAPI_FsFileTell)(SCfileHandle handle);
#endif

/*simcom_ntp&&simcom_htp*/
typedef SChtpReturnCode (*_sAPI_HtpSrvConfig)(SChtpOperationType commad_type, char *return_string, char *cmd, char* host_or_idx, int host_port, int http_version, char *proxy, int proxy_port);
typedef SChtpReturnCode (*_sAPI_HtpUpdate)(sMsgQRef magQ_urc);
typedef SCntpReturnCode (*_sAPI_NtpUpdate)(SCntpOperationType commad_type, char* host_addr, int time_zone, sMsgQRef magQ_urc);
typedef void (*_sAPI_GetSysLocalTime)(SCsysTime_t *currUtcTime);
typedef void (*_sAPI_SetSysLocalTime)(char* timeStr);
typedef void (*_sAPI_disableDUMP)(void);
typedef void (*_sAPI_enableDUMP)(void);
typedef void (*_sAPI_DisableDump)(void);
typedef void (*_sAPI_EnableDump)(void);
typedef char* (*_sAPI_GetFlashType)(void);
typedef unsigned int (*_sAPI_GetCusdataPartitionSize)(void);
typedef unsigned int (*_sAPI_GetAppPartitionSize)(void);


//ntp

/*simcom_tcpip*/

typedef INT32 (*_sAPI_TcpipGetErrno)(void);
typedef INT32 (*_sAPI_TcpipPdpActive)(int cid, int channel);
typedef INT32 (*_sAPI_TcpipPdpDeactive)(int cid, int channel);
typedef INT32 (*_sAPI_TcpipPdpDeactiveNotify)(sMsgQRef msgQ);
typedef INT32 (*_sAPI_TcpipGetSocketPdpAddr)(int cid, int channel, struct SCipInfo *info);
typedef INT32 (*_sAPI_TcpipSocket)(INT32 domain,INT32 type,INT32 protocol);
typedef INT32 (*_sAPI_TcpipBind)(INT32 sockfd, const SCsockAddr *addr,UINT32 addrlen);
typedef INT32 (*_sAPI_TcpipListen)(INT32 sockfd, INT32 backlog);
typedef INT32 (*_sAPI_TcpipAccept)(INT32 sockfd, SCsockAddr *addr, UINT32 *addrlen);
typedef INT32 (*_sAPI_TcpipConnect)(INT32 sockfd, const SCsockAddr *addr,UINT32 addrlen);
typedef INT32 (*_sAPI_TcpipSend)(INT32 sockfd, const void *buf, INT32 len, INT32 flags);
typedef INT32 (*_sAPI_TcpipRecv)(INT32 sockfd, void *buf, INT32 len, INT32 flags);
typedef INT32 (*_sAPI_TcpipSendto)(INT32 sockfd, const void *buf, INT32 len, INT32 flags,const SCsockAddr *dest_addr, UINT32 addrlen);
typedef INT32 (*_sAPI_TcpipRecvfrom)(INT32 sockfd, void *buf, INT32 len, INT32 flags,SCsockAddr *src_addr, UINT32 *addrlen);
typedef INT32 (*_sAPI_TcpipClose)(INT32 fd);
typedef INT32 (*_sAPI_TcpipShutdown)(INT32 sockfd, INT32 how);
typedef INT32 (*_sAPI_TcpipGetsockname)(INT32 sockfd, SCsockAddr *addr, UINT32 *addrlen);
typedef INT32 (*_sAPI_TcpipGetpeername)(INT32 sockfd, SCsockAddr *addr, UINT32 *addrlen);
typedef INT32 (*_sAPI_TcpipGetsockopt)(INT32 sockfd, INT32 level, INT32 optname,void *optval, UINT32 *optlen);
typedef INT32 (*_sAPI_TcpipSetsockopt)(INT32 sockfd, INT32 level, INT32 optname,const void *optval, UINT32 optlen);
typedef INT32 (*_sAPI_TcpipIoctlsocket)(INT32 fd,INT32 level,void*argp);
typedef SChostent * (*_sAPI_TcpipGethostbyname)(const INT8 *name);
typedef INT32 (*_sAPI_TcpipSelect)(INT32 nfds, SCfdSet *readfds, SCfdSet *writefds,SCfdSet *exceptfds, SCtimeval *timeout);
typedef UINT32 (*_sAPI_TcpipInet_addr)(const INT8 *cp);
typedef INT16 (*_sAPI_TcpipHtons)(UINT16 hostshort);
typedef INT16 (*_sAPI_TcpipNtohs)(UINT16 hostshort);
typedef INT8* (*_sAPI_TcpipInet_ntoa)(UINT32 in);
typedef INT8* (*_sAPI_TcpipInet_ntop)(INT32 af, const void *src,INT8 *dst, UINT32 size);
typedef INT32 (*_sAPI_TcpipGetSocketErrno)(int sockfd);
typedef UINT32 (*_sAPI_TcpipHtonl)(UINT32 hostlong);

typedef int (*_sAPI_TcpipGetaddrinfo)(const char *nodename,const char *servname,const struct addrinfo *hints,struct addrinfo **res);
typedef int (*_sAPI_TcpipGetaddrinfo_with_pcid)(const char *nodename,const char *servname,const struct addrinfo *hints,struct addrinfo **res, u8_t pcid);
typedef void (*_sAPI_TcpipFreeaddrinfo)(struct addrinfo *ai);
typedef int (*_sAPI_TcpipSocket_with_callback)(int domain, int type, int protocol, socket_callback callback);
typedef INT32 (*_sAPI_TcpipInet_pton)(INT32 af, const void *src,void *dst);
typedef	enum socket_state (*_sAPI_GetSocketState)(int fd);


/*simcom_http*/
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpInit)(int channel, sMsgQRef magQ_urc);
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpTerm)(void);
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpPara)(char *command_type, char *parameters);
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpAction)(int methods);
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpData)(char *filename,int dir);
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpHead)(void);
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpRead)(int commd_type, int start_offset, int byte_size);
typedef SC_HTTP_RETURNCODE (*_sAPI_HttpPostfile)(char * filename,int dir);

/*simcom_ftp*/
typedef void (*_sAPI_FtpsInit)(SC_PDP_ACTIVE_TYPE type,sMsgQRef msgQRef);
typedef void (*_sAPI_FtpsDeInit)(SC_PDP_ACTIVE_TYPE TYPE);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsLogin)(SCftpsLoginMsg msg);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsLogout)(void);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsDownloadFile)(char* fileName,SC_FTPS_FILE_LOCATION loc);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsDownloadFileToBuffer)(char* fileName,int startPos);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsUploadFile)(char* fileName,SC_FTPS_FILE_LOCATION loc,int startPos);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsDeleteFile)(char* fileName);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsCreateDirectory)(char* dir);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsDeleteDirectory)(char* dir);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsChangeDirectory)(char* dir);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsGetCurrentDirectory)(void);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsList)(char* dir);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsGetFileSize)(char* fileName);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsTransferType)(char* type);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsGetTransferType)(void);
typedef SC_FTPS_RETURNCODE (*_sAPI_FtpsSslConfig)(int session,int sslId);

/*simcom_mqtt*/
typedef SCmqttReturnCode (*_sAPI_MqttStart)(int channel);
typedef SCmqttReturnCode (*_sAPI_MqttStop)(void);
typedef SCmqttReturnCode (*_sAPI_MqttAccq)(SCmqttOperationType commad_type, char *string_return, int client_index, char *clientID, int server_type, sMsgQRef msgQ_urc);
typedef SCmqttReturnCode (*_sAPI_MqttRel)(int client_index);
typedef SCmqttReturnCode (*_sAPI_MqttConnect)(SCmqttOperationType commad_type, char *string_return, int client_index, char *server_addr, int keepalive_time, int clean_session, char *user_name, char *pass_word);
typedef SCmqttReturnCode (*_sAPI_MqttDisConnect)(SCmqttOperationType commad_type,char *string_return, int client_index, int timeout);
typedef SCmqttReturnCode (*_sAPI_MqttWillTopic)(int client_index, char *topic_data, int topic_length);
typedef SCmqttReturnCode (*_sAPI_MqttWillMsg)(int client_index, char *msg_data, int msg_length);
typedef SCmqttReturnCode (*_sAPI_MqttTopic)(int client_index, char *topic_data, int topic_length);
typedef SCmqttReturnCode (*_sAPI_MqttPayload)(int client_index, char *payload_data, int payload_length);
typedef SCmqttReturnCode (*_sAPI_MqttPub)(int client_index, int qos, int pub_timeout, int ratained, int dup);
typedef SCmqttReturnCode (*_sAPI_MqttSubTopic)(int client_index, char *sub_topic_data, int sub_topic_length, int qos);
typedef SCmqttReturnCode (*_sAPI_MqttSub)(int client_index, char *topic_data, int topic_length, int qos, int dup);
typedef SCmqttReturnCode (*_sAPI_MqttUnSubTopic)(int client_index, char *unsub_topic_data, int unsub_topic_length);
typedef SCmqttReturnCode (*_sAPI_MqttUnsub)(int client_index, char *topic_data, int topic_length, int dup);
typedef SCmqttReturnCode (*_sAPI_MqttSslCfg)(SCmqttOperationType commad_type, char *string_return, int client_index, int ssl_ctx_index);
typedef SCmqttReturnCode (*_sAPI_MqttCfg)(SCmqttOperationType commad_type, char *string_return, int client_index, int config_type, int related_value);
typedef void (*_sAPI_MqttConnLostCb)(mqtt_connlost_cb cb);

/*simcom_ssl*/
typedef SCsslContent (*_sAPI_SslGetContextIdMsg)(int sslId);
typedef int (*_sAPI_SslSetContextIdMsg)(char *op,int sslId,char *value);

/*simcom_version*/
typedef void (*_sAPI_SysGetVersion)(SIMComVersion *simcominfo);
typedef void (*_sAPI_SysGetRFVersion)(RFVersion *RFinfo);
typedef void (*_sAPI_SysGetCusVersion)(CUSVersion *CUSinfo);
typedef unsigned int (*_sAPI_SysGetImei)(char *ImeiValue);
typedef void (*_sAPI_SysGetBuildTime)(char *SysBuildTime);


/*simcom_urc*/
typedef int (*_sAPI_UrcRefRegister)(sMsgQRef ref, UINT32 mask);
typedef int (*_sAPI_UrcRefRelease)(sMsgQRef ref);

typedef int (*_sAPI_AppPackageOpen)(char *mode);
typedef int (*_sAPI_AppPackageWrite)(char * data, unsigned int size);
typedef int (*_sAPI_AppPackageRead)(char *data, unsigned int size);
typedef int (*_sAPI_AppPackageClose)(void);
typedef int (*_sAPI_AppPackageCrc)(SCAppPackageInfo *pInfo);

/*simcom_usb_vcom*/
typedef SC_Usb_Vbus_Status (*_sAPI_UsbVbusDetect)(void);
typedef int (*_sAPI_UsbVcomWrite)(unsigned char* data, unsigned long length);
typedef int (*_sAPI_UsbVcomRead)(unsigned char* data, unsigned long length);
typedef int (*_sAPI_UsbVcomRegisterCallback)(SC_Usb_Callback cb);
typedef int (*_sAPI_UsbVcomRegisterCallbackEX)(SC_Usb_CallbackEX cb, void *reserve);

/*simcom_ssl*/


typedef INT32 (*_sAPI_SslHandShake)(SCSslCtx_t *ctx);
typedef INT32 (*_sAPI_SslRead)(UINT32 ClientID,INT8 *buf, INT32 len);
typedef INT32 (*_sAPI_SslSend)(UINT32 ClientID,INT8 *buf, INT32 len);
typedef void (*_sAPI_SslClose)(UINT32 ClientID);

typedef unsigned int (*_sAPI_AppPartitionBinSize)(void);
typedef int (*_sAPI_ReadFlashAppPartition)(unsigned int offset, char *buff, unsigned int size);
typedef int (*_sAPI_ReadFlash)(unsigned int offset, char *buff, unsigned int size);
typedef int (*_sAPI_WriteFlash)(unsigned int offset, char *buff, unsigned int size);
typedef int (*_sAPI_EraseFlashSector)(unsigned int offset, unsigned int size);
typedef int (*_sAPI_ReadFlashNvmPartition)(unsigned int offset, char *buff, unsigned int size);
typedef int (*_sAPI_WriteFlashNvmPartition)(unsigned int offset, char *buff, unsigned int size);
typedef int (*_sAPI_EraseFlashNvmPartition)(unsigned int offset, unsigned int size);

typedef BOOL (*_sAPI_AudioPlaySampleRate)(AUD_SampleRate rate);
typedef BOOL (*_sAPI_AudioPlay)(char *file, BOOL direct,BOOL isSingle);
typedef BOOL (*_sAPI_AudioStop)(void);
typedef BOOL (*_sAPI_AudioRecord)(BOOL enable, AUD_RecordSrcPath path, char *file);
typedef int (*_sAPI_AudRec)(UINT8 oper,char *file,UINT8 duration,sAPI_AudRecCallback callback);
typedef BOOL (*_sAPI_AmrStreamRecord)(sAPI_GetAmrFrameCB callback);
typedef BOOL (*_sAPI_AmrStopStreamRecord)(void);
typedef int (*_sAPI_RegisterXipTaskCb)(UINT8 TaskId, void (*suspend)(void), void (*resume)(void));
typedef void (*_sAPI_CacheCleanMemory)(void *pMem, unsigned int size);
typedef void (*_sAPI_AudioSetAmrRateLevel)(int level);
typedef int (*_sAPI_GetStatisticsData)(SCstatisticsData *pStatisticsData);
typedef BOOL (*_sAPI_AudioPlayWavCont)(char *file, BOOL startplay);



typedef BOOL (*_sAPI_AudioPlayMp3Cont)(char *file, BOOL startplay,BOOL frame);
typedef BOOL (*_sAPI_AudioPcmPlay)(char *buffer, UINT32 len, BOOL direct);
typedef BOOL (*_sAPI_AudioPcmStop)(void);
typedef BOOL (*_sAPI_AudioMp3StreamPlay)(char *buffer, UINT32 len,BOOL direct);
typedef BOOL (*_sAPI_AudioMp3StreamStop)(void);
typedef BOOL (*_sAPI_AudioAmrStreamPlay)(char *buffer, UINT32 len,BOOL direct);
typedef BOOL (*_sAPI_AudioAmrStreamStop)(void);
typedef BOOL (*_sAPI_AudioPlayAmrCont)(char *file, BOOL startplay);
typedef BOOL (*_sAPI_AudioWavFilePlay)(char *file, BOOL direct);
typedef UINT8 (*_sAPI_AudioStatus)(void);
typedef BOOL (*_sAPI_AudioSetPlayPath)(UINT8 path);
typedef UINT8 (*_sAPI_AudioGetPlayPath)(void);
typedef void (*_sAPI_AudioSetVolume)(AUD_Volume volume);
typedef AUD_Volume (*_sAPI_AudioGetVolume)(void);
typedef int (*_sAPI_PocInitLib)(sAPI_NotifyBufStateCb callback);
typedef int (*_sAPI_PocPlaySound)(const char *data, int length);
typedef int (*_sAPI_PocStopSound)(void);
typedef int (*_sAPI_PocGetPcmAvail)(void);
typedef int (*_sAPI_PocCleanBufferData)(void);
typedef int (*_sAPI_PocStartRecord)(sAPI_ProcessRecordCb callback,int mode);
typedef int (*_sAPI_PocStopRecord)(void);
typedef int (*_sAPI_PocPcmRead)(char *buffer ,int dataLen);
typedef void (*_sAPI_AudioSetMicGain)(UINT32 micgain);
typedef AUD_MICGAIN (*_sAPI_AudioGetMicGain)(void);
typedef SC_GPIOReturnCode (*_sAPI_AudioSetPaCtrlConfig)(SC_Module_GPIONumbers GpioNum, UINT8 ActiveLevel);

typedef void (*_sAPI_GetSystemInfo)(unsigned int *cpuUsedRate, unsigned int *heapFreeSize);
typedef unsigned long long (*_sAPI_GetCpuUid)(void);
typedef SC_LBS_RETURNCODE(*_sAPI_LocGet)(int channel, sMsgQRef magQ_urc, int type);

/*simcom_app_download*/
typedef SCAppDwonLoadReturnCode (*_sAPI_AppDownload)(SCAppDownloadPram *pram);
/*simcom_aes*/
typedef INT32 (*_sAPI_AesEncrypt)(const UINT8 * key,UINT32 keybits,const UINT8 input[16], UINT8 output[16]);
typedef INT32 (*_sAPI_AesDecrypt)(const UINT8 * key,UINT32 keybits,const UINT8 input[16], UINT8 output[16]);
/*simcom_base64*/
typedef INT32 (*_sAPI_Base64Encode)(UINT8 * dst, INT32 dlen,INT32 *olen,const UINT8 * src, INT32 slen);
typedef INT32 (*_sAPI_Base64Decode)(UINT8 * dst, INT32 dlen,INT32 *olen,const UINT8 * src, INT32 slen);

/*simcom_rsa*/
typedef int (*_sAPI_RsaEncrypt)(char *output,const char *input, int length,const char *publickey);
typedef int (*_sAPI_RsaDecrypt)(char *output,int output_max_len,const char *input, int *length,const char *privatekey);


typedef BOOL (*_sAPI_TTSPlay)(UINT8 option,char *inputText,UINT8 playMode);
typedef BOOL (*_sAPI_TTSStop)(void);
typedef BOOL (*_sAPI_TTSSetParameters)(UINT8 volume,UINT8 sysVolume,UINT8 digitMode,UINT8 pitch,UINT8 speed);
typedef void (*_sAPI_TTSSetStatusCallBack)(sAPI_TTSStatussCb callback);

/*simcom_at*/
typedef BOOL (*_sAPI_AtSend)(void *data,UINT32 length);

/*simcom_ping*/

typedef SCPingResultCode (*_sAPI_Ping)(SCping *ctx);
/*simcom_call*/
typedef SC_CALLReturnCode (*_sAPI_CallDialMsg)(UINT8* dialstring,sMsgQRef msgQ);
typedef SC_CALLReturnCode (*_sAPI_CallAnswerMsg)(sMsgQRef msgQ);
typedef SC_CALLReturnCode (*_sAPI_CallEndMsg)(sMsgQRef msgQ);
typedef UINT8 (*_sAPI_CallStateMsg)(void);
typedef SC_CALLReturnCode (*_sAPI_CallAutoAnswer)(INT32 seconds,sMsgQRef msgQ);

//FOTA
typedef int (*_sAPI_FotaServiceBegin)(void *pfotaParam);
typedef int (*_sAPI_FotaImageWrite)(char * data, int len, int file_size);
typedef int (*_sAPI_FotaImageVerify)(int file_size);

typedef int (*_sAPI_Snprintf)(char *str, int size, const char *format, ...);
typedef int (*_sAPI_Sprintf)(char *str, const char *format, ...);
typedef int (*_sAPI_Vsnprintf)(char *str, UINT32 size, const char *format, va_list ap);
typedef int (*_sAPI_sscanf)(char *str, const char *format, ...);

typedef void (*_sAPI_ForceDownload)(void);


typedef int (*_sAPI_InitNetLight)(unsigned int dummy);

/*simcom_pwm*/
typedef PWM_ReturnCode (*_sAPI_PWMConfig)(SC_PWM_DEVICE_NUM     pwm_num, PWM_SWITCH on_off, unsigned char frq_div,unsigned short high_level_cnt, unsigned short total_cnt);
typedef PWM_ReturnCode (*_sAPI_PWMConfigEx)(SC_PWM_CONFIG *handle);

/*simcom_lcd*/
typedef INT8 (*_sAPI_LcdOpen)(sc_lcd_data_t *lcd_data);
typedef INT8 (*_sAPI_LcdWriteCmd)(unsigned int cmd);
typedef INT8 (*_sAPI_LcdWriteData)(unsigned char *pbuffer, unsigned int sum_pixel);
typedef INT8 (*_sAPI_LcdClearScreen)(unsigned short *color_data);
typedef INT8 (*_sAPI_LcdSetBrightness)(unsigned int level);
typedef INT8 (*_sAPI_LcdDisplayOn)(void);
typedef INT8 (*_sAPI_LcdDisplayOff)(void);
typedef void (*_sAPI_LcdBacklightAndIOVccCtrlPin)(SC_Module_GPIONumbers Backlight, SC_Module_GPIONumbers iovcc);
typedef INT8 (*_sAPI_LcdWrite)(unsigned char *pbuffer, unsigned short start_x, unsigned short start_y, unsigned short end_x, unsigned short end_y);
typedef INT8 (*_sAPI_LcdWriteU16Data)(unsigned short *pbuffer, unsigned int sum_pixel);
typedef INT8 (*_sAPI_LcdSetBLPWM)(unsigned int level);
typedef void (*_sAPI_LcdClose)(void);

/*simcom_hardware*/
typedef int (*_sAPI_NetStatusService)(unsigned int dummy);
typedef SC_GPIOReturnCode (*_sAPI_PinmuxFunc)(unsigned int gpio, unsigned int mf);

/*file system get disk size*/
typedef long long (*_sAPI_GetSize)(char *disc);
typedef long long (*_sAPI_GetFreeSize)(char *disc);
typedef long long (*_sAPI_GetUsedSize)(char *disc);

/*simcom_rtc*/
typedef void (*AlCallback)(void);
typedef INT16 (*_sAPI_RtcSetRealTime)(t_rtc *rtcSetTime);
typedef INT16 (*_sAPI_RtcGetRealTime)(t_rtc *RtcTime);
typedef void  (*_sAPI_RtcSetAlarm)(t_rtc *rtcSetTime);
typedef void  (*_sAPI_RtcGetAlarm)(t_rtc *rtcSetTime);
typedef void  (*_sAPI_RtcEnableAlarm)(BOOL onoff);
typedef void  (*_sAPI_RtcRegisterCB)(AlCallback callback);

typedef int (*_sAPI_CamOpen)(void);
typedef int (*_sAPI_CamClose) (void);
typedef int (*_sAPI_StartShowLCD)(void);
typedef int (*_sAPI_StopShowLCD)(void);
typedef int (*_sAPI_GetCamBuf)(int func,CamBufferHandle callbackfunc);
typedef int (*_sAPI_YUVtoRGBZoomOut)(unsigned short *pRgbData,unsigned int rgbWidth,unsigned int rgbHeight,
                                           char *pYuvData,unsigned int yuvWidth,unsigned int yuvHeight);
typedef void (*_sAPI_CamPinConfig)(cam_user_pin_t *cam_pins);
typedef void (*_sAPI_CamPinConfigUninstall)(void);
typedef int (*_sAPI_CamParaSet)(unsigned char param_mode, void *pValue);
typedef int (*_sAPI_CamOpenEx)(sc_cam_data_t *cam_data);
typedef int (*_sAPI_CamWriteReg)(const struct cam_i2c_attr *attr, unsigned int reg, unsigned int val);
typedef void (*_sAPI_CamMclkOnOff)(int onoff);
typedef sc_cam_global_config_t * (*_sAPI_CamGetGlobalConfig)(void);
typedef void(*_sAPI_CamIspsizeSet)(unsigned short in_w,unsigned short in_h,unsigned short out_w,unsigned short out_h);
typedef void (*_sAPI_CamLdoOnOff)(int onoff,enum cam_ldo_chl channel);
typedef BOOL (*_sAPI_EchoParaModeSet)(unsigned char SaveMode, unsigned char Level);
typedef BOOL (*_sAPI_EchoSuppression_PARA)(SC_Dspconfiguration dspconfig);
void get_sAPI(void* argv);


#if 0
extern _strcpy strcpy;
extern _strlen strlen;
extern _strncpy strncpy;
extern _strcat strcat;
extern _strncat strncat;
extern _strcmp strcmp;
extern _strncmp strncmp;
extern _sprintf sprintf;
extern _snprintf snprintf;
extern _sscanf sscanf;
extern _atoi atoi;
extern _memcpy memcpy;
extern _memchr memchr;
extern _memcmp memcmp;
extern _memset memset;
#endif
extern _sAPI_Debug sAPI_Debug;
extern _sAPI_Printf sAPI_Printf;
//extern _sAPI_Malloc sAPI_Malloc;
void *sAPI_Malloc(UINT32 size);

void sAPI_Free(void* p);


//extern _sAPI_Free sAPI_Free;
extern _sAPI_TaskCreate sAPI_TaskCreate;
extern _sAPI_TaskDelete sAPI_TaskDelete;
extern _sAPI_TaskSuspend sAPI_TaskSuspend;
extern _sAPI_TaskResume sAPI_TaskResume;
extern _sAPI_TaskSleep sAPI_TaskSleep;
extern _sAPI_TaskGetCurrentRef sAPI_TaskGetCurrentRef;
extern _sAPI_TaskTerminate sAPI_TaskTerminate;
extern _sAPI_MsgQCreate sAPI_MsgQCreate;
extern _sAPI_MsgQDelete sAPI_MsgQDelete;
extern _sAPI_MsgQSend sAPI_MsgQSend;
extern _sAPI_MsgQSendSuspend sAPI_MsgQSendSuspend;
extern _sAPI_MsgQRecv sAPI_MsgQRecv;
extern _sAPI_MsgQPoll sAPI_MsgQPoll;
extern _sAPI_SemaphoreCreate sAPI_SemaphoreCreate;
extern _sAPI_SemaphoreDelete sAPI_SemaphoreDelete;
extern _sAPI_SemaphoreAcquire sAPI_SemaphoreAcquire;
extern _sAPI_SemaphoreRelease sAPI_SemaphoreRelease;
extern _sAPI_SemaphorePoll sAPI_SemaphorePoll;
extern _sAPI_MutexCreate sAPI_MutexCreate;
extern _sAPI_MutexDelete sAPI_MutexDelete;
extern _sAPI_MutexLock sAPI_MutexLock;
extern _sAPI_MutexUnLock sAPI_MutexUnLock;
extern _sAPI_FlagCreate sAPI_FlagCreate;
extern _sAPI_FlagDelete sAPI_FlagDelete;
extern _sAPI_FlagSet sAPI_FlagSet;
extern _sAPI_FlagWait sAPI_FlagWait;
extern _sAPI_TimerCreate sAPI_TimerCreate;
extern _sAPI_TimerStart sAPI_TimerStart;
extern _sAPI_TimerStop sAPI_TimerStop;
extern _sAPI_TimerDelete sAPI_TimerDelete;
extern _sAPI_TimerGetStatus sAPI_TimerGetStatus;
extern _sAPI_GetTicks sAPI_GetTicks;
extern _sAPI_Gettimeofday sAPI_Gettimeofday;
extern _sAPI_GettimeofdaySyncRtc sAPI_GettimeofdaySyncRtc;
extern _sAPI_Time sAPI_Time;
extern _sAPI_DelayUs sAPI_DelayUs;
extern _sAPI_ContextLock sAPI_ContextLock;
extern _sAPI_ContextUnlock sAPI_ContextUnlock;
extern _sAPI_Realloc sAPI_Realloc;
extern _sAPI_Calloc sAPI_Calloc;
extern _sAPI_DisableInterrupts sAPI_DisableInterrupts;
extern _sAPI_EnableInterrupts  sAPI_EnableInterrupts;
extern _sAPI_RestoreInterrupts sAPI_RestoreInterrupts;
extern _sAPI_GetTaskInfo sAPI_GetTaskInfo;

extern _sAPI_NetworkInit sAPI_NetworkInit;
extern _sAPI_NetworkGetCsq sAPI_NetworkGetCsq;
extern _sAPI_NetworkGetCreg sAPI_NetworkGetCreg;
extern _sAPI_NetworkGetCpsi sAPI_NetworkGetCpsi;
extern _sAPI_NetworkGetCgreg sAPI_NetworkGetCgreg;
extern _sAPI_NetworkGetCnmp sAPI_NetworkGetCnmp;
extern _sAPI_NetworkSetCnmp sAPI_NetworkSetCnmp;
extern _sAPI_NetworkGetCops sAPI_NetworkGetCops;
extern _sAPI_NetworkSetCops sAPI_NetworkSetCops;
extern _sAPI_NetworkGetCgdcont sAPI_NetworkGetCgdcont;
extern _sAPI_NetworkSetCgdcont sAPI_NetworkSetCgdcont;
extern _sAPI_NetworkGetCGAUTH sAPI_NetworkGetCGAUTH;
extern _sAPI_NetworkGetCgauth sAPI_NetworkGetCgauth;
extern _sAPI_NetworkSetCGAUTH sAPI_NetworkSetCGAUTH;
extern _sAPI_NetworkSetCgauth sAPI_NetworkSetCgauth;
extern _sAPI_NetworkGetCgact sAPI_NetworkGetCgact;
extern _sAPI_NetworkSetCgact sAPI_NetworkSetCgact;
extern _sAPI_NetworkGetCgatt sAPI_NetworkGetCgatt;
extern _sAPI_NetworkSetCgatt sAPI_NetworkSetCgatt;
extern _sAPI_NetworkSetCfun sAPI_NetworkSetCfun;
extern _sAPI_NetworkGetCfun sAPI_NetworkGetCfun;
extern _sAPI_NetworkSetCtzu sAPI_NetworkSetCtzu;
extern _sAPI_NetworkGetCgpaddr sAPI_NetworkGetCgpaddr;
extern _sAPI_NetworkGetCnetci sAPI_NetworkGetcnetci;
extern _sAPI_NetworkGetCnetci sAPI_NetworkGetCnetci;
extern _sAPI_SysGetimsi sAPI_SysGetimsi;
extern _sAPI_SysGetImsi sAPI_SysGetImsi;
extern _sAPI_SysGetHplmn sAPI_SysGetHplmn;
extern _sAPI_SimPinGet sAPI_SimPinGet;
extern _sAPI_SimcardPinGet sAPI_SimcardPinGet;
extern _sAPI_SysGetIccid sAPI_SysGetIccid;
extern _sAPI_SimcardSwitchMsg sAPI_SimcardSwitchMsg;
extern _sAPI_SimcardHotSwapMsg sAPI_SimcardHotSwapMsg;
extern _sAPI_SimPinSet sAPI_SimPinSet;
extern _sAPI_SimcardPinSet sAPI_SimcardPinSet;
extern _sAPI_SimLockSet sAPI_SimLockSet;
extern _sAPI_SysGetSubscriber sAPI_SysGetSubscriber;

extern _sAPI_SmsWriteMsg sAPI_SmsWriteMsg;
extern _sAPI_SmsSendMsg sAPI_SmsSendMsg;
extern _sAPI_SmsSendStrogeMsg sAPI_SmsSendStrogeMsg;
extern _sAPI_SmsSendStorageMsg sAPI_SmsSendStorageMsg;
extern _sAPI_SmsSendLongMsg sAPI_SmsSendLongMsg;
extern _sAPI_SmsReadMsg sAPI_SmsReadMsg;
extern _sAPI_SmsDelAllMsg sAPI_SmsDelAllMsg;
extern _sAPI_SmsDelOneMsg sAPI_SmsDelOneMsg;
extern _sAPI_SmsSetNewMsgInd sAPI_SmsSetNewMsgInd;

extern _sAPI_UartWrite sAPI_UartWrite;
extern _sAPI_UartRead sAPI_UartRead;
extern _sAPI_UartWriteString sAPI_UartWriteString;
extern _sAPI_UartSetConfig sAPI_UartSetConfig;
extern _sAPI_UartGetConfig sAPI_UartGetConfig;
extern _sAPI_UartPrintf sAPI_UartPrintf;
extern _sAPI_SendATCMDWaitResp sAPI_SendATCMDWaitResp;
extern _sAPI_UartRxStatus sAPI_UartRxStatus;
extern _sAPI_UartControl sAPI_UartControl;
extern _sAPI_UartRefRegister sAPI_UartRefRegister;
extern _sAPI_UartRegisterCallback sAPI_UartRegisterCallback;
extern _sAPI_UartRegisterCallbackEX sAPI_UartRegisterCallbackEX;
extern _sAPI_UartRs485DePinAssign sAPI_UartRs485DePinAssign;
extern _sAPI_UartRs485DePinAssignEx sAPI_UartRs485DePinAssignEx;

extern _sAPI_GnssPowerStatusSet sAPI_GnssPowerStatusSet;
extern _sAPI_GnssPowerStatusGet sAPI_GnssPowerStatusGet;

#ifdef FEATURE_SIMCOM_GPS
extern _sAPI_GnssApFlashSet sAPI_GnssApFlashSet;
extern _sAPI_GnssApFlashGet sAPI_GnssApFlashGet;
extern _sAPI_GnssNmeaDataGet sAPI_GnssNmeaDataGet;
extern _sAPI_GnssStartMode sAPI_GnssStartMode;
extern _sAPI_GnssBaudRateSet sAPI_GnssBaudRateSet;
extern _sAPI_GnssBaudRateGet sAPI_GnssBaudRateGet;
extern _sAPI_GnssModeSet sAPI_GnssModeSet;
extern _sAPI_GnssModeGet sAPI_GnssModeGet;
extern _sAPI_GnssNmeaRateSet sAPI_GnssNmeaRateSet;
extern _sAPI_GnssNmeaRateGet sAPI_GnssNmeaRateGet;
extern _sAPI_GnssNmeaSentenceSet sAPI_GnssNmeaSentenceSet;
extern _sAPI_GnssNmeaSentenceGet sAPI_GnssNmeaSentenceGet;
extern _sAPI_GPSInfoGet sAPI_GPSInfoGet;
extern _sAPI_GNSSInfoGet sAPI_GNSSInfoGet;
extern _sAPI_SendCmd2GNSS sAPI_SendCmd2GNSS;
extern _sAPI_AGPS sAPI_AGPS;
extern _sAPI_GpsInfoGet sAPI_GpsInfoGet;
extern _sAPI_GnssInfoGet sAPI_GnssInfoGet;
extern _sAPI_SendCmd2Gnss sAPI_SendCmd2Gnss;
extern _sAPI_GnssAgpsSeviceOpen sAPI_GnssAgpsSeviceOpen;
#endif

extern _sAPI_SystemSleepSet sAPI_SystemSleepSet;
extern _sAPI_SystemSleepGet sAPI_SystemSleepGet;
extern _sAPI_SystemAlarmClock2Wakeup sAPI_SystemAlarmClock2Wakeup;
extern _sAPI_SystemSleepExSet sAPI_SystemSleepExSet;
extern _sAPI_SystemSleepExGet sAPI_SystemSleepExGet;

extern _sAPI_GpioSetValue sAPI_GpioSetValue;
extern _sAPI_GpioGetValue sAPI_GpioGetValue;
extern _sAPI_GpioConfig sAPI_GpioConfig;
extern _sAPI_GpioConfigInterrupt  sAPI_GpioConfigInterrupt;
extern _sAPI_GpioWakeupEnable sAPI_GpioWakeupEnable;
extern _sAPI_setGpioDirection sAPI_setGpioDirection;
extern _sAPI_GpioSetDirection sAPI_GpioSetDirection;
extern _sAPI_GpioGetDirection sAPI_GpioGetDirection;
extern _sAPI_SimDetectPinStatus sAPI_SimDetectPinStatus;
extern _sAPI_setPinFunction sAPI_setPinFunction;
extern _sAPI_ReadAdc sAPI_ReadAdc;
extern _sAPI_ReadVbat sAPI_ReadVbat;
extern _sAPI_SysPowerOff sAPI_SysPowerOff;
extern _sAPI_SysReset sAPI_SysReset;
extern _sAPI_SetVddAux sAPI_SetVddAux;
extern _sAPI_GetPowerUpEvent sAPI_GetPowerUpEvent;
extern _sAPI_GetPowerDownEvent sAPI_GetPowerDownEvent;
extern _sAPI_GetPowerKeyStatus sAPI_GetPowerKeyStatus;
extern _sAPI_SetPowerKeyOffFunc sAPI_SetPowerKeyOffFunc;
extern _sAPI_PowerKeyRegisterCallback sAPI_PowerKeyRegisterCallback;

/*  spi nor */
extern _sAPI_ExtNorFlashInit sAPI_ExtNorFlashInit;
extern _sAPI_ExtNorFlashShowSupportList sAPI_ExtNorFlashShowSupportList;
extern _sAPI_ExtNorFlashBlock64kErase sAPI_ExtNorFlashBlock64kErase;
extern _sAPI_ExtNorFlashSectorErase sAPI_ExtNorFlashSectorErase;
extern _sAPI_ExtNorFlashRead sAPI_ExtNorFlashRead;
extern _sAPI_ExtNorFlashWrite sAPI_ExtNorFlashWrite;
extern _sAPI_ExtNorFlashReadID sAPI_ExtNorFlashReadID;
extern _sAPI_ExtNorFlashCs sAPI_ExtNorFlashCs;


/*  spi */
extern _sAPI_SPIReadBytes sAPI_SPIReadBytes;
extern _sAPI_ExtSpiReadBytes sAPI_ExtSpiReadBytes;
extern _sAPI_SPIWriteBytes sAPI_SPIWriteBytes;
extern _sAPI_SPIConfigInit sAPI_SPIConfigInit;
extern _sAPI_SpiConfigInitEx sAPI_SpiConfigInitEx;
extern _sAPI_SpiReadBytesEx sAPI_SpiReadBytesEx;
extern _sAPI_SpiWriteBytesEx sAPI_SpiWriteBytesEx;



extern _sAPI_I2CRead sAPI_I2CRead;
extern _sAPI_I2CWrite sAPI_I2CWrite;
extern _sAPI_I2CReadEx sAPI_I2CReadEx;
extern _sAPI_I2CWriteEx sAPI_I2CWriteEx;
extern _sAPI_I2CConfigInit sAPI_I2CConfigInit;

extern _sAPI_fopen sAPI_fopen;
extern _sAPI_fclose sAPI_fclose;
extern _sAPI_fread sAPI_fread;
extern _sAPI_fwrite sAPI_fwrite;
extern _sAPI_fseek sAPI_fseek;
extern _sAPI_ftell sAPI_ftell;
extern _sAPI_frewind sAPI_frewind;
extern _sAPI_fsize sAPI_fsize;
extern _sAPI_fsync sAPI_fsync;
extern _sAPI_mkdir sAPI_mkdir;
extern _sAPI_opendir sAPI_opendir;
extern _sAPI_closedir sAPI_closedir;
extern _sAPI_readdir sAPI_readdir;
extern _sAPI_seekdir sAPI_seekdir;
extern _sAPI_telldir sAPI_telldir;
extern _sAPI_remove sAPI_remove;
extern _sAPI_rename sAPI_rename;
extern _sAPI_access sAPI_access;
extern _sAPI_stat sAPI_stat;
extern _sAPI_GetSize sAPI_GetSize;
extern _sAPI_GetFreeSize sAPI_GetFreeSize;
extern _sAPI_GetUsedSize sAPI_GetUsedSize;
extern _sAPI_ftruncate sAPI_ftruncate;
extern _sAPI_GetFsSize sAPI_GetFsSize;
extern _sAPI_GetFsFreeSize sAPI_GetFsFreeSize;
#ifdef FEATURE_SIMCOM_FS_OLD
extern _sAPI_FsIsFileNameValid sAPI_FsIsFileNameValid;
extern _sAPI_FsDirProcessor sAPI_FsDirProcessor;
extern _sAPI_FsFileOpen sAPI_FsFileOpen;
extern _sAPI_FsFileRead sAPI_FsFileRead;
extern _sAPI_FsFileSeek sAPI_FsFileSeek;
extern _sAPI_FsFileWrite sAPI_FsFileWrite;
extern _sAPI_FsFileClose sAPI_FsFileClose;
extern _sAPI_FsFileRename sAPI_FsFileRename;
extern _sAPI_FsFindFileAndGetSize sAPI_FsFindFileAndGetSize;
extern _sAPI_FsFileDelete sAPI_FsFileDelete;
extern _sAPI_FsGetDiskSize sAPI_FsGetDiskSize;
extern _sAPI_FsIsPathExist sAPI_FsIsPathExist;
extern _sAPI_FsFileSave sAPI_FsFileSave;
extern _sAPI_FsFileTraverse sAPI_FsFileTraverse;
extern _sAPI_FsNameSeparate sAPI_FsNameSeparate;
extern _sAPI_FsSync sAPI_FsSync;
extern _sAPI_FsFileTell sAPI_FsFileTell;
#endif

extern _sAPI_HtpSrvConfig sAPI_HtpSrvConfig;
extern _sAPI_HtpUpdate sAPI_HtpUpdate;
extern _sAPI_NtpUpdate sAPI_NtpUpdate;
extern _sAPI_TcpipGetErrno sAPI_TcpipGetErrno;
extern _sAPI_TcpipPdpActive sAPI_TcpipPdpActive;
extern _sAPI_TcpipPdpDeactive sAPI_TcpipPdpDeactive;
extern _sAPI_TcpipPdpDeactiveNotify sAPI_TcpipPdpDeactiveNotify;
extern _sAPI_TcpipGetSocketPdpAddr sAPI_TcpipGetSocketPdpAddr;
extern _sAPI_TcpipSocket sAPI_TcpipSocket;
extern _sAPI_TcpipBind sAPI_TcpipBind;
extern _sAPI_TcpipListen sAPI_TcpipListen;
extern _sAPI_TcpipAccept sAPI_TcpipAccept;
extern _sAPI_TcpipConnect sAPI_TcpipConnect;
extern _sAPI_TcpipSend sAPI_TcpipSend;
extern _sAPI_TcpipRecv sAPI_TcpipRecv;
extern _sAPI_TcpipSendto sAPI_TcpipSendto;
extern _sAPI_TcpipRecvfrom sAPI_TcpipRecvfrom;
extern _sAPI_TcpipClose sAPI_TcpipClose;
extern _sAPI_TcpipShutdown sAPI_TcpipShutdown;
extern _sAPI_TcpipGetsockname sAPI_TcpipGetsockname;
extern _sAPI_TcpipGetpeername sAPI_TcpipGetpeername;
extern _sAPI_TcpipGetsockopt sAPI_TcpipGetsockopt;
extern _sAPI_TcpipSetsockopt sAPI_TcpipSetsockopt;
extern _sAPI_TcpipIoctlsocket sAPI_TcpipIoctlsocket;
extern _sAPI_TcpipGethostbyname sAPI_TcpipGethostbyname;
extern _sAPI_TcpipSelect sAPI_TcpipSelect;
extern _sAPI_TcpipInet_addr sAPI_TcpipInet_addr;
extern _sAPI_TcpipInet_addr sAPI_TcpipInetAddr;
extern _sAPI_TcpipHtons sAPI_TcpipHtons;
extern _sAPI_TcpipNtohs sAPI_TcpipNtohs;
extern _sAPI_TcpipInet_ntoa sAPI_TcpipInet_ntoa;
extern _sAPI_TcpipInet_ntoa sAPI_TcpipInetNtoa;
extern _sAPI_TcpipInet_ntop sAPI_TcpipInet_ntop;
extern _sAPI_TcpipInet_ntop sAPI_TcpipInetNtop;
extern _sAPI_TcpipGetSocketErrno sAPI_TcpipGetSocketErrno;
extern _sAPI_TcpipHtonl sAPI_TcpipHtonl;
extern _sAPI_TcpipGetaddrinfo sAPI_TcpipGetaddrinfo;
extern _sAPI_TcpipGetaddrinfo_with_pcid sAPI_TcpipGetaddrinfo_with_pcid;
extern _sAPI_TcpipGetaddrinfo_with_pcid sAPI_TcpipGetaddrinfoWithPcid;
extern _sAPI_TcpipFreeaddrinfo sAPI_TcpipFreeaddrinfo;
extern _sAPI_TcpipSocket_with_callback sAPI_TcpipSocket_with_callback;
extern _sAPI_TcpipSocket_with_callback sAPI_TcpipSocketWithCallback;
extern _sAPI_TcpipInet_pton sAPI_TcpipInet_pton;
extern _sAPI_TcpipInet_pton sAPI_TcpipInetPton;
extern _sAPI_GetSocketState sAPI_GetSocketState;


extern _sAPI_HttpInit sAPI_HttpInit;
extern _sAPI_HttpTerm sAPI_HttpTerm;
extern _sAPI_HttpPara sAPI_HttpPara;
extern _sAPI_HttpAction sAPI_HttpAction;
extern _sAPI_HttpData sAPI_HttpData;
extern _sAPI_HttpHead sAPI_HttpHead;
extern _sAPI_HttpRead sAPI_HttpRead;
extern _sAPI_HttpPostfile sAPI_HttpPostfile;
extern _sAPI_FtpsInit sAPI_FtpsInit;
extern _sAPI_FtpsDeInit sAPI_FtpsDeInit;
extern _sAPI_FtpsLogin sAPI_FtpsLogin;
extern _sAPI_FtpsLogout sAPI_FtpsLogout;
extern _sAPI_FtpsDownloadFile sAPI_FtpsDownloadFile;
extern _sAPI_FtpsDownloadFileToBuffer sAPI_FtpsDownloadFileToBuffer;
extern _sAPI_FtpsUploadFile sAPI_FtpsUploadFile;
extern _sAPI_FtpsDeleteFile sAPI_FtpsDeleteFile;
extern _sAPI_FtpsCreateDirectory sAPI_FtpsCreateDirectory;
extern _sAPI_FtpsDeleteDirectory sAPI_FtpsDeleteDirectory;
extern _sAPI_FtpsChangeDirectory sAPI_FtpsChangeDirectory;
extern _sAPI_FtpsGetCurrentDirectory sAPI_FtpsGetCurrentDirectory;
extern _sAPI_FtpsList sAPI_FtpsList;
extern _sAPI_FtpsGetFileSize sAPI_FtpsGetFileSize;
extern _sAPI_FtpsTransferType sAPI_FtpsTransferType;
extern _sAPI_FtpsGetTransferType sAPI_FtpsGetTransferType;
extern _sAPI_FtpsSslConfig sAPI_FtpsSslConfig;
extern _sAPI_MqttStart sAPI_MqttStart;
extern _sAPI_MqttStop sAPI_MqttStop;
extern _sAPI_MqttAccq sAPI_MqttAccq;
extern _sAPI_MqttRel sAPI_MqttRel;
extern _sAPI_MqttConnect sAPI_MqttConnect;
extern _sAPI_MqttDisConnect sAPI_MqttDisConnect;
extern _sAPI_MqttWillTopic sAPI_MqttWillTopic;
extern _sAPI_MqttWillMsg sAPI_MqttWillMsg;
extern _sAPI_MqttTopic sAPI_MqttTopic;
extern _sAPI_MqttPayload sAPI_MqttPayload;
extern _sAPI_MqttPub sAPI_MqttPub;
extern _sAPI_MqttSubTopic sAPI_MqttSubTopic;
extern _sAPI_MqttSub sAPI_MqttSub;
extern _sAPI_MqttUnSubTopic sAPI_MqttUNSubTopic;
extern _sAPI_MqttUnsub sAPI_MqttUnsub;
extern _sAPI_MqttSslCfg sAPI_MqttSslCfg;
extern _sAPI_MqttCfg sAPI_MqttCfg;
extern _sAPI_MqttConnLostCb sAPI_MqttConnLostCb;
extern _sAPI_SslGetContextIdMsg sAPI_SslGetContextIdMsg;
extern _sAPI_SslSetContextIdMsg sAPI_SslSetContextIdMsg;
extern _sAPI_SysGetVersion sAPI_SysGetVersion;
extern _sAPI_SysGetRFVersion sAPI_SysGetRFVersion;
extern _sAPI_SysGetCusVersion sAPI_SysGetCusVersion;
extern _sAPI_SysGetImei sAPI_SysGetImei;
extern _sAPI_SysGetBuildTime sAPI_SysGetBuildTime;

extern _sAPI_UrcRefRegister sAPI_UrcRefRegister;
extern _sAPI_UrcRefRelease sAPI_UrcRefRelease;
extern _sAPI_UsbVbusDetect sAPI_UsbVbusDetect;
extern _sAPI_UsbVcomWrite sAPI_UsbVcomWrite;
extern _sAPI_UsbVcomRead sAPI_UsbVcomRead;
extern _sAPI_UsbVcomRegisterCallback sAPI_UsbVcomRegisterCallback;
extern _sAPI_UsbVcomRegisterCallbackEX sAPI_UsbVcomRegisterCallbackEX;
extern _sAPI_AppPackageOpen sAPI_AppPackageOpen;
extern _sAPI_AppPackageWrite sAPI_AppPackageWrite;
extern _sAPI_AppPackageRead sAPI_AppPackageRead;
extern _sAPI_AppPackageClose sAPI_AppPackageClose;
extern _sAPI_AppPackageCrc sAPI_AppPackageCrc;

extern _sAPI_SslHandShake sAPI_SslHandShake;
extern _sAPI_SslRead sAPI_SslRead;
extern _sAPI_SslSend sAPI_SslSend;
extern _sAPI_SslClose sAPI_SslClose;

extern _sAPI_GetSysLocalTime sAPI_GetSysLocalTime;
extern _sAPI_SetSysLocalTime sAPI_SetSysLocalTime;
extern _sAPI_disableDUMP sAPI_disableDUMP;//old name
extern _sAPI_enableDUMP sAPI_enableDUMP;//old name
extern _sAPI_DisableDump sAPI_DisableDump;
extern _sAPI_EnableDump sAPI_EnableDump;
extern _sAPI_GetFlashType sAPI_GetFlashType;
extern _sAPI_GetCusdataPartitionSize sAPI_GetCusdataPartitionSize;
extern _sAPI_GetAppPartitionSize sAPI_GetAppPartitionSize;

extern _sAPI_EraseFlashSector sAPI_EraseFlashSector;
extern _sAPI_WriteFlash sAPI_WriteFlash;
extern _sAPI_ReadFlash sAPI_ReadFlash;
extern _sAPI_ReadFlashNvmPartition sAPI_ReadFlashNvmPartition;
extern _sAPI_WriteFlashNvmPartition sAPI_WriteFlashNvmPartition;
extern _sAPI_EraseFlashNvmPartition sAPI_EraseFlashNvmPartition;
extern _sAPI_ReadFlashAppPartition sAPI_ReadFlashAppPartition;
extern _sAPI_AppPartitionBinSize sAPI_AppPartitionBinSize;


extern _sAPI_AudioPlaySampleRate sAPI_AudioPlaySampleRate;
extern _sAPI_AudioPlay sAPI_AudioPlay;
extern _sAPI_AudioStop sAPI_AudioStop;
extern _sAPI_AudioRecord sAPI_AudioRecord;
extern _sAPI_AudioPcmPlay sAPI_AudioPcmPlay;
extern _sAPI_AudioPcmStop sAPI_AudioPcmStop;
extern _sAPI_AudioSetVolume sAPI_AudioSetVolume;
extern _sAPI_AudioGetVolume sAPI_AudioGetVolume;
extern _sAPI_PocInitLib sAPI_PocInitLib;
extern _sAPI_PocPlaySound sAPI_PocPlaySound;
extern _sAPI_PocStopSound sAPI_PocStopSound;
extern _sAPI_PocGetPcmAvail sAPI_PocGetPcmAvail;
extern _sAPI_PocCleanBufferData sAPI_PocCleanBufferData;
extern _sAPI_PocStartRecord sAPI_PocStartRecord;
extern _sAPI_PocStopRecord sAPI_PocStopRecord;
extern _sAPI_PocPcmRead sAPI_PocPcmRead;
extern _sAPI_AudRec sAPI_AudRec;
extern _sAPI_AmrStreamRecord sAPI_AmrStreamRecord;
extern _sAPI_AmrStopStreamRecord sAPI_AmrStopStreamRecord;
extern _sAPI_RegisterXipTaskCb   sAPI_RegisterXipTaskCb;
extern _sAPI_CacheCleanMemory    sAPI_CacheCleanMemory;
extern _sAPI_AudioSetAmrRateLevel   sAPI_AudioSetAmrRateLevel;
extern _sAPI_GetStatisticsData sAPI_GetStatisticsData;


extern _sAPI_AudioPlayMp3Cont sAPI_AudioPlayMp3Cont;
extern _sAPI_AudioSetMicGain sAPI_AudioSetMicGain;
extern _sAPI_AudioGetMicGain sAPI_AudioGetMicGain;
extern _sAPI_AudioMp3StreamPlay sAPI_AudioMp3StreamPlay;
extern _sAPI_AudioMp3StreamStop sAPI_AudioMp3StreamStop;
extern _sAPI_AudioSetPaCtrlConfig sAPI_AudioSetPaCtrlConfig;
extern _sAPI_AudioAmrStreamPlay sAPI_AudioAmrStreamPlay;
extern _sAPI_AudioAmrStreamStop sAPI_AudioAmrStreamStop;
extern _sAPI_AudioPlayAmrCont sAPI_AudioPlayAmrCont;
extern _sAPI_AudioWavFilePlay sAPI_AudioWavFilePlay;
extern _sAPI_AudioStatus sAPI_AudioStatus;
extern _sAPI_AudioSetPlayPath sAPI_AudioSetPlayPath;
extern _sAPI_AudioGetPlayPath sAPI_AudioGetPlayPath;
extern _sAPI_AudioPlayWavCont sAPI_AudioPlayWavCont;

extern _sAPI_GetSystemInfo sAPI_GetSystemInfo;
extern _sAPI_GetCpuUid sAPI_GetCpuUid;


extern _sAPI_LocGet sAPI_LocGet;

extern _sAPI_AppDownload sAPI_AppDownload;

extern _sAPI_AesEncrypt   sAPI_AesEncrypt;
extern _sAPI_AesDecrypt   sAPI_AesDecrypt;
extern _sAPI_Base64Encode sAPI_Base64Encode;
extern _sAPI_Base64Decode sAPI_Base64Decode;

extern _sAPI_RsaEncrypt sAPI_RsaEncrypt;
extern _sAPI_RsaDecrypt sAPI_RsaDecrypt;



extern _sAPI_TTSPlay sAPI_TTSPlay;
extern _sAPI_TTSStop  sAPI_TTSStop;
extern _sAPI_TTSSetParameters sAPI_TTSSetParameters;
extern _sAPI_TTSSetStatusCallBack sAPI_TTSSetStatusCallBack;

extern _sAPI_AtSend sAPI_AtSend;
extern _sAPI_Ping sAPI_Ping;

extern _sAPI_CallDialMsg sAPI_CallDialMsg;
extern _sAPI_CallAnswerMsg sAPI_CallAnswerMsg;
extern _sAPI_CallEndMsg sAPI_CallEndMsg;
extern _sAPI_CallStateMsg sAPI_CallStateMsg;
extern _sAPI_CallAutoAnswer sAPI_CallAutoAnswer;

//fota
extern _sAPI_FotaServiceBegin sAPI_FotaServiceBegin;
extern _sAPI_FotaImageWrite sAPI_FotaImageWrite;
extern _sAPI_FotaImageVerify sAPI_FotaImageVerify;

extern _sAPI_Snprintf sAPI_Snprintf;
extern _sAPI_InitNetLight sAPI_InitNetLight;

extern _sAPI_Sprintf   sAPI_Sprintf;
extern _sAPI_Vsnprintf sAPI_Vsnprintf;
extern _sAPI_sscanf sAPI_sscanf;

extern _sAPI_PWMConfig sAPI_PWMConfig;
extern _sAPI_PWMConfigEx sAPI_PWMConfigEx;

extern _sAPI_ForceDownload sAPI_ForceDownload;

extern _sAPI_LcdOpen sAPI_LcdOpen;
extern _sAPI_LcdWriteCmd sAPI_LcdWriteCmd;
extern _sAPI_LcdWriteData sAPI_LcdWriteData;
extern _sAPI_LcdClearScreen sAPI_LcdClearScreen;
extern _sAPI_LcdSetBrightness sAPI_LcdSetBrightness;
extern _sAPI_LcdDisplayOn sAPI_LcdDisplayOn;
extern _sAPI_LcdDisplayOff sAPI_LcdDisplayOff;
extern _sAPI_LcdBacklightAndIOVccCtrlPin sAPI_LcdBacklightAndIOVccCtrlPin;
extern _sAPI_LcdWrite sAPI_LcdWrite;
extern _sAPI_LcdWriteData sAPI_LcdWriteU16Data;
extern _sAPI_LcdSetBLPWM sAPI_LcdSetBLPWM;
extern _sAPI_LcdClose sAPI_LcdClose;

extern _sAPI_GetSize sAPI_GetSize;
extern _sAPI_GetFreeSize sAPI_GetFreeSize;
extern _sAPI_GetUsedSize sAPI_GetUsedSize;

extern _sAPI_RtcSetRealTime sAPI_SetRealTimeClock;
extern _sAPI_RtcGetRealTime sAPI_GetRealTimeClock;
extern _sAPI_RtcSetAlarm sAPI_RtcSetAlarm;
extern _sAPI_RtcGetAlarm sAPI_RtcGetAlarm;
extern _sAPI_RtcEnableAlarm sAPI_RtcEnableAlarm;
extern _sAPI_RtcRegisterCB sAPI_RtcRegisterCB;

extern _sAPI_CamOpen sAPI_CamOpen;
extern _sAPI_CamClose sAPI_CamClose;
extern _sAPI_StartShowLCD sAPI_StartShowLCD;
extern _sAPI_StopShowLCD sAPI_StopShowLCD;
extern _sAPI_GetCamBuf sAPI_GetCamBuf;
extern _sAPI_YUVtoRGBZoomOut sAPI_YUVtoRGBZoomOut;
extern _sAPI_CamPinConfig sAPI_CamPinConfig;
extern _sAPI_CamPinConfigUninstall sAPI_CamPinConfigUninstall;
extern _sAPI_CamParaSet sAPI_CamParaSet;
extern _sAPI_CamOpenEx sAPI_CamOpenEx;
extern _sAPI_CamWriteReg sAPI_CamWriteReg;
extern _sAPI_CamMclkOnOff sAPI_CamMclkOnOff;
extern _sAPI_CamGetGlobalConfig sAPI_CamGetGlobalConfig;
extern _sAPI_CamIspsizeSet sAPI_CamIspsizeSet;
extern _sAPI_CamLdoOnOff sAPI_CamLdoOnOff;
extern _sAPI_EchoParaModeSet sAPI_EchoParaModeSet;
extern _sAPI_EchoSuppression_PARA sAPI_EchoSuppression_PARA;
#endif
