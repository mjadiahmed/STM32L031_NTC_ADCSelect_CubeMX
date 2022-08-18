################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SelectADC/SelectADC.c 

OBJS += \
./Drivers/SelectADC/SelectADC.o 

C_DEPS += \
./Drivers/SelectADC/SelectADC.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SelectADC/%.o Drivers/SelectADC/%.su: ../Drivers/SelectADC/%.c Drivers/SelectADC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L031xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/mjadi/STM32CubeIDE/STM32L031_ADC/L031_ADC_NTC/Drivers/NTC_LIB" -I"C:/Users/mjadi/STM32CubeIDE/STM32L031_ADC/L031_ADC_NTC/Drivers/SelectADC" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SelectADC

clean-Drivers-2f-SelectADC:
	-$(RM) ./Drivers/SelectADC/SelectADC.d ./Drivers/SelectADC/SelectADC.o ./Drivers/SelectADC/SelectADC.su

.PHONY: clean-Drivers-2f-SelectADC

