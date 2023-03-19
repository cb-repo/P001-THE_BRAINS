/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "Radio.h"
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

	CORE_Idle();

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


