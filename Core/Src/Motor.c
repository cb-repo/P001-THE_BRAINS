/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Motor.h"
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

void MOTOR_Init (void)
{
	TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);

//	TIM_EnablePwm(TIM_MOTOR, MOTOR_LPWM1_CH, MOTOR_LPWM1_GPIO, MOTOR_LPWM1_PIN, TIM_MOTOR_AF);
//	GPIO_EnableOutput(MOTOR_LPWM1_GPIO, MOTOR_LPWM1_PIN, GPIO_PIN_SET);
//	TIM_EnablePwm(TIM_MOTOR, MOTOR_LPWM2_CH, MOTOR_LPWM2_GPIO, MOTOR_LPWM2_PIN, TIM_MOTOR_AF);
//	TIM_EnablePwm(TIM_MOTOR, MOTOR_RPWM1_CH, MOTOR_RPWM1_GPIO, MOTOR_RPWM1_PIN, TIM_MOTOR_AF);
	GPIO_EnableOutput(MOTOR_RPWM1_GPIO, MOTOR_RPWM1_PIN, GPIO_PIN_SET);
	TIM_EnablePwm(TIM_MOTOR, MOTOR_RPWM2_CH, MOTOR_RPWM2_GPIO, MOTOR_RPWM2_PIN, TIM_MOTOR_AF);

	TIM_Start(TIM_MOTOR);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, MOTOR_OFF);

//	MOTOR_Stop();
}


void MOTOR_Deinit (void)
{
	TIM_Stop(TIM_MOTOR);
	GPIO_Disable(MOTOR_ALL_GPIO, MOTOR_ALL_PIN);
	TIM_Deinit(TIM_MOTOR);
}

void MOTOR_Stop (void)
{
//	MOTOR_M1_Update(MOTOR_OFF);
	MOTOR_M2_Update(MOTOR_OFF);
}

void MOTOR_M1_Update (int32_t throttle)
{
	bool reverse = false;
	uint16_t pwm1;
	uint16_t pwm2;

	if (throttle < 0)
	{
		reverse = true;
		throttle = -throttle;
	}

	if (throttle > MOTOR_MAX)
	{
		throttle = MOTOR_MAX;
	}

	if (throttle == MOTOR_OFF)
	{
		if (MOTOR_BRAKE)
		{
			pwm1 = MOTOR_MAX;
			pwm2 = MOTOR_MAX;
		}
		else
		{
			pwm1 = MOTOR_OFF;
			pwm2 = MOTOR_OFF;
		}
	}
	else if (reverse)
	{
		pwm1 = 0;
		pwm2 = throttle;
	}
	else
	{
		pwm1 = throttle;
		pwm2 = 0;
	}

	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, pwm1);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, pwm2);
}

void MOTOR_M2_Update (int32_t throttle)
{
//	bool reverse = false;
//	uint16_t pwm1;
//	uint16_t pwm2;
//
//	if (throttle < 0)
//	{
//		reverse = true;
//		throttle = -throttle;
//	}
//
//	if (throttle > MOTOR_MAX)
//	{
//		throttle = MOTOR_MAX;
//	}
//
//	if (throttle == MOTOR_OFF)
//	{
//		if (MOTOR_BRAKE)
//		{
//			pwm1 = MOTOR_MAX;
//			pwm2 = MOTOR_MAX;
//		}
//		else
//		{
//			pwm1 = MOTOR_OFF;
//			pwm2 = MOTOR_OFF;
//		}
//	}
//	else if (reverse)
//	{
//		pwm1 = 0;
//		pwm2 = throttle;
//	}
//	else
//	{
//		pwm1 = throttle;
//		pwm2 = 0;
//	}
//
//	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, pwm1);
	GPIO_Write(MOTOR_RPWM1_GPIO, MOTOR_RPWM1_PIN, GPIO_PIN_SET);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, throttle);
}

void MOTOR_Update (int32_t motorL_throttle, int32_t motorR_throttle)
{
	bool motorL_rev = false;
	uint16_t motorL_pwm1;
	uint16_t motorL_pwm2;

	bool motorR_rev = false;
	uint16_t motorR_pwm1;
	uint16_t motorR_pwm2;

	// Test for Reverse Condition
	if (motorL_throttle < 0) {
		motorL_rev = true;
		motorL_throttle = -motorL_throttle;
	}

	if (motorR_throttle < 0) {
		motorR_rev = true;
		motorR_throttle = -motorR_throttle;
	}

	// Drive Left motor
	if (motorL_throttle == MOTOR_OFF) {
		if (MOTOR_BRAKE) {
			motorL_pwm1 = MOTOR_MAX;
			motorL_pwm2 = MOTOR_MAX;
		} else {
			motorL_pwm1 = MOTOR_OFF;
			motorL_pwm2 = MOTOR_OFF;
		}
	} else if (motorL_rev) {
		motorL_pwm1 = 0;
		motorL_pwm2 = motorL_throttle;
	} else {
		motorL_pwm1 = motorL_throttle;
		motorL_pwm2 = 0;
	}

	// Drive right motor
	if (motorR_throttle == MOTOR_OFF) {
		if (MOTOR_BRAKE) {
			motorR_pwm1 = MOTOR_MAX;
			motorR_pwm2 = MOTOR_MAX;
		} else {
			motorR_pwm1 = MOTOR_OFF;
			motorR_pwm2 = MOTOR_OFF;
		}
	} else if (motorR_rev) {
		motorR_pwm1 = 0;
		motorR_pwm2 = motorR_throttle;
	} else {
		motorR_pwm1 = motorR_throttle;
		motorR_pwm2 = 0;
	}

	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, motorL_pwm1);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, motorL_pwm2);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, motorR_pwm1);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, motorR_pwm2);
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
