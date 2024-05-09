SDK_DIR:=$(TOP_DIR)\..
BUILD_PATH:=build

#-------------------------------------------------------------------------------
# build tool define.
#-------------------------------------------------------------------------------
TOOL_PATH:=$(SDK_DIR)\sc_tool
GCC_PATH:=$(TOOL_PATH)\cross_tool\gcc-arm-none-eabi

CC:=$(GCC_PATH)\bin\arm-none-eabi-gcc.exe
CPP:=$(GCC_PATH)\bin\arm-none-eabi-g++.exe
LD:=$(GCC_PATH)\bin\arm-none-eabi-ld.exe
AR:=$(GCC_PATH)\bin\arm-none-eabi-ar.exe
OBJCOPY:=$(GCC_PATH)\bin\arm-none-eabi-objcopy.exe

MAKE:=$(TOOL_PATH)\gnumake\gnumake.exe
CRC_SET:=$(TOOL_PATH)\crc_set.exe


