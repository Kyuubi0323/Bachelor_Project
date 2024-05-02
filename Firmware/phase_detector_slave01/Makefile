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
  apps/app_simcom.c \
  apps/app_mqtt.c \
#-------------------------------------------------------------------------------
# the include directories
#-------------------------------------------------------------------------------
INC_DIRS+= \
  -I${TOP_DIR}/simcom_lib/inc \
  -I${TOP_DIR}/phase_detector_slave01 \
  -I${TOP_DIR}/third_lib/cjson \
  -I${TOP_DIR}/phase_detector_slave01/drivers/i2c \
  -I${TOP_DIR}/phase_detector_slave01/drivers/system \
  -I${TOP_DIR}/phase_detector_slave01/drivers/io \
  -I${TOP_DIR}/phase_detector_slave01/drivers/flash \
  -I${TOP_DIR}/phase_detector_slave01/drivers/fs \
  -I${TOP_DIR}/phase_detector_slave01/drivers/adc \
  -I${TOP_DIR}/phase_detector_slave01/services/simcom \
  -I${TOP_DIR}/phase_detector_slave01/services/utils \
  -I${TOP_DIR}/phase_detector_slave01/services/log \
  -I${TOP_DIR}/phase_detector_slave01/services/syslog \
  -I${TOP_DIR}/phase_detector_slave01/services/fifo \
  
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
