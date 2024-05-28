################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Service/atc/atc.c 

OBJS += \
./User/Service/atc/atc.o 

C_DEPS += \
./User/Service/atc/atc.d 


# Each subdirectory must supply rules for building sources it contributes
User/Service/atc/%.o User/Service/atc/%.su User/Service/atc/%.cyclo: ../User/Service/atc/%.c User/Service/atc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../User/Service/datetime -I../User/Service/fifo -I../User/Service/libnmea -I../User/Service/stimer -I../User/App -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Service-2f-atc

clean-User-2f-Service-2f-atc:
	-$(RM) ./User/Service/atc/atc.cyclo ./User/Service/atc/atc.d ./User/Service/atc/atc.o ./User/Service/atc/atc.su

.PHONY: clean-User-2f-Service-2f-atc

