/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Servo.h"
#include "GPIO.h"
#include "TIM.h"
#include "Core.h"

/*
 * PRIVATE DEFINITIONS
 */

#define PULSE_PERIOD	20
#define PULSE_MIN		1000
#define PULSE_CENTER	1500
#define PULSE_MAX		2000

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

static void SERVO1_TimerReloadISR(void);
static void SERVO1_TimerPulseISR(void);
static void SERVO2_TimerReloadISR(void);
static void SERVO2_TimerPulseISR(void);

/*
 * PRIVATE VARIABLES
 */

/*
 * PUBLIC FUNCTIONS
 */

void SERVO_Init(void)
{
	GPIO_EnableOutput(SERVO1_GPIO, SERVO1_PIN, GPIO_PIN_RESET);
	TIM_Init(TIM_SERVO1, TIM_SERVO1_FREQ, TIM_SERVO1_RELOAD);
	TIM_OnReload(TIM_SERVO1, SERVO1_TimerReloadISR);
	TIM_OnPulse(TIM_SERVO1, 0, SERVO1_TimerPulseISR);
	SERVO_S1_Update(0);
	TIM_Start(TIM_SERVO1);

	GPIO_EnableOutput(SERVO2_GPIO, SERVO2_GPIO, GPIO_PIN_RESET);
	TIM_Init(TIM_SERVO2, TIM_SERVO2_FREQ, TIM_SERVO2_RELOAD);
	TIM_OnReload(TIM_SERVO2, SERVO2_TimerReloadISR);
	TIM_OnPulse(TIM_SERVO2, 0, SERVO2_TimerPulseISR);
	SERVO_S2_Update(0);
	TIM_Start(TIM_SERVO2);
}

void SERVO_Deinit(void)
{
	TIM_Stop(TIM_SERVO1);
	TIM_Deinit(TIM_SERVO1);
	GPIO_Disable(SERVO1_GPIO, SERVO1_PIN);

	TIM_Stop(TIM_SERVO2);
	TIM_Deinit(TIM_SERVO2);
	GPIO_Disable(SERVO2_GPIO, SERVO2_PIN);
}

void SERVO_S1_Update(int16_t pulse)
{
	if (pulse > PULSE_MAX)
	{
		pulse = PULSE_MAX;
	}
	else if (pulse < PULSE_MIN)
	{
		pulse = PULSE_MIN;
	}
	TIM_SetPulse(TIM_SERVO1, 0, pulse);
}

void SERVO_S2_Update(int16_t pulse)
{
	if (pulse > PULSE_MAX)
	{
		pulse = PULSE_MAX;
	}
	else if (pulse < PULSE_MIN)
	{
		pulse = PULSE_MIN;
	}
	TIM_SetPulse(TIM_SERVO2, 0, pulse);
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

static void SERVO1_TimerReloadISR(void)
{
	GPIO_Set(SERVO1_GPIO, SERVO1_PIN);
}

static void SERVO1_TimerPulseISR(void)
{
	GPIO_Reset(SERVO1_GPIO, SERVO1_PIN);
}

static void SERVO2_TimerReloadISR(void)
{
	GPIO_Set(SERVO2_GPIO, SERVO2_PIN);
}

static void SERVO2_TimerPulseISR(void)
{
	GPIO_Reset(SERVO2_GPIO, SERVO2_PIN);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
