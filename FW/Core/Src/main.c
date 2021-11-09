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


/*
 * PUBLIC FUNCTIONS
 */

/*
 * PRIVATE FUNCTIONS
 */

int main (void)
{
	CORE_Init();
	RADIO_Init();
	MOTOR_Init();
	SERVO_Init();

	while (1)
	{
//		MOTOR_Update(radio2motor(radio_S3), radio2motor(radio_S2));
		MOTOR_Update(MOTOR_MAX, MOTOR_MAX);
		SERVO_S1_Update(radio_S1);
		SERVO_S2_Update(radio_S4);

		CORE_Idle();
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


