/**
  ******************************************************************************
  * @file    demo_cam.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of camera operation.
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

#ifdef CAM_DECODE
#include "tiny_scanner.h"
#endif

//#define USER_CAM_DRIVER
//#define LCD_ST7735S
#define LCD_ST7789V

#define CAM_GC032A
//#define CAM_BF30A2
//#define CAM_GC6153

//#define ST7735S_128_128

#ifdef USER_CAM_DRIVER
extern sc_cam_data_t CAM_GC032a_spi;
#endif

#ifdef LCD_ST7735S
#define LCD_WIDTH    128
#ifdef ST7735S_128_128
#define LCD_HEIGHT   128
#else
#define LCD_HEIGHT   160
#endif
#endif

#ifdef LCD_ST7789V
#define LCD_WIDTH    240
#define LCD_HEIGHT   320
#endif

#ifdef CAM_GC032A
#define CAM_WIDTH    640
#define CAM_HEIGHT   480
#endif

#if defined(CAM_BF30A2) || defined(CAM_GC6153)
#define CAM_WIDTH    240
#define CAM_HEIGHT   320
#endif

typedef enum{
	SC_CAM_DEMO_CAM_TAKE_PICTURES       = 1,
	SC_CAM_DEMO_CAM_SHOW_LCD            = 2,
	SC_CAM_DEMO_CAM_SHOW_PICTURES       = 3,
#ifdef CAM_DECODE
	SC_CAM_DEMO_CAM_DECODE_NOLCD        = 4,
	SC_CAM_DEMO_CAM_DECODE_ONLCD        = 5,
#endif
	SC_CAM_DEMO_CAM_SET_PARA            = 6,
	SC_CAM_DEMO_CAM_SET_IMAGESIZE       = 7,
	SC_CAM_DEMO_CAM_SET_MIRROR          = 8,
	SC_CAM_DEMO_CAM_MAX                 = 99
}SC_LCD_DEMO_TYPE;

extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);

#ifdef LCD_ST7735S
static sc_spi_lcd_write_t st7735s_init_data[] =
{
#ifndef ST7735S_128_128
    { SC_LCD_CMD, 0x11, 0 },
    { SC_LCD_DELAY, 120, 0 },
    { SC_LCD_CMD, 0xB1, 3 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_CMD, 0xB2, 3 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_CMD, 0xB3, 6 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_CMD, 0xB4, 1 },
    { SC_LCD_DATA, 0x03, 0 },
    { SC_LCD_CMD, 0xC0, 3 },
    { SC_LCD_DATA, 0x62, 0 },
    { SC_LCD_DATA, 0x02, 0 },
    { SC_LCD_DATA, 0x04, 0 },
    { SC_LCD_CMD, 0xC1, 1 },
    { SC_LCD_DATA, 0xC0, 0 },
    { SC_LCD_CMD, 0xC2, 2 },
    { SC_LCD_DATA, 0x0D, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_CMD, 0xC3, 2 },
    { SC_LCD_DATA, 0x8D, 0 },
    { SC_LCD_DATA, 0x6A, 0 },
    { SC_LCD_CMD, 0xC4, 2 },
    { SC_LCD_DATA, 0x8D, 0 },
    { SC_LCD_DATA, 0xEE, 0 },
    { SC_LCD_CMD, 0xC5, 1 },
    { SC_LCD_DATA, 0x12, 0 },
    { SC_LCD_CMD, 0xE0, 16 },
    { SC_LCD_DATA, 0x03, 0 },
    { SC_LCD_DATA, 0x1B, 0 },
    { SC_LCD_DATA, 0x12, 0 },
    { SC_LCD_DATA, 0x11, 0 },
    { SC_LCD_DATA, 0x3F, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x32, 0 },
    { SC_LCD_DATA, 0x34, 0 },
    { SC_LCD_DATA, 0x2F, 0 },
    { SC_LCD_DATA, 0x2B, 0 },
    { SC_LCD_DATA, 0x30, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_DATA, 0x01, 0 },
    { SC_LCD_DATA, 0x02, 0 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_CMD, 0xE1, 16 },
    { SC_LCD_DATA, 0x03, 0 },
    { SC_LCD_DATA, 0x1B, 0 },
    { SC_LCD_DATA, 0x12, 0 },
    { SC_LCD_DATA, 0x11, 0 },
    { SC_LCD_DATA, 0x32, 0 },
    { SC_LCD_DATA, 0x2F, 0 },
    { SC_LCD_DATA, 0x2A, 0 },
    { SC_LCD_DATA, 0x2F, 0 },
    { SC_LCD_DATA, 0x2E, 0 },
    { SC_LCD_DATA, 0x2C, 0 },
    { SC_LCD_DATA, 0x35, 0 },
    { SC_LCD_DATA, 0x3F, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_DATA, 0x01, 0 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_CMD, 0xFC, 1 },
    { SC_LCD_DATA, 0x8C, 0 },
    { SC_LCD_CMD, 0x3A, 1 },
    { SC_LCD_DATA, 0x05, 0 },   /* Format is RGB565, if for RGB666, 0x05 -> 0x06 */
    { SC_LCD_CMD, 0x36, 1 },
    { SC_LCD_DATA, 0xC0, 0 },   //C0  //A0
    { SC_LCD_CMD, 0x29, 0 },
    { SC_LCD_DELAY, 100, 0 },
#else
    { SC_LCD_CMD, 0x11, 0 },
    { SC_LCD_DELAY, 120, 0 },
    { SC_LCD_CMD, 0xB1, 3 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_CMD, 0xB2, 3 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_CMD, 0xB3, 6 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_CMD, 0xB4, 1 },
    { SC_LCD_DATA, 0x03, 0 },
    { SC_LCD_CMD, 0xC0, 3 },
    { SC_LCD_DATA, 0x62, 0 },
    { SC_LCD_DATA, 0x02, 0 },
    { SC_LCD_DATA, 0x04, 0 },
    { SC_LCD_CMD, 0xC1, 1 },
    { SC_LCD_DATA, 0xC0, 0 },
    { SC_LCD_CMD, 0xC2, 2 },
    { SC_LCD_DATA, 0x0C, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_CMD, 0xC3, 2 },
    { SC_LCD_DATA, 0x8C, 0 },
    { SC_LCD_DATA, 0x6A, 0 },
    { SC_LCD_CMD, 0xC4, 2 },
    { SC_LCD_DATA, 0x8B, 0 },
    { SC_LCD_DATA, 0xEE, 0 },
    { SC_LCD_CMD, 0xC5, 1 },
    { SC_LCD_DATA, 0x12, 0 },
    { SC_LCD_CMD, 0xE0, 16 },
    { SC_LCD_DATA, 0x03, 0 },
    { SC_LCD_DATA, 0x1B, 0 },
    { SC_LCD_DATA, 0x12, 0 },
    { SC_LCD_DATA, 0x11, 0 },
    { SC_LCD_DATA, 0x3F, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x32, 0 },
    { SC_LCD_DATA, 0x34, 0 },
    { SC_LCD_DATA, 0x2F, 0 },
    { SC_LCD_DATA, 0x2B, 0 },
    { SC_LCD_DATA, 0x30, 0 },
    { SC_LCD_DATA, 0x3A, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_DATA, 0x01, 0 },
    { SC_LCD_DATA, 0x02, 0 },
    { SC_LCD_DATA, 0x05, 0 },
    { SC_LCD_CMD, 0xE1, 16 },
    { SC_LCD_DATA, 0x03, 0 },
    { SC_LCD_DATA, 0x1B, 0 },
    { SC_LCD_DATA, 0x12, 0 },
    { SC_LCD_DATA, 0x11, 0 },
    { SC_LCD_DATA, 0x32, 0 },
    { SC_LCD_DATA, 0x2F, 0 },
    { SC_LCD_DATA, 0x2A, 0 },
    { SC_LCD_DATA, 0x2F, 0 },
    { SC_LCD_DATA, 0x2E, 0 },
    { SC_LCD_DATA, 0x2C, 0 },
    { SC_LCD_DATA, 0x35, 0 },
    { SC_LCD_DATA, 0x3F, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_DATA, 0x00, 0 },
    { SC_LCD_DATA, 0x01, 0 },
    { SC_LCD_DATA, 0x05, 0 },
    //{ SC_LCD_CMD, 0xFC, 1 },
    //{ SC_LCD_DATA, 0x8C, 0 },
    { SC_LCD_CMD, 0x3A, 1 },
    { SC_LCD_DATA, 0x05, 0 },  /* Format is RGB565, if for RGB666, 0x05 -> 0x06 */
    { SC_LCD_CMD, 0x36, 1 },
    { SC_LCD_DATA, 0x60, 0 }, //C0	//A0
    { SC_LCD_CMD, 0x29, 0 },
    { SC_LCD_DELAY, 120, 0 },
    { SC_LCD_CMD, 0x2C, 0 },
    { SC_LCD_DELAY, 120, 0 },
#endif
};

static sc_lcd_data_t st7735s_spi_data = {
	.init_para = st7735s_init_data,
	.init_para_len = sizeof(st7735s_init_data)/sizeof(sc_spi_lcd_write_t),
	.width = 128,
#ifndef ST7735S_128_128
	.height = 160,
#else
	.height = 128,
#endif
	.te_signal = SC_LCD_CAP_NOTE,
	.spi_clk = SC_SPI_LCD_CLK_26M,
	.lcd_id = 0x7c89f0,
	.format = SC_SPI_FORMAT_RGB565,
	.sample_edge = SC_SPI_EDGE_RISING,
	.endian = SC_SPI_ENDIAN_MSB,
	.data_lane = 1,
	.line_num = 4,
};
#endif

#ifdef LCD_ST7789V
static sc_spi_lcd_write_t st7789v_init_data[] =
{
    { SC_LCD_CMD, 0x11, 0},
    { SC_LCD_DELAY, 120, 0},
    { SC_LCD_CMD, 0x36, 1},
    { SC_LCD_DATA, 0x00, 0},
    { SC_LCD_CMD, 0x3A, 1},
    { SC_LCD_DATA, 0x06, 0},
    { SC_LCD_CMD, 0x35, 1},
    { SC_LCD_DATA, 0x00, 0},
    { SC_LCD_CMD, 0xC7, 1},
    { SC_LCD_DATA, 0x00, 0},
    { SC_LCD_CMD, 0xCC, 1},
    { SC_LCD_DATA, 0x09, 0},
    { SC_LCD_CMD, 0xB2, 5},
    { SC_LCD_DATA, 0x0C, 0},
    { SC_LCD_DATA, 0x0C, 0},
    { SC_LCD_DATA, 0x00, 0},
    { SC_LCD_DATA, 0x33, 0},
    { SC_LCD_DATA, 0x33, 0},
    { SC_LCD_CMD, 0x3A, 1},
    { SC_LCD_DATA, 0x05, 0}, //format is RGB565
    { SC_LCD_CMD, 0x36, 1},
    { SC_LCD_DATA, 0x00, 0},
    { SC_LCD_CMD, 0xB7, 1},
    { SC_LCD_DATA, 0x35, 0},
    { SC_LCD_CMD, 0xBB, 1},
    { SC_LCD_DATA, 0x36, 0},
    { SC_LCD_CMD, 0xC0, 1},
    { SC_LCD_DATA, 0x2C, 0},
    { SC_LCD_CMD, 0xC2, 1},
    { SC_LCD_DATA, 0x01, 0},
    { SC_LCD_CMD, 0xC3, 1},
    { SC_LCD_DATA, 0x0D, 0},
    { SC_LCD_CMD, 0xC4, 1},
    { SC_LCD_DATA, 0x20, 0},
    { SC_LCD_CMD, 0xC6, 1},
    { SC_LCD_DATA, 0x15, 0},
    { SC_LCD_CMD, 0xD0, 2},
    { SC_LCD_DATA, 0xA4, 0},
    { SC_LCD_DATA, 0xA1, 0},
    { SC_LCD_CMD, 0xE0, 14},
    { SC_LCD_DATA, 0xD0, 0},
    { SC_LCD_DATA, 0x17, 0},
    { SC_LCD_DATA, 0x19, 0},
    { SC_LCD_DATA, 0x04, 0},
    { SC_LCD_DATA, 0x03, 0},
    { SC_LCD_DATA, 0x04, 0},
    { SC_LCD_DATA, 0x32, 0},
    { SC_LCD_DATA, 0x41, 0},
    { SC_LCD_DATA, 0x43, 0},
    { SC_LCD_DATA, 0x09, 0},
    { SC_LCD_DATA, 0x14, 0},
    { SC_LCD_DATA, 0x12, 0},
    { SC_LCD_DATA, 0x33, 0},
    { SC_LCD_DATA, 0x2C, 0},
    { SC_LCD_CMD, 0xE1, 14},
    { SC_LCD_DATA, 0xD0, 0},
    { SC_LCD_DATA, 0x18, 0},
    { SC_LCD_DATA, 0x17, 0},
    { SC_LCD_DATA, 0x04, 0},
    { SC_LCD_DATA, 0x03, 0},
    { SC_LCD_DATA, 0x04, 0},
    { SC_LCD_DATA, 0x31, 0},
    { SC_LCD_DATA, 0x46, 0},
    { SC_LCD_DATA, 0x43, 0},
    { SC_LCD_DATA, 0x09, 0},
    { SC_LCD_DATA, 0x14, 0},
    { SC_LCD_DATA, 0x13, 0},
    { SC_LCD_DATA, 0x31, 0},
    { SC_LCD_DATA, 0x2D, 0},
    { SC_LCD_CMD, 0x29, 0 },
    { SC_LCD_DELAY, 100, 0 },
};

static sc_lcd_data_t st7789v_spi_data = {
	.init_para = st7789v_init_data,
	.init_para_len = sizeof(st7789v_init_data)/sizeof(sc_spi_lcd_write_t),
	.width = 240,
	.height = 320,
	.te_signal = SC_LCD_CAP_NOTE,
	.spi_clk = SC_SPI_LCD_CLK_26M,
	.lcd_id = 0x858552,
	.format = SC_SPI_FORMAT_RGB565,
	.sample_edge = SC_SPI_EDGE_RISING,
	.endian = SC_SPI_ENDIAN_MSB,
	.data_lane = 1,
	.line_num = 4,
};
#endif

/**
  * @brief  Save picture to file system
  * @param  buf-- image buffer from camera
  * @note   
  * @retval void
  */
void save_picture(void *buf)
{
	int ret = 0;
	SCFILE *file_hdl = NULL;
	UINT64 buff_data_len = 0;
	UINT64 actul_write_len = 0;
	char file_name[32] = {0};
	memcpy(file_name, "c:/image.nv12", strlen("c:/image.nv12"));
	file_hdl = sAPI_fopen(file_name, "wb");

	if(file_hdl == NULL)
		sAPI_Debug("CAM file open err");

	buff_data_len = CAM_WIDTH*CAM_HEIGHT*3/2;
	actul_write_len = sAPI_fwrite(buf, buff_data_len,1,file_hdl);

	if(actul_write_len != buff_data_len)
		sAPI_Debug("cam image data write err,write length: %d\r\n", actul_write_len);

	sAPI_fclose(file_hdl);
}

#ifdef CAM_DECODE
static unsigned char *cam_decode_heap = NULL;
static unsigned char decode_init_flag = 0;

/**
  * @brief  camera decode image
  * @param  buf-- image buffer from camera
  * @note   
  * @retval void
  */
void cam_decode(void *buf)
{
	int ret = 0;
	char result[256];
	int len = sizeof(result);
	int lcd_w = 128;
	int lcd_h = 128;

	static UINT8 *lcd_buf_decode = NULL;

	if(!lcd_buf_decode)
		lcd_buf_decode = sAPI_Malloc(lcd_w*lcd_h*2);

	ret = tiny_scanner_set_preview(lcd_buf_decode, CAM_WIDTH, CAM_HEIGHT, &lcd_w, &lcd_h);
	if(0 == ret)
		PrintfResp("\r\nCAM decode preview init ok...\r\n");
	else
		PrintfResp("\r\nCAM decode preview init fail...\r\n");

	ret = tiny_scanner_read(buf, CAM_WIDTH, CAM_HEIGHT, result, &len);
	if(ret>=0)
	{
		PrintfResp("\r\nCAM decode ok\r\n");
		PrintfResp(result);
		sAPI_Debug("\r\nCAM decode ok,and type = %d\r\n",ret);
		sAPI_Debug("sc","data:%s\r\n",result);
	}
	else
		PrintfResp("\r\nCAM decode fail\r\n");

	if(lcd_buf_decode)
	{
		sAPI_Free(lcd_buf_decode);
		lcd_buf_decode = NULL;
	}
}

/**
  * @brief  camera decode to LCD image
  * @param  buf-- image buffer from camera
  * @note   
  * @retval void
  */
void cam_decode_with_lcd(void *buf)
{
	int ret = 0;
	char result[256];
	int len = sizeof(result);
	int lcd_w = LCD_WIDTH;
	int lcd_h = LCD_HEIGHT;

	static UINT8 *lcd_buf_decode = NULL;

	if(!lcd_buf_decode)
		lcd_buf_decode = sAPI_Malloc(lcd_w*lcd_h*2);
	if(sAPI_YUVtoRGBZoomOut((UINT16 *)lcd_buf_decode,LCD_WIDTH,LCD_HEIGHT,buf,CAM_WIDTH,CAM_HEIGHT) < 0)
	{
		if(lcd_buf_decode)
		{
			sAPI_Free(lcd_buf_decode);
			lcd_buf_decode = NULL;
		}
		return;
	}
	sAPI_LcdWrite((UINT8 *)lcd_buf_decode, 0, 0, lcd_w,lcd_h);

	ret = tiny_scanner_set_preview(lcd_buf_decode, CAM_WIDTH, CAM_HEIGHT, &lcd_w, &lcd_h);
	if(0 == ret)
		PrintfResp("\r\nCAM decode preview init ok...\r\n");
	else
		PrintfResp("\r\nCAM decode preview init fail...\r\n");

	ret = tiny_scanner_read(buf, CAM_WIDTH, CAM_HEIGHT, result, &len);
	if(ret>=0)
	{
		PrintfResp("\r\nCAM decode ok\r\n");
		PrintfResp(result);
		sAPI_Debug("\r\nCAM decode ok,and type = %d\r\n",ret);
		sAPI_Debug("sc===data:%s\r\n",result);
	}
	else
	{
		PrintfResp("\r\nCAM decode fail\r\n");
		sAPI_Debug("\r\nCAM decode fail,and type = %d\r\n",ret);
	}
	if(lcd_buf_decode)
	{
		sAPI_Free(lcd_buf_decode);
		lcd_buf_decode = NULL;
	}
}

#endif

/**
  * @brief  camera demo operation
  * @param  void
  * @note   
  * @retval void
  */
void CamDemo(void)
{
	SIM_MSG_T optionMsg ={0,0,0,NULL};
	UINT32 opt = 0;
	UINT64 buff_data_len = 0;
	UINT32 actul_read_len = 0;
	void *cam_buf = NULL;
	UINT16 *lcd_buf = NULL;
	SCFILE *file_hdl = NULL;
	char file_name[32] = {0};
	int ret = 0;
#if defined(CAM_GC032A)
	int set_para[9][2] = {{1,5},{1,9},{1,0},{4,5},{4,8},{5,0},{5,1},{5,2},{5,3}};
#elif defined(CAM_GC6153)
	int set_para[9][2] = {{1,5},{1,9},{1,0},{4,4},{4,5},{5,0},{5,1},{5,2},{5,3}};
#elif defined(CAM_BF30A2)
	int set_para[9][2] = {{1,5},{1,9},{1,0},{4,1},{4,5},{5,0},{5,1},{5,2},{5,3}};
#endif
	INT8 *note = "\r\nPlease select an option to test from the items listed below, demo just for CAM.\r\n";
	INT8 *options_list[] = {
		"1. CAM take pictures",
		"2. CAM show images on LCD",
		"3. CAM show the pictures",
		#ifdef CAM_DECODE
		"4. CAM decode no lcd",
		"5. CAM decode and show LCD",
		#endif
		"6. CAM set paras",
		"7. CAM set image size",
		"8. CAM set mirror",
		"99. back",
	};

	memcpy(file_name, "c:/image.nv12", strlen("c:/image.nv12"));
	while(1)
	{
		PrintfResp(note);
		PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
reSelect:
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
			case SC_CAM_DEMO_CAM_TAKE_PICTURES:
			{
				if(SRV_UART != optionMsg.msg_id)
				{
					sAPI_Debug("%s,msg_id is error!!",__func__);
					break;
				}
				sAPI_CamOpen();
				ret = sAPI_GetCamBuf(0,save_picture);
				if(ret != 1)
				{
					sAPI_Debug("CAM get buf err");
					PrintfResp("\r\nCAM get buf err...\r\n");
					sAPI_CamClose();
					break;
				}
				sAPI_CamClose();
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM take pictures and save ok...\r\n");
				goto reSelect;
				break;
			}
			case SC_CAM_DEMO_CAM_SHOW_LCD:
			{
				if(SRV_UART != optionMsg.msg_id)
				{
					sAPI_Debug("%s,msg_id is error!!",__func__);
					break;
				}
				sAPI_CamOpen();
				#ifdef LCD_ST7735S
				sAPI_LcdOpen(&st7735s_spi_data);
				#endif
				#ifdef LCD_ST7789V
				sAPI_LcdOpen(&st7789v_spi_data);
				#endif
				sAPI_LcdSetBLPWM(5);
				sAPI_StartShowLCD();
				sAPI_TaskSleep(2000);
				sAPI_StopShowLCD();
				sAPI_CamClose();
				sAPI_LcdSetBLPWM(0);
				sAPI_LcdClose();
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM show images on lcd...\r\n");
				goto reSelect;
				break;
			}
			case SC_CAM_DEMO_CAM_SHOW_PICTURES:
			{
				if(SRV_UART != optionMsg.msg_id)
				{
					sAPI_Debug("%s,msg_id is error!!",__func__);
					break;
				}
				//do read images data form file system and show the pic on LCD;
				buff_data_len = 0;
				file_hdl = sAPI_fopen(file_name, "rb");
				if(file_hdl == NULL)
				{
					sAPI_Debug("CAM open image data err");
					PrintfResp("\r\nCAM open image data err...\r\n");
					break;
				}
				buff_data_len = sAPI_fsize(file_hdl);
				sAPI_Debug("sAPI_fsize buff_data_len: %d",buff_data_len);
				cam_buf = sAPI_Malloc(CAM_WIDTH*CAM_HEIGHT*3/2);
				if(!cam_buf)
					sAPI_Debug("cam buf malloc err");
				memset(cam_buf, 0, CAM_WIDTH*CAM_HEIGHT*3/2);
				actul_read_len = sAPI_fread(cam_buf,buff_data_len, 1, file_hdl);
				if(actul_read_len <= 0)
				{
					sAPI_Debug("CAM image data read err");
					PrintfResp("\r\nCAM image data read err...\r\n");
					sAPI_Free(cam_buf);
					sAPI_fclose(file_hdl);
					break;
				}
				lcd_buf = sAPI_Malloc(LCD_WIDTH*LCD_HEIGHT*2);
				if(!lcd_buf)
					sAPI_Debug("lcd buf malloc err");
				sAPI_YUVtoRGBZoomOut(lcd_buf,LCD_WIDTH,LCD_HEIGHT,cam_buf,CAM_WIDTH,CAM_HEIGHT);
				#ifdef LCD_ST7735S
				sAPI_LcdOpen(&st7735s_spi_data);
				#endif
				#ifdef LCD_ST7789V
				sAPI_LcdOpen(&st7789v_spi_data);
				#endif
				sAPI_LcdSetBLPWM(5);
				sAPI_LcdWrite((UINT8 *)lcd_buf, 0, 0, LCD_WIDTH,LCD_HEIGHT);
				ret = sAPI_fclose(file_hdl);
				if(ret != 0)
				{
					sAPI_Debug("CAM close image data err");
					PrintfResp("\r\nCAM close image data err...\r\n");
					sAPI_Free(cam_buf);
					sAPI_Free(lcd_buf);
					break;
				}else{
					file_hdl = NULL;
				}
				sAPI_TaskSleep(1000);
				sAPI_LcdSetBLPWM(0);
				sAPI_Free(cam_buf);
				sAPI_Free(lcd_buf);
				sAPI_LcdClose();
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM show the pictures taken...\r\n");
				goto reSelect;
				break;
			}
			#ifdef CAM_DECODE
			case SC_CAM_DEMO_CAM_DECODE_NOLCD:
			{
				if(!cam_decode_heap)
					cam_decode_heap = sAPI_Malloc(120*1024);
				sAPI_CamOpen();
				if(!decode_init_flag)
				{
					ret = tiny_scanner_init(&sAPI_Printf, cam_decode_heap, 120*1024);
					if(0 == ret)
					{
						decode_init_flag = 1;
						PrintfResp("\r\nCAM decode init ok...\r\n");
					}
					else
						PrintfResp("\r\nCAM decode init fail...\r\n");
				}
				sAPI_GetCamBuf(1,cam_decode);
				sAPI_TaskSleep(200);
				sAPI_CamClose();
				decode_init_flag = 0;
				if(cam_decode_heap)
				{
					sAPI_Free(cam_decode_heap);
					cam_decode_heap = NULL;
				}
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM decode done...\r\n");
				goto reSelect;
				break;
			}
			case SC_CAM_DEMO_CAM_DECODE_ONLCD:
			{
				#if 0 //def CAM_GC032A
				PrintfResp("\r\nCAM decode can not support,not enough ram\r\n");
				#else
				if(!cam_decode_heap)
					cam_decode_heap = sAPI_Malloc(120*1024);
				sAPI_CamOpen();
				if(!decode_init_flag)
				{
					ret = tiny_scanner_init(&sAPI_Printf, cam_decode_heap, 120*1024);
					if(0 == ret)
					{
						decode_init_flag = 1;
						PrintfResp("\r\nCAM decode init ok...\r\n");
					}
					else
						PrintfResp("\r\nCAM decode init fail...\r\n");
				}
				#ifdef LCD_ST7735S
				sAPI_LcdOpen(&st7735s_spi_data);
				#endif
				#ifdef LCD_ST7789V
				sAPI_LcdOpen(&st7789v_spi_data);
				#endif
				sAPI_LcdSetBLPWM(5);
				sAPI_GetCamBuf(1,cam_decode_with_lcd);
				sAPI_TaskSleep(1000);
				sAPI_LcdSetBLPWM(0);
				sAPI_CamClose();
				sAPI_LcdClose();
				decode_init_flag = 0;
				if(cam_decode_heap)
				{
					sAPI_Free(cam_decode_heap);
					cam_decode_heap = NULL;
				}
				#endif
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM decode done...\r\n");
				goto reSelect;
				break;
			}
			#endif
			case SC_CAM_DEMO_CAM_SET_PARA:
			{
				#ifdef LCD_ST7735S
				sAPI_LcdOpen(&st7735s_spi_data);
				#endif
				#ifdef LCD_ST7789V
				sAPI_LcdOpen(&st7789v_spi_data);
				#endif
				sAPI_LcdSetBLPWM(5);
				#ifdef USER_CAM_DRIVER
				sAPI_CamOpenEx(&CAM_GC032a_spi);
				#else
				sAPI_CamOpen();
				#endif
				sAPI_StartShowLCD();
				for(int i = 0;i < 3;i++)
				{
					sAPI_CamParaSet(set_para[i][0],&set_para[i][1]);
					sAPI_TaskSleep(1000);
				}
				sAPI_StopShowLCD();
				sAPI_CamClose();
				sAPI_LcdSetBLPWM(0);
				sAPI_LcdClose();
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM set para demo...\r\n");
				goto reSelect;
				break;
			}
			case SC_CAM_DEMO_CAM_SET_IMAGESIZE:
			{
				for(int i = 3;i < 5;i++)
				{
					#ifdef LCD_ST7735S
					sAPI_LcdOpen(&st7735s_spi_data);
					#endif
					#ifdef LCD_ST7789V
					sAPI_LcdOpen(&st7789v_spi_data);
					#endif
					sAPI_LcdSetBLPWM(5);
					sAPI_CamParaSet(set_para[i][0],&set_para[i][1]);
					#ifdef USER_CAM_DRIVER
					sAPI_CamOpenEx(&CAM_GC032a_spi);
					#else
					sAPI_CamOpen();
					#endif
					sAPI_StartShowLCD();
					sAPI_TaskSleep(1000);
					sAPI_StopShowLCD();
					sAPI_CamClose();
					sAPI_LcdSetBLPWM(0);
					sAPI_LcdClose();
				}
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM set image size demo...\r\n");
				goto reSelect;
				break;
			}
			case SC_CAM_DEMO_CAM_SET_MIRROR:
			{
				#ifdef LCD_ST7735S
				sAPI_LcdOpen(&st7735s_spi_data);
				#endif
				#ifdef LCD_ST7789V
				sAPI_LcdOpen(&st7789v_spi_data);
				#endif
				sAPI_LcdSetBLPWM(5);
				#ifdef USER_CAM_DRIVER
				sAPI_CamOpenEx(&CAM_GC032a_spi);
				#else
				sAPI_CamOpen();
				#endif
				sAPI_StartShowLCD();
				for(int i = 5;i < 9;i++)
				{
					sAPI_CamParaSet(set_para[i][0],&set_para[i][1]);
					sAPI_TaskSleep(1000);
				}
				sAPI_StopShowLCD();
				sAPI_CamClose();
				sAPI_LcdSetBLPWM(0);
				sAPI_LcdClose();
				PrintfResp(note);
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				PrintfResp("\r\nCAM set mirror demo...\r\n");
				goto reSelect;
				break;
			}
			case SC_CAM_DEMO_CAM_MAX:
			{
				sAPI_Debug("Return to the previous menu!");
				PrintfResp("\r\nReturn to the previous menu!\r\n");
				return;
			}
			default:
				PrintfResp("\r\nPlease select again:\r\n");
				PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
				goto reSelect;
				break;
		}
	}
}

/*******************************************************************************************************
int sAPI_CamOpen(void);
DES: open camera              RETURN: 0:OK other:FAIL        PARA: NO

int sAPI_CamClose(void);
DES: close camera             RETURN: 0:OK other:FAIL        PARA: NO

int sAPI_StartShowLCD(void);
DES: show image data on lcd   RETURN: 0:OK other:FAIL        PARA: NO

int sAPI_StopShowLCD(void);
DES: stop show image data     RETURN: 0:OK other:FAIL        PARA: NO

int sAPI_GetCamBuf(int func,CamBufferHandle callbackfunc);
DES: get camera image data    RETURN: 1:OK 0:ERR             PARA1: 0:take picture 1:decode  PARA2:user image data handle

int sAPI_YUVtoRGB_ZoomOut(unsigned short *pRgbData,unsigned int rgbWidth,unsigned int rgbHeight,
                          char *pYuvData,unsigned int yuvWidth,unsigned int yuvHeight);
DES: YUV data to RBG565       RETURN: 0:OK -1:FAIL           PARA: ...

void sAPI_CamPinConfig(cam_user_pin_t *cam_pins);
DES: camera pinconfig         RETURN: none
PARA: typedef struct {
	unsigned char pin_pwdn;
	unsigned char pin_rst;
	unsigned char pin_iovdd;
	unsigned char pin_avdd;
}cam_user_pin_t;

void sAPI_CamPinConfigUninstall(void);
DES: camera pinconfig         RETURN: none                   PARA: NO

int sAPI_CamParaSet(unsigned char param_mode, void *pValue);
DES: set camera para          RETURN: 0:OK -1:FAIL
NOTE: just set the output image size of the camera before opening the camera
PARA1:
      =1:set camera effect
      PARA2:effect val
                0  HAL_EFFECT_NORMAL
                1  HAL_EFFECT_COLORINV
                2  HAL_EFFECT_BLACKBOARD
                3  HAL_EFFECT_WHITEBOARD
                4  HAL_EFFECT_ANTIQUE
                5  HAL_EFFECT_RED
                6  HAL_EFFECT_GREEN
                7  HAL_EFFECT_BLUE
                8  HAL_EFFECT_BLACKWHITE
                9  HAL_EFFECT_NEGATIVE
      =2:set camera contrast
      PARA2:contrastt val
                0  CAM_CONTRAST_HIGH
                1  CAM_CONTRAST_MEDIUM
                2  CAM_CONTRAST_LOW
      =3:set camera exposure
      PARA2:exposure val
                0  CAM_EV_NEG_4_3
                1  CAM_EV_NEG_3_3
                2  CAM_EV_NEG_2_3
                3  CAM_EV_NEG_1_3
                4  CAM_EV_ZERO
                5  CAM_EV_POS_1_3
                6  CAM_EV_POS_2_3
                7  CAM_EV_POS_3_3
                8  CAM_EV_POS_4_3
      =4:set camera output image size
      PARA2:image size val
              //0  {   96,   96  }, // 96x96
              //1  {  160,  120  }, // QQVGA
              //2  {  176,  144  }, // QCIF
              //3  {  240,  176  }, // HQVGA
              //4  {  240,  240  }, // 240x240
              //5  {  320,  240  }, // QVGA
              //6  {  400,  296  }, // CIF
              //7  {  480,  320  }, // HVGA
              //8  {  640,  480  }, // VGA
              //9  {  800,  600  }, // SVGA
              //10 { 1024,  768  }, // XGA
              //11 { 1280,  720  }, // HD
              //12 { 1280, 1024  }, // SXGA
              //13 { 1600, 1200  }, // UXGA
      =5:set camera mirror
      PARA2:mirror val
                0  HAL_MIRROR_H
                1  HAL_MIRROR_V
                2  HAL_MIRROR_HV
                3  HAL_MIRROR_NORMAL
*******************************************************************************************************/
