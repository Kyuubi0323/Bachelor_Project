
#-------------------------------------------------------------------------------
# Configure variable
#-------------------------------------------------------------------------------
TOP_DIR:=$(TOP_DIR)
ROOT_DIR:=$(TOP_DIR)/..
MAKEFILE_PATH :=$(subst $(strip \),/,$(abspath $(lastword $(MAKEFILE_LIST))))


#-------------------------------------------------------------------------------
# Configure source code files
#-------------------------------------------------------------------------------
SRC_FILES+= \
	demo_mqtt.c \
	demo_file_system.c \
	demo_ftps.c \
	demo_ftps_test.c \
	demo_uart.c \
	demo_ssl.c \
	demo_htp.c \
	demo_network.c \
	demo_ntp.c\
	demo_simcard.c \
	demo_sms.c \
	demo_tcpip.c \
	demo_https.c \
	cus_urc.c \
	cus_uart.c \
	cus_usb_vcom.c \
	cus_key.c \
	demo_gpio.c \
	demo_flash.c \
	demo_audio.c \
	demo_tts.c \
	demo_version.c \
	simcom_demo.c \
	demo_loc.c \
	demo_app_download.c\
	demo_encrypt.c \
	demo_cjson.c \
	demo_helloworld.c \
	demo_at.c \
	demo_i2c.c \
	demo_spi.c\
	demo_ping.c\
	demo_call.c\
	demo_wifi.c\
	demo_gps.c\
	demo_lcd.c\
	utils/core_auth.c\
	utils/core_sha256.c\
	utils/core_string.c\
	demo_fota.c \
	mqtt_OneNET.c \
	mqtt_tencent.c \
	token/onenet_base64.c \
	token/onenet_hmac_sha1.c \
	token/onenet_md5.c \
	token/token.c \
	demo_loc_test.c \
	demo_ssl_test.c \
	demo_pwm.c \
	demo_ble.c \
	demo_rtc.c  \
	demo_file_system_old.c \
	demo_pm.c \
	demo_cam.c \
	demo_le_client.c

ifneq (,$(findstring MMI,$(MODULE)))
  SRC_FILES += demo_cam_drive.c
endif
#-------------------------------------------------------------------------------
# the include directories
#-------------------------------------------------------------------------------
INC_DIRS+= \
  -I${TOP_DIR}/simcom_lib/inc \
  -I${TOP_DIR}/simcom_lib/inc/utils \
  -I${TOP_DIR}/third_lib/cjson \
  -I${TOP_DIR}/simcom_lib/inc/token
#-------------------------------------------------------------------------------
# Configure compile flag
#-------------------------------------------------------------------------------
U_FLAGS:=

#-------------------------------------------------------------------------------
# Configure default macro define
#-------------------------------------------------------------------------------
DFLAGS+=


#-------------------------------------------------------------------------------
# include mk file
#-------------------------------------------------------------------------------
include ${TOP_DIR}\script\package.mk
