/**
  ******************************************************************************
  * @file    simcom_camera.h
  * @author  SIMCom OpenSDK Team
  * @brief   Header file of camera peripherals,usually for MMI module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __SIMCOM_CAM_H__
#define __SIMCOM_CAM_H__

typedef void (*CamBufferHandle)(void *);
typedef struct {
	unsigned char pin_pwdn;
	unsigned char pin_rst;
	unsigned char pin_iovdd;
	unsigned char pin_avdd;
}cam_user_pin_t;

#define SC_MEDIA_BUS_FMT_UYVY8_2X8			0x2006
#define SC_MEDIA_BUS_FMT_VYUY8_2X8			0x2007
#define SC_MEDIA_BUS_FMT_YUYV8_2X8			0x2008
#define SC_MEDIA_BUS_FMT_YVYU8_2X8			0x2009

//frame size
enum {
	FRAMESIZE_96X96,    // 96x96
	FRAMESIZE_QQVGA,    // 160x120
	FRAMESIZE_QCIF,     // 176x144
	FRAMESIZE_HQVGA,    // 240x176
	FRAMESIZE_240X240,  // 240x240
	FRAMESIZE_QVGA,     // 320x240
	FRAMESIZE_CIF,      // 400x296
	FRAMESIZE_HVGA,     // 480x320
	FRAMESIZE_VGA,      // 640x480
	FRAMESIZE_SVGA,     // 800x600
	FRAMESIZE_XGA,      // 1024x768
	FRAMESIZE_HD,       // 1280x720
	FRAMESIZE_SXGA,     // 1280x1024
	FRAMESIZE_UXGA,     // 1600x1200
	FRAMESIZE_INVALID
};

//effect setting
typedef enum {
	HAL_EFFECT_NONE = -1,
	HAL_EFFECT_NORMAL=0,
	HAL_EFFECT_GRAYSCALE,
	HAL_EFFECT_SEPIA,
	HAL_EFFECT_SEPIAGREEN,
	HAL_EFFECT_SEPIABLUE,
	HAL_EFFECT_COLORINV=5,
	HAL_EFFECT_GRAYINV,
	HAL_EFFECT_BLACKBOARD,
	HAL_EFFECT_WHITEBOARD,
	HAL_EFFECT_COPPERPLATE,
	HAL_EFFECT_EMBOSS=10,
	HAL_EFFECT_BLUECARVING,
	HAL_EFFECT_CONTRAST,
	HAL_EFFECT_JEAN,
	HAL_EFFECT_SKETCH,
	HAL_EFFECT_OIL=15,
	HAL_EFFECT_NO_OF_EFFECT=16,
	HAL_EFFECT_ANTIQUE,
	HAL_EFFECT_RED,
	HAL_EFFECT_GREEN,
	HAL_EFFECT_BLUE=20,
	HAL_EFFECT_BLACKWHITE,
	HAL_EFFECT_NEGATIVE,

} CAM_HAL_EFFECT;

//white balance
typedef enum {
	HAL_WB_AUTO = 0x0,
	HAL_WB_DAYLIGHT,
	HAL_WB_TUNGSTEN,
	HAL_WB_OFFICE,
	HAL_WB_CLOUDY,
	HAL_WB_INCANDESCENT,
} CAM_HAL_WB;

//contrast
typedef enum {
	CAM_CONTRAST_HIGH,
	CAM_CONTRAST_MEDIUM,
	CAM_CONTRAST_LOW,
} CAM_MINI_CONTRAST;

//EXPOSURE
typedef enum {
	CAM_EV_NEG_4_3,
	CAM_EV_NEG_3_3,
	CAM_EV_NEG_2_3,
	CAM_EV_NEG_1_3,
	CAM_EV_ZERO,
	CAM_EV_POS_1_3,
	CAM_EV_POS_2_3,
	CAM_EV_POS_3_3,
	CAM_EV_POS_4_3,
} CAM_HAL_EXPOSURE;

//SATURATION
typedef enum {
	HAL_SATURATION_n3,
	HAL_SATURATION_n2,
	HAL_SATURATION_n1,
	HAL_SATURATION_mid,
	HAL_SATURATION_p1,
	HAL_SATURATION_p2,
	HAL_SATURATION_p3,
} CAM_HAL_SATURATION;

//BRIGHTNESS
typedef enum {
	HAL_BRIGHTNESS_1 = 1,
	HAL_BRIGHTNESS_2,
	HAL_BRIGHTNESS_3,
	HAL_BRIGHTNESS_4,
	HAL_BRIGHTNESS_mid,
	HAL_BRIGHTNESS_6,
	HAL_BRIGHTNESS_7,
	HAL_BRIGHTNESS_8,
	HAL_BRIGHTNESS_9,
} CAM_HAL_BRIGHTNESS;

//FTM
typedef enum {
	HAL_MIRROR_H,
	HAL_MIRROR_V,
	HAL_MIRROR_HV,
	HAL_MIRROR_NORMAL,
} CAM_HAL_MIRROR;


enum cam_format{
	SC_SENSOR_FMT_RAW8 = 0,
	SC_SENSOR_FMT_RAW10,
	SC_SENSOR_FMT_RAW12,
	SC_SENSOR_FMT_YUV422,
};

//LDO
enum cam_ldo_chl{
	SC_CAM_AVDD = 0,
	SC_CAM_IOVDD,
	SC_CAM_DVDD,
};

//comunication interface
enum cam_sensor_interface{
	SC_CAM_INF_SPI = 0,
	SC_CAM_INF_MIPI = 1,
	SC_CAM_INF_PARALLEL = 2,
};

//interface lane
enum cam_sensor_interface_lane{
	SC_SPI_1_LAN = 0x0,
	SC_SPI_2_LAN = 0x1,
	SC_SPI_4_LAN = 0x2,
	SC_MIPI_1_LAN =  0x3,
	SC_PARA_8_LAN = 0x4,
};

//iic data length
enum cam_i2c_len {
	SC_I2C_8BIT = 0,
	SC_I2C_16BIT,
	SC_I2C_32BIT
};

enum cam_sensor_pos {
	SC_CAMERA_BACK = 0,
	SC_CAMERA_FRONT,
	SC_CAMERA_MAX,
};

enum cam_sensor_res_prop {
	SC_SENSOR_RES_BINING1,
	SC_SENSOR_RES_BINING2,
	SC_SENSOR_RES_CROPPED,
	SC_SENSOR_RES_MAX,
};

enum cam_band{
	SC_BANDING_50HZ = 0,
	SC_BANDING_60HZ,
	SC_BANDING_AUTO,
};

struct cam_regval_tab {
	unsigned short reg;
	unsigned short val;
	unsigned short mask;
};

struct cam_i2c_attr {
	enum cam_i2c_len reg_len;
	enum cam_i2c_len val_len;
	unsigned char addr; /* 7 bit i2c address*/
};

struct cam_sensor_regs {
	const struct cam_regval_tab *tab;
	unsigned int num;
};

struct cam_resolution {
	unsigned int sensor_out_format; // CZM
	unsigned short mbus_code;
	unsigned short width;
	unsigned short height;
	unsigned int hts;
	unsigned int min_vts;
	unsigned char max_fps;
	unsigned char min_fps;
	enum cam_sensor_res_prop prop;
	struct cam_sensor_regs regs;
};

struct cam_sensor_ata{
	unsigned int on;
	int read_crc_value;
};

struct cam_sensor_aec{
	unsigned int exp_high;
	unsigned int exp_low;
	unsigned int gain;
	unsigned int aec_en;
	unsigned int min_fps;
	unsigned int max_fps;
	enum cam_band banding;
	unsigned int sensor_out_yuv;
};

typedef struct {
	char * name;
	struct cam_sensor_spec_ops *ops;
	int position;
	struct cam_i2c_attr *i2c_attr;
	int num_i2c_attr;
	struct cam_sensor_regs id;
	struct cam_resolution *res;
	int num_res;
	struct cam_sensor_regs streamon;
	struct cam_sensor_regs streamoff;
	int interface_type;
	unsigned char lane_num;
	unsigned char spi_sdr;
	unsigned char spi_crc;
	int reset_delay;
	int cur_i2c_index;
	int cur_res_index;
}sc_cam_data_t;

typedef struct {
	sc_cam_data_t *sensor_data;
	void *ops;
	int sensor_init;
	int stream_cnt;
	unsigned char sensor_lock;/*lock interface.*/
	unsigned int ata_on;
}sc_cam_ctrl_t;

typedef struct {
	unsigned char cam_bri;
	unsigned char cam_effect;
	unsigned char cam_contrast;
	unsigned char cam_exposure;
	unsigned char cam_mirror;
	unsigned char cam_image_size;
}sc_cam_global_config_t;

struct cam_sensor_spec_ops {
	int (*get_pixel_rate)(void);//not support,Just for alignment
	int (*get_dphy_desc)(void); //not support,Just for alignment
	int (*update_otp)(void);    //not support,Just for alignment
	int (*s_power)(int onoff);
	int (*ata_open)(struct cam_sensor_ata *ata);
	int (*update_aec)(struct cam_sensor_aec *aec);
	int (*set_effect)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_brightness)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_contrast)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_wb)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_saturation)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_banding)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_exposure)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_framesize)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
	int (*set_mirror)(sc_cam_ctrl_t *sensor_ctrl, unsigned short level);
};

typedef struct {
	unsigned short cam_width;
	unsigned short cam_height;
} framesize_info_t;
#endif

