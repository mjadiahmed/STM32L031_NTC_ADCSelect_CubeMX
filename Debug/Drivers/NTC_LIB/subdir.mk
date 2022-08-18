################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/NTC_LIB/ntc_table.c \
../Drivers/NTC_LIB/sensor.c 

OBJS += \
./Drivers/NTC_LIB/ntc_table.o \
./Drivers/NTC_LIB/sensor.o 

C_DEPS += \
./Drivers/NTC_LIB/ntc_table.d \
./Drivers/NTC_LIB/sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/NTC_LIB/%.o Drivers/NTC_LIB/%.su: ../Drivers/NTC_LIB/%.c Drivers/NTC_LIB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L031xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/mjadi/STM32CubeIDE/STM32L031_ADC/L031_ADC_NTC/Drivers/NTC_LIB" -I"C:/Users/mjadi/STM32CubeIDE/STM32L031_ADC/L031_ADC_NTC/Drivers/SelectADC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-NTC_LIB

clean-Drivers-2f-NTC_LIB:
	-$(RM) ./Drivers/NTC_LIB/ntc_table.d ./Drivers/NTC_LIB/ntc_table.o ./Drivers/NTC_LIB/ntc_table.su ./Drivers/NTC_LIB/sensor.d ./Drivers/NTC_LIB/sensor.o ./Drivers/NTC_LIB/sensor.su

.PHONY: clean-Drivers-2f-NTC_LIB

