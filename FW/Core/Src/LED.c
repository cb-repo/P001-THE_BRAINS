/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "LED.h"
#include "GPIO.h"
#include "Core.h"

/*
 * PRIVATE DEFINITIONS
 */

#define PULSE100 		100

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void LED_GreenON (void);
void LED_RedON (void);
void LED_GreenOFF (void);
void LED_RedOFF (void);

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

void LED_Update (void)
{

}

void LED_Pulse (void)
{
	static uint32_t tick = CORE_GetTick();
	LED_RedON();
	LED_GreenON();
	while (PULSE100 > (CORE_GetTick() - tick)) { CORE_Idle(); }
	LED_RedOFF();
	LED_GreenOFF();
}

void LED_TriPulse (void)
{
	LED_Pulse();
	static tick = CORE_GetTick();
	while (PULSE100 > (CORE_GetTick() - tick)) { CORE_Idle(); }
	LED_Pulse();
	tick = CORE_GetTick();
	while (PULSE100 > (CORE_GetTick() - tick)) { CORE_Idle(); }
	LED_Pulse();
}


/*
 * PRIVATE FUNCTIONS
 */


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


/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
