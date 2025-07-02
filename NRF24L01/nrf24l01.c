/*
 * nrf24l01.c
 *
 *  Created on: Dec 5, 2012
 *      Author: Marcel
 *
 * Modified on: Dec 20, 2013
* Adapted to STM32CubeIDE: May 15, 2024
 *
 */
#include "nrf24l01.h"
#include "led.h"
#include <string.h>
#include "stm32f769i_discovery_lcd.h"

SPI_HandleTypeDef *hspi_nrf;

// RX data control variables
uint8_t NRF24_RecivData[NRF24_NUM_PIPES][NRF24_PLOAD_WIDTH];
uint8_t NRF24_isRecivedData[NRF24_NUM_PIPES];

// Status variables
NR24Mode_TypeDef NRF24_currentMode;
uint8_t NRF24_lastAddressTX[NRF24_ADR_WIDTH];
uint8_t NRF24_confOptions;

// Private prototype functions
static void NRF24_SPI_Error(void);
static void NRF24_SPI_Init(void);


// Initialization
uint8_t NRF24_Init(SPI_HandleTypeDef *hspi) {
	uint8_t result, i;

	// SPI handle link
	hspi_nrf = hspi;

	// Initialize RX flag variables
	for (i=0; i<NRF24_NUM_PIPES; i++) {
		NRF24_isRecivedData[i] = RESET; // No received data at start
	}

	// Default Initial Mode
	NRF24_currentMode = NRF24_MODE_POWERDOWN;

	// Save options for configuration register
	NRF24_confOptions = NRF24_CONFIG_OPT;
#ifndef NRF24_RX_IRQ_MODE
	NRF24_confOptions |= (1<<MASK_RX_DR); // Disable RX IRQ Generation
#endif

	// Chip Not active: CE Low
	NRF24_CE(0);

	// Time for assure the module is internally initialized
	HAL_Delay(100);

	// nRF24L01 Configuration
	result = NRF24_Config();

	return result;

}

// General NRF24L01 Configuration
// Sets the main registers in the NRF24 module and powers the module in receiving mode
uint8_t NRF24_Config(void) {
	uint8_t ch;

	NRF24_Mode(NRF24_MODE_STANDBYI);

	// Disable all RX pipes (configure it by specific function)
	NRF24_WriteCMD(CMD_W_REG | REG_EN_RXADDR, 0x00); // PIPE0-5 PRX=OFF
	NRF24_WriteCMD(CMD_W_REG | REG_EN_AA, 0x00);     // Disable PIPE0-5 AutoACK=0

	// General configuration
	NRF24_WriteCMD(CMD_W_REG | REG_SETUP_AW, (NRF24_ADR_WIDTH-2));			// TX/RX address size = 0x03 (0b11) = 5 bytes (have to subtract 2).
	NRF24_WriteCMD(CMD_W_REG | REG_RF_CH, NRF24_CH);    	            	// Select the RF frequency = 2400+40 Mhz.
	NRF24_WriteCMD(CMD_W_REG | REG_RF_SETUP, 0x02 /*0x26*/);    			// 0x26: 250kBps (bit 3 off and 5 on), TX max PWR = 0 dBm (bit 1 and 2 on), LNA (Low Noise Amplifier) Off (bit 0 off)
																			// The LNA gain makes it possible to reduce the current consumption in RX
																			// mode with 0.8mA at the cost of 1.5dB reduction in receiver sensitivity.
	NRF24_WriteCMD(CMD_W_REG | REG_SETUP_RETR, 0x10 | NRF24_NUM_RETRANS);	// Time between retransmissions = 500us, Max. retransmission = 15 times


	// Check if SPI is working fine
	ch = NRF24_ReadCMD(CMD_R_REG | REG_RF_CH);
	if (ch != NRF24_CH) {
		return NRF24_ERROR;
	}

	// Clear the FIFO buffers and flags
	NRF24_WriteByte(CMD_FLUSH_TX); // This command doesn't need a data byte
	NRF24_WriteByte(CMD_FLUSH_RX); // This command doesn't need a data byte

	// IRQ flag clean
	NRF24_WriteCMD(CMD_W_REG | REG_STATUS, (1<<RX_DR));
	NRF24_WriteCMD(CMD_W_REG | REG_STATUS, (1<<TX_DS));
	NRF24_WriteCMD(CMD_W_REG | REG_STATUS, (1<<MAX_RT));


	// Enable Chip => Ready to receive data
	NRF24_Mode(NRF24_MODE_RX);

	return NRF24_SUCCESS;
}


uint8_t NRF24_EnablePipeRX(NR24Pipe_TypeDef pipe, uint8_t *rxAddr) {
	uint8_t previousMode;
	uint8_t value;
	uint8_t data[NRF24_ADR_WIDTH];
	uint8_t REG_PLOAD_WIDTH[NRF24_NUM_PIPES]= {REG_RX_PW_P0, REG_RX_PW_P1, REG_RX_PW_P2, \
											   REG_RX_PW_P3, REG_RX_PW_P4, REG_RX_PW_P5};

	uint8_t REG_RX_ADDR[NRF24_NUM_PIPES]= {REG_RX_ADDR_P0, REG_RX_ADDR_P1, REG_RX_ADDR_P2, \
										   REG_RX_ADDR_P3, REG_RX_ADDR_P4, REG_RX_ADDR_P5};

	// Get the current Mode
	previousMode = NRF24_GetMode();

	// Standby-I Mode
	NRF24_Mode(NRF24_MODE_STANDBYI);


	// Enable the selected pipe
	value = NRF24_ReadCMD(CMD_R_REG | REG_EN_RXADDR);
	NRF24_WriteCMD(CMD_W_REG | REG_EN_RXADDR, value | (1<<pipe));

	// Enable AutoACK for the selected pipe
	value = NRF24_ReadCMD(CMD_R_REG | REG_EN_AA);
	NRF24_WriteCMD(CMD_W_REG | REG_EN_AA, value | (1<<pipe));

	// Set the PayLoad length
	NRF24_WriteCMD(CMD_W_REG | REG_PLOAD_WIDTH[pipe], NRF24_PLOAD_WIDTH);

	// Configure the address
	if (pipe == NRF24_PIPE0 || pipe == NRF24_PIPE1) {
		// Write complete 5-byte addr
		NRF24_WriteLargeCMD(CMD_W_REG | REG_RX_ADDR[pipe], rxAddr, NRF24_ADR_WIDTH);

	} else {
		// Update the 4 last bytes of addr P1 (Warning! it changes also the 4 last bytes of P1, P2, P3, P4, P5)
		// 4 last addr bytes of P1, P2, P3, P4, P5 must be the same
		NRF24_ReadLargeCMD(CMD_R_REG | REG_RX_ADDR_P1, data, NRF24_ADR_WIDTH);
		data[1] = rxAddr[1];
		data[2] = rxAddr[2];
		data[3] = rxAddr[3];
		data[4] = rxAddr[4];
		NRF24_WriteLargeCMD(CMD_W_REG | REG_RX_ADDR_P1, data, NRF24_ADR_WIDTH);


		// Store the first addr byte
		NRF24_WriteLargeCMD(CMD_W_REG | REG_RX_ADDR[pipe], &rxAddr[0], 1);

	}

	// Go to previous mode
	NRF24_Mode(previousMode);

	return NRF24_SUCCESS;
}


uint8_t NRF24_DisablePipeRX(NR24Pipe_TypeDef pipe) {
	uint8_t previousMode;
	uint8_t value;

	// Get the current Mode
	previousMode = NRF24_GetMode();

	// Standby-I Mode
	NRF24_Mode(NRF24_MODE_STANDBYI);

	// Read Enable RX Pipe register
	value = NRF24_ReadCMD(CMD_R_REG | REG_EN_RXADDR);

	// Update the pipe enabled register
	// Put '0' to specific position (disable specific rx pipe)
	NRF24_WriteCMD(CMD_W_REG | REG_EN_RXADDR, value & (~(1<<pipe)) );

	// Go to previous mode
	NRF24_Mode(previousMode);

	return NRF24_SUCCESS;
}


NR24Mode_TypeDef NRF24_GetMode(void) {
	return NRF24_currentMode;
}

void NRF24_Mode(NR24Mode_TypeDef mode) {

	if (NRF24_currentMode != mode) {

		switch (mode) {

			// Power down mode
			case NRF24_MODE_POWERDOWN:
				NRF24_CE(0);
				NRF24_WriteCMD(CMD_W_REG | REG_CONFIG, NRF24_confOptions |  (0<<PWR_UP) | (0<<PRIM_RX));
				break;

			// Standby-I mode
			case NRF24_MODE_STANDBYI:
				NRF24_CE(0);

				if (NRF24_currentMode == NRF24_MODE_POWERDOWN){
					NRF24_WriteCMD(CMD_W_REG | REG_CONFIG, NRF24_confOptions |  (1<<PWR_UP) | (0<<PRIM_RX));
					HAL_Delay(2);
				}
				break;

			// RX mode
			case NRF24_MODE_RX:
				if (NRF24_currentMode != NRF24_MODE_POWERDOWN && NRF24_currentMode != NRF24_MODE_STANDBYI) {
					NRF24_CE(0);
				}

				NRF24_WriteCMD(CMD_W_REG | REG_CONFIG, NRF24_confOptions |  (1<<PWR_UP) | (1<<PRIM_RX));

				if (NRF24_currentMode == NRF24_MODE_POWERDOWN) {
					HAL_Delay(5); // Wait some time to wake up from power down
				}

				NRF24_CE(1);
				break;

			// TX mode
			case NRF24_MODE_TX:

				if (NRF24_currentMode != NRF24_MODE_POWERDOWN && NRF24_currentMode != NRF24_MODE_STANDBYI) {
					NRF24_CE(0);
				}

				NRF24_WriteCMD(CMD_W_REG | REG_CONFIG, NRF24_confOptions |  (1<<PWR_UP) | (0<<PRIM_RX));

				if (NRF24_currentMode == NRF24_MODE_POWERDOWN) {
					HAL_Delay(5); // Wait some time to wake up from power down
				}

				NRF24_CE(1);
				break;

		}

		// Update the flag to new mode
		NRF24_currentMode = mode;
	}
}


uint8_t NRF24_isRecivData(NR24Pipe_TypeDef pipe) {
	return NRF24_isRecivedData[pipe];
}


uint8_t NRF24_ReadData(NR24Pipe_TypeDef pipe, uint8_t *data, uint8_t *len) {

	if (NRF24_isRecivedData[pipe] == RESET) return NRF24_ERROR;

	// In this implementation always receives and sends 32-bit packet sizes (NRF24_PLOAD_WIDTH)
	*len = NRF24_PLOAD_WIDTH;

	// Copy the buffer to the external array
	memcpy(data, &NRF24_RecivData[pipe][0], *len);


	// Releases the received data flag
	NRF24_isRecivedData[pipe] = RESET;

	return NRF24_SUCCESS;
}

uint8_t NRF24_SendData(uint8_t *txAddr, uint8_t *data, uint8_t len) {
	uint32_t timeout;
	uint8_t previousMode;
	uint8_t result, status;
	uint8_t addrP0[NRF24_ADR_WIDTH];
	uint8_t oldregAA, oldregENP;

	// 32 bytes maximum (a packet). It could be improved sending as packets as data we have.
	if (len > NRF24_PLOAD_WIDTH) {
		return NRF24_ERROR;
	}

	// Get the current Mode
	previousMode = NRF24_GetMode();

	// Standby-I Mode
	NRF24_Mode(NRF24_MODE_STANDBYI);

	// Set the TX address if different from previous
	if (memcmp(txAddr, NRF24_lastAddressTX, NRF24_ADR_WIDTH) != 0) {

		// Stores the last TX address
		memcpy(NRF24_lastAddressTX, txAddr,NRF24_ADR_WIDTH);

		// Updates the TX address
		NRF24_WriteLargeCMD(CMD_W_REG | REG_TX_ADDR, (uint8_t*)txAddr, NRF24_ADR_WIDTH);
	}

	// Configure for receive AACK
	// For AutoACK:     pipe0 Enabled, pipe0 AACK enabled, pipe0 RX addr = TX addr

		// Enable of pipe0
		oldregENP = NRF24_ReadCMD(CMD_R_REG | REG_EN_RXADDR);
		NRF24_WriteCMD(CMD_W_REG | REG_EN_RXADDR, oldregENP | (1<<ERX_P0));

		// Enable AutoACK for pipe0
		oldregAA = NRF24_ReadCMD(CMD_R_REG | REG_EN_AA);
		NRF24_WriteCMD(CMD_W_REG | REG_EN_AA,  oldregAA | (1<<ENAA_P0));

		// Save current pipe0 address for restore after
		NRF24_ReadLargeCMD(CMD_R_REG | REG_RX_ADDR_P0, addrP0, NRF24_ADR_WIDTH);

		// Set RX PIPE0 Addr = TX Addr for receive the Auto-ACK
		NRF24_WriteLargeCMD(CMD_W_REG | REG_RX_ADDR_P0, txAddr, NRF24_ADR_WIDTH);


	// Flush TX FIFO
	NRF24_WriteByte(CMD_FLUSH_TX);

	// Fill the TX PAYLOAD
	NRF24_WriteLargeCMD(CMD_WR_TX_PLOAD, data, NRF24_PLOAD_WIDTH);

	// Enable Chip => TX Mode => Start transmission
	NRF24_Mode(NRF24_MODE_TX);


	// Wait to receive the TX_DS or MAX_RT flag (without interrupt, disabled)
	timeout = NRF24_SEND_TIMEOUT;
	status = NRF24_ReadStatus();
	while(!(status & (1<<TX_DS)) && !(status & (1<<MAX_RT))) {
		if((timeout--) == 0) break;
		status = NRF24_ReadStatus();
	}

	// TX Finish OK
	if(status & (1<<TX_DS)) {
		BSP_LED_Toggle(LED3);
		result = NRF24_SUCCESS;

	// Not ACK received (reached maximum retransmissions)
	} else if (status & (1<<MAX_RT)) {
		result = NRF24_MAXRT;

	// Internal protocol error (software timeout, timeout = 0)
	} else {
		result = NRF24_ERROR;
	}


	// Standby-I Mode
	NRF24_Mode(NRF24_MODE_STANDBYI);

	// Clear the flags related with TX
	NRF24_WriteCMD(CMD_W_REG | REG_STATUS, (1<<TX_DS) | (1<<MAX_RT));


	// Leaves the pipe0 RX in its previous state

		// Restore Enable/Disable of pipe0
		NRF24_WriteCMD(CMD_W_REG | REG_EN_RXADDR, oldregENP);

		// Restore AutoACK for pipe0
		NRF24_WriteCMD(CMD_W_REG | REG_EN_AA,  oldregAA);

		// Restore RX PIPE0 Addr
		NRF24_WriteLargeCMD(CMD_W_REG | REG_RX_ADDR_P0, addrP0, NRF24_ADR_WIDTH);


	// Flush TX FIFO
	NRF24_WriteByte(CMD_FLUSH_TX);


	// Return previous mode;
	NRF24_Mode(previousMode);


	return result;
}

void NRF24_observeTX(uint8_t *numretrans, uint8_t *totalpacketlost) {
	uint8_t observe;

	observe = NRF24_ReadCMD(CMD_R_REG | REG_OBSERVE_TX);
	*totalpacketlost= observe >> 4;  // Total packets lost since the beginning (resets when reach 15) (4 bits)
	*numretrans = observe & 0x0F;  // Number of retransmissions in the last TX (4 bits)
}


/* The RX_DR IRQ is asserted by a new packet arrival event.
 * The procedure for handling this interrupt should be:
 * 		1) read payload through SPI,
 * 		2) clear RX_DR IRQ,
 * 		3) read FIFO_STATUS to check if there are more payloads available in RX FIFO,
 * 		4) if there are more data in RX FIFO, repeat from 1
*/
void NRF24_CheckDataRX(void) {
	uint8_t status, previousMode;
	uint8_t rxPipe;
	uint8_t rxBuffer[NRF24_PLOAD_WIDTH];

	// Read NRF24 status register
	status = NRF24_ReadStatus();

	// RX data received
	if(status & (1<<RX_DR)) {
		BSP_LED_Toggle(LED3);

		// Get the current Mode
		previousMode = NRF24_GetMode();

		// Standby-I Mode
		NRF24_Mode(NRF24_MODE_STANDBYI);


		// Get the pipe number (bits 1 to 3 of status)
		rxPipe = (status >> 1) & 0b00000111;

		// Check if RX FIFO is empty
		while(rxPipe != 0x07) {

			// Read RX payload
			NRF24_ReadLargeCMD(CMD_RD_RX_PLOAD, rxBuffer, NRF24_PLOAD_WIDTH);


			// Copy the data to the external buffer if it is free
			if (NRF24_isRecivedData[rxPipe] == RESET) {
				memcpy(&NRF24_RecivData[rxPipe][0], rxBuffer, NRF24_PLOAD_WIDTH);
				NRF24_isRecivedData[rxPipe] = SET;
			}


			// Clear flag  RX_DR
			NRF24_WriteCMD(CMD_W_REG | REG_STATUS, (1<<RX_DR));


			// Read the status reg and get the next pipe number (bits 1 to 3 of status)
			// If it is 0x07 the RX FIFO is empty, oterwise it has the pipe number
			status = NRF24_ReadStatus();
			rxPipe = (status >> 1) & 0b00000111;
		}


		// Return previous mode;
		NRF24_Mode(previousMode);

	}

}


//------------------------------------------------------------------------------//
// BSP COMMUNICATION FUNCTIONS
//------------------------------------------------------------------------------//

// Clocks only one byte to target device and returns the received one
uint8_t NRF24_TransferByte(uint8_t data) {
	  uint8_t receivedbyte = 0;

	  // Send a Byte through the SPI peripheral
	  // and receives a byte at the same time (Full-Duplex)
	  if(HAL_SPI_TransmitReceive(hspi_nrf, (uint8_t*) &data, (uint8_t*) &receivedbyte, 1, NRF24_SEND_TIMEOUT) != HAL_OK) {
		  NRF24_SPI_Error();
	  }

	  return receivedbyte;
}

// Writes a value of a command (1 byte of cmd + 1 byte of data)
uint8_t NRF24_WriteCMD(uint8_t cmd, uint8_t value) {
	uint8_t status;
	NRF24_CSN(0);
	status = NRF24_TransferByte(cmd); // (The first byte returns always the value of the STATUS register)
	NRF24_TransferByte(value);
	NRF24_CSN(1);
	return status;
}

// Reads a value of a command (1 byte of cmd + 1 byte of data)
uint8_t NRF24_ReadCMD(uint8_t cmd) {
	uint8_t value;
	NRF24_CSN(0);
	NRF24_TransferByte(cmd);
	value=NRF24_TransferByte(0xFF);
	NRF24_CSN(1);
	return value;
}

// Writes an array of values of a command (1 byte of cmd + n bytes of data)
uint8_t NRF24_WriteLargeCMD(uint8_t cmd, uint8_t *pBuf, uint8_t bytes) {
	uint8_t status,byte_ctr;
	NRF24_CSN(0);
	status = NRF24_TransferByte(cmd); // (The first byte returns always the value of the STATUS register)
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		NRF24_TransferByte(*pBuf++);
	NRF24_CSN(1);
	return(status);
}

// Reads an array of values of a command (1 byte of cmd + n bytes of data)
uint8_t NRF24_ReadLargeCMD(uint8_t cmd, uint8_t *pBuf, uint8_t bytes) {
	uint8_t status, byte_ctr;
	NRF24_CSN(0);
	status = NRF24_TransferByte(cmd); // El primer byte retorna per defecte el valor del reg. STATUS
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)
		pBuf[byte_ctr] = NRF24_TransferByte(0);
	NRF24_CSN(1);
	return(status);
}

// Sends only one byte (no command = 0xFF) and returns the STATUS register value
uint8_t NRF24_ReadStatus(void) {
	uint8_t status;
	NRF24_CSN(0);
	status = NRF24_TransferByte(0xFF);
	NRF24_CSN(1);
	return(status);
}


// Sends only one byte by SPI
void NRF24_WriteByte(uint8_t value) {
	NRF24_CSN(0);
	NRF24_TransferByte(value);
	NRF24_CSN(1);
}


// SPI Error treatment function.
static void NRF24_SPI_Error(void) {
	// De-initialize the SPI communication bus
	HAL_SPI_DeInit(hspi_nrf);

	// Re-Initialize the SPI communication bus
	NRF24_SPI_Init();
}


// SPI re-initialization after bus error
static void NRF24_SPI_Init(void) {
	if(HAL_SPI_GetState(hspi_nrf) == HAL_SPI_STATE_RESET) {
		// SPI configuration
		hspi_nrf->Init.Mode = SPI_MODE_MASTER;
		hspi_nrf->Init.Direction = SPI_DIRECTION_2LINES;
		hspi_nrf->Init.DataSize = SPI_DATASIZE_8BIT;
		hspi_nrf->Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi_nrf->Init.CLKPhase = SPI_PHASE_1EDGE;
		hspi_nrf->Init.NSS = SPI_NSS_SOFT;
		hspi_nrf->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
		hspi_nrf->Init.FirstBit = SPI_FIRSTBIT_MSB;
		hspi_nrf->Init.TIMode = SPI_TIMODE_DISABLE;
		hspi_nrf->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		hspi_nrf->Init.CRCPolynomial = 10;
		HAL_SPI_Init(hspi_nrf);
	}
}

//------------------------------------------------------------------------------//
//------------------------------------------------------------------------------//

// Interrupt handler
// This function handles EXTI interrupt request.
void NRF24_IRQ_Callback(void) {
	BSP_LED_Toggle(LED4);

#ifdef NRF24_RX_IRQ_MODE
	NRF24_CheckDataRX();
#endif

}

