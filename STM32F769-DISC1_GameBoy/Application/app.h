/******************************************************************************
 * @file	app.h
 * @author  UdL
 * @version V1.0.0
 * @date    1-Jan-2022
 * @brief   This file contains the main application process.
 ******************************************************************************
 * @attention
 *
 * <h2>&copy; Copyright (c) 2022 UDL. All rights reserved.</h2>
 *
 ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_H
#define __APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void APP_Init(void);
void APP_Task(void);
void mainInit(void);
void mainTask(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_H */




