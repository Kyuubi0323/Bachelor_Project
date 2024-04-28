include ${TOP_DIR}/script/env.mk

SRC_FILES:=$(subst \,/,$(SRC_FILES))
INC_DIRS:=$(subst \,/,$(INC_DIRS))

SRC_DIRS:=$(dir $(SRC_FILES))
MAKE_PATH:=$(patsubst %/,%,$(dir $(abspath $(firstword $(MAKEFILE_LIST)))))
TARGET_NAME:=$(lastword $(subst /, ,$(subst \, ,$(MAKE_PATH))))
BUILD_PATH:=${TOP_DIR}/$(BUILD_PATH)/packag-$(TARGET_NAME)
OBJ_PATH:=$(BUILD_PATH)/obj

OBJS:=$(SRC_FILES:.c=.o)
OBJS:=$(OBJS:.cpp=.o)
OBJS:=$(OBJS:.S=.o)
OBJS:=$(OBJS:.s=.o)
OBJS:=$(patsubst %,$(OBJ_PATH)/%,$(OBJS))

DEF_FILE_TEMP:=$(SRC_FILES:.c=.d)
DEF_FILE_TEMP:=$(DEF_FILE_TEMP:.cpp=.d)
DEF_FILE_TEMP:=$(patsubst %,$(subst $(strip \),/,$(OBJ_PATH))/%,$(DEF_FILE_TEMP))
DEF_FILE:=$(DEF_FILE_TEMP)

#-------------------------------------------------------------------------------
# build flags setting
#-------------------------------------------------------------------------------
FNO_BUILTIN_FLAGS := -fno-builtin-printf \
					   -fno-builtin-time \
					   -fno-builtin-gmtime \
					   -fno-builtin-gettimeofday

SFLAGS:=-c -mlong-calls -march=armv7-r  -mcpu=cortex-r4 -mlittle-endian -mthumb -mthumb-interwork -mfloat-abi=soft -Wall -Wstrict-prototypes -Os
CFLAGS:=-c -MMD -mlong-calls -march=armv7-r  -mcpu=cortex-r4 -mlittle-endian -mthumb -mthumb-interwork  -mfloat-abi=soft -Wall -ffunction-sections -fdata-sections $(FNO_BUILTIN_FLAGS) -Wstrict-prototypes -std=c99 -g -Os
CPPFLAGS:=-c -MMD -mlong-calls -march=armv7-r  -mcpu=cortex-r4  -mlittle-endian -mthumb -mthumb-interwork  -mfloat-abi=soft -Wall -ffunction-sections -fdata-sections $(FNO_BUILTIN_FLAGS) -Wstrict-prototypes -std=c99 -g -Os
ARFLAG:=-rc


.PHONY: all clean makeDir

TARGET:=$(BUILD_PATH)/$(TARGET_NAME).lib

all:makeDir $(TARGET)

makeDir:
	@if not exist $(BUILD_PATH) (md $(subst /,\,$(BUILD_PATH)))
	@if not exist $(OBJ_PATH) (md $(subst /,\,$(OBJ_PATH)))
	@for /d %%y in ($(subst $(strip /),\,$(SRC_DIRS))) do ( \
		@if not exist $(OBJ_PATH)\%%y ( \
			(@echo creating direcotry $(OBJ_PATH)\%%y) & (md $(subst $(strip /),\,$(OBJ_PATH)\%%y)) \
		) \
	)

clean:
	@if exist $(BUILD_PATH) (@echo clean $(subst /,\,$(BUILD_PATH)) & rd /s/q $(subst /,\,$(BUILD_PATH)))
	@if exist $(TARGET) (@echo clean $(subst /,\,$(TARGET)) & rd /s/q $(subst /,\,$(TARGET)))

$(TARGET):$(OBJS)
	@echo ---------------create lib [$@] to [$(BUILD_PATH)]-------------------
	$(AR) $(ARFLAG) $@ $^

$(OBJ_PATH)/%.o: %.S
	$(CC)  $(SFLAGS) -o $@ $<

$(OBJ_PATH)/%.o: %.c
	$(CC) $(CFLAGS) $(DFLAG) $(DFLAGS) $(U_FLAGS) $(INC_DIRS) -o $@ $<

$(OBJ_PATH)/%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(DFLAG) $(DFLAGS) $(U_FLAGS) $(INC_DIRS)  -o $@ $<

-include $(DEF_FILE)
