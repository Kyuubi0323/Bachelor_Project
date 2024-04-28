/**
  ******************************************************************************
  * @file    demo_lcd.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of lcd configuration operation.
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
#include "demo_lcd.h"

typedef enum{
	SC_LCD_DEMO_LCD_OPEN				= 1,
	SC_LCD_DEMO_LCD_CLOSE				= 2,
	SC_LCD_DEMO_LCD_SHOW_FONT			= 3,
	SC_LCD_DEMO_LCD_CLEAR_SCREEN		= 4,
	SC_LCD_DEMO_LCD_SET_BRIGHTNESS		= 5,
	SC_LCD_DEMO_LCD_MAX					= 99
}SC_LCD_DEMO_TYPE;

extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);
extern SIM_MSG_T GetParamFromUart(void);

UINT16 lcd_width = 0,lcd_height = 0;
char lcd_type,lcd_bri;

static sc_lcd_data_t lcd_st7735s_spi_data_128_128 = {
	.init_para = lcd_st7735s_spi_init_data_128_128,
	.init_para_len = sizeof(lcd_st7735s_spi_init_data_128_128)/sizeof(sc_spi_lcd_write_t),
	.width = 128,
	.height = 128,
	.te_signal = SC_LCD_CAP_NOTE,
	.spi_clk = SC_SPI_LCD_CLK_26M,
	.lcd_id = 0x7c89f0,
	.format = SC_SPI_FORMAT_RGB565,
	.sample_edge = SC_SPI_EDGE_RISING,
	.endian = SC_SPI_ENDIAN_MSB,
	.data_lane = 1,
	.line_num = 4,
};

static sc_lcd_data_t lcd_st7735s_spi_data_128_160 = {
	.init_para = lcd_st7735s_spi_init_data_128_160,
	.init_para_len = sizeof(lcd_st7735s_spi_init_data_128_160)/sizeof(sc_spi_lcd_write_t),
	.width = 128,
	.height = 160,
	.te_signal = SC_LCD_CAP_NOTE,
	.spi_clk = SC_SPI_LCD_CLK_26M,
	.lcd_id = 0x7c89f0,
	.format = SC_SPI_FORMAT_RGB565,
	.sample_edge = SC_SPI_EDGE_RISING,
	.endian = SC_SPI_ENDIAN_MSB,
	.data_lane = 1,
	.line_num = 4,
};

static sc_lcd_data_t lcd_st7789v_spi_data_240_240 = {
	.init_para = lcd_st7789v_spi_init_data_240_240,
	.init_para_len = sizeof(lcd_st7789v_spi_init_data_240_240)/sizeof(sc_spi_lcd_write_t),
	.width = 240,
	.height = 320,
	.te_signal = SC_LCD_CAP_NORMAL,
	.spi_clk = SC_SPI_LCD_CLK_26M,
	.lcd_id = 0x858552,
	.format = SC_SPI_FORMAT_RGB565,
	.sample_edge = SC_SPI_EDGE_RISING,
	.endian = SC_SPI_ENDIAN_MSB,
	.data_lane = 1,
	.line_num = 4,
};

static sc_lcd_data_t lcd_st7789v_spi_data_240_320 = {
	.init_para = lcd_st7789v_spi_init_data_240_320,
	.init_para_len = sizeof(lcd_st7789v_spi_init_data_240_320)/sizeof(sc_spi_lcd_write_t),
	.width = 240,
	.height = 320,
	.te_signal = SC_LCD_CAP_NORMAL,
	.spi_clk = SC_SPI_LCD_CLK_26M,
	.lcd_id = 0x858552,
	.format = SC_SPI_FORMAT_RGB565,
	.sample_edge = SC_SPI_EDGE_RISING,
	.endian = SC_SPI_ENDIAN_MSB,
	.data_lane = 1,
	.line_num = 4,
};

static sc_lcd_data_t lcd_st7567a_spi_data = {
	.init_para = lcd_st7567a_spi_init_data,
	.init_para_len = sizeof(lcd_st7567a_spi_init_data)/sizeof(sc_spi_lcd_write_t),
	.width = 128,
	.height = 64,
	.te_signal = SC_LCD_CAP_NORMAL,
	.spi_clk = SC_SPI_LCD_CLK_26M,
	.lcd_id = 0x0,
	.format = SC_SPI_FORMAT_RGB565,
	.sample_edge = SC_SPI_EDGE_RISING,
	.endian = SC_SPI_ENDIAN_MSB,
	.data_lane = 1,
	.line_num = 4,
};

/**
  * @brief  LCD show 16x16 character
  * @param  x--x begin position
  * @param  y--y begin position
  * @param  fc--font color
  * @param  bc--background color
  * @param  s--string pointer
  * @note   This demo will show how to full fill fxixed color to whole screen
  * @retval void
  */
void show_font_16x16(UINT16 x, UINT16 y, UINT16 fc, UINT16 bc, UINT8 *s)
{
	UINT8 len = strlen(s);
	UINT16 *pBuf = NULL;
	if (pBuf == NULL)
	{
		pBuf = (UINT16 *)sAPI_Malloc(8*16*2*len);
		if (pBuf == NULL)
		{
			sAPI_Debug("wrong malloc");
			return;
		}
	}
	UINT8 Xb = x;
	UINT8 Yb = y;
	UINT8 Lenb = len;
	UINT8 i,j;
	UINT16 k,x0;
	x0=x;
	while(len)
	{
		while(*s)
		{
			if((*s) < 128)
			{
				k=*s;
				if (k==13)
				{
					x=x0;
					y+=16;
				}
				else
				{
					if(k>32)
						k-=32;
					else
						k=0;
					for(i=0;i<16;i++)
					{
						for(j=0;j<8;j++)
						{
							if(ASCII_8X16[k*16+i]&(0x80>>j))
								pBuf[(x+j-Xb)+(y+i-Yb)*8*Lenb]=fc;
							else
								pBuf[(x+j-Xb)+(y+i-Yb)*8*Lenb]=bc;
						}
					}
					x+=8;
				}
				s++;
				len-=1;
			}
			else
			{
				for(k=0;k<GB_16_num;k++)
				{
					if((GB_16[k].Index[0]==*(s))&&(GB_16[k].Index[1]==*(s+1)))
					{
						for(i=0;i<16;i++)
						{
							for(j=0;j<8;j++)
							{
								if(GB_16[k].Msk[i*2]&(0x80>>j))
									pBuf[(x+j-Xb)+(y+i-Yb)*8*Lenb]=fc;
								else
									if((bc!=0)&&(fc!=bc)) pBuf[(x+j-Xb)+(y+i-Yb)*8*Lenb]=bc;
							}
							for(j=0;j<8;j++)
							{
								if(GB_16[k].Msk[i*2+1]&(0x80>>j))
									pBuf[(x+j+8-Xb)+(y+i-Yb)*8*Lenb]=fc;
								else
									if ((bc!=0)&&(fc!=bc)) pBuf[(x+j+8-Xb)+(y+i-Yb)*8*Lenb]=bc;
							}
						}
					}
				}
				s+=2;
				x+=16;
				len-=2;
			}
		}
	}
	sAPI_LcdWrite(pBuf, Xb,Yb,Xb+(8*Lenb),Yb+16);
	sAPI_Free(pBuf);
}

/**
  * @brief  LCD clean screen to fixed colr
  * @param  bg_color--background color
  * @note   This demo will show how to full fill fxixed color to whole screen
  * @retval void
  */
static void lcd_clean_screen(unsigned short bg_color)
{
	UINT16 *buf = NULL;
	UINT32 i;
	if(5 == lcd_type)
	{
		UINT8 m,n;
		UINT8 ComTable[]={7,6,5,4,3,2,1,0};
		for(m = 0;m < 8;m++)
		{
			sAPI_LcdWriteCmd(0xB0|ComTable[m]);
			sAPI_LcdWriteCmd(0x10);
			sAPI_LcdWriteCmd(0x01);
			for(n = 0;n < 128;n++)
			sAPI_LcdWriteData(&bg_color,1);
		}
	}
	else
	{
		if(0 == lcd_width || 0 == lcd_height)
		{
			sAPI_Debug("%s,lcd width or lcd height err!",__func__);
			return;
		}
		buf = sAPI_Malloc(lcd_width * lcd_height * 2);
		for(i = 0; i < lcd_width * lcd_height; i++)
			buf[i] = bg_color;
		sAPI_LcdClearScreen(buf);
		sAPI_TaskSleep(30);
		sAPI_Free(buf);
	}
}

/**
  * @brief  LCD show font
  * @param  void
  * @note   This demo will show TN_GB_16 font
  * @retval void
  */
void tn_lcd_show_font(void)
{
	UINT8 page, i;
	const UINT8 *dat = 0;
	UINT8 s_col = 0;

	for(i=0; i < sizeof(TN_GB_16)/sizeof(typFNT_GB16_t); i++)
	{
		dat = TN_GB_16[i].Msk;
		s_col = (i*8);
		for(page = 1; page < 3; page++)
		{
			sAPI_LcdWriteCmd(0xb0 + page);
			sAPI_LcdWriteCmd(0x10|((s_col>>4)&0x0f));
			sAPI_LcdWriteCmd(0x00|(s_col&0x0f));
			sAPI_LcdWriteData(dat+(page - 1)*16, 16);
		}
	}
}

/**
  * @brief  LCD Demo
  * @param  void
  * @note   This demo will show how to operate LCD panal.
  * @retval void
  */
void LcdDemo(void)
{
	SIM_MSG_T optionMsg ={0,0,0,NULL};
	UINT32 opt = 0;
	UINT8 *str1="��ӭʹ��";
	UINT8 *str2="SIMCom";
	UINT16 x_pos,y_pos;
	INT8 *note = "\r\nPlease select an option to test from the items listed below, demo just for LCD.\r\n";
	INT8 *options_list[] = {
		"1. LCD open",
		"2. LCD close",
		"3. LCD show font",
		"4. LCD clear screen",
		"5. LCD set brightness",
		"99. back",
	};

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
			case SC_LCD_DEMO_LCD_OPEN:
			{
				if(SRV_UART != optionMsg.msg_id)
				{
					sAPI_Debug("%s,msg_id is error!!",__func__);
					break;
				}
				PrintfResp("\r\nPlease input LCD TYPE num\r\n");
				PrintfResp(" 1:ST7735S(128*128)\r\n 2:ST7735S(128*160)\r\n 3:ST7789V(240*240)\r\n 4:ST7789V(240*320)\r\n 5:ST7567A(7565R)(128*64)\r\n");
				optionMsg = GetParamFromUart();
				strcpy(&lcd_type,optionMsg.arg3);
				sAPI_Free(optionMsg.arg3);
				lcd_type = atoi(&lcd_type);

				if(1 == lcd_type)
				{
					sAPI_LcdOpen(&lcd_st7735s_spi_data_128_128);
					lcd_width = 128;
					lcd_height = 128;
				}
				if(2 == lcd_type)
				{
					sAPI_LcdOpen(&lcd_st7735s_spi_data_128_160);
					lcd_width = 128;
					lcd_height = 160;
				}
				if(3 == lcd_type)
				{
					sAPI_LcdOpen(&lcd_st7789v_spi_data_240_240);
					lcd_width = 240;
					lcd_height = 240;
				}
				if(4 == lcd_type)
				{
					sAPI_LcdOpen(&lcd_st7789v_spi_data_240_320);
					lcd_width = 240;
					lcd_height = 320;
				}
				if(5 == lcd_type)
				{
					sAPI_LcdOpen(&lcd_st7567a_spi_data);
					lcd_width = 128;
					lcd_height = 64;
				}
				sAPI_LcdSetBLPWM(5);
				PrintfResp("\r\nsAPI_LcdOpen...\r\n");
				break;
			}
			case SC_LCD_DEMO_LCD_CLOSE:
			{
				sAPI_LcdSetBLPWM(0);
				sAPI_LcdClose();
				PrintfResp("\r\nsAPI_LcdClose...\r\n");
				break;
			}
			case SC_LCD_DEMO_LCD_SHOW_FONT:
			{
				if(5 == lcd_type)
					tn_lcd_show_font();
				else
				{
					//Chinese
					x_pos = (lcd_width - strlen(str1)*8)/2;
					y_pos = (lcd_height - 16)/2;
					show_font_16x16(x_pos,y_pos, BLACK, WHITE, str1);
					//Ascii
					x_pos = (lcd_width - strlen(str2)*8)/2;
					y_pos += 16;
					show_font_16x16(x_pos,y_pos, BLACK, WHITE, str2);
				}
				PrintfResp("\r\nsAPI_LcdShowFont...\r\n");
				break;
			}
			case SC_LCD_DEMO_LCD_CLEAR_SCREEN:
			{
				lcd_clean_screen(WHITE);
				PrintfResp("\r\nsAPI_LcdClearScreen...\r\n");
				break;
			}
			case SC_LCD_DEMO_LCD_SET_BRIGHTNESS:
			{
				PrintfResp("\r\nPlease input LCD BRIGHTNESS level(0-5)\r\n");
				optionMsg = GetParamFromUart();
				strcpy(&lcd_bri,optionMsg.arg3);
				sAPI_Free(optionMsg.arg3);
				lcd_bri = atoi(&lcd_bri);
				sAPI_LcdSetBLPWM((unsigned int)lcd_bri);
				PrintfResp("\r\nsAPI_LcdSetBrightness...\r\n");
				break;
			}
			case SC_LCD_DEMO_LCD_MAX:
			{
				sAPI_Debug("Return to the previous menu!");
				PrintfResp("\r\nReturn to the previous menu!\r\n");
				return;
			}
			default:
				PrintfResp("\r\nPlease select again:\r\n");
				break;
		}
	}
}
