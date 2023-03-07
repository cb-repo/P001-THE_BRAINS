/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "System.h"
#include "Motor.h"
#include "Servo.h"
#include "US.h"

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

	US_Delay(1000);
//	CORE_Idle();

	while (1)
	{
		SYSTEM_Update();
		US_Delay(1000);
//		CORE_Idle();
	}
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


