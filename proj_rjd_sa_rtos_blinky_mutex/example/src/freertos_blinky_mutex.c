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
#include "semphr.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define LED_ON		false
#define LED_OFF		true

#define LED_ON_TIME	(configTICK_RATE_HZ/2)

xSemaphoreHandle xLedSemaphore;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

    // Set the LEDs to the state of "Off"
    Board_LED_Set(LED_RED, LED_OFF);
    Board_LED_Set(LED_GREEN, LED_OFF);
    Board_LED_Set(LED_BLUE, LED_OFF);
}

/* Red LED toggle thread */
static void vLEDTask(void *pvParameters) {
	rgb_led_t *taskLed = (rgb_led_t *)pvParameters;

	while (1) {
		xSemaphoreTake(xLedSemaphore, portMAX_DELAY);

		Board_LED_Set(*taskLed, LED_ON);

		/* wait with LED on */
		vTaskDelay(LED_ON_TIME);

		Board_LED_Set(*taskLed, LED_OFF);

		vTaskDelay(LED_ON_TIME);

		xSemaphoreGive(xLedSemaphore);

		/* wait for other LEDs periods */
		vTaskDelay(4*LED_ON_TIME);
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

	xLedSemaphore = xSemaphoreCreateMutex();

	if (xLedSemaphore != NULL) {
		/* Red LED toggle thread */
		redTaskLed = LED_RED;
		xTaskCreate(vLEDTask, (signed char *) "vTaskLedRed",
				configMINIMAL_STACK_SIZE, &redTaskLed, (tskIDLE_PRIORITY + 3UL),
				NULL);

		/* Green LED toggle thread */
		greenTaskLed = LED_GREEN;
		xTaskCreate(vLEDTask, (signed char *) "vTaskLedGreen",
				configMINIMAL_STACK_SIZE, &greenTaskLed, (tskIDLE_PRIORITY + 2UL),
				NULL);

		/* Blue LED toggle thread */
		blueTaskLed = LED_BLUE;
		xTaskCreate(vLEDTask, (signed char *) "vTaskLedBlue",
				configMINIMAL_STACK_SIZE, &blueTaskLed, (tskIDLE_PRIORITY + 1UL),
				NULL);

		/* Start the scheduler */
		vTaskStartScheduler();
	}

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
