/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "System.h"
#include "Motor.h"
#include "Radio.h"
#include "Servo.h"
#include "LED.h"
#include "Config.h"

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
	RADIO_Init();
	MOTOR_Init();
	SERVO_Init();

	if (configSetFlag)
	{
		CONFIG_Set();
	}

	while (1)
	{
		SYSTEM_Update();
		MOTOR_Update(inputS1, inputS2);
		SERVO_Update(inputS3, inputS4);
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


