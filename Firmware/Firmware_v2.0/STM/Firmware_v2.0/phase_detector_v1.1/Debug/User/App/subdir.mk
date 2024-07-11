################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/App/app_measure.c \
../User/App/app_uart.c \
../User/App/interrupt.c 

OBJS += \
./User/App/app_measure.o \
./User/App/app_uart.o \
./User/App/interrupt.o 

C_DEPS += \
./User/App/app_measure.d \
./User/App/app_uart.d \
./User/App/interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
User/App/%.o User/App/%.su User/App/%.cyclo: ../User/App/%.c User/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../User/Service/datetime -I../User/Service/fifo -I../User/Service/libnmea -I../User/Service/stimer -I../User/App -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-App

clean-User-2f-App:
	-$(RM) ./User/App/app_measure.cyclo ./User/App/app_measure.d ./User/App/app_measure.o ./User/App/app_measure.su ./User/App/app_uart.cyclo ./User/App/app_uart.d ./User/App/app_uart.o ./User/App/app_uart.su ./User/App/interrupt.cyclo ./User/App/interrupt.d ./User/App/interrupt.o ./User/App/interrupt.su

.PHONY: clean-User-2f-App

