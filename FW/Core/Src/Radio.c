/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Radio.h"
#include "GPIO.h"
#include "Tim.h"
#include "Core.h"

/*
 * PRIVATE DEFINITIONS
 */

#define RADIO_THRESH		500

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void RADIO_SPWM1_IRQ (void);
void RADIO_SPWM2_IRQ (void);
void RADIO_SPWM3_IRQ (void);
void RADIO_SPWM4_IRQ (void);

/*
 * PRIVATE VARIABLES
 */

volatile uint16_t inputS1 = RADIO_CENTER;
volatile uint16_t inputS2 = RADIO_CENTER;
volatile uint16_t inputS3 = RADIO_CENTER;
volatile uint16_t inputS4 = RADIO_CENTER;
volatile uint32_t inputHeartbeat = 0;

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void)
{
	TIM_Init(TIM_RADIO, TIM_RADIO_FREQ, TIM_RADIO_RELOAD);
	TIM_Start(TIM_RADIO);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_Both, RADIO_SPWM1_IRQ);
	GPIO_OnChange(RADIO_S2_GPIO, RADIO_S2_PIN, GPIO_IT_Both, RADIO_SPWM2_IRQ);
	GPIO_OnChange(RADIO_S3_GPIO, RADIO_S3_PIN, GPIO_IT_Both, RADIO_SPWM3_IRQ);
	GPIO_OnChange(RADIO_S4_GPIO, RADIO_S4_PIN, GPIO_IT_Both, RADIO_SPWM4_IRQ);
}

/*
 * PRIVATE FUNCTIONS
 */


/*
 * INTERRUPT ROUTINES
 */

void RADIO_SPWM1_IRQ (void)
{
	uint16_t now = TIM_Read(TIM_RADIO);
	uint16_t pulse = 0;
	static uint16_t tick;

	if (GPIO_Read(RADIO_S1_GPIO, RADIO_S1_PIN))
	{
		tick = now;
	}
	else
	{
		pulse = now - tick;

		// Check pulse is valid
		if (pulse <= (RADIO_MAX + RADIO_THRESH) && pulse >= (RADIO_MIN - RADIO_THRESH))
		{
			inputS1 = pulse;
			inputHeartbeat = CORE_GetTick();
		}
	}
}

void RADIO_SPWM2_IRQ (void)
{
	uint16_t now = TIM_Read(TIM_RADIO);
	uint16_t pulse = 0;
	static uint16_t tick;

	if (GPIO_Read(RADIO_S2_GPIO, RADIO_S2_PIN))
	{
		tick = now;
	}
	else
	{
		pulse = now - tick;

		// Check pulse is valid
		if (pulse <= (RADIO_MAX + RADIO_THRESH) && pulse >= (RADIO_MIN - RADIO_THRESH))
		{
			inputS2 = pulse;
			inputHeartbeat = CORE_GetTick();
		}
	}
}

void RADIO_SPWM3_IRQ (void)
{
	uint16_t now = TIM_Read(TIM_RADIO);
	uint16_t pulse = 0;
	static uint16_t tick;

	if (GPIO_Read(RADIO_S3_GPIO, RADIO_S3_PIN))
	{
		tick = now;
	}
	else
	{
		pulse = now - tick;

		// Check pulse is valid
		if (pulse <= (RADIO_MAX + RADIO_THRESH) && pulse >= (RADIO_MIN - RADIO_THRESH))
		{
			inputS3 = pulse;
			inputHeartbeat = CORE_GetTick();
		}
	}
}

void RADIO_SPWM4_IRQ (void)
{
	uint16_t now = TIM_Read(TIM_RADIO);
	uint16_t pulse = 0;
	static uint16_t tick;

	if (GPIO_Read(RADIO_S4_GPIO, RADIO_S4_PIN))
	{
		tick = now;
	}
	else
	{
		pulse = now - tick;

		// Check pulse is valid
		if (pulse <= (RADIO_MAX + RADIO_THRESH) && pulse >= (RADIO_MIN - RADIO_THRESH))
		{
			inputS4 = pulse;
			inputHeartbeat = CORE_GetTick();
		}
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
