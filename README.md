# STM32F769_GameBoy
An adaptation from an other project made in STM32F429I-DISCO (stm32boy) made by jnz (Jan Zwiener), also added a DSI-HDMI video output and a conection to a Gamepad made on another own project. 

The gamepad can be easily replaced by any device with a NRF24 connection
The NRF24 Connections on the STM32F769 board are these:

SPI5:
NRF24_SCK: PF7 --> D6
NRF24_MOSI: PF9 --> A5
NRF24_MISO: PF8 --> A4
NRF24_CE: PJ0 --> D4
NRF24_CSN: PJ1 --> D2
NRF24_IRQ: PJ4 --> D8

jnz repo: https://github.com/jnz/stm32boy

Uses STMF7xx_HAL_Driver and has best performance on Release on STM32CubeIDE.
