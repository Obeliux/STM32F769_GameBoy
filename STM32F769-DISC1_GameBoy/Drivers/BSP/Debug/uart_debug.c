/*
 * uart_debug.c
 *
 *  Created on: Oct 27, 2021
 *      Author: Marcel Tresanchez
 */

#include "uart_debug.h"

 /* Includes ------------------------------------------------------------------*/
#include "stdio.h"    // for setvbuf()
#include "string.h"   // for strcpy()

/* Private variables -----------------------------------------------------------------*/
// Private rx buffer variables
#define UART_RX_LEN 100
__IO uint8_t UART_isRecived = RESET;
__IO char UART_RxBuf[UART_RX_LEN];
__IO uint8_t UART_RxPos = 0;

// Private status variables
__IO UART_HandleTypeDef *huart_debug;


/* Functions -------------------------------------------------------------------------*/
#ifdef UART_PRINF_REDIRECTION
	// Printf() Redirection
	// https://community.st.com/s/question/0D50X0000B8kTyt/printf-not-working-write-never-gets-called
	#ifdef __GNUC__
		#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
	#else
		#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#endif // __GNUC__
	PUTCHAR_PROTOTYPE
	{
		UART_Put(ch);
		return ch;
	}
#endif

// Initialize UART
void UART_Init(UART_HandleTypeDef *huart) {
	#ifdef UART_PRINF_REDIRECTION
		// printf redirection purpose:
		// stdio printf() buffered off. Able to do printf() without \n
		setvbuf(stdout, NULL, _IONBF, 0);
	#endif

	#ifdef UART_RX_INTERRUPT
	  // Enable the RX interrupt
	  __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
	#else
	  // DISABLE RX IRQ
	  __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);

	#endif

	// Save a pointer reference of the UART peripheral handle
	huart_debug = huart;
}

// Update new received byte and store it to buffer
void UART_task(void){
	uint16_t c;

	// Check if new byte arrived
	if (__HAL_UART_GET_FLAG(huart_debug, UART_FLAG_RXNE) == SET) {
		// Read the received data
		c = (uint8_t)(huart_debug->Instance->RDR & 0x0FF);

		// If there is a string pending to read, then discard new data
		if (UART_isRecived == SET) {
			return;
		}

		// Store the new data to the buffer
		UART_RxBuf[UART_RxPos] = c;

		// if is a return, '/r' = char(13) then indicates there is a new string
		if (c == 13) {
			UART_RxBuf[UART_RxPos] = '\0';
			UART_isRecived = SET;
			UART_RxPos = 0;
		} else if (UART_RxPos < UART_RX_LEN-1) UART_RxPos++;
	}
}

// Check flag if is received data
uint8_t UART_isDataRecived(void) {
	return UART_isRecived;
}


// Get string from buffer
uint8_t UART_GetStr(char s[]) {
	if (UART_isRecived == RESET) return 0;
	strcpy(s, (char*)UART_RxBuf);
	UART_isRecived = RESET;
	return 1;
}


// Send String
void UART_PutStr(char s[]) {
	int16_t i = 0;
	while(s[i]!='\0') {
		UART_Put(s[i]);
		i++;
	}
}


// Put char
void UART_Put(uint16_t d) {

	// Using registers
	//huart_debug->Instance->DR = (uint8_t)(d & 0xFFU);
	//while(!__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC));

	// Using the HAL layer:
	if(HAL_UART_Transmit((UART_HandleTypeDef*)huart_debug,(uint8_t*)&d, 1, 100) != HAL_OK)
		Error_Handler();
}



