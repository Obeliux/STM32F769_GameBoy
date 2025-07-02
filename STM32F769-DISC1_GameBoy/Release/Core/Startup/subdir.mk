################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f769nihx.s 

OBJS += \
./Core/Startup/startup_stm32f769nihx.o 

S_DEPS += \
./Core/Startup/startup_stm32f769nihx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -c -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Peanut-GB" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/NRF24L01" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/DSP" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/DSP/Include" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/Debug" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/Discovery" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/HDMI" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/BSP/HDMI/Fonts" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device/ST" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device/ST/STM32F7xx" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Device/ST/STM32F7xx/Source" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/CMSIS/Include" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/STM32F7xx_HAL_Driver" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/STM32F7xx_HAL_Driver/Src" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Application" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Core/Src" -I"C:/Users/Usuario/STM32CubeIDE/workspace_1.17.0/STM32F769-DISC1_GameBoy/Core/Startup" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f769nihx.d ./Core/Startup/startup_stm32f769nihx.o

.PHONY: clean-Core-2f-Startup

