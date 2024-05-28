################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Service/fifo/fifo.c \
../User/Service/fifo/fifo_old.c 

OBJS += \
./User/Service/fifo/fifo.o \
./User/Service/fifo/fifo_old.o 

C_DEPS += \
./User/Service/fifo/fifo.d \
./User/Service/fifo/fifo_old.d 


# Each subdirectory must supply rules for building sources it contributes
User/Service/fifo/%.o User/Service/fifo/%.su User/Service/fifo/%.cyclo: ../User/Service/fifo/%.c User/Service/fifo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../User/Service/datetime -I../User/Service/fifo -I../User/Service/libnmea -I../User/Service/stimer -I../User/App -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Service-2f-fifo

clean-User-2f-Service-2f-fifo:
	-$(RM) ./User/Service/fifo/fifo.cyclo ./User/Service/fifo/fifo.d ./User/Service/fifo/fifo.o ./User/Service/fifo/fifo.su ./User/Service/fifo/fifo_old.cyclo ./User/Service/fifo/fifo_old.d ./User/Service/fifo/fifo_old.o ./User/Service/fifo/fifo_old.su

.PHONY: clean-User-2f-Service-2f-fifo

