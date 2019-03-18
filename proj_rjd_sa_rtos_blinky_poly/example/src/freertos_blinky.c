/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define LED_ON		false
#define LED_OFF		true

#define LED_ON_TIME	4e6

static xTaskHandle redTaskHandle, greenTaskHandle, blueTaskHandle;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void prfDelayLoop(uint32_t delayCnt) {
	for (int i = 0; i < delayCnt; i++);
}

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

    // Set the red LED to the state of "On"
    Board_LED_Set(LED_RED, LED_ON);
    // Set the green and blue LEDs to the state of "Off"
    Board_LED_Set(LED_GREEN, LED_OFF);
    Board_LED_Set(LED_BLUE, LED_OFF);
}

/* Red LED toggle thread */
static void vLEDTask(void *pvParameters) {
	rgb_led_t *taskLed = (rgb_led_t *)pvParameters;

	while (1) {
		Board_LED_Set(*taskLed, LED_ON);

		/* wait with LED on */
		prfDelayLoop(LED_ON_TIME);

		Board_LED_Set(*taskLed, LED_OFF);

		/* wait for other LEDs periods */
		prfDelayLoop(LED_ON_TIME);

		switch(*taskLed) {
		case LED_RED: {
			taskENTER_CRITICAL();
			vTaskPrioritySet(greenTaskHandle, (tskIDLE_PRIORITY + 2UL));
			vTaskPrioritySet(redTaskHandle, (tskIDLE_PRIORITY + 1UL));
			taskEXIT_CRITICAL();
			break;
		}
		case LED_GREEN: {
			taskENTER_CRITICAL();
			vTaskPrioritySet(blueTaskHandle, (tskIDLE_PRIORITY + 2UL));
			vTaskPrioritySet(greenTaskHandle, (tskIDLE_PRIORITY + 1UL));
			taskEXIT_CRITICAL();
			break;
		}
		case LED_BLUE: {
			taskENTER_CRITICAL();
			vTaskPrioritySet(redTaskHandle, (tskIDLE_PRIORITY + 2UL));
			vTaskPrioritySet(blueTaskHandle, (tskIDLE_PRIORITY + 1UL));
			taskEXIT_CRITICAL();
			break;
		}
		}
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{
	static rgb_led_t redTaskLed, greenTaskLed, blueTaskLed;
	prvSetupHardware();

	/* Red LED toggle thread */
	redTaskLed = LED_RED;
	xTaskCreate(vLEDTask, (signed char *) "vTaskLedRed",
				configMINIMAL_STACK_SIZE, &redTaskLed, (tskIDLE_PRIORITY + 2UL),
				&redTaskHandle);

	/* Green LED toggle thread */
	greenTaskLed = LED_GREEN;
	xTaskCreate(vLEDTask, (signed char *) "vTaskLedGreen",
				configMINIMAL_STACK_SIZE, &greenTaskLed, (tskIDLE_PRIORITY + 1UL),
				&greenTaskHandle);

	/* Blue LED toggle thread */
	blueTaskLed = LED_BLUE;
	xTaskCreate(vLEDTask, (signed char *) "vTaskLedBlue",
				configMINIMAL_STACK_SIZE, &blueTaskLed, (tskIDLE_PRIORITY + 1UL),
				&blueTaskHandle);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
