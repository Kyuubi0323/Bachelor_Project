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
  services/log/log.c \
  app_main.c \
  drivers/system/system.c \
  drivers/i2c/swiic.c \
  drivers/io/gpio.c \
  drivers/flash/flash.c \
  drivers/fs/fs.c \
  drivers/system/system.c \
  drivers/adc/adc.c \
  services/utils/utils.c \
  services/syslog/syslog.c \
  services/syslog/gps_syslog_def.c \
  services/simcom/simcom.c \
  services/simcom/network.c \
  services/simcom/gps.c \
  services/simcom/simcard.c \
  services/simcom/mqtt.c \
  services/fifo/fifo.c \
  services/modbus/lwmb.c \
  apps/app_uart.c \
  apps/app_simcom.c \
  apps/app_mqtt.c \
#-------------------------------------------------------------------------------
# the include directories
#-------------------------------------------------------------------------------
INC_DIRS+= \
  -I${TOP_DIR}/simcom_lib/inc \
  -I${TOP_DIR}/phase_detector_master \
  -I${TOP_DIR}/third_lib/cjson \
  -I${TOP_DIR}/phase_detector_master/drivers/i2c \
  -I${TOP_DIR}/phase_detector_master/drivers/system \
  -I${TOP_DIR}/phase_detector_master/drivers/io \
  -I${TOP_DIR}/phase_detector_master/drivers/flash \
  -I${TOP_DIR}/phase_detector_master/drivers/fs \
  -I${TOP_DIR}/phase_detector_master/drivers/adc \
  -I${TOP_DIR}/phase_detector_master/services/simcom \
  -I${TOP_DIR}/phase_detector_master/services/utils \
  -I${TOP_DIR}/phase_detector_master/services/log \
  -I${TOP_DIR}/phase_detector_master/services/syslog \
  -I${TOP_DIR}/phase_detector_master/services/fifo \
  -I${TOP_DIR}/phase_detector_master/services/modbus\
#-------------------------------------------------------------------------------
# Configure compile flag
#-------------------------------------------------------------------------------
U_FLAGS:= -D_DEBUG

#-------------------------------------------------------------------------------
# Configure default macro define
#-------------------------------------------------------------------------------
DFLAGS+=


#-------------------------------------------------------------------------------
# include mk file
#-------------------------------------------------------------------------------
include ${TOP_DIR}\script\package.mk
