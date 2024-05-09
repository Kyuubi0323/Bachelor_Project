################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/Service/stimer/subdir.mk
-include User/Service/libnmea/subdir.mk
-include User/Service/fifo/subdir.mk
-include User/Service/datetime/subdir.mk
-include User/Service/atc/subdir.mk
-include User/App/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := phase_detector_v1.1
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
phase_detector_v1.1.elf \

MAP_FILES += \
phase_detector_v1.1.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
phase_detector_v1.1.list \

OBJCOPY_HEX += \
phase_detector_v1.1.hex \

OBJCOPY_BIN += \
phase_detector_v1.1.bin \


# All Target
all: main-build

# Main-build Target
main-build: phase_detector_v1.1.elf secondary-outputs

# Tool invocations
phase_detector_v1.1.elf phase_detector_v1.1.map: $(OBJS) $(USER_OBJS) D:\Advance_Kyuubi_Main_1S\MANDEVICES\IMET\phase_detector_v1\phase_detector_v1.1\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "phase_detector_v1.1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\Advance_Kyuubi_Main_1S\MANDEVICES\IMET\phase_detector_v1\phase_detector_v1.1\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="phase_detector_v1.1.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

phase_detector_v1.1.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "phase_detector_v1.1.list"
	@echo 'Finished building: $@'
	@echo ' '

phase_detector_v1.1.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "phase_detector_v1.1.hex"
	@echo 'Finished building: $@'
	@echo ' '

phase_detector_v1.1.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "phase_detector_v1.1.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout phase_detector_v1.1.bin phase_detector_v1.1.elf phase_detector_v1.1.hex phase_detector_v1.1.list phase_detector_v1.1.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
