/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "LED.h"
#include "GPIO.h"
#include "Core.h"

/*
 * PRIVATE DEFINITIONS
 */

#define PULSEON 		200
#define PULSEOFF		300

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
	GPIO_EnableOutput(LED_FAULT_GPIO, LED_FAULT_PIN, false);
}

void LED_GreenON (void)
{
	GPIO_Write(LED_STATUS_GPIO, LED_STATUS_PIN, true);
}

void LED_RedON (void)
{
	GPIO_Write(LED_FAULT_GPIO, LED_FAULT_PIN, true);
}

void LED_GreenOFF (void)
{
	GPIO_Write(LED_STATUS_GPIO, LED_STATUS_PIN, false);
}

void LED_RedOFF (void)
{
	GPIO_Write(LED_FAULT_GPIO, LED_FAULT_PIN, false);
}

bool LED_GreenState (void)
{
	return GPIO_Read(LED_STATUS_GPIO, LED_STATUS_PIN);
}

bool LED_RedState (void)
{
	return GPIO_Read(LED_FAULT_GPIO, LED_FAULT_PIN);
}

void LED_Pulse (void)
{
	uint32_t tick = CORE_GetTick();
	LED_RedON();
	LED_GreenON();
	while (PULSEON > (CORE_GetTick() - tick)) { CORE_Idle(); }
	LED_RedOFF();
	LED_GreenOFF();
}

void LED_TriPulse (void)
{
	LED_nPulse(3);
}

void LED_nPulse (uint8_t n)
{
	uint32_t tick = CORE_GetTick();
	LED_GreenOFF();
	LED_RedOFF();
	while (PULSEOFF > (CORE_GetTick() - tick)) { CORE_Idle(); }

	for (uint8_t i = 0; i < n; i++)
	{
		LED_Pulse();
		tick = CORE_GetTick();
		while (PULSEOFF > (CORE_GetTick() - tick)) { CORE_Idle(); }
	}
}


/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
