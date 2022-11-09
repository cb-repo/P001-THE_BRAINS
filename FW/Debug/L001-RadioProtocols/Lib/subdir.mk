################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L001-RadioProtocols/Lib/IBUS.c \
../L001-RadioProtocols/Lib/PPM.c \
../L001-RadioProtocols/Lib/PWM.c \
../L001-RadioProtocols/Lib/Radio.c \
../L001-RadioProtocols/Lib/SBUS.c 

OBJS += \
./L001-RadioProtocols/Lib/IBUS.o \
./L001-RadioProtocols/Lib/PPM.o \
./L001-RadioProtocols/Lib/PWM.o \
./L001-RadioProtocols/Lib/Radio.o \
./L001-RadioProtocols/Lib/SBUS.o 

C_DEPS += \
./L001-RadioProtocols/Lib/IBUS.d \
./L001-RadioProtocols/Lib/PPM.d \
./L001-RadioProtocols/Lib/PWM.d \
./L001-RadioProtocols/Lib/Radio.d \
./L001-RadioProtocols/Lib/SBUS.d 


# Each subdirectory must supply rules for building sources it contributes
L001-RadioProtocols/Lib/%.o L001-RadioProtocols/Lib/%.su: ../L001-RadioProtocols/Lib/%.c L001-RadioProtocols/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../STM32X/Lib -I../L001-RadioProtocols/Lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-L001-2d-RadioProtocols-2f-Lib

clean-L001-2d-RadioProtocols-2f-Lib:
	-$(RM) ./L001-RadioProtocols/Lib/IBUS.d ./L001-RadioProtocols/Lib/IBUS.o ./L001-RadioProtocols/Lib/IBUS.su ./L001-RadioProtocols/Lib/PPM.d ./L001-RadioProtocols/Lib/PPM.o ./L001-RadioProtocols/Lib/PPM.su ./L001-RadioProtocols/Lib/PWM.d ./L001-RadioProtocols/Lib/PWM.o ./L001-RadioProtocols/Lib/PWM.su ./L001-RadioProtocols/Lib/Radio.d ./L001-RadioProtocols/Lib/Radio.o ./L001-RadioProtocols/Lib/Radio.su ./L001-RadioProtocols/Lib/SBUS.d ./L001-RadioProtocols/Lib/SBUS.o ./L001-RadioProtocols/Lib/SBUS.su

.PHONY: clean-L001-2d-RadioProtocols-2f-Lib

