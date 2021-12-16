/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Motor.h"
#include "GPIO.h"
#include "TIM.h"

/*
 * PRIVATE DEFINITIONSb
 */

#define MOTOR_BRAKE		1

#if defined(USE_DRV8837)
#define MODE_LOWPOWER	GPIO_PIN_RESET
#define MODE_COAST 		GPIO_PIN_SET
#define MODE_BRAKE 		GPIO_PIN_SET
#define PWM_LOWPOWER	GPIO_PIN_RESET
#define PWM_COAST		GPIO_PIN_RESET
#define PWM_BRAKE 		GPIO_PIN_SET
#elif defined(USE_DRV8212)
#define MODE_LOWPOWER 	GPIO_PIN_RESET
#define MODE_COAST		GPIO_PIN_RESET
#define MODE_BRAKE 		GPIO_PIN_RESET
#define PWM_LOWPOWER	GPIO_PIN_RESET
#define PWM_COAST		GPIO_PIN_RESET
#define PWM_BRAKE 		GPIO_PIN_SET
#endif

/*
 * PRIVATE TYPES
 */


/*
 * PRIVATE PROTOTYPES
 */

void MOTOR_Sleep (void);
void MOTOR_M1_Sleep (void);
void MOTOR_M2_Sleep (void);
void MOTOR_Brake (void);
void MOTOR_M1_Brake (void);
void MOTOR_M2_Brake (void);
void MOTOR_Coast (void);
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

	GPIO_EnableOutput(MOTOR_LMODE_GPIO, MOTOR_LMODE_PIN, MODE_BRAKE);
	GPIO_EnableOutput(MOTOR_RMODE_GPIO, MOTOR_RMODE_PIN, MODE_BRAKE);

	TIM_EnablePwm(TIM_MOTOR, MOTOR_LPWM1_CH, MOTOR_LPWM1_GPIO, MOTOR_LPWM1_PIN, TIM_MOTOR_AF5);
	TIM_EnablePwm(TIM_MOTOR, MOTOR_LPWM2_CH, MOTOR_LPWM2_GPIO, MOTOR_LPWM2_PIN, TIM_MOTOR_AF2);
	TIM_EnablePwm(TIM_MOTOR, MOTOR_RPWM1_CH, MOTOR_RPWM1_GPIO, MOTOR_RPWM1_PIN, TIM_MOTOR_AF2);
	TIM_EnablePwm(TIM_MOTOR, MOTOR_RPWM2_CH, MOTOR_RPWM2_GPIO, MOTOR_RPWM2_PIN, TIM_MOTOR_AF2);

	MOTOR_Stop();
	TIM_Start(TIM_MOTOR);
}

void MOTOR_Deinit (void)
{
	TIM_Stop(TIM_MOTOR);
	GPIO_Deinit(MOTOR_LMODE_GPIO, MOTOR_LMODE_PIN);
	GPIO_Deinit(MOTOR_RMODE_GPIO, MOTOR_RMODE_PIN);
	GPIO_Deinit(MOTOR_LPWM1_GPIO, MOTOR_LPWM1_PIN);
	GPIO_Deinit(MOTOR_LPWM2_GPIO, MOTOR_LPWM2_PIN);
	GPIO_Deinit(MOTOR_RPWM1_GPIO, MOTOR_RPWM1_PIN);
	GPIO_Deinit(MOTOR_RPWM2_GPIO, MOTOR_RPWM2_PIN);
	TIM_Deinit(TIM_MOTOR);
}

void MOTOR_Stop (void)
{
	MOTOR_M1_Update(MOTOR_OFF);
	MOTOR_M2_Update(MOTOR_OFF);
}

//void MOTOR_Update(void)
//{
//
//	}
//	else //config.mode == ARCADE
//	{
//	}
//}

void MOTOR_Update (int32_t M1, int32_t M2)
{
	MOTOR_M1_Update(M1);
	MOTOR_M2_Update(M2);
}

/*
 * PRIVATE FUNCTIONS
 */

void MOTOR_Sleep (void)
{
	MOTOR_M1_Sleep();
	MOTOR_M2_Sleep();
}

void MOTOR_M1_Sleep (void)
{
	GPIO_Write(MOTOR_LMODE_GPIO, MOTOR_LMODE_PIN, MODE_LOWPOWER);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, PWM_LOWPOWER);
	TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, PWM_LOWPOWER);
}

void MOTOR_M2_Sleep (void)
{
	GPIO_Write(MOTOR_RMODE_GPIO, MOTOR_RMODE_PIN, MODE_LOWPOWER);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, PWM_LOWPOWER);
	TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, PWM_LOWPOWER);
}


void MOTOR_Brake (void)
{
	MOTOR_M1_Brake();
	MOTOR_M2_Brake();
}

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

void MOTOR_Coast (void)
{
	MOTOR_M1_Coast();
	MOTOR_M2_Coast();
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
			MOTOR_M1_Brake();
		} else {
			MOTOR_M1_Coast();
		}
	} else if (reverse) {
		TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, 0);
		TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, throttle);
	} else {
		TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM1_CH, throttle);
		TIM_SetPulse(TIM_MOTOR, MOTOR_LPWM2_CH, 0);
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
	} else if (reverse) {
		TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, 0);
		TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, throttle);
	} else {
		TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, throttle);
		TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, 0);
	}
}

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
