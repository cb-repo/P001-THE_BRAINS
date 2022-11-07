################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32X/Lib/ADC.c \
../STM32X/Lib/CAN.c \
../STM32X/Lib/CLK.c \
../STM32X/Lib/COMP.c \
../STM32X/Lib/CRC.c \
../STM32X/Lib/Core.c \
../STM32X/Lib/DMA.c \
../STM32X/Lib/EEPROM.c \
../STM32X/Lib/FLASH.c \
../STM32X/Lib/GPIO.c \
../STM32X/Lib/I2C.c \
../STM32X/Lib/RTC.c \
../STM32X/Lib/SPI.c \
../STM32X/Lib/TIM.c \
../STM32X/Lib/UART.c \
../STM32X/Lib/US.c \
../STM32X/Lib/USB.c \
../STM32X/Lib/WDG.c 

OBJS += \
./STM32X/Lib/ADC.o \
./STM32X/Lib/CAN.o \
./STM32X/Lib/CLK.o \
./STM32X/Lib/COMP.o \
./STM32X/Lib/CRC.o \
./STM32X/Lib/Core.o \
./STM32X/Lib/DMA.o \
./STM32X/Lib/EEPROM.o \
./STM32X/Lib/FLASH.o \
./STM32X/Lib/GPIO.o \
./STM32X/Lib/I2C.o \
./STM32X/Lib/RTC.o \
./STM32X/Lib/SPI.o \
./STM32X/Lib/TIM.o \
./STM32X/Lib/UART.o \
./STM32X/Lib/US.o \
./STM32X/Lib/USB.o \
./STM32X/Lib/WDG.o 

C_DEPS += \
./STM32X/Lib/ADC.d \
./STM32X/Lib/CAN.d \
./STM32X/Lib/CLK.d \
./STM32X/Lib/COMP.d \
./STM32X/Lib/CRC.d \
./STM32X/Lib/Core.d \
./STM32X/Lib/DMA.d \
./STM32X/Lib/EEPROM.d \
./STM32X/Lib/FLASH.d \
./STM32X/Lib/GPIO.d \
./STM32X/Lib/I2C.d \
./STM32X/Lib/RTC.d \
./STM32X/Lib/SPI.d \
./STM32X/Lib/TIM.d \
./STM32X/Lib/UART.d \
./STM32X/Lib/US.d \
./STM32X/Lib/USB.d \
./STM32X/Lib/WDG.d 


# Each subdirectory must supply rules for building sources it contributes
STM32X/Lib/%.o STM32X/Lib/%.su: ../STM32X/Lib/%.c STM32X/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../STM32X/Lib -I../L001-RadioProtocols/Lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32X-2f-Lib

clean-STM32X-2f-Lib:
	-$(RM) ./STM32X/Lib/ADC.d ./STM32X/Lib/ADC.o ./STM32X/Lib/ADC.su ./STM32X/Lib/CAN.d ./STM32X/Lib/CAN.o ./STM32X/Lib/CAN.su ./STM32X/Lib/CLK.d ./STM32X/Lib/CLK.o ./STM32X/Lib/CLK.su ./STM32X/Lib/COMP.d ./STM32X/Lib/COMP.o ./STM32X/Lib/COMP.su ./STM32X/Lib/CRC.d ./STM32X/Lib/CRC.o ./STM32X/Lib/CRC.su ./STM32X/Lib/Core.d ./STM32X/Lib/Core.o ./STM32X/Lib/Core.su ./STM32X/Lib/DMA.d ./STM32X/Lib/DMA.o ./STM32X/Lib/DMA.su ./STM32X/Lib/EEPROM.d ./STM32X/Lib/EEPROM.o ./STM32X/Lib/EEPROM.su ./STM32X/Lib/FLASH.d ./STM32X/Lib/FLASH.o ./STM32X/Lib/FLASH.su ./STM32X/Lib/GPIO.d ./STM32X/Lib/GPIO.o ./STM32X/Lib/GPIO.su ./STM32X/Lib/I2C.d ./STM32X/Lib/I2C.o ./STM32X/Lib/I2C.su ./STM32X/Lib/RTC.d ./STM32X/Lib/RTC.o ./STM32X/Lib/RTC.su ./STM32X/Lib/SPI.d ./STM32X/Lib/SPI.o ./STM32X/Lib/SPI.su ./STM32X/Lib/TIM.d ./STM32X/Lib/TIM.o ./STM32X/Lib/TIM.su ./STM32X/Lib/UART.d ./STM32X/Lib/UART.o ./STM32X/Lib/UART.su ./STM32X/Lib/US.d ./STM32X/Lib/US.o ./STM32X/Lib/US.su ./STM32X/Lib/USB.d ./STM32X/Lib/USB.o ./STM32X/Lib/USB.su ./STM32X/Lib/WDG.d ./STM32X/Lib/WDG.o ./STM32X/Lib/WDG.su

.PHONY: clean-STM32X-2f-Lib

