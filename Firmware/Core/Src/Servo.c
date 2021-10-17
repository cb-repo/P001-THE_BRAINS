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

static void SERVO_TimerReloadISR(void);
static void SERVO_TimerPulseISR(void);
static void WEPON_TimerReloadISR(void);
static void WEPON_TimerPulseISR(void);

/*
 * PRIVATE VARIABLES
 */

/*
 * PUBLIC FUNCTIONS
 */

void SERVO_Init(void)
{
	GPIO_EnableOutput(SERVO_GPIO, SERVO_PIN, GPIO_PIN_RESET);
	TIM_Init(TIM_SERVO, TIM_SERVO_FREQ, TIM_SERVO_RELOAD);
	TIM_OnReload(TIM_SERVO, SERVO_TimerReloadISR);
	TIM_OnPulse(TIM_SERVO, 0, SERVO_TimerPulseISR);
	SERVO_Update(0);
	TIM_Start(TIM_SERVO);
}

void SERVO_Deinit(void)
{
	TIM_Stop(TIM_SERVO);
	TIM_Deinit(TIM_SERVO);
	GPIO_Disable(SERVO_GPIO, SERVO_PIN);
}

void SERVO_Update(int16_t pulse)
{
	if (pulse > PULSE_MAX)
	{
		pulse = PULSE_MAX;
	}
	else if (pulse < PULSE_MIN)
	{
		pulse = PULSE_MIN;
	}
	TIM_SetPulse(TIM_SERVO, 0, pulse);
}

void WEPON_Init(void)
{
	GPIO_EnableOutput(WEPON_GPIO, WEPON_PIN, GPIO_PIN_RESET);
	TIM_Init(TIM_WEPON, TIM_WEPON_FREQ, TIM_WEPON_RELOAD);
	TIM_OnReload(TIM_WEPON, WEPON_TimerReloadISR);
	TIM_OnPulse(TIM_WEPON, 0, WEPON_TimerPulseISR);
	WEPON_Update(0);
	TIM_Start(TIM_WEPON);
}

void WEPON_Deinit(void)
{
	TIM_Stop(TIM_WEPON);
	TIM_Deinit(TIM_WEPON);
	GPIO_Disable(WEPON_GPIO, WEPON_PIN);
}

void WEPON_Update(int16_t pulse)
{
	if (pulse > PULSE_MAX)
	{
		pulse = PULSE_MAX;
	}
	else if (pulse < PULSE_MIN)
	{
		pulse = PULSE_MIN;
	}
	TIM_SetPulse(TIM_WEPON, 0, pulse);
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

static void SERVO_TimerReloadISR(void)
{
	GPIO_Set(SERVO_GPIO, SERVO_PIN);
}

static void SERVO_TimerPulseISR(void)
{
	GPIO_Reset(SERVO_GPIO, SERVO_PIN);
}

static void WEPON_TimerReloadISR(void)
{
	GPIO_Set(WEPON_GPIO, WEPON_PIN);
}

static void WEPON_TimerPulseISR(void)
{
	GPIO_Reset(WEPON_GPIO, WEPON_PIN);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
