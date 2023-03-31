/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Motor.h"
#include "GPIO.h"
#include "TIM.h"

/*
 * PRIVATE DEFINITIONSb
 */

#define MOTOR_BRAKE		1

#if USE_DRV8212
#define MODE_PWM		GPIO_PIN_RESET
#define MODE_COAST		GPIO_PIN_RESET
#define MODE_BRAKE		GPIO_PIN_RESET
#define PWM_COAST		MOTOR_OFF
#define PWM_BRAKE 		MOTOR_MAX
#elif USE_DRV8837
#define MODE_PWM		GPIO_PIN_SET
#define MODE_COAST		GPIO_PIN_SET
#define MODE_BRAKE		GPIO_PIN_SET
#define PWM_COAST		MOTOR_OFF
#define PWM_BRAKE 		MOTOR_MAX
#endif

/*
 * PRIVATE TYPES
 */


/*
 * PRIVATE PROTOTYPES
 */

void MOTOR_M1_Brake (void);
void MOTOR_M2_Brake (void);
void MOTOR_M1_Coast (void);
void MOTOR_M2_Coast (void);

void MOTOR_M1_Update (int32_t);
void MOTOR_M2_Update (int32_t);

/*
 * PRIVATE VARIABLES
 */


/*
 * PUBLIC FUNCTIONS
 */

void MOTOR_Init (void)
{
	TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);

#if defined(USE_DRV8212)
	GPIO_EnableOutput(MOTOR_LMODE_GPIO, MOTOR_LMODE_PIN, MODE_PWM);
	GPIO_EnableOutput(MOTOR_RMODE_GPIO, MOTOR_RMODE_PIN, MODE_PWM);
#endif

	TIM_EnablePwm(TIM_MOTOR, MOTOR_LPWM1_CH, MOTOR_LPWM1_GPIO, MOTOR_LPWM1_PIN, TIM_MOTOR_AF2);
	TIM_EnablePwm(TIM_MOTOR, MOTOR_LPWM2_CH, MOTOR_LPWM2_GPIO, MOTOR_LPWM2_PIN, TIM_MOTOR_AF2);
	TIM_EnablePwm(TIM_MOTOR, MOTOR_RPWM1_CH, MOTOR_RPWM1_GPIO, MOTOR_RPWM1_PIN, TIM_MOTOR_AF2);
	TIM_EnablePwm(TIM_MOTOR, MOTOR_RPWM2_CH, MOTOR_RPWM2_GPIO, MOTOR_RPWM2_PIN, TIM_MOTOR_AF2);

	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	TIM_Start(TIM_MOTOR);
}


void MOTOR_Update (int32_t M1, int32_t M2)
{
	MOTOR_M1_Update(M1);
	MOTOR_M2_Update(M2);
}

/*
 * PRIVATE FUNCTIONS
 */

void MOTOR_M1_Brake (void)
{
	GPIO_Write(MOTOR_LMODE_GPIO, MOTOR_LMODE_PIN, MODE_BRAKE);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, PWM_BRAKE);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, PWM_BRAKE);
}

void MOTOR_M2_Brake (void)
{
	GPIO_Write(MOTOR_RMODE_GPIO, MOTOR_RMODE_PIN, MODE_BRAKE);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, PWM_BRAKE);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, PWM_BRAKE);
}

void MOTOR_M1_Coast(void)
{
	GPIO_Write(MOTOR_LMODE_GPIO, MOTOR_LMODE_PIN, MODE_COAST);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, PWM_COAST);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, PWM_COAST);
}

void MOTOR_M2_Coast(void)
{
	GPIO_Write(MOTOR_RMODE_GPIO, MOTOR_RMODE_PIN, MODE_COAST);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, PWM_COAST);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, PWM_COAST);
}

void MOTOR_M1_Update (int32_t throttle)
{
	bool reverse = false;

	// Check if reversing
	if (throttle < MOTOR_OFF) {
		reverse = true;
		throttle = -throttle;
	}

	// Clamp maximum speed
	if (throttle > MOTOR_MAX) {
		throttle = MOTOR_MAX;
	}

	if ( throttle <= ( MOTOR_OFF + MOTOR_STALL ) ) {
		if (MOTOR_BRAKE) {
			MOTOR_M1_Brake();
		} else {
			MOTOR_M1_Coast();
		}
	}
	else
	{
		if ( throttle >= (MOTOR_MAX - MOTOR_MAX_THRESH ) ) {
			throttle = MOTOR_MAX;
		}
//		else {
//			throttle += throttle * (MOTOR_MAX - throttle) / MOTOR_MAX;
//		}

		if (reverse) {
			TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, 0);
			TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, throttle);
		} else {
			TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, throttle);
			TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, 0);
		}
	}
}

void MOTOR_M2_Update (int32_t throttle)
{
	bool reverse = false;

	// Check if reversing
	if (throttle < 0) {
		reverse = true;
		throttle = -throttle;
	}

	// Clamp maximum speed
	if (throttle > MOTOR_MAX) {
		throttle = MOTOR_MAX;
	}

	if (throttle <= MOTOR_OFF + MOTOR_STALL) {
		if (MOTOR_BRAKE) {
			MOTOR_M2_Brake();
		} else {
			MOTOR_M2_Coast();
		}
	}
	else
	{
		if ( throttle >= (MOTOR_MAX - MOTOR_MAX_THRESH ) ) {
			throttle = MOTOR_MAX;
		}
//		else {
//			throttle += throttle * (MOTOR_MAX - throttle) / MOTOR_MAX;
//		}

		if (reverse) {
			TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, 0);
			TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, throttle);
		} else {
			TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, throttle);
			TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, 0);
		}
	}
}

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
