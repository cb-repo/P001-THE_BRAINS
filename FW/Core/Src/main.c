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

// TO DO:
// - Write Config Set Script (SYSTEM_RunConfig())
// - Write Config Reset Script (SYSTEM_ResetConfig())
// - Read Config from EEPROM (SYSTEM_Config())
// - Fix motor jerk on boot
// - Implement Overtemp Warning / Fauls
// - Implement Undervolt Warning/ Fault

int main (void)
{
	CORE_Init();
	SYSTEM_Init();

	while (1)
	{
		SYSTEM_Update();
		MOTOR_M1_Update(input[IP3]);
		MOTOR_M2_Update(input[IP2]);
		SERVO_S1_Update(input[IP1]);
		SERVO_S2_Update(input[IP4]);
		LED_Update();
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


