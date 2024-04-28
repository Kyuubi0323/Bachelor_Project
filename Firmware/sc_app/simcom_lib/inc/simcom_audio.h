/**
  ******************************************************************************
  * @file    simcom_audio.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of audio peripheral.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef _SIMCOM_AUDIO_H_
#define _SIMCOM_AUDIO_H_

#include "simcom_file_system.h"
#include "simcom_os.h"


#define AUDIO_FILENAME_MAX_LEN  	60
#define WAV_FILE_HEADER_LENGTH 44


#define REC_TASK_STACK_SIZE         	6114
#define REC_TASK_PRIORITY           	190
#define REC_START            0x1
#define REC_STOP             0x2
#define REC_WRITE            0x4
#define REC_FULL             0x8

#define PLAY_TASK_STACK_SIZE         	4096
#define PLAY_TASK_PRIORITY           	150

#define PLAY_START           0x1
#define PLAY_END             0x2
#define PLAY_PLAYING         0x4

#define RECORD_VOICE (1)
#define PLAY_VOICE (2)

#define DisableInterrupt() 
#define EnableInterrupt()
#define REC_FILE_NAME_SIZE 25


typedef enum
{
    SAMPLE_RATE_8K = 0,
    SAMPLE_RATE_16K,
    SAMPLE_RATE_MAX
}AUD_SampleRate;

typedef enum
{
    REC_PATH_LOCAL = 0,
    REC_PATH_MAX
}AUD_RecordSrcPath;

typedef enum
{
    AUDIO_VOLUME_MUTE = 0,
    AUDIO_VOLUME_1,
    AUDIO_VOLUME_2,
    AUDIO_VOLUME_3,
    AUDIO_VOLUME_4,
    AUDIO_VOLUME_5,
    AUDIO_VOLUME_6,
    AUDIO_VOLUME_7,
    AUDIO_VOLUME_8,
    AUDIO_VOLUME_9,
    AUDIO_VOLUME_10,
    AUDIO_VOLUME_11,
} AUD_Volume;

typedef enum
{
    AUDIOHAL_SPK_GAIN_MUTE = 0,
    AUDIOHAL_SPK_VOL_GAIN_1,
    AUDIOHAL_SPK_VOL_GAIN_2,
    AUDIOHAL_SPK_VOL_GAIN_3,
    AUDIOHAL_SPK_VOL_GAIN_4,
    AUDIOHAL_SPK_VOL_GAIN_5,
    AUDIOHAL_SPK_VOL_GAIN_6,
    AUDIOHAL_SPK_VOL_GAIN_7,
} AUD_MICGAIN;

enum
{
    AUDIO_STATUS_IDLE,
    AUDIO_STATUS_BUSY,
};


typedef int(*sAPI_ProcessRecordCb)(char *buffer,int len);
typedef int(*sAPI_NotifyBufStateCb)(UINT32 DataLen);
typedef void(*sAPI_AudRecCallback)(UINT8 status);
typedef void(*sAPI_GetAmrFrameCB)(const UINT8* buf, UINT32 size);

#endif
