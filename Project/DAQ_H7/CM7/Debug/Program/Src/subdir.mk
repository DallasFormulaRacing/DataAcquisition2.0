################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Program/Src/app.cpp 

OBJS += \
./Program/Src/app.o 

CPP_DEPS += \
./Program/Src/app.d 


# Each subdirectory must supply rules for building sources it contributes
Program/Src/%.o Program/Src/%.su Program/Src/%.cyclo: ../Program/Src/%.cpp Program/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H755xx -c -I../Core/Inc -I../Program -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Program-2f-Src

clean-Program-2f-Src:
	-$(RM) ./Program/Src/app.cyclo ./Program/Src/app.d ./Program/Src/app.o ./Program/Src/app.su

.PHONY: clean-Program-2f-Src

