################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32X/Lib/usb/msc/SCSI.c \
../STM32X/Lib/usb/msc/USB_MSC.c 

OBJS += \
./STM32X/Lib/usb/msc/SCSI.o \
./STM32X/Lib/usb/msc/USB_MSC.o 

C_DEPS += \
./STM32X/Lib/usb/msc/SCSI.d \
./STM32X/Lib/usb/msc/USB_MSC.d 


# Each subdirectory must supply rules for building sources it contributes
STM32X/Lib/usb/msc/%.o STM32X/Lib/usb/msc/%.su: ../STM32X/Lib/usb/msc/%.c STM32X/Lib/usb/msc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L051xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../STM32X/Lib -I../L001-RadioProtocols/Lib -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32X-2f-Lib-2f-usb-2f-msc

clean-STM32X-2f-Lib-2f-usb-2f-msc:
	-$(RM) ./STM32X/Lib/usb/msc/SCSI.d ./STM32X/Lib/usb/msc/SCSI.o ./STM32X/Lib/usb/msc/SCSI.su ./STM32X/Lib/usb/msc/USB_MSC.d ./STM32X/Lib/usb/msc/USB_MSC.o ./STM32X/Lib/usb/msc/USB_MSC.su

.PHONY: clean-STM32X-2f-Lib-2f-usb-2f-msc

