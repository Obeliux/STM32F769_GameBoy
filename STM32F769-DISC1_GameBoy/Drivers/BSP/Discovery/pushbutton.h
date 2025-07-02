/**
  ******************************************************************************
  * @file    pushbutton.h
  * @author  Marcel Tresanchez
  * @version 1.0 (9-12-21)
  * @brief   BSP (Board Support Package) Pushbutton peripherals
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

typedef enum {
	BUTTON_USER = 0,
} Button_TypeDef;

typedef enum {
	BUTTON_MODE_GPIO = 0,
	BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#define BUTTONn                          1  // Number of user pushbuttons

#define BUTTON_USER_PIN                          GPIO_PIN_0
#define BUTTON_USER_GPIO_PORT                    GPIOA
#define BUTTON_USER_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_USER_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUTTON_USER_EXTI_IRQn                    EXTI0_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do{if((__INDEX__) == 0) BUTTON_USER_GPIO_CLK_ENABLE(); \
                                                }while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    do{if((__INDEX__) == 0) BUTTON_USER_GPIO_CLK_DISABLE(); \
                                                 }while(0)

/* Functions ------------------------------------------------------------------*/
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void BSP_PB_DeInit(Button_TypeDef Button);
uint32_t BSP_PB_GetState(Button_TypeDef Button);


#ifdef __cplusplus
}
#endif

#endif /* PUSHBUTTON_H_ */
