################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include STM32X/Lib/usb/msc/subdir.mk
-include STM32X/Lib/usb/cdc/subdir.mk
-include STM32X/Lib/usb/subdir.mk
-include STM32X/Lib/subdir.mk
-include Drivers/STM32L0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
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

BUILD_ARTIFACT_NAME := THE_BRAINS
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
THE_BRAINS.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
THE_BRAINS.list \

OBJCOPY_BIN += \
THE_BRAINS.bin \


# All Target
all: main-build

# Main-build Target
main-build: THE_BRAINS.elf secondary-outputs

# Tool invocations
THE_BRAINS.elf: $(OBJS) $(USER_OBJS) C:\dev\GitHub\THE_BRAINS\FW\STM32L051K8UX_FLASH.ld
	arm-none-eabi-gcc -o "THE_BRAINS.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\dev\GitHub\THE_BRAINS\FW\STM32L051K8UX_FLASH.ld" --specs=nosys.specs -Wl,-Map="THE_BRAINS.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

THE_BRAINS.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "THE_BRAINS.list"
	@echo 'Finished building: $@'
	@echo ' '

THE_BRAINS.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "THE_BRAINS.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
