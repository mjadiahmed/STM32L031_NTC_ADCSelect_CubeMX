################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32l031k6ux.s 

OBJS += \
./Core/Startup/startup_stm32l031k6ux.o 

S_DEPS += \
./Core/Startup/startup_stm32l031k6ux.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -DDEBUG -c -I"C:/Users/mjadi/STM32CubeIDE/STM32L031_ADC/L031_ADC_NTC/Drivers/NTC_LIB" -I"C:/Users/mjadi/STM32CubeIDE/STM32L031_ADC/L031_ADC_NTC/Drivers/SelectADC" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32l031k6ux.d ./Core/Startup/startup_stm32l031k6ux.o

.PHONY: clean-Core-2f-Startup

