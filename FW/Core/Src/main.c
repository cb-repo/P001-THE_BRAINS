/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Core.h"
#include "System.h"
#include "Motor.h"
#include "Radio.h"
#include "Servo.h"
#include "LED.h"

#include "UART.h"
#include "string.h"
#include "stdio.h"
#include "ADC.h"

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

	uint16_t R1 = 0;
	uint16_t R2 = 0;
	uint16_t R3 = 0;
	uint16_t R4 = 0;
	int32_t M1 = 0;
	int32_t M2 = 0;
	uint16_t S1 = 0;
	uint16_t S2 = 0;

	CORE_Delay(INPUT_TIMEOUT); // Have time to check for a valid radio before proceeding

	while (1)
	{
		SYSTEM_Update();

		if (status.faultTemp || status.faultInput)
		{
			MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
			if (!(status_p.faultTemp || status_p.faultInput))
			{
				SERVO_Deinit();
			}
		}
		else
		{
			// REINITIALISE THE SERVO/WEPON OUTPUT
			if (status_p.faultTemp || status_p.faultInput)
			{
				SERVO_Init();
			}

			// UPDATE THE MOTORS
			R1 = input[config.primaryCh];
			R1 = SYSTEM_RadioTruncate(R1);
			if (config.primaryRev == CH_RVS) {
				R1 = SYSTEM_ReverseRadio(R1);
			}
			R2 = input[config.secondaryCh];
			R2 = SYSTEM_RadioTruncate(R2);
			if (config.secondaryRev == CH_RVS) {
				R2 = SYSTEM_ReverseRadio(R2);
			}
			if (config.mode == ARCADE) {
				M1 = (R1 - (RADIO_CENTER - R2));
				M2 = (R1 + (RADIO_CENTER - R2));
			} else {
				M1 = R1;
				M2 = R2;
			}
			M1 = SYSTEM_RadioToMotor(M1);
			M2 = SYSTEM_RadioToMotor(M2);
			MOTOR_Update(M1, M2);

			// UPDATE THE SERVO/WEPON OUTPUTS
			R3 = input[config.servoChA];
			if (config.servoRevA == CH_RVS) {
				R3 = SYSTEM_ReverseRadio(R3);
			}
			S1 = R3;
			R4 = input[config.servoChB];
			if (config.servoRevB == CH_RVS) {
				R4 = SYSTEM_ReverseRadio(R4);
			}
			S2 = R4;
			SERVO_Update(S1, S2);
		}

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


