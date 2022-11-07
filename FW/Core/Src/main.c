/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "System.h"
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

/*
 * PRIVATE VARIABLES
 */

/*
 * PUBLIC FUNCTIONS
 */

int main (void)
{
	CORE_Init();
	RADIO_Init();
	MOTOR_Init();
	SERVO_Init();
	SYSTEM_Init();

	CORE_Delay(INPUT_TIMEOUT); // Have time to check for a valid radio before proceeding

	while (1)
	{
		RADIO_Update();
		SYSTEM_Update();

		CORE_Idle();
	}
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


