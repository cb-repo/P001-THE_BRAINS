/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Radio.h"
#include "GPIO.h"
#include "Tim.h"
#include "System.h"
#include "string.h"
#include "Config.h"

/*
 * PRIVATE DEFINITIONS
 */

#define IP_ARRAY_LEN 		16
#define IP_ARRAY_INIT		RADIO_CENTER
#define IP_TICK_ARRAY_LEN	4
#define IP_TICK_ARRAY_INIT	0

#define PPM_PULSE_THRESH	3000
#define PULSE_WIDTH			2

#define PULSE_PERIOD		20


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
void RADIO_PPM_IRQ (void);

/*
 * PRIVATE VARIABLES
 */

static uint32_t input_tick[IP_TICK_ARRAY_LEN];
uint16_t input[IP_ARRAY_LEN];
uint8_t loopcount = 0;

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void)
{
	if (config.radio == PPM)
	{
		PPM_Init();
	}
	else if (config.radio == IBUS)
	{
		IBUS_Init();
	}
	else if (config.radio == SBUS)
	{
		SBUS_Init();
	}
	else
	{
		SPWM_Init();
	}
}

/*
 * PRIVATE FUNCTIONS
 */

void SPWM_Init (void)
{
	TIM_Init(TIM_RADIO, TIM_RADIO_FREQ, TIM_RADIO_RELOAD);
	TIM_Start(TIM_RADIO);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_Both, RADIO_SPWM1_IRQ);
	GPIO_OnChange(RADIO_S2_GPIO, RADIO_S2_PIN, GPIO_IT_Both, RADIO_SPWM2_IRQ);
	GPIO_OnChange(RADIO_S3_GPIO, RADIO_S3_PIN, GPIO_IT_Both, RADIO_SPWM3_IRQ);
	GPIO_OnChange(RADIO_S4_GPIO, RADIO_S4_PIN, GPIO_IT_Both, RADIO_SPWM4_IRQ);
	memset(input, IP_ARRAY_INIT, IP_ARRAY_LEN);
	memset(input_tick, IP_TICK_ARRAY_INIT, IP_TICK_ARRAY_LEN);
	loopcount = 0;
}

void SPWM_Deinit (void)
{
	TIM_Stop(TIM_RADIO);
	TIM_Deinit(TIM_RADIO);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_None, NULL);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_None, NULL);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_None, NULL);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_None, NULL);
}

void PPM_Init (void)
{
	TIM_Start(TIM_RADIO);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_Rising, RADIO_PPM_IRQ);
	memset(input, IP_ARRAY_INIT, IP_ARRAY_LEN);
	memset(input_tick, IP_TICK_ARRAY_INIT, IP_TICK_ARRAY_LEN);
	loopcount = 0;
}

void PPM_Deinit (void)
{
	TIM_Stop(TIM_RADIO);
	TIM_Deinit(TIM_RADIO);
	GPIO_OnChange(RADIO_S1_GPIO, RADIO_S1_PIN, GPIO_IT_None, NULL);
}

void IBUS_Init (void)
{
	// Add functionality soon

	//Init UART
	memset(input, IP_ARRAY_INIT, IP_ARRAY_LEN);
	memset(input_tick, IP_TICK_ARRAY_INIT, IP_TICK_ARRAY_LEN);
	loopcount = 0;
}

void SBUS_Init (void)
{
	// Add functionality soon

	//INIT UART
	memset(input, IP_ARRAY_INIT, IP_ARRAY_LEN);
	memset(input_tick, IP_TICK_ARRAY_INIT, IP_TICK_ARRAY_LEN);
	loopcount = 0;
}

/*
 * INTERRUPT ROUTINES
 */

void RADIO_SPWM1_IRQ (void)
{
	uint32_t now = TIM_Read(TIM_RADIO);
	if (GPIO_Read(RADIO_S1_GPIO, RADIO_S1_PIN))
	{
		input_tick[IP1] = now;
	}
	else
	{
		input[IP1] = now - input_tick[IP1];
	}
}

void RADIO_SPWM2_IRQ (void)
{
	uint32_t now = TIM_Read(TIM_RADIO);
	if (GPIO_Read(RADIO_S2_GPIO, RADIO_S2_PIN))
	{
		input_tick[IP2] = now;
	}
	else
	{
		input[IP2] = now - input_tick[IP2];
	}
}

void RADIO_SPWM3_IRQ (void)
{
	uint32_t now = TIM_Read(TIM_RADIO);
	if (GPIO_Read(RADIO_S3_GPIO, RADIO_S3_PIN))
	{
		input_tick[IP3] = now;
	}
	else
	{
		input[IP3] = now - input_tick[IP3];
	}
}

void RADIO_SPWM4_IRQ (void)
{
	uint32_t now = TIM_Read(TIM_RADIO);
	if (GPIO_Read(RADIO_S4_GPIO, RADIO_S4_PIN))
	{
		input_tick[IP4] = now;
	}
	else
	{
		input[IP4] = now - input_tick[IP4];
	}
}

void RADIO_PPM_IRQ (void)
{
	uint32_t now = TIM_Read(TIM_RADIO);
	uint32_t pulse = now - input_tick[0];
	if (loopcount == 0)
	{
	}
	else if (pulse > PPM_PULSE_THRESH)
	{
		loopcount = 0;
	}
	else
	{
		input[count-1] = pulse;
	}
	input_tick[0] = now;
	if (loopcount >= PPM_CH_MAX)
	{
		loopcount = 0;
	}
	else
	{
		loopcount += 1;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
