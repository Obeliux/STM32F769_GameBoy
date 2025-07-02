/*
 * uart_debug.h
 *
 *	Created on: Oct 27, 2021
 *	Author: Marcel Tresanchez
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UART_DEBUG_H_
#define UART_DEBUG_H_

#ifdef __cplusplus
 extern "C" {
#endif

 /* Includes ------------------------------------------------------------------*/
#include "main.h"

#define UART_PRINF_REDIRECTION		// Uncomment to use the redirection of the printf()
#define UART_RX_INTERRUPT			// Uncomment to use the Rx UART Interrupt

 /* Functions ------------------------------------------------------------------*/
void UART_Init(UART_HandleTypeDef *huart);
void UART_task(void);
uint8_t UART_isDataRecived(void);
uint8_t UART_GetStr(char s[]);
void UART_PutStr(char s[]);
void UART_Put(uint16_t d);

#ifdef __cplusplus
}
#endif

#endif /* UART_DEBUG_H_ */
