/*
===============================================================================
 Name        : proj_rjd_sa_blinky.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#define LEDG_GPIO_PORT_NUM                      3
#define LEDG_GPIO_BIT_NUM                       25
#define LEDB_GPIO_PORT_NUM                      3
#define LEDB_GPIO_BIT_NUM                       26

#define TICKRATE_HZ1 (10)	/* 10 ticks per second */

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	static uint8_t ledNum = 0, hertzCnt = 0;

	if (hertzCnt++ >= 10) {
		Board_LED_Toggle(ledNum);
		if (++ledNum >= 3) {
			ledNum = 0;
		}
		hertzCnt = 0;
	}
}

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(LED_RED, true);
    Board_LED_Set(LED_GREEN, true);
    Board_LED_Set(LED_BLUE, true);

#endif
#endif

    // TODO: insert code here

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
