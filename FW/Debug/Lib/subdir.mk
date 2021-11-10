################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/ADC.c \
../Lib/Core.c \
../Lib/EEPROM.c \
../Lib/GPIO.c \
../Lib/SPI.c \
../Lib/TIM.c \
../Lib/UART.c \
../Lib/WDG.c 

OBJS += \
./Lib/ADC.o \
./Lib/Core.o \
./Lib/EEPROM.o \
./Lib/GPIO.o \
./Lib/SPI.o \
./Lib/TIM.o \
./Lib/UART.o \
./Lib/WDG.o 

C_DEPS += \
./Lib/ADC.d \
./Lib/Core.d \
./Lib/EEPROM.d \
./Lib/GPIO.d \
./Lib/SPI.d \
./Lib/TIM.d \
./Lib/UART.d \
./Lib/WDG.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/ADC.o: ../Lib/ADC.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/ADC.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/Core.o: ../Lib/Core.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/Core.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/EEPROM.o: ../Lib/EEPROM.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/EEPROM.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/GPIO.o: ../Lib/GPIO.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/GPIO.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/SPI.o: ../Lib/SPI.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/SPI.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/TIM.o: ../Lib/TIM.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/TIM.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/UART.o: ../Lib/UART.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/UART.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/WDG.o: ../Lib/WDG.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Lib -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lib/WDG.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

