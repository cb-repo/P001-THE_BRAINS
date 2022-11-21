/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "System.h"
#include "Motor.h"
#include "Servo.h"
#include "US.h"

/*
 * PRIVATE DEFINITIONS
 */
Motor_Update(  );
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

	uint32_t tick = CORE_GetTick();
	while (100 > CORE_GetTick() - tick) {
		RADIO_Update();
		CORE_Idle();
	}
	US_Delay(2000);

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


