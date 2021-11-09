/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Radio.h"
#include "GPIO.h"
#include "TIM.h"
#include "Motor.h"

/*
 * PRIVATE DEFINITIONS
 */

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void RADIO_S1_IRQ (void);
void RADIO_S2_IRQ (void);
void RADIO_S3_IRQ (void);
void RADIO_S4_IRQ (void);

/*
 * PRIVATE VARIABLES
 */

static uint16_t radio_S1tick = 0;
uint16_t radio_S1 = 1500;
static uint16_t radio_S2tick = 0;
uint16_t radio_S2 = 1500;
static uint16_t radio_S3tick = 0;
uint16_t radio_S3 = 1500;
static uint16_t radio_S4tick = 0;
uint16_t radio_S4 = 1500;

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void)
{
	TIM_Init(TIM_RADIO, TIM_RADIO_FREQ, TIM_RADIO_RELOAD);
	TIM_Start(TIM_RADIO);
	GPIO_EnableIRQ(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_NOPULL, GPIO_IT_BOTH, RADIO_S1_IRQ);
	GPIO_EnableIRQ(RADIO_S2_GPIO, RADIO_S2_PIN, GPIO_NOPULL, GPIO_IT_BOTH, RADIO_S2_IRQ);
	GPIO_EnableIRQ(RADIO_S3_GPIO, RADIO_S3_PIN, GPIO_NOPULL, GPIO_IT_BOTH, RADIO_S3_IRQ);
	GPIO_EnableIRQ(RADIO_S4_GPIO, RADIO_S4_PIN, GPIO_NOPULL, GPIO_IT_BOTH, RADIO_S4_IRQ);
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

void RADIO_S1_IRQ (void)
{
	uint16_t now = TIM_GetCounter(TIM_RADIO);
	if (GPIO_Read(RADIO_S1_GPIO, RADIO_S1_PIN))
	{
		radio_S1tick = now;
	}
	else
	{
		radio_S1 = now - radio_S1tick;
	}
}

void RADIO_S2_IRQ (void)
{
	uint16_t now = TIM_GetCounter(TIM_RADIO);
	if (GPIO_Read(RADIO_S2_GPIO, RADIO_S2_PIN))
	{
		radio_S2tick = now;
	}
	else
	{
		radio_S2 = now - radio_S2tick;
	}
}

void RADIO_S3_IRQ (void)
{
	uint16_t now = TIM_GetCounter(TIM_RADIO);
	if (GPIO_Read(RADIO_S3_GPIO, RADIO_S3_PIN))
	{
		radio_S3tick = now;
	}
	else
	{
		radio_S3 = now - radio_S3tick;
	}
}

void RADIO_S4_IRQ (void)
{
	uint16_t now = TIM_GetCounter(TIM_RADIO);
	if (GPIO_Read(RADIO_S4_GPIO, RADIO_S4_PIN))
	{
		radio_S4tick = now;
	}
	else
	{
		radio_S4 = now - radio_S4tick;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
