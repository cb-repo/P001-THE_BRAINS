/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "System.h"
#include "Motor.h"
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
	SYSTEM_Init();
	MOTOR_Init();
	SERVO_Init();

	CORE_Delay(100); // Have time to check for a valid radio before proceeding
	while (1)
	{
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


