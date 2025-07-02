/******************************************************************************
 * @file	app.c
 * @author  UdL
 * @version V1.0.0
 * @date    1-Jan-2021
 * @brief   This file contains the body of the main application.
 ******************************************************************************
 * @attention
 *
 * <h2>&copy; Copyright (c) 2022 UDL. All rights reserved.</h2>
 *
 ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "app.h"
#include "main.h"
#include "led.h"
#include <string.h>
#include "pushbutton.h"
#include "uart_debug.h"
#include "stdlib.h" // For atoi()
#include "stdio.h"    	// for printf()/sprintf()
#include "arm_math.h" // for float32_t
#include "stringcmd.h"  // string user functions
#include "nrf24l01.h" // NRF24 transceiver

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_sdram.h"

#define PEANUT_GB_HIGH_LCD_ACCURACY 0
#include "../Peanut-GB/peanut_gb.h"

extern DMA2D_HandleTypeDef hdma2d;

// Adding static const in front so that the ROM data
// ends up read-only in flash memory.
static const
#include "gameboy_rom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//HDMI
#define LAYER0_ADDRESS               (LCD_FB_START_ADDRESS)


// Address definitions NRF24L01
char ADDR_1[5]="ADR11";
char ADDR_2[5]="BDR11";
char ADDR_3[5]="ADR22";
char ADDR_4[5]="BDR22";

char *ADDR_TX_1;
char *ADDR_TX_2;

char *ADDR_RX_P2;
char *ADDR_RX_P3;


//GAMEBOY
#define BPP 8 /**< bytes/pixel */
#define COLOR(r,g,b)   ((uint32_t)(0xff000000 | ((r) << 16) | ((g) << 8) | (b)))
#define BACKGROUNDCOLOR  COLOR(255,255,255)
#define BORDERCOLOR1    COLOR(128,128,128)
#define BORDERCOLOR2    COLOR(80,80,80)
#define BUTTONCOLOR     COLOR(0x88,0x0f,0x42)
#define BUTTONCOLOR_ON  COLOR(0xff,0x3f,0x82)

#define GBCOLOR1        COLOR(155,188,15)  // COLOR(255,255,255)
#define GBCOLOR2        COLOR(139,172,15)  // COLOR(0xA5,0xA5,0xA5)
#define GBCOLOR3        COLOR(48,98,48)    // COLOR(0x52,0x52,0x52)
#define GBCOLOR4        COLOR(15,56,15)    // COLOR(0,0,0)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO int players[4] = {0,0,0,0};
__IO uint32_t t_falling[4] = {0,0,0,0};
__IO int state[4] = {0,0,0,0};
__IO int sleep[4] = {0,0,0,0};
static int comando[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int comando_wait[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int x[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

__IO uint32_t p_m = 0;

//GAMEBOY
static int LCD_LAYER_FRONT; // active display layer (front buffer)
static int LCD_LAYER_BACK;
static uint32_t* g_fb[2];

// setup game boy
 struct gb_s gb;
 struct priv_t priv; // pass-through data for peanut
 uint8_t g_ram[0x20000]; // excessive, but should work in every case


//HDMI
static uint32_t LCD_X_Size = 0;
static uint32_t LCD_Y_Size = 0;
static uint16_t scale_factor;
static uint16_t x_offset;
static uint16_t y_offset;

/* Private function prototypes -----------------------------------------------*/
void render(const float dt_sec, float forward, float left);
void clearFrameBuffer(void);

static void sleep_f(uint32_t delayMs);

/* Private user code ---------------------------------------------------------*/

// Main application Initialization
void APP_Init(void) {

	  // Printf redirected to USART2 send data
      printf("Init Ok! waiting commands...\r");
	  printf(">BOARD 1<\r");
	  ADDR_TX_1 = ADDR_3;
	  ADDR_TX_2 = ADDR_4;


	  ADDR_RX_P2 = ADDR_1;
	  ADDR_RX_P3 = ADDR_2;

	  // Prepare the RX pipes (as example, use pipe 2 and 3)
	  NRF24_DisablePipeRX(NRF24_PIPE1);
	  NRF24_EnablePipeRX(NRF24_PIPE2, (uint8_t *)ADDR_RX_P2);
	  NRF24_DisablePipeRX(NRF24_PIPE3);
	  NRF24_DisablePipeRX(NRF24_PIPE4);
	  NRF24_DisablePipeRX(NRF24_PIPE5);

}

// Application Task
void APP_Task(void) {
	char cmd[50];
	char cmd2[2];
	char transponse[40] = "";

	uint8_t len;
	uint8_t retrans, pktslost;
	uint32_t i;

	#ifndef NRF24_RX_IRQ_MODE
		NRF24_CheckDataRX();
	#endif

	#ifndef UART_RX_INTERRUPT
	  // Polling mode
	  UART_task();
	#endif

	// Check if an string is received
	if (UART_isDataRecived() == SET) {

		len = strlen(cmd);

	}

	//----------------------NRF24L01-CODE---------------------------------------------------
	// NRF24 Update (ONLY In Polling mode)
	NRF24_CheckDataRX();

	// Check if pending data to read
	if (NRF24_isRecivData(NRF24_PIPE2) == SET) {
		// Read pipe i received data
		NRF24_ReadData(NRF24_PIPE2, (uint8_t*)cmd, &len);
		//printf("Received %d bytes in the PIPE %d: --%s--\r", len, 1, cmd);
		strncpy(transponse,cmd,10);
		printf("%d\r", len);

		printf("NRF24 data: %s\r", transponse);

		if (strcmp(transponse, "NEW_PLAYER") == 0) {
			if (players[0] == 0) {
				players[0] = 1;
				strcpy(cmd2,"1");
				len = strlen(cmd2);
				NRF24_SendData((uint8_t *)ADDR_TX_1, (uint8_t*)cmd2, len+1);
				NRF24_observeTX(&retrans, &pktslost);
				//printf("Data TX Finish. Packets Lost: %2d, Retrans: %2d, Res: %d \r", pktslost, retrans, res);
				//NRF24_EnablePipeRX(NRF24_PIPE2, (uint8_t *)ADDR_RX_P2);
			}
		}else if(len == 32){
			for (i = 0; i < 4; i++){
				if (sleep[i] == 1) {

					sleep[i] = 0;
					players[i] = 0;
					//NRF24_DisablePipeRX(i+2);

				}
			}
			printf("%s\r",cmd);
			for (int l = 0; l < 20; l++) {
				if (strcmp(cmd[l], '0') == 0){
					comando[l] = 0;
				}else if(strcmp(cmd[l], '1') == 0){
					comando[l] = 1;
				}else{
					memset(comando, 0, sizeof(comando));
					memcpy(x, comando, sizeof(comando));
					return;
				}
			}

			int comparador = 0;
			for (int a = 0; a < 20; a++){
				if(comando[a] == comando_wait[a]){
					comparador += 1;
				}

			}

			if (comparador != 20) {

				memcpy(comando_wait, comando, sizeof(comando));
				memcpy(x, comando, sizeof(comando));
				return;
			}else{
				memset(comando, 0, sizeof(comando));
				memcpy(comando_wait, comando, sizeof(comando));
				memcpy(x, comando, sizeof(comando));
				return;
			}
		}
		// More tasks here
		memset(comando, 0, sizeof(comando));
		memcpy(x, comando, sizeof(comando));
	}

}
// Main task

void mainInit(void) {

    /* Inputs & LEDs */
    /* ------------- */
    BSP_LED_On(LED4); /* RED LED active until init is complete */

    /* Display setup */
    /* ------------- */
    BSP_LCD_Init();

	/* Get the LCD Width */
	LCD_X_Size = BSP_LCD_GetXSize();
	LCD_Y_Size = BSP_LCD_GetYSize();


	//scale_factor = floor(LCD_Y_Size/LCD_HEIGHT); // MAX FACTOR without loose the screen ratio
	scale_factor = 2;
	x_offset = floor((LCD_X_Size - (LCD_WIDTH*scale_factor))/2);
	y_offset = floor((LCD_Y_Size - (LCD_HEIGHT*scale_factor))/2);

    LCD_LAYER_FRONT = 1;
    LCD_LAYER_BACK = 0;
    g_fb[0] = (uint32_t*)LAYER0_ADDRESS;
    g_fb[1] = (uint32_t*)(LAYER0_ADDRESS + LCD_X_Size * LCD_Y_Size * BPP );
    BSP_LCD_LayerDefaultInit(0, (uint32_t)g_fb[0]);
    BSP_LCD_LayerDefaultInit(1, (uint32_t)g_fb[1]);
    BSP_LCD_SetLayerVisible(0, DISABLE);
    BSP_LCD_SetLayerVisible(1, ENABLE);
    BSP_LCD_SelectLayer(LCD_LAYER_BACK);

    /* Enable CPU cycle counter */
    /* ------------------------ */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    // access the cycle counter at: DWT->CYCCNT
}

void screen_flip_buffers(void)
{
  // wait for VSYNC
    while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) {}

    BSP_LCD_SetLayerVisible(LCD_LAYER_FRONT, DISABLE);
    LCD_LAYER_BACK = LCD_LAYER_FRONT;
    LCD_LAYER_FRONT ^= 1;
    BSP_LCD_SetLayerVisible(LCD_LAYER_FRONT, ENABLE);
    BSP_LCD_SelectLayer(LCD_LAYER_BACK);

}

/* Using the Systick 1000 Hz millisecond timer to sleep */
static void sleep_f(uint32_t delayMs)
{
    const uint32_t tickstart = HAL_GetTick();
    while((HAL_GetTick() - tickstart) < delayMs)
    {
        __WFE(); // save a bit of power while we are waiting
    }
}

// -----------------------------------------------------------------------
// GB specific
// -----------------------------------------------------------------------

struct priv_t
{
    uint8_t *rom; /* Pointer to allocated memory holding GB file. */
    uint8_t *cart_ram; /* Pointer to allocated memory holding save file. */
};

/**
 * Returns a byte from the ROM file at the given address.
 */
uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr)
{
    const struct priv_t * const p = gb->direct.priv;
    return p->rom[addr];
}

/**
 * Returns a byte from the cartridge RAM at the given address.
 */
uint8_t gb_cart_ram_read(struct gb_s *gb, const uint_fast32_t addr)
{
    const struct priv_t * const p = gb->direct.priv;
    return p->cart_ram[addr];
}

/**
 * Writes a given byte to the cartridge RAM at the given address.
 */
void gb_cart_ram_write(struct gb_s *gb, const uint_fast32_t addr,
               const uint8_t val)
{
    const struct priv_t * const p = gb->direct.priv;
    p->cart_ram[addr] = val;
}

/**
 * Ignore all errors.
 */
void gb_error(struct gb_s *gb, const enum gb_error_e gb_err, const uint16_t val)
{
    assert(0);
}

/**
 * Draws scanline into framebuffer.
 */
void lcd_draw_line(struct gb_s *gb, const uint8_t pixels[160],
           const uint_fast8_t line)
{
    // The Game Boy has 4 colors, map them to 32bit RGB colors
    const uint32_t palette[] = { GBCOLOR1, GBCOLOR2, GBCOLOR3, GBCOLOR4 };
    uint32_t* fb = g_fb[LCD_LAYER_BACK];

    uint32_t repeat_x, repeat_y;

    for(unsigned int x = 0; x < 160; x++)
    {
    	for(repeat_x=0; repeat_x < scale_factor; repeat_x++) {
    		for(repeat_y=0; repeat_y < scale_factor ; repeat_y++) {
    			fb[(line * scale_factor + repeat_y + y_offset) * LCD_X_Size + p_m + x_offset] = palette[pixels[x] & 3];
    		}
    		p_m++;
    	}
        /* mask with 3 there are other bits set */
    }
    p_m = 0;
    //160 x 144 es el tamaño original del emulador
}

#define SETBIT(w, m, f) if (f) { w |= m; } else { w &= ~m; }
static void checkbuttons(struct gb_s* gb)
{
	APP_Task();

    SETBIT(gb->direct.joypad, JOYPAD_A, x[0] != 1);
    SETBIT(gb->direct.joypad, JOYPAD_B, x[1] != 1);
    SETBIT(gb->direct.joypad, JOYPAD_START,  x[2] != 1);
    SETBIT(gb->direct.joypad, JOYPAD_SELECT, x[3] != 1);
	SETBIT(gb->direct.joypad, JOYPAD_UP, x[9] != 1);
    SETBIT(gb->direct.joypad, JOYPAD_DOWN, x[10] != 1);
    SETBIT(gb->direct.joypad, JOYPAD_LEFT, x[11] != 1);
    SETBIT(gb->direct.joypad, JOYPAD_RIGHT, x[12] != 1);

}

static void setupframebuffer(const struct gb_s* gb)
{
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
}

void mainTask(void)
{
    int frameTimeMs = 0; // current frametime in ms
    const int setpointframeTimeMs = 15;
    int lastfpsupdate = HAL_GetTick();
    int fps = 0;
    int fpscounter = 0;
    unsigned char fpstext[] = {'0', '0', 'f', 'p', 's', 0};
    char string[100];

    priv.rom = (uint8_t*)gameboy_rom_gb;
    priv.cart_ram = g_ram;

    gb_init(&gb, &gb_rom_read, &gb_cart_ram_read,
            &gb_cart_ram_write, &gb_error, &priv);

    // Make sure front & backbuffer are in the same state
    setupframebuffer(&gb);
    screen_flip_buffers();
    setupframebuffer(&gb);

    gb_init_lcd(&gb, &lcd_draw_line);

    BSP_LED_Off(LED4); /* init complete, clear RED LED */

    for(uint32_t epoch=0;;epoch++)
    {
        uint32_t tickStart = HAL_GetTick();

        checkbuttons(&gb);
        gb_run_frame(&gb);

        BSP_LCD_DisplayStringAt(0, 0, fpstext, LEFT_MODE); // 120 es la distancia de la pantalla al borde, 288 la largada de la pantalla

        if (HAL_GetTick() < 5000) {
			sprintf(string,"Scale factor: %d", scale_factor);
			BSP_LCD_DisplayStringAt(0, 15*1, (unsigned char*)string, LEFT_MODE);

			sprintf(string,"x Offset:%d", x_offset);
			BSP_LCD_DisplayStringAt(0, 15*2, (unsigned char*)string, LEFT_MODE);

			sprintf(string,"Screen resolution:%03ldx%03ld", LCD_X_Size, LCD_Y_Size);
			BSP_LCD_DisplayStringAt(0, 15*3, (unsigned char*)string, LEFT_MODE);

			sprintf(string,"Game resolution:%03dx%03d", LCD_WIDTH*scale_factor, LCD_HEIGHT*scale_factor);
			BSP_LCD_DisplayStringAt(0, 15*4, (unsigned char*)string, LEFT_MODE);
        }

        screen_flip_buffers(); /* swap backbuffer and frontbuffer */

        /* Sleep for the rest of the frame? */
        frameTimeMs = (int)(HAL_GetTick() - tickStart);
        int timeleftMs = setpointframeTimeMs - frameTimeMs;
        timeleftMs = timeleftMs > 0 ? timeleftMs : 0;
        sleep_f(timeleftMs);
        fpscounter++;

        if (HAL_GetTick() - lastfpsupdate >= 1000)
        {
            lastfpsupdate = HAL_GetTick();
            fps = fpscounter;
            fpscounter = 0;
            fpstext[0] = '0' + (fps/10) % 10;
            fpstext[1] = '0' + fps % 10;
        }
    }
}

/*************************************************************************
 * INTERRUPT CALLBACKS                                                   *
 *************************************************************************/

/**
  * @brief EXTI line detection callback.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin) {
    	case BUTTON_USER_PIN:
    		// Do here pushbutton actions
    		// BSP_LED_On(LED_RED);
    		break;

    	default:
    		break;
  }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */


  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();// Increments the user overflow counter

    for(int y = 0; y < 4; y++){

    	if(state[y] != 0){
    		t_falling[y] = uwTick ;
    	} else if(state[y] == 0 && players[y] == 1){
    		uint32_t t_press = uwTick - t_falling[y];

    		if(t_press > 50000){
    			sleep[y] = 1;
    		}
    	}
    }

  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}



