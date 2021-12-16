/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Servo.h"
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

	GPIO_EnableOutput(SERVO2_GPIO, SERVO2_PIN, GPIO_PIN_RESET);
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
	GPIO_Deinit(SERVO1_GPIO, SERVO1_PIN);

	TIM_Stop(TIM_SERVO2);
	TIM_Deinit(TIM_SERVO2);
	GPIO_Deinit(SERVO2_GPIO, SERVO2_PIN);
}

void SERVO_S1_Update(int16_t pulse)
{
	if (pulse > SERVO_MAX)
	{
		pulse = SERVO_MAX;
	}
	else if (pulse < SERVO_MIN)
	{
		pulse = SERVO_MIN;
	}
	TIM_SetPulse(TIM_SERVO1, 0, pulse);
}

void SERVO_S2_Update(int16_t pulse)
{
	if (pulse > SERVO_MAX)
	{
		pulse = SERVO_MAX;
	}
	else if (pulse < SERVO_MIN)
	{
		pulse = SERVO_MIN;
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
