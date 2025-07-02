/**
  ******************************************************************************
  * @file    led.h
  * @author  Marcel Tresanchez
  * @version 1.0 (9-12-21)
  * @brief   Peripherals for the board (LEDs, pushbuttons,...)
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LED_H_
#define LED_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

// LED Exported Types
typedef enum {
	LED4 = 0,
	LED3 = 1,
	/* Color led aliases */
	LED_RED  = LED4,
	LED_GREEN = LED3,
}Led_TypeDef;


// LED Constants
#define LEDn                             4

#define LED4_PIN                         GPIO_PIN_13
#define LED4_GPIO_PORT                   GPIOJ
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define LED3_PIN                         GPIO_PIN_5
#define LED3_GPIO_PORT                   GPIOJ
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()



#define LEDx_GPIO_CLK_ENABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 1) LED3_GPIO_CLK_ENABLE(); \
                                           }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED3_GPIO_CLK_DISABLE(); \
                                            }while(0)

// LED Functions
void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_DeInit(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);
void BSP_LED_Init_All(void);
void BSP_LED_On_All(void);
void BSP_LED_Off_All(void);
void BSP_LED_Toggle_All(void);


#ifdef __cplusplus
}
#endif

#endif /* LED_H_ */
