/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "TIM.h"
#include "GPIO.h"
#include "Motor.h"
#include "Radio.h"
#include "Servo.h"

/*
 * PRIVATE DEFINITIONS
 */

void TEST_Nordic(void);
void TEST_Purge(void);
void TEST_FlowRate(void);

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

int32_t radio2motor (int16_t);

/*
 * PRIVATE VARIABLES
 */

// WHICH TEST TO RUN
int8_t test_nordic = 				1;
int8_t test_purge = 				0;
int8_t test_flowRate = 				0;

// TEST VARIABLES
int8_t motor2_on =					0; //This is the DC pump motor
int32_t purgeSpeed = 				185;

int8_t nordicState = 0;
int8_t nordicState_prev = 0;

int32_t tick = 						0;
int32_t tickFlowRate = 				0;

int32_t startDelay_ms = 		 	1000; //10*60*1000;

//int32_t purgeDelay_ms = 		 	10*1000;
//int32_t flowRateON_ms = 			5*1000;
//int32_t flowRateOFF_ms = 			5*1000;
//int32_t flowRate_count = 			0;
////int32_t flowRate_cycles = 			15;
//int32_t flowRate_cycles = 			40;

//24HR Test Boxxer Motor
//int32_t purgeDelay_ms = 		 	5*1000;
//int32_t flowRateON_ms = 			1000;//1*1000;
//int32_t flowRateOFF_ms = 			491521-1000;//(10*1000)-(200);//1846500; //923250;//*1000;
//int32_t flowRate_count = 			0;
//int32_t flowRate_cycles = 			176;

//Pump Evaluation Settings
int32_t purgeDelay_ms = 		 	0*1000;
int32_t flowRateON_ms = 			500;//1*1000;
int32_t flowRateOFF_ms = 			(10*1000)-(500);//1846500; //923250;//*1000;
int32_t flowRate_count = 			0;
int32_t flowRate_cycles = 			100000;

/*
 * PUBLIC FUNCTIONS
 */

/*
 * PRIVATE FUNCTIONS
 */

int main (void)
{
	CORE_Init();
//	RADIO_Init();
	GPIO_EnableInput(RADIO_S4_GPIO, RADIO_S4_PIN, GPIO_NOPULL);
	MOTOR_Init();
//	SERVO_Init();
//	WEPON_Init();

	tick = CORE_GetTick();

	// BUSY WAIT FOR START DELAY
	while(CORE_GetTick() < tick + startDelay_ms)
	{
	}

	while (1)
	{

		// COMBAT CODE
//		MOTOR_Update(radio2motor(radio_S3), radio2motor(radio_S2));
//		SERVO_Update(radio_S1);
//		WEPON_Update(radio_S4);

		// ESR CODE
//		if (test_nordic)
//		{
		TEST_Nordic();
//		}
//		else if (test_purge)
//		{
//			TEST_Purge();
//		}
//		else if (test_flowRate)
//		{
//			TEST_FlowRate();
//		}
//		else
//		{
//			MOTOR_Stop();
//		}

//		tick = CORE_GetTick();

		CORE_Idle();
	}
}


void TEST_Nordic (void)
{
	nordicState = GPIO_Read(RADIO_S4_GPIO, RADIO_S4_PIN);
	if (nordicState && !motor2_on) {
		MOTOR_M2_Update(purgeSpeed);
//		TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM1_CH, 180);
//		TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, 0);
		motor2_on = 1;
	}
	else if (!nordicState && motor2_on)
	{
		MOTOR_M2_Update(MOTOR_OFF);
//		TIM_SetPulse(TIM_MOTOR, MOTOR_RPWM2_CH, MOTOR_OFF);
		motor2_on = 0;
	}
//	nordicState_prev = nordicState;
}


void TEST_Purge (void)
{
	// EN pin 5
	// PH pin 6
	if (!motor2_on)
		{
		MOTOR_M2_Update(purgeSpeed);
		motor2_on = 1;

		}
}


void TEST_FlowRate (void)
{
	tick = CORE_GetTick();
	tickFlowRate = tick + purgeDelay_ms;

	if (test_flowRate && purgeDelay_ms != 0)
	{
		MOTOR_M2_Update(MOTOR_MAX);
		motor2_on = 1;
	}

	if (!motor2_on && tick >= tickFlowRate && flowRate_count < flowRate_cycles)
	{
		tickFlowRate = tick + flowRateON_ms;
		motor2_on = 1;
		flowRate_count += 1;
		MOTOR_M2_Update(purgeSpeed);
	}
	else if (motor2_on && tick >= tickFlowRate)
	{
		tickFlowRate = tick + flowRateOFF_ms;
		motor2_on = 0;
		MOTOR_M2_Update(MOTOR_OFF);
	}
}


int32_t radio2motor (int16_t radio_S)
{
	return (((int32_t)radio_S - 1500) * 256) / 500;
}


/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


