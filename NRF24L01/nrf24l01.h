/*
 * nrf24l01.h
 *
 *  Created on: Dec 5, 2012
 *      Author: Marcel
 *
 * Modified on: Dec 20, 2013
 * Adapted to STM32CubeIDE: May 15, 2024
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _NRF24L01_H
#define _NRF24L01_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "main.h"


// CE Pin & CSN Pin & IRQ Pin control defines
#define NRF24_CE(x)	 x ? HAL_GPIO_WritePin(NRF24_CE_GPIO_Port, NRF24_CE_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(NRF24_CE_GPIO_Port, NRF24_CE_Pin, GPIO_PIN_RESET);
#define NRF24_CSN(x) x ? HAL_GPIO_WritePin(NRF24_CSN_GPIO_Port, NRF24_CSN_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(NRF24_CSN_GPIO_Port, NRF24_CSN_Pin, GPIO_PIN_RESET);
#define NRF24_IRQ(x) HAL_GPIO_ReadPin(NRF24_IRQ_GPIO_Port, NRF24_IRQ_Pin)


// nRF24L01 COMMANDS
// ---------------------------------
#define CMD_R_REG		   		0x00	// Define read command to register (VA SEGUIT DEL REGISTRE)
#define CMD_W_REG		   		0x20	// Define write command to register (VA SEGUIT DEL REGISTRE)
#define CMD_RD_RX_PLOAD 		0x61 	// Define RX payload register address
#define CMD_WR_TX_PLOAD			0xA0 	// Define TX payload register address
#define CMD_FLUSH_TX			0xE1 	// Define flush TX register command
#define CMD_FLUSH_RX			0xE2 	// Define flush RX register command
#define CMD_REUSE_TX_PL			0xE3 	// Define reuse TX payload register command
#define CMD_ACTIVATE			0x50	// ACTIVATE additional features (followed by data 0x73 to activate them)
#define CMD_R_RX_PL_WID			0x60	// Read RX-payload width command when use the Dynamic Payload Length (DPL)
#define CMD_W_ACK_PAYLOAD		0xA8	// Write payload to be used in ACK packet on pipe PPP (VA SEGUIT DEL NUMERO DE PIPE)
#define CMD_W_TX_PAYLOAD_NOACK	0xB0	// Used in TX mode, Disable AUTOACK on this specific packet
#define CMD_NOP    				0xFF 	// Define No Operation, might be used to read status register


 //nRF24L01 REGISTERNS (addresses)
// ---------------------------------
#define REG_CONFIG      		0x00	// 'Config' register address
#define REG_EN_AA       		0x01	// 'Enable Auto Acknowledgment' register address
#define REG_EN_RXADDR   		0x02    // 'Enabled RX addresses' register address
#define REG_SETUP_AW    		0x03    // 'Setup address width' register address
#define REG_SETUP_RETR  		0x04    // 'Setup Auto. Retrans' register address
#define REG_RF_CH       		0x05    // 'RF channel' register address
#define REG_RF_SETUP    		0x06 	// 'RF setup' register address
#define REG_STATUS      		0x07 	// 'Status' register address
#define REG_OBSERVE_TX  		0x08 	// 'Observe TX' register address
#define REG_CD          		0x09 	//'Carrier Detect' register address
#define REG_RX_ADDR_P0  		0x0A	// 'RX address pipe0' register address
#define REG_RX_ADDR_P1  		0x0B 	// 'RX address pipe1' register address
#define REG_RX_ADDR_P2  		0x0C 	// 'RX address pipe2' register address
#define REG_RX_ADDR_P3  		0x0D 	// 'RX address pipe3' register address
#define REG_RX_ADDR_P4  		0x0E 	// 'RX address pipe4' register address
#define REG_RX_ADDR_P5  		0x0F	// 'RX address pipe5' register address
#define REG_TX_ADDR     		0x10	// 'TX address' register address
#define REG_RX_PW_P0    		0x11	// 'RX payload width, pipe0' register address
#define REG_RX_PW_P1    		0x12	// 'RX payload width, pipe1' register address
#define REG_RX_PW_P2    		0x13	// 'RX payload width, pipe2' register address
#define REG_RX_PW_P3    		0x14 	// 'RX payload width, pipe3' register address
#define REG_RX_PW_P4    		0x15 	// 'RX payload width, pipe4' register address
#define REG_RX_PW_P5    		0x16 	// 'RX payload width, pipe5' register address
#define REG_FIFO_STATUS 		0x17 	// 'FIFO Status Register' register address
#define REG_DYNPD				0x1C	// Enable dynamic payload length (DPL)  (activar regitre amb el CMD_ACTIVATE)
#define REG_FEATURE 			0x1D	// Feature Register (activar regitre amb el CMD_ACTIVATE)

 // Bit Mnemonics
 #define MASK_RX_DR		6
 #define MASK_TX_DS		5
 #define MASK_MAX_RT	4
 #define EN_CRC     	3
 #define CRCO       	2
 #define PWR_UP     	1
 #define PRIM_RX    	0
 #define ENAA_P5    	5
 #define ENAA_P4    	4
 #define ENAA_P3    	3
 #define ENAA_P2   		2
 #define ENAA_P1    	1
 #define ENAA_P0    	0
 #define ERX_P5     	5
 #define ERX_P4     	4
 #define ERX_P3     	3
 #define ERX_P2     	2
 #define ERX_P1     	1
 #define ERX_P0     	0
 #define AW         	0
 #define ARD        	4
 #define ARC        	0
 #define PLL_LOCK   	4
 #define RF_DR      	3
 #define RF_PWR    		1
 #define LNA_HCURR   	0
 #define RX_DR      	6
 #define TX_DS       	5
 #define MAX_RT      	4
 #define RX_P_NO     	1
 #define TX_FULL     	0
 #define PLOS_CNT    	4
 #define ARC_CNT     	0
 #define TX_REUSE    	6
 #define FIFO_FULL   	5
 #define TX_EMPTY    	4
 #define RX_FULL     	1
 #define RX_EMPTY    	0
 #define DPL_P0			0
 #define DPL_P1			1
 #define DPL_P2			2
 #define DPL_P3			3
 #define DPL_P4			4
 #define DPL_P5			5
 #define EN_DPL			2
 #define EN_ACK_PAY		1
 #define EN_DYN_ACK		0


// MRF24 Settings
#define NRF24_CH		        40
#define NRF24_ADR_WIDTH   		5   // 5-byte per la adreca de transmissio / recepcio
#define NRF24_NUM_PIPES			6   // Num of rx pipes chip has
#define NRF24_PLOAD_WIDTH		32  // Ancho efectivo del canal TX/RX de datos (cuando va sin DPL)
#define NRF24_NUM_RETRANS		15	// Maximas retransmisiones de el paquete si no recibe el Auto-ACK (de 0 a 15)
#define NRF24_CONFIG_OPT		((1<<MASK_TX_DS) | (1<<MASK_MAX_RT) | (1<<EN_CRC)) // No TX IRQs, RX IRQ, Enable CRC, CRC of 1 bytes


// NRF24 timeout waiting the TX_DS or MAX_RT flag when is transmitting
#define NRF24_SEND_TIMEOUT	((uint32_t)10000)


// RX data receiveing mode
//#define NRF24_RX_IRQ_MODE  // Uncomment to use the NRF24 with EXTI IRQ

// Operation Response
typedef enum {
	NRF24_MODE_RX		 =  0, // Mode PowerUo Receiving
	NRF24_MODE_TX 		 =  1, // Mode PowerUp Transmition (when TX FIFO is free then is in Standby-II mode)
	NRF24_MODE_STANDBYI  =  2, // Standby-I mode (CS line to low)
	NRF24_MODE_POWERDOWN =  3  // Is transmiting
} NR24Mode_TypeDef;


// Pipe Enumeration List
typedef enum {
	NRF24_PIPE0	=  0,
	NRF24_PIPE1	=  1,
	NRF24_PIPE2	=  2,
	NRF24_PIPE3	=  3,
	NRF24_PIPE4	=  4,
	NRF24_PIPE5	=  5
} NR24Pipe_TypeDef;


// Operation Response
typedef enum {
	NRF24_SUCCESS	=  0, // OK!
	NRF24_MAXRT 	=  1, // Reach the maximum retransmitions (NO AACK received in a TX)
	NRF24_ERROR 	=  2, // Global error
	NRF24_TRANSMIT 	=  3  // Is transmiting
} NR24Res_TypeDef;



// F U N C T I O N S
// -----------------------------------------

// Initialization functions
uint8_t NRF24_Init(SPI_HandleTypeDef *hspi);
uint8_t NRF24_Config(void);

// RX Pipes configuration functions
uint8_t NRF24_EnablePipeRX(NR24Pipe_TypeDef pipe, uint8_t *rxAddr);
uint8_t NRF24_DisablePipeRX(NR24Pipe_TypeDef pipe);

// Communication functions
uint8_t NRF24_TransferByte(uint8_t data);
uint8_t NRF24_WriteCMD(uint8_t cmd, uint8_t value);
uint8_t NRF24_ReadCMD(uint8_t cmd);
uint8_t NRF24_WriteLargeCMD(uint8_t cmd, uint8_t *pBuf, uint8_t bytes);
uint8_t NRF24_ReadLargeCMD(uint8_t cmd, uint8_t *pBuf, uint8_t bytes);
uint8_t NRF24_ReadStatus(void);
void NRF24_WriteByte(uint8_t value);

// Mode configuration functions
NR24Mode_TypeDef NRF24_GetMode(void);
void NRF24_Mode(NR24Mode_TypeDef mode);

// Wireless communication functions
uint8_t NRF24_isRecivData(NR24Pipe_TypeDef pipe);
uint8_t NRF24_ReadData(NR24Pipe_TypeDef pipe, uint8_t *data, uint8_t *len);
uint8_t NRF24_SendData(uint8_t *txAddr, uint8_t *data, uint8_t len);
void NRF24_observeTX(uint8_t *numretrans, uint8_t *totalpacketlost);
void NRF24_CheckDataRX(void);

void NRF24_IRQ_Callback(void);

#ifdef __cplusplus
}
#endif

#endif /*_NRF24L01_H*/

