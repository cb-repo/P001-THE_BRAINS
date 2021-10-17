/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "LED.h"
#include "GPIO.h"
#include "TIM.h"

/*
 * PRIVATE DEFINITIONS
 */

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

/*
 * PRIVATE VARIABLES
 */

/*
 * PUBLIC FUNCTIONS
 */

void LED_Init (void)
{
	GPIO_EnableOutput(LED_STATUS_GPIO, LED_STATUS_PIN, false);
}

void LED_StatusON (void)
{
	GPIO_Write(LED_STATUS_GPIO, LED_STATUS_PIN, true);
}

void LED_StatusOFF (void)
{
	GPIO_Write(LED_STATUS_GPIO, LED_STATUS_PIN, false);
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
