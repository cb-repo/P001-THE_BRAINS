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
#include "LED.h"

/*
 * PRIVATE DEFINITIONS
 */


#define FAULT_LED_FLASH 		500
#define FAULT_LED_FLASH_FAST	100
#define FAULT_LED_FLASH_SLOW	FAULT_LED_FLASH
#define WARNING_LED_FLASH		500

// Battery Voltage Limits
#define BATT_1S_LOW				3300
#define BATT_1S_HIGH			4200
#define BATT_2S_LOW				6600
#define BATT_2S_HIGH			8400
// Battery Warning Above Lower Limit
#define BATT_WARNING			200

#define BATT_HYST				100

#define INPUT_TIMEOUT 			50


/*
 * PRIVATE TYPES
 */


/*
 * PRIVATE PROTOTYPES
 */


/*
 * PRIVATE VARIABLES
 */

Status_t status;

uint32_t battery_warn;
uint32_t battery_fault;

/*
 * PUBLIC FUNCTIONS
 */


void SYSTEM_Init(void)
{
	CONFIG_Init();
	ADC_Init();
	LED_Init();

	CORE_Delay(10); // Let stabilize before taking reading
	uint32_t volt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL),BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
	if (volt >= BATT_1S_LOW && volt <= BATT_1S_HIGH)
	{
		battery_fault = BATT_1S_LOW;
		battery_warn = BATT_1S_LOW + BATT_WARNING;
	}
	else if (volt >= BATT_2S_LOW && volt <= BATT_2S_HIGH)
	{
		battery_fault = BATT_2S_LOW;
		battery_warn = BATT_2S_LOW + BATT_WARNING;
	}
	else
	{
		battery_fault = BATT_1S_LOW;
		battery_warn = BATT_1S_LOW + BATT_WARNING;
	}
}


void SYSTEM_Update(void)
{
	int32_t SystemVolt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL),BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
	int32_t SystemTemp = ADC_ReadDieTemp();

	if (status.faultBatt == false) {
		if (SystemVolt <= battery_fault) { status.faultBatt = true; }
	}
	else { //status.faultBatt == true
		if (SystemVolt >= (battery_fault + BATT_HYST)) { status.faultBatt = false; }
	}

	if (status.warnBatt == false) {
		if (SystemVolt <= battery_warn) { status.warnBatt = true; }
	}
	else { //status.warnBatt == true
		if (SystemVolt >= (battery_warn + BATT_HYST)) { status.warnBatt = false; }
	}

	if (status.faultTemp == false) {
		if (SystemTemp <= battery_fault) { status.faultTemp = true; }
	}
	else { //status.faultTemp == true
		if (SystemTemp >= (battery_fault + BATT_HYST)) { status.faultTemp = false; }
	}

	if (status.warnTemp== false) {
		if (SystemTemp <= battery_warn) { status.warnTemp = true; }
	}
	else { //status.warnTemp == true
		if (SystemTemp >= (battery_warn + BATT_HYST)) { status.warnTemp = false; }
	}

	if (INPUT_TIMEOUT <= CORE_GetTick() - inputHeartbeat)
	{
		status.faultInput = true;
	}
	else
	{
		status.faultInput = false;
	}


	static uint32_t tick;
	uint32_t now = CORE_GetTick();

	if (status.faultTemp)
	{
		LED_RedOFF();
		if (FAULT_LED_FLASH_FAST <= (now - tick))
		{
			if (LED_GreenState())
			{
				LED_GreenOFF();
			}
			else
			{
				LED_GreenON();
			}
			tick = now;
		}
	}
	else if (status.faultInput)
	{
		LED_RedOFF();
		if (FAULT_LED_FLASH_SLOW <= (now - tick))
		{
			if (LED_GreenState())
			{
				LED_GreenOFF();
			}
			else
			{
				LED_GreenON();
			}
			tick = now;
		}
	}
	else if (status.faultBatt)
	{
		LED_RedOFF();
		LED_GreenON();
	}
	else if (status.warnTemp)
	{
		LED_RedON();
		if (FAULT_LED_FLASH_FAST <= (now - tick))
		{
			if (LED_GreenState())
			{
				LED_GreenOFF();
			}
			else
			{
				LED_GreenON();
			}
			tick = now;
		}
	}
	else if (status.warnBatt)
	{
		LED_RedON();
		LED_GreenON();
	}
	else
	{
		LED_RedON();
		LED_GreenOFF();
	}
}


/*
 * PRIVATE FUNCTIONS
 */


/*
 * INTERRUPT ROUTINES
 */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
