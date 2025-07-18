
/**
  ******************************************************************************
  * @file    adv7533.h
  * @author  MCD Application Team
  * @brief   This file contains all the constants parameters for the ADV7533
  *          which is the HDMI bridge between DSI and HDMI
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADV7533_H
#define __ADV7533_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "audio.h"




/** @addtogroup BSP
  * @{
  */
 
/** @addtogroup Components
  * @{
  */

/** @addtogroup adv7533
  * @{
  */

/** @addtogroup ADV7533_Exported_Types
  * @{
  */

typedef struct {
  uint8_t       DSI_LANES;
  uint16_t      HACT;
  uint16_t      HSYNC;
  uint16_t      HBP;
  uint16_t      HFP;
  uint16_t      VACT;
  uint16_t      VSYNC;
  uint16_t      VBP;
  uint16_t      VFP;
  uint8_t       ASPECT_RATIO;
} adv7533ConfigTypeDef;

/** @defgroup ADV7533_Exported_Constants
  * @{
  */

/** 
  * @brief HDMI audio output DEVICE
  */ 
#define OUTPUT_DEVICE_ADV7533_HDMI       ((uint16_t)0x1000)

/** 
  * @brief  ADV7533 I2C Addresses 0x7A / 0x78
  */  
#define  ADV7533_MAIN_I2C_ADDR           ((uint8_t)0x7A)
#define  ADV7533_CEC_DSI_I2C_ADDR        ((uint8_t)0x78)

/** 
  * @brief  ADV7533 Aspect ratio
  */  
#define ADV7533_ASPECT_RATIO_16_9        ((uint8_t)0x00)
#define ADV7533_ASPECT_RATIO_4_3         ((uint8_t)0x01)

/** 
  * @brief  ADV7533 Aspect ratio
  */  
#define ADV7533_MODE_HDMI                0x0
#define ADV7533_MODE_DVI                 0x1
   
/** 
  * @brief  ADV7533 Main Registers
  */
#define  ADV7533_MAIN_SYNC_REG           ((uint8_t)0x17)
#define  ADV7533_MAIN_POWER_DOWN_REG     ((uint8_t)0x41)
#define  ADV7533_MAIN_HPG_REG            ((uint8_t)0x42)

/**
 * @brief   ADV7533 Main Features Parameters
 */

/**
  * @brief ADV7533 CEC DSI Registers
  */
#define ADV7533_CEC_DSI_INTERNAL_TIMING_REG  ((uint8_t)0x27)
#define ADV7533_CEC_DSI_TOTAL_WIDTH_H_REG    ((uint8_t)0x28)
#define ADV7533_CEC_DSI_TOTAL_WIDTH_L_REG    ((uint8_t)0x29)
#define ADV7533_CEC_DSI_HSYNC_H_REG          ((uint8_t)0x2A)
#define ADV7533_CEC_DSI_HSYNC_L_REG          ((uint8_t)0x2B)
#define ADV7533_CEC_DSI_HFP_H_REG            ((uint8_t)0x2C)
#define ADV7533_CEC_DSI_HFP_L_REG            ((uint8_t)0x2D)
#define ADV7533_CEC_DSI_HBP_H_REG            ((uint8_t)0x2E)
#define ADV7533_CEC_DSI_HBP_L_REG            ((uint8_t)0x2F)

#define ADV7533_CEC_DSI_TOTAL_HEIGHT_H_REG   ((uint8_t)0x30)
#define ADV7533_CEC_DSI_TOTAL_HEIGHT_L_REG   ((uint8_t)0x31)
#define ADV7533_CEC_DSI_VSYNC_H_REG          ((uint8_t)0x32)
#define ADV7533_CEC_DSI_VSYNC_L_REG          ((uint8_t)0x33)
#define ADV7533_CEC_DSI_VFP_H_REG            ((uint8_t)0x34)
#define ADV7533_CEC_DSI_VFP_L_REG            ((uint8_t)0x35)
#define ADV7533_CEC_DSI_VBP_H_REG            ((uint8_t)0x36)
#define ADV7533_CEC_DSI_VBP_L_REG            ((uint8_t)0x37)

/** @Brief adv7533 ID
  */
#define ADV7533_ID  0x7533
   
/** @Brief device ID register 
  */
#define   ADV7533_CHIPID_ADDR0   0x00
#define   ADV7533_CHIPID_ADDR1   0x01    

/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0
    
/* AUDIO FREQUENCY */
#define AUDIO_FREQUENCY_192K           ((uint32_t)192000)
#define AUDIO_FREQUENCY_176K           ((uint32_t)176400)
#define AUDIO_FREQUENCY_96K            ((uint32_t)96000)
#define AUDIO_FREQUENCY_88K            ((uint32_t)88200)
#define AUDIO_FREQUENCY_48K            ((uint32_t)48000)
#define AUDIO_FREQUENCY_44K            ((uint32_t)44100)
#define AUDIO_FREQUENCY_32K            ((uint32_t)32000) 

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
   
/** @defgroup ADV7533_Exported_Macros ADV7533 Exported Macros
  * @{
  */ 

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
  
/** @addtogroup ADV7533_Exported_Functions
  * @{
  */

/*------------------------------------------------------------------------------
                           HDMI video functions 
------------------------------------------------------------------------------*/  
uint8_t ADV7533_Init(void);
void ADV7533_PowerOn(void);
void ADV7533_PowerDown(void);
void ADV7533_Configure(adv7533ConfigTypeDef * config);
void ADV7533_PatternEnable(void);
void ADV7533_PatternDisable(void);

/*------------------------------------------------------------------------------
                           HDMI Audio functions 
------------------------------------------------------------------------------*/
uint32_t adv7533_AudioInit(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume,uint32_t AudioFreq);
void adv7533_DeInit(void);
uint32_t adv7533_ReadID(uint16_t DeviceAddr);
uint32_t adv7533_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size);
uint32_t adv7533_Stop(uint16_t DeviceAddr,uint32_t Cmd);
uint32_t adv7533_Pause(uint16_t DeviceAddr);
uint32_t adv7533_Resume(uint16_t DeviceAddr);
uint32_t adv7533_SetMute(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t adv7533_SetVolume(uint16_t, uint8_t);
uint32_t adv7533_Reset(uint16_t DeviceAddr);
uint32_t adv7533_SetOutputMode(uint16_t DeviceAddr, uint8_t Output);
uint32_t adv7533_SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq);


/**
  * @}
  */ 

/* HDMI Audio driver structure */
extern AUDIO_DrvTypeDef adv7533_drv;







/**
  * @brief User can use this section to tailor I2C4/I2C4 instance used and associated
  * resources (audio codec).
  * Definition for I2C4 clock resources
  */
#define DISCOVERY_AUDIO_I2Cx                             I2C4
#define DISCOVERY_AUDIO_I2Cx_CLK_ENABLE()                __HAL_RCC_I2C4_CLK_ENABLE()
#define DISCOVERY_AUDIO_I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define DISCOVERY_AUDIO_I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()

#define DISCOVERY_AUDIO_I2Cx_FORCE_RESET()               __HAL_RCC_I2C4_FORCE_RESET()
#define DISCOVERY_AUDIO_I2Cx_RELEASE_RESET()             __HAL_RCC_I2C4_RELEASE_RESET()

/** @brief Definition for I2C4 Pins
  */
#define DISCOVERY_AUDIO_I2Cx_SCL_PIN                     GPIO_PIN_12 /*!< PD12 */
#define DISCOVERY_AUDIO_I2Cx_SCL_AF                      GPIO_AF4_I2C4
#define DISCOVERY_AUDIO_I2Cx_SCL_GPIO_PORT               GPIOD
#define DISCOVERY_AUDIO_I2Cx_SDA_PIN                     GPIO_PIN_7 /*!< PB7 */
#define DISCOVERY_AUDIO_I2Cx_SDA_AF                      GPIO_AF11_I2C4
#define DISCOVERY_AUDIO_I2Cx_SDA_GPIO_PORT               GPIOB
/** @brief Definition of I2C4 interrupt requests
  */
#define DISCOVERY_AUDIO_I2Cx_EV_IRQn                     I2C4_EV_IRQn
#define DISCOVERY_AUDIO_I2Cx_ER_IRQn                     I2C4_ER_IRQn



/**
  * @brief User can use this section to tailor I2C1/I2C1 instance used and associated
  * resources.
  * Definition for I2C1 clock resources
  */
#define DISCOVERY_EXT_I2Cx                             I2C1
#define DISCOVERY_EXT_I2Cx_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define DISCOVERY_DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define DISCOVERY_EXT_I2Cx_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define DISCOVERY_EXT_I2Cx_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/** @brief Definition for I2C1 Pins
  */
#define DISCOVERY_EXT_I2Cx_SCL_PIN                     GPIO_PIN_8 /*!< PB8 */
#define DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_PORT           GPIOB
#define DISCOVERY_EXT_I2Cx_SCL_SDA_AF                  GPIO_AF4_I2C1
#define DISCOVERY_EXT_I2Cx_SDA_PIN                     GPIO_PIN_9 /*!< PB9 */

/** @brief Definition of I2C interrupt requests
  */
#define DISCOVERY_EXT_I2Cx_EV_IRQn                     I2C1_EV_IRQn
#define DISCOVERY_EXT_I2Cx_ER_IRQn                     I2C1_ER_IRQn

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from APB1 source clock = 50 MHz */
/* Due to the big MOFSET capacity for adapting the camera level the rising time is very large (>1us) */
/* 0x40912732 takes in account the big rising and aims a clock of 100khz */
#ifndef DISCOVERY_I2Cx_TIMING
#define DISCOVERY_I2Cx_TIMING                      ((uint32_t)0x40912732)
#endif /* DISCOVERY_I2Cx_TIMING */





/* HDMI IO functions */
void            HDMI_IO_Init(void);
void            HDMI_IO_Delay(uint32_t Delay);
void            HDMI_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t         HDMI_IO_Read(uint8_t Addr, uint8_t Reg);



#endif /* __ADV7533_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
