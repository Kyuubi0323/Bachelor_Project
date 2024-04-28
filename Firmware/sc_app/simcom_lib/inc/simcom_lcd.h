/**
  ******************************************************************************
  * @file    simcom_lcd.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of LCD peripherals operation,usually for MMI version.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_LCD_H__
#define __SIMCOM_LCD_H__

//packet type
enum {
    SC_LCD_CMD = 0,
    SC_LCD_DATA,
    SC_LCD_DELAY
};

enum {
	SC_LCD_CAP_NORMAL = 0,
	SC_LCD_CAP_FAKE = 1,
	SC_LCD_CAP_POWERON = 2,
	SC_LCD_CAP_NOTE = 4, /*has no te signal*/
	SC_LCD_CAP_LIMIT
};

typedef enum SC_SPI_LCD_CLK_ENUM {
	SC_SPI_LCD_CLK_6_5M = 6500,
	SC_SPI_LCD_CLK_10M = 10400,
	SC_SPI_LCD_CLK_13M = 13000,
	SC_SPI_LCD_CLK_26M = 26000,
	SC_SPI_LCD_CLK_52M = 52000,
}SC_SPI_LCD_CLK_E;

enum{
	SC_SPI_FORMAT_RGB565 = 0,
	SC_SPI_FORMAT_RGB666 = 1,
	SC_SPI_FORMAT_RGB666_2_3 = 2,
	SC_SPI_FORMAT_RGB888 = 3,
	SC_SPI_FORMAT_RGB888_2_3 = 4,
	SC_SPI_FORMAT_LIMIT
};

enum{
	SC_SPI_EDGE_RISING = 0,
	SC_SPI_EDGE_FALLING = 1,
	SC_SPI_EDGE_LIMIT
};

enum{
	SC_SPI_ENDIAN_LSB = 0,
	SC_SPI_ENDIAN_MSB = 1,
	SC_SPI_ENDIAN_LIMIT
};

typedef struct sc_spi_lcd_write_struct {
    UINT8 type;
    UINT8 value;
    UINT8 data_length;
}sc_spi_lcd_write_t;

typedef struct {
    sc_spi_lcd_write_t *init_para;
    UINT16 init_para_len;
    UINT32 width;
    UINT32 height;
    UINT16 te_signal;
    SC_SPI_LCD_CLK_E spi_clk;
    UINT32 lcd_id;
    UINT16 format;
    UINT16 sample_edge;
    UINT16 endian;
    UINT16 data_lane;
    UINT16 line_num;
} sc_lcd_data_t;

#endif
