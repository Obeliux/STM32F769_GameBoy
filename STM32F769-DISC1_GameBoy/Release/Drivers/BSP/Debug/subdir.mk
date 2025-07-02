################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Debug/uart_debug.c 

OBJS += \
./Drivers/BSP/Debug/uart_debug.o 

C_DEPS += \
./Drivers/BSP/Debug/uart_debug.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Debug/%.o Drivers/BSP/Debug/%.su Drivers/BSP/Debug/%.cyclo: ../Drivers/BSP/Debug/%.c Drivers/BSP/Debug/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_LCD_HDMI -DARM_MATH_CM7 -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Peanut-GB" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/NRF24L01" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/DSP" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/DSP/Include" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/Debug" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/Discovery" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/HDMI" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/HDMI/Fonts" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device/ST" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device/ST/STM32F7xx" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device/ST/STM32F7xx/Source" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/STM32F7xx_HAL_Driver" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/STM32F7xx_HAL_Driver/Src" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Application" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Core/Src" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Core/Startup" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Debug

clean-Drivers-2f-BSP-2f-Debug:
	-$(RM) ./Drivers/BSP/Debug/uart_debug.cyclo ./Drivers/BSP/Debug/uart_debug.d ./Drivers/BSP/Debug/uart_debug.o ./Drivers/BSP/Debug/uart_debug.su

.PHONY: clean-Drivers-2f-BSP-2f-Debug

