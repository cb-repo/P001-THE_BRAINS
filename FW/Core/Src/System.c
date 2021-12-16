/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "System.h"
#include "Core.h"
#include "Board.h"
#include "Config.h"
#include "GPIO.h"
#include "Radio.h"
#include "Servo.h"
#include "Motor.h"
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

Status_t status = BOOT;
WarningStates_t StateWarning = {.batt = 0, .temp = 0};
FaultStates_t StateFault = {.batt = 0, .input = 0, .temp = 0};


/*
 * PUBLIC FUNCTIONS
 */


void SYSTEM_Init(void)
{
	static int32_t tick = CORE_GetTick();
	status = BOOT;
	CONFIG_Init();
	ADC_Init();
	SERVO_Init();
	MOTOR_Init();
	RADIO_Init();
	while (BOOT_TIMEOUT >= CORE_GetTick() - tick)
	{
		if (CONFIG_Set()) { break; }
		CORE_Idle();
	}
	status = RUN;
}


void SYSTEM_Update(void)
{
	int32_t SystemVolt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL),BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
	int32_t SystemTemp = ADC_ReadDieTemp();
	int32_t SystemInput = 0; // Need to update... time between radio comms

	if (StateFault.batt == 0) {
		if (SystemVolt < SYSTEM_BATT_FAULT) { StateFault.batt = 1; }
	}
	else { //StateFault.batt == 1
		if (SystemVolt > SYSTEM_BATT_FAULT + SYSTEM_BATT_HYST) { StateFault.batt = 0; }
	}

	if (StateWarning.batt == 0) {
		if (SystemVolt < SYSTEM_BATT_WARN) { StateWarning.batt = 1; }
	}
	else { //StateWarning.batt == 1
		if (SystemVolt > SYSTEM_BATT_WARN + SYSTEM_BATT_HYST) { StateWarning.batt = 0; }
	}

	if (StateFault.temp == 0) {
		if (SystemTemp > SYSTEM_TEMP_FAULT) { StateFault.temp = 1; }
	}
	else { //StateFault.temp == 1
		if (SystemTemp < SYSTEM_TEMP_FAULT - SYSTEM_TEMP_HYST) { StateFault.temp = 0; }
	}

	if (StateWarning.temp == 0) {
		if (SystemTemp > SYSTEM_TEMP_WARN) { StateWarning.temp = 1; }
	}
	else { //StateWarning.temp == 1
		if (SystemTemp < SYSTEM_TEMP_WARN - SYSTEM_TEMP_HYST) { StateWarning.temp = 0; }
	}

	if (StateFault.input == 0) {
		if (SystemInput > SYSTEM_INPUT_TIMEOUT) { StateFault.input = 1; }
	}
	else { //StateFault.input == 1
		if (SystemInput < SYSTEM_INPUT_TIMEOUT) { StateFault.input = 0; }
	}

	if (StateFault.batt || StateFault.temp || StateFault.input) {
		status = FAULT;
	} else if (StateWarning.batt || StateWarning.temp) {
		status = WARNING;
	} else {
		status = RUN;
		CONFIG_CheckForReset(); //
	}
}


/*
 * PRIVATE FUNCTIONS
 */


/*
 * INTERRUPT ROUTINES
 */


int32_t CONVERT_RadioToMotor (uint32_t radio)
{
	return ((int32_t)radio - SERVO_CENTER) / SERVO_HALFSCALE * MOTOR_MAX;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
