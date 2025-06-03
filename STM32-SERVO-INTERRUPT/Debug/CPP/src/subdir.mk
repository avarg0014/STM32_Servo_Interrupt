################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CPP/SRC/main.cpp 

OBJS += \
./CPP/SRC/main.o 

CPP_DEPS += \
./CPP/SRC/main.d 


# Each subdirectory must supply rules for building sources it contributes
CPP/SRC/%.o CPP/SRC/%.su CPP/SRC/%.cyclo: ../CPP/SRC/%.cpp CPP/SRC/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0plus -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G071xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CPP-2f-SRC

clean-CPP-2f-SRC:
	-$(RM) ./CPP/SRC/main.cyclo ./CPP/SRC/main.d ./CPP/SRC/main.o ./CPP/SRC/main.su

.PHONY: clean-CPP-2f-SRC

