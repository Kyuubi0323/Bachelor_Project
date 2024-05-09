/**
  ******************************************************************************
  * @file    demo_audio.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of audio operation,this demo will show how to control the audio component including play\record\set audio configuration\set TX\RX gain ,etc.
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
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "simcom_dspgain.h"

extern SIM_MSG_T GetParamFromUart(void);
extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);

typedef enum{
    SC_AUDIO_DEMO_SET_SAMPLE_RATE       = 1,
    SC_AUDIO_DEMO_PLAY_FILE             = 2,
    SC_AUDIO_DEMO_STOP_PLAY_FILE        = 3,
    SC_AUDIO_DEMO_RECORD                = 4,
    SC_AUDIO_DEMO_RECORD_STOP           = 5,
    SC_AUDIO_DEMO_PLAY_PCM              = 6,
    SC_AUDIO_DEMO_STOP_PLAY_PCM         = 7,
    SC_AUDIO_DEMO_SET_VOLUME            = 8,
    SC_AUDIO_DEMO_GET_VOLUME            = 9,
    SC_AUDIO_DEMO_POC_PLAYPCM           = 10,
    SC_AUDIO_DEMO_POC_STOPPCM           = 11,
    SC_AUDIO_DEMO_POC_RECORDSTART       = 12,
    SC_AUDIO_DEMO_POC_RECORDSTOP        = 13,
    SC_AUDIO_DEMO_CUS_AUDREC            = 14,
    SC_AUDIO_DEMO_PLAY_MP3_CONT         = 15,
    SC_AUDIO_DEMO_SET_MICGAIN           = 16,
    SC_AUDIO_DEMO_GET_MICGAIN           = 17,
    SC_AUDIO_DEMO_PLAY_MP3_BUFFER       = 18,
    SC_AUDIO_DEMO_STOP_MP3_BUFFER       = 19,
    SC_AUDIO_DEMO_PA_CONFIG             = 20,
    SC_AUDIO_DEMO_PLAY_AMR_BUFFER       = 21,
    SC_AUDIO_DEMO_STOP_AMR_BUFFER       = 22,
    SC_AUDIO_DEMO_PLAY_AMR_CONT         = 23,
    SC_AUDIO_DEMO_PLAY_WAV_FILE         = 24,
    SC_AUDIO_DEMO_GET_AMR_FRAME         = 25,
    SC_AUDIO_DEMO_STOP_AMR_FRAME        = 26,
    SC_AUDIO_DEMO_AMR_ENCODE_RATE       = 27,
    SC_AUDIO_DEMO_PLAY_WAV_CONT         = 28,
    SC_EchoSuppressionParameter         = 29,
    SC_EchoParaModeSet                  = 30,
    SC_AUDIO_DEMO_MAX                   = 99
}SC_AUDIO_DEMO_TYPE;

#define MAX_VALID_USER_NAME_LENGTH  255-7

/**
  * @brief  Dummy API.
  * @param  status.
  * @note   
  * @retval void
  */
void recordCallBac_Test(UINT8 status)
{
    sAPI_Debug("%s status is %d!",__func__,status);
}

/**
  * @brief  Get amr file frame bytes.
  * @param  *buf pointer.
  * @param  file size.
  * @note   
  * @retval void
  */
void GetAmrFrame(const UINT8* buf, UINT32 size)
{
    for(int i = 0; i < size; i++)
    {
        sAPI_Debug("buf[%d] = %x", i, buf[i]);
    }
}

/**
  * @brief  Create audio demo.
  * @param  void
  * @note   This demo will show how to control audio component with SIMCom OpenSDK APIs.
  * @retval void
  */
void AudioDemo(void)
{
    SIM_MSG_T optionMsg ={0,0,0,NULL};
    CHAR audioFile[26] = {0};
    char fileNameInfo[MAX_VALID_USER_NAME_LENGTH] = {0};
    SCFILE* fh = NULL;
    static CHAR *buffer = NULL;
    CHAR tempBuffer[64];
    CHAR recording_file[26] = {0};
    UINT32 len, opt = 0;
    AUD_SampleRate sampleRate = 0;
    AUD_Volume volume;
    BOOL directPlay = 0;
    BOOL isSingle = 0;
    BOOL startplay = 0;
    BOOL frame = 0;
    BOOL ret = 0;
    int RateLevel;
    unsigned int gpioNum;
    UINT8 activeLevel;
    CHAR *note = "\r\nPlease select an option to test from the items listed below.\r\n";
    CHAR *optionsList[] = {
        "1. SampleRate",
        "2. Play file",
        "3. Stop play file",
        "4. Record",
        "5. Stop record",
        "6. Play PCM",
        "7. Stop PCM",
        "8. Set volume",
        "9. Get volume",
        "10. POC Play PCM",
        "11. POC Stop PCM",
        "12. POC Start record",
        "13. POC Stop record",
        "14. slxk record",
        "15, Play MP3 cont",
        "16. Set micgain",
        "17. Get micgain",
        "18. play mp3 buffer",
        "19, stop MP3 buffer",
        "20. PA control config",
        "21. play amr buffer",
        "22, stop amr buffer",
        "23, Play amr cont",
        "24, Play WAV with high sampling rate",
        "25. get amr record frame",
        "26. stop amr frame",
        "27. set amr encoder rate",
        "28. Play Wav cont",
        "29. EchoSuppressionParameter",
        "30. EchoParaModeSet",
        "99. back",
    };

    buffer = sAPI_Malloc(100*1024);

    while(1)
    {
        PrintfResp(note);
        PrintfOptionMenu(optionsList,sizeof(optionsList)/sizeof(optionsList[0]));
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
            case SC_AUDIO_DEMO_SET_SAMPLE_RATE:
            {
                PrintfResp("\r\nPlease input sample rate, 0(8K) or 1(16K)\r\n");
                optionMsg = GetParamFromUart();
                sampleRate = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                ret = sAPI_AudioPlaySampleRate(sampleRate);
                if(ret)
                  sAPI_Debug("sAPI_AudioPlaySampleRate: %d success !", sampleRate);
                else
                  sAPI_Debug("sAPI_AudioPlaySampleRate: %d failed !", sampleRate);

                break;
            }

            case SC_AUDIO_DEMO_PLAY_FILE:
            {
                UINT8 path;
                PrintfResp("\r\nPlease input File Name, like c:/test.wav\r\n");
                optionMsg = GetParamFromUart();
                memset(audioFile, 0, sizeof(audioFile));
                strcpy(audioFile, optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input direct or not, 1 or 0\r\n");
                optionMsg = GetParamFromUart();
                directPlay = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input is single file palyback, 1 or 0\r\n");
                optionMsg = GetParamFromUart();
                isSingle = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input playback path, 1(remote) or 0(local)\r\n");
                optionMsg = GetParamFromUart();
                path = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);
                ret = sAPI_AudioSetPlayPath(path);
                sAPI_Debug("sAPI_AudioSetPlayPath: play path set result %d ",ret);

                ret = sAPI_AudioPlay(audioFile, directPlay,isSingle);
                if(ret)
                  sAPI_Debug("sAPI_AudioPlay: playing %s success ", audioFile);
                else
                  sAPI_Debug("sAPI_AudioPlay: playing %s failed", audioFile);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());

                break;
            }

            case SC_AUDIO_DEMO_STOP_PLAY_FILE:
            {
                ret = sAPI_AudioStop();
                if(ret)
                  sAPI_Debug("sAPI_AudioStop: stop %s success ", audioFile);
                else
                  sAPI_Debug("sAPI_AudioStop: stop %s failed", audioFile);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());

                break;
            }

            case SC_AUDIO_DEMO_RECORD:
            {
                PrintfResp("\r\nPlease input File Name.\r\n");
                optionMsg = GetParamFromUart();
                strcpy(recording_file,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                ret = sAPI_AudioRecord(1, REC_PATH_LOCAL, recording_file);
                if(ret)
                  sAPI_Debug("sAPI_AudioRecord: record to %s success ",recording_file);
                else
                  sAPI_Debug("sAPI_AudioRecord: record to %s failed",recording_file);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());

                break;
            }

            case SC_AUDIO_DEMO_RECORD_STOP:
            {
                PrintfResp("\r\nPlease input File Name.\r\n");
                optionMsg = GetParamFromUart();
                strcpy(recording_file,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                ret = sAPI_AudioRecord(0, REC_PATH_LOCAL, recording_file);
                if(ret)
                  sAPI_Debug("sAPI_AudioRecord: stop success ");
                else
                  sAPI_Debug("sAPI_AudioRecord: stop failed");
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_PLAY_PCM:
            {
                UINT8 path;
                PrintfResp("\r\nPlease input File Name to test, like test.wav\r\n");
                optionMsg = GetParamFromUart();
                memset(audioFile, 0, sizeof(audioFile));
                strcpy(audioFile,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input direct or not, 1 or 0\r\n");
                optionMsg = GetParamFromUart();
                directPlay = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);
                memset(fileNameInfo,0,sizeof(fileNameInfo));
                memcpy(fileNameInfo, "c:/", strlen("c:/"));
                strcat(fileNameInfo, audioFile);

                sAPI_Debug("fileNameInfo[%s]",fileNameInfo);

                PrintfResp("\r\nPlease input playback path, 1(remote) or 0(local)\r\n");
                optionMsg = GetParamFromUart();
                path = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);
                ret = sAPI_AudioSetPlayPath(path);
                sAPI_Debug("sAPI_AudioSetPlayPath: play path set result %d ",ret);

                fh = sAPI_fopen(fileNameInfo, "rb");
                //buffer = sAPI_Malloc(100*1024);
                if(buffer == NULL && fh != NULL)
                {
                    sAPI_Debug("sAPI_Malloc fail !");
                }
                else
                {
                    len = sAPI_fread(buffer, 1,100*1024, fh);
                    sAPI_Debug("sAPI_FsFileRead len:%d !", len);
                    if(len > 44)
                    {
                        ret = sAPI_AudioPcmPlay(&buffer[44], (len - 44), directPlay);
                        sAPI_Debug("sAPI_AudioPcmPlay ret:%d !", ret);
                    }
                }
                sAPI_fclose(fh);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_STOP_PLAY_PCM:
            {
                PrintfResp("\r\nPlease input any key.\r\n");
                optionMsg = GetParamFromUart();
                sAPI_Free(optionMsg.arg3);

                ret = sAPI_AudioPcmStop();
                if(ret)
                  sAPI_Debug("sAPI_AudioPcmStop: stop success !");
                else
                  sAPI_Debug("sAPI_AudioPcmStop: stop failed !");
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_SET_VOLUME:
            {
                PrintfResp("\r\nPlease input volume: 0~11.\r\n");
                optionMsg = GetParamFromUart();
                volume = (AUD_Volume)atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                sAPI_AudioSetVolume(volume);
                sAPI_Debug("sAPI_AudioSetVolume !");
                break;
            }

            case SC_AUDIO_DEMO_GET_VOLUME:
            {
                volume = sAPI_AudioGetVolume();
                snprintf(tempBuffer, sizeof(tempBuffer), "\r\nvolume = %d\r\n", volume);

                PrintfResp(tempBuffer);
                sAPI_Debug("sAPI_AudioGetVolume: %d !", volume);
                break;
            }
            case SC_AUDIO_DEMO_POC_PLAYPCM:
            {
                SCFILE* file_hdl = {0};
                char file_name[MAX_VALID_USER_NAME_LENGTH] = {0};
                char buff[320];
                int i = 0;
                UINT32 actul_read_len = 1;
                int ret = -1;
                memcpy(file_name, "c:/test.wav", strlen("c:/test.wav"));

                file_hdl = sAPI_fopen(file_name, "rb");
                sAPI_Debug("sAPI_FsFileOpen file_hdl = %d",file_hdl);
                actul_read_len = sAPI_fread(buff, 1,44, file_hdl);
                sAPI_Debug("sAPI_FsFileRead actul_read_len = %d",actul_read_len);
                while(actul_read_len > 0)
                {
                    ret = -1;
                    actul_read_len = sAPI_fread(buff, 1,320, file_hdl);
                    if(actul_read_len == 320)
                    {
                        ret = sAPI_PocPlaySound(buff,320);
                        i++;
                    }
                    sAPI_Debug("sAPI_PocPlaySound ret = %d actul_read_len = %d!,i = %d", ret,actul_read_len,i);
                    if(i % 10 == 0)
                        sAPI_TaskSleep(20);
                }
                sAPI_fclose(file_hdl);
                ret = sAPI_PocStopSound();
                sAPI_Debug("sAPI_PocStopSound ret = %d", ret);
                break;
            }
            case SC_AUDIO_DEMO_POC_STOPPCM:
            {
                ret = sAPI_PocStopSound();
                sAPI_Debug("sAPI_PocStopSound: %d !", ret);
                break;
            }
            case SC_AUDIO_DEMO_POC_RECORDSTART:
            {
                int i = 0;
                int ret = sAPI_PocStartRecord(NULL,2);
                sAPI_Debug("sAPI_PocStartRecord ret = %d !", ret);
                while(i<200)
                {
                    char bufferPCm[640];
                    sAPI_TaskSleep(5);
                    ret = sAPI_PocPcmRead(bufferPCm,320);
                    sAPI_Debug("sAPI_PocPcmRead bufferPCm length = %d ret %d!", sizeof(bufferPCm),ret);
                    i++;
                }
                break;
            }
            case SC_AUDIO_DEMO_POC_RECORDSTOP:
            {
                int ret = sAPI_PocStopRecord();
                sAPI_Debug("sAPI_PocStopRecord ret = %d !", ret);
                break;
            }
            case SC_AUDIO_DEMO_CUS_AUDREC:
            {
                int duration,oper;
                PrintfResp("\r\nPlease input oper, 1(start record)/0(stop record)\r\n");
                optionMsg = GetParamFromUart();
                oper = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input file name, like C:/test.amr\r\n");
                optionMsg = GetParamFromUart();
                memset(audioFile, 0, sizeof(audioFile));
                strcpy(audioFile, optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input record time amr(1-180s),wav/pcm(1-15s)\r\n");
                optionMsg = GetParamFromUart();
                duration = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                ret = sAPI_AudRec(oper, audioFile,duration,recordCallBac_Test);
                if(ret)
                  sAPI_Debug("sAPI_AudRec: %d operate success", ret);
                else
                  sAPI_Debug("sAPI_AudRec: %d operate failed", ret);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_PLAY_MP3_CONT:
            {
                do
                {
                    PrintfResp("\r\nPlease input File Name, like c:/test.mp3\r\n");
                    optionMsg = GetParamFromUart();
                    memset(audioFile, 0, sizeof(audioFile));
                    strcpy(audioFile, optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);
                    
                    PrintfResp("\r\nPlease input startplay or not, 1 or 0\r\n");
                    optionMsg = GetParamFromUart();
                    startplay = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);
                    
                    PrintfResp("\r\nPlease input is frame, 0 ~ 2\r\n");
                    optionMsg = GetParamFromUart();
                    frame = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    ret = sAPI_AudioPlayMp3Cont(audioFile, startplay,frame);
                    if(ret)
                      sAPI_Debug("sAPI_AudioPlayMp3Cont: load %s success ", audioFile);
                    else
                      sAPI_Debug("sAPI_AudioPlayMp3Cont: load %s failed", audioFile);

                }while(!startplay);
                    
                sAPI_Debug("sAPI_AudioPlayMp3Cont: start play!", audioFile);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_SET_MICGAIN:
            {
                int micgain,ret;
                PrintfResp("\r\nPlease input micgain(0-7)\r\n");
                optionMsg = GetParamFromUart();
                micgain = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);
                if(micgain < 0 || micgain > 7)
                {
                    PrintfResp("\r\nparameter error\r\n");
                    break;
                }
                sAPI_AudioSetMicGain(micgain);
                sAPI_Debug("sAPI_AudioSetMicGain !");
                PrintfResp("\r\nset parameter OK\r\n");
                break;
            }

            case SC_AUDIO_DEMO_GET_MICGAIN:
            {
                char buf[30];
                int ret = sAPI_AudioGetMicGain();
                sprintf(buf,"sAPI_AudioGetMicGain ret = %d !", ret);
                PrintfResp(buf);
                break;
            }
            case SC_AUDIO_DEMO_PLAY_MP3_BUFFER:
            {
                PrintfResp("\r\nPlease input File Name to test, like c:/test.mp3\r\n");
                optionMsg = GetParamFromUart();
                memset(audioFile, 0, sizeof(audioFile));
                strcpy(audioFile,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input direct or not, 1 or 0\r\n");
                optionMsg = GetParamFromUart();
                directPlay = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                memcpy(fileNameInfo, audioFile, strlen(audioFile));
                sAPI_Debug("fileNameInfo[%s]",fileNameInfo);

                fh = sAPI_fopen(fileNameInfo, "rb");
                //buffer = sAPI_Malloc(100*1024);
                if(buffer == NULL || fh == NULL)
                {
                    sAPI_Debug("sAPI_Malloc fail !");
                }
                else
                {
                    len = sAPI_fread(buffer, 1,100*1024, fh);
                    sAPI_Debug("sAPI_FsFileRead len:%d !", len);
                    if(len > 0)
                    {
                        ret = sAPI_AudioMp3StreamPlay(buffer, len, directPlay);
                        sAPI_Debug("sAPI_AudioMp3StreamPlay ret:%d !", ret);
                    }
                }
                sAPI_fclose(fh);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_STOP_MP3_BUFFER:
            {
                ret = sAPI_AudioMp3StreamStop();
                sAPI_Debug("sAPI_AudioMp3StreamStop ret:%d !", ret);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_PA_CONFIG:
            {
                PrintfResp("\r\nPlease input gpio number.\r\n");
                optionMsg = GetParamFromUart();
                gpioNum = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                if(gpioNum >= SC_MODULE_GPIO_MAX)
                {
                    PrintfResp("\r\nincorrect module number.\r\n");
                    break;
                }

                PrintfResp("\r\nPlease input active level.    0:low  1:high.\r\n");
                optionMsg = GetParamFromUart();
                activeLevel = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                ret = sAPI_AudioSetPaCtrlConfig(gpioNum, activeLevel);
                if(ret != SC_GPIORC_OK)
                    sAPI_Debug("sAPI_AudioSetPaCtrlConfig failed.");
                else
                    sAPI_Debug("sAPI_AudioSetPaCtrlConfig success.");

                PrintfResp("\r\noperation successful!\r\n");

                break;
            }
            case SC_AUDIO_DEMO_PLAY_AMR_BUFFER:
            {
                PrintfResp("\r\nPlease input File Name to test, like c:/test.amr\r\n");
                optionMsg = GetParamFromUart();
                memset(audioFile, 0, sizeof(audioFile));
                strcpy(audioFile,optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input direct or not, 1 or 0\r\n");
                optionMsg = GetParamFromUart();
                directPlay = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                memcpy(fileNameInfo, audioFile, strlen(audioFile));
                sAPI_Debug("fileNameInfo[%s]",fileNameInfo);

                fh = sAPI_fopen(fileNameInfo, "rb");
                //buffer = sAPI_Malloc(100*1024);
                if(buffer == NULL || fh == NULL)
                {
                    sAPI_Debug("sAPI_Malloc fail !");
                }
                else
                {
                    len = sAPI_fread(buffer, 1,100*1024, fh);
                    sAPI_Debug("sAPI_FsFileRead len:%d !", len);
                    if(len > 0)
                    {
                        ret = sAPI_AudioAmrStreamPlay(buffer, len, directPlay);
                        sAPI_Debug("sAPI_AudioAmrStreamPlay ret:%d !", ret);
                    }
                }
                sAPI_fclose(fh);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_STOP_AMR_BUFFER:
            {
                ret = sAPI_AudioAmrStreamStop();
                sAPI_Debug("sAPI_AudioAmrStreamStop ret:%d !", ret);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_PLAY_AMR_CONT:
            {
                do
                {
                    PrintfResp("\r\nPlease input File Name, like c:/test.amr\r\n");
                    optionMsg = GetParamFromUart();
                    memset(audioFile, 0, sizeof(audioFile));
                    strcpy(audioFile, optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);
                    
                    PrintfResp("\r\nPlease input startplay or not, 1 or 0\r\n");
                    optionMsg = GetParamFromUart();
                    startplay = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    ret = sAPI_AudioPlayAmrCont(audioFile, startplay);
                    if(ret)
                      sAPI_Debug("sAPI_AudioPlayAmrCont: load %s success ", audioFile);
                    else
                      sAPI_Debug("sAPI_AudioPlayAmrCont: load %s failed", audioFile);
                }while(!startplay);
                    
                sAPI_Debug("sAPI_AudioPlayAmrCont: start play!", audioFile);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_PLAY_WAV_FILE:
            {
                PrintfResp("\r\nPlease input File Name, like c:/test.wav\r\n");
                optionMsg = GetParamFromUart();
                memset(audioFile, 0, sizeof(audioFile));
                strcpy(audioFile, optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                PrintfResp("\r\nPlease input direct or not, 1 or 0\r\n");
                optionMsg = GetParamFromUart();
                directPlay = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                ret = sAPI_AudioWavFilePlay(audioFile, directPlay);
                if(ret)
                  sAPI_Debug("sAPI_AudioWavFilePlay: playing %s success ", audioFile);
                else
                  sAPI_Debug("sAPI_AudioWavFilePlay: playing %s failed", audioFile);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_AUDIO_DEMO_GET_AMR_FRAME:
            {
                ret = sAPI_AmrStreamRecord(GetAmrFrame);
                if(ret)
                    sAPI_Debug("sAPI_AmrStreamRecord: start record!");
                else
                    sAPI_Debug("sAPI_AmrStreamRecord: fail record!");
                break;
            }

            case SC_AUDIO_DEMO_STOP_AMR_FRAME:
            {
                ret = sAPI_AmrStopStreamRecord();
                if(ret)
                    sAPI_Debug("sAPI_AmrStreamRecord: stop record!");
                else
                    sAPI_Debug("sAPI_AmrStreamRecord: stop failed!");
                break;
            }

            case SC_AUDIO_DEMO_AMR_ENCODE_RATE:
            {
                PrintfResp("\r\nPlease input is amr encoder rate, 0 ~ 7\r\n");
                optionMsg = GetParamFromUart();
                RateLevel = atoi(optionMsg.arg3);
                sAPI_Free(optionMsg.arg3);

                sAPI_AudioSetAmrRateLevel(RateLevel);
                sAPI_Debug("RateLevel = %d", RateLevel);
                break;
            }

            case SC_AUDIO_DEMO_PLAY_WAV_CONT:
            {
                do
                {
                    PrintfResp("\r\nPlease input File Name, like c:/test.WAV\r\n");
                    optionMsg = GetParamFromUart();
                    memset(audioFile, 0, sizeof(audioFile));
                    strcpy(audioFile, optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    PrintfResp("\r\nPlease input startplay or not, 1 or 0\r\n");
                    optionMsg = GetParamFromUart();
                    startplay = atoi(optionMsg.arg3);
                    sAPI_Free(optionMsg.arg3);

                    ret = sAPI_AudioPlayWavCont(audioFile, startplay);
                    if(ret)
                      sAPI_Debug("sAPI_AudioPlayWavCont: load %s success ", audioFile);
                    else
                      sAPI_Debug("sAPI_AudioPlayWavCont: load %s failed", audioFile);
                }while(!startplay);

                sAPI_Debug("sAPI_AudioPlayWavCont: start play!", audioFile);
                sAPI_TaskSleep(1);
                sAPI_Debug("sAPI_AudioStatus: %d", sAPI_AudioStatus());
                break;
            }

            case SC_EchoSuppressionParameter:
            {
                SC_Dspconfiguration dspconfig;
                dspconfig.resmode =2;
                dspconfig.dtTresh =40;
                dspconfig.gamma = 0x1000;
                dspconfig.nonlinear = 0x8122;
                dspconfig.noiseFloorDbov = -87;
                dspconfig.txBoostMode =1;
                dspconfig.txBoostGainDb = 5;
                dspconfig.txPeakGainDb = -2;
                dspconfig.txBoostNoiseGainDb = -2;
                dspconfig.txAgcMaxGainDb = 3;
                dspconfig.rxBoostMode =1;
                dspconfig.rxBoostGainDb = 5;
                dspconfig.rxPeakGainDb = -2;
                dspconfig.rxBoostNoiseGainDb = -2;
                dspconfig.rxAgcMaxGainDb = 3;
                dspconfig.DAC_DigGain = 201;
                dspconfig.DAC_DigGain = 1;
                dspconfig.RCV_Gain = 3;
                dspconfig.PGA_Stage1 = 3;
                dspconfig.PGA_Stage2 = 4;
                dspconfig.ADC_DIgGain = 1;
                dspconfig.sampleDelay = 0;
                dspconfig.numOfTaps = 128;
                dspconfig.convergenceSpeed = 7;
                dspconfig.ref2EchoPowerRatio =6;
                unsigned char ret = 0;
                ret = sAPI_EchoSuppression_PARA(dspconfig);
                if (ret!=0)
                    sAPI_Debug("error audio echosupperssion");
                break;
            }
            case SC_EchoParaModeSet:
            {
                sAPI_EchoParaModeSet(1,1);
                break;
            }

            case SC_AUDIO_DEMO_MAX:
            {
                return;
            }
        }
    }
}
 
