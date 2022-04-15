/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "System.h"
#include "Core.h"
#include "Board.h"
#include "GPIO.h"
#include "Radio.h"
#include "Servo.h"
#include "Motor.h"
#include "ADC.h"
#include "LED.h"
#include "EEPROM.h"

/*
 * PRIVATE DEFINITIONS
 */

#define EEPROM_OFFSET				0
#define CONFIG_WRITE_HASHA			0x7ee12326
#define CONFIF_WRITE_HASHB			0x8771a50d

#define CALIBRATE_TIMEOUT			5000
#define CALIBRATE_RADIO_POLL_PERIOD	500
#define CALIBRATE_INPUT_THRESHOLD	100
#define CALIBRATE_INPUT_HYST		20

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

uint32_t SYSTEM_GetBatteryVoltage(void);
void SYSTEM_Calibrate(void);
void SYSTEM_WaitForResetInputs(uint16_t*);


/*
 * PRIVATE VARIABLES
 */

SYSTEM_Status status;
SYSTEM_Status status_p;
SYSTEM_Config config;
SYSTEM_Config configDefault = {CONFIG_WRITE_HASHA, TANK, IP3, 1, IP2, 1, IP1, 1, IP4, 1, CONFIF_WRITE_HASHB};

uint32_t battery_fault;

bool calibrateWindow = true;

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init(void)
{
//	ADC_Init();
//	GPIO_Init(BATTERY_GPIO, BATTERY_PIN, GPIO_Mode_Analog);
//	if (SYSTEM_GetBatteryVoltage() >= BATT_2S_LOW)
//	{
//		battery_fault = BATT_2S_LOW;
//	}
//	else
//	{
//		battery_fault = BATT_1S_LOW;
//	}
	// INIT THE CALIBRATION INPUT
	GPIO_EnableInput(CALIBRATE_GPIO, CALIBRATE_PIN, GPIO_Pull_Up);

	// INIT STATUS LED
	LED_Init();

	// READ CONFIGURATION FROM EEPROM
	EEPROM_Read(EEPROM_OFFSET, &config, sizeof(config));

	// CHECK FOR VALID CONFIG
	if ((config.hashA != configDefault.hashA) || (config.hashB != configDefault.hashB))
	{
		config.hashA = configDefault.hashA;
		config.mode = configDefault.mode;
		config.primaryCh = configDefault.primaryCh;
		config.primaryRev = configDefault.primaryRev;
		config.secondaryCh = configDefault.secondaryCh;
		config.secondaryRev = configDefault.secondaryRev;
		config.servoChA = configDefault.servoChA;
		config.servoRevA = configDefault.servoRevA;
		config.servoChB = configDefault.servoChB;
		config.servoRevB = configDefault.servoRevB;
		config.hashB = configDefault.hashB;
		EEPROM_Write(EEPROM_OFFSET, &config, sizeof(config));
		LED_nPulse(5);
	}
}

void SYSTEM_Update(void)
{
	status_p = status;

	static uint32_t tick;
	uint32_t now = CORE_GetTick();

//	uint32_t SystemVolt = SYSTEM_GetBatteryVoltage();
//	int32_t SystemTemp = ADC_ReadDieTemp();

//	if (status.faultBatt == false) {
//		if (SystemVolt <= battery_fault) { status.faultBatt = true; }
//	}
//	else { //status.faultBatt == true
//		if (SystemVolt >= (battery_fault + BATT_HYST)) { status.faultBatt = false; }
//	}

//	if (status.faultTemp == false) {
//		if (SystemTemp >= TEMP_HIGH) { status.faultTemp = true; }
//	}
//	else { //status.faultTemp == true
//		if (SystemTemp < (TEMP_HIGH + TEMP_HYST)) { status.faultTemp = false; }
//	}

	if (INPUT_TIMEOUT <= CORE_GetTick() - inputHeartbeat)
	{
		status.faultInput = true;
	}
	else
	{
		status.faultInput = false;
	}

	// CHECK FOR CALIBRATION INPUT
	if (calibrateWindow && now < CALIBRATE_TIMEOUT && !GPIO_Read(CALIBRATE_GPIO, CALIBRATE_PIN) && !status.faultInput)
	{
		SYSTEM_Calibrate();
		calibrateWindow = false;
	}
	else if (calibrateWindow && now > CALIBRATE_TIMEOUT)
	{
		calibrateWindow = false;
	}

	if (status.faultTemp)
	{
		LED_RedOFF();
		LED_GreenON();
	}
	else if (status.faultInput)
	{
		LED_RedOFF();
		if (FAULT_LED_FLASH <= (now - tick))
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
		LED_RedON();
		LED_GreenON();
	}
	else
	{
		LED_RedON();
		LED_GreenOFF();
	}
}

int32_t SYSTEM_RadioToMotor (uint16_t radio)
{
	return (((int32_t)radio - RADIO_CENTER) * 256) / 500;
}

uint16_t SYSTEM_ReverseRadio(uint16_t radio)
{
	return ((2 * RADIO_CENTER) - radio);
}

uint16_t SYSTEM_RadioTruncate (uint16_t radio)
{
	if (radio > RADIO_MAX)
	{
		radio = RADIO_MAX;
	}
	else if (radio < RADIO_MIN)
	{
		radio = RADIO_MIN;
	}
	return radio;
}

/*
 * PRIVATE FUNCTIONS
 */

uint32_t SYSTEM_GetBatteryVoltage(void)
{
	uint32_t ain = ADC_Read(BATTERY_CHANNEL);
	uint32_t mv = AIN_AinToMv(ain);
	return AIN_AinToDivider(mv, BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
}

void SYSTEM_Calibrate(void)
{
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	SERVO_Deinit();

	// CREATE LOCAL VARIABLE TO ASSIT IN CALIBRATION
	SYSTEM_Config configInt;
	bool calibrated;
	uint16_t inputZero[NUM_TOTALINPUTS];

	// SAMPLE INPUTS FOR ZERO REFERENCE
	for (uint8_t i = 0; i < NUM_TOTALINPUTS; i++)
	{
		inputZero[i] = input[i];
	}

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_nPulse(4);
	CORE_Delay(300);
	LED_GreenON();
	LED_RedON();

	// CHECK FOR OPERATOR INPUT
	calibrated = false;
	while (!calibrated)
	{
		CORE_Delay(CALIBRATE_RADIO_POLL_PERIOD);

		uint8_t numSticks = 0;

		for (uint8_t i = 0; i < NUM_TOTALINPUTS; i++)
		{
			if (input[i] > (inputZero[i] + CALIBRATE_INPUT_THRESHOLD))
			{
				numSticks += 1;
				if (numSticks == 1)
				{
					configInt.primaryCh = i;
					configInt.primaryRev = CH_FWD;
				}
				else if (numSticks == 2)
				{
					configInt.secondaryCh = i;
					configInt.secondaryRev = CH_FWD;
				}
			}
			else if (input[i] < (inputZero[i] - CALIBRATE_INPUT_THRESHOLD))
			{
				numSticks += 1;
				if (numSticks == 1)
				{
					configInt.primaryCh = i;
					configInt.primaryRev = CH_RVS;
				}
				else if (numSticks == 2)
				{
					configInt.secondaryCh = i;
					configInt.secondaryRev = CH_RVS;
				}
			}
		}
		if (numSticks == 1)
		{
			configInt.mode = ARCADE;
			calibrated = true;
		}
		else if (numSticks == 2)
		{
			configInt.mode = TANK;
			calibrated = true;
		}
	}

	// WAIT FOR INPUTS TO FALL BACK TO ZERO REFERENCE
	SYSTEM_WaitForResetInputs(inputZero);
	CORE_Delay(100);
	LED_RedOFF();
	LED_GreenOFF();

	// 	DELAY BETWEEN TESTS
	CORE_Delay(2000);

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_GreenON();
	LED_RedON();

	// CHECK FOR OPERATOR INPUT
	calibrated = false;
	SYSTEM_Config configInt2;
	while (!calibrated)
	{
		CORE_Delay(CALIBRATE_RADIO_POLL_PERIOD);

		uint8_t numSticks = 0;

		if (configInt.mode == ARCADE)
		{
			for (uint8_t i = 0; i < NUM_TOTALINPUTS; i++)
			{
				if (input[i] > (inputZero[i] + CALIBRATE_INPUT_THRESHOLD))
				{
					numSticks += 1;
					if ((numSticks == 1) && (configInt.primaryCh != i))
					{
						configInt.secondaryCh = i;
						configInt.secondaryRev = CH_FWD;
					}
				}
				else if (input[i] < (inputZero[i] - CALIBRATE_INPUT_THRESHOLD))
				{
					numSticks += 1;
					if ((numSticks == 1) && (configInt.primaryCh != i))
					{
						configInt.secondaryCh = i;
						configInt.secondaryRev = CH_RVS;
					}
				}
			}
			if (numSticks == 1)
			{
				calibrated = true;
			}
		}
		else if (configInt.mode == TANK)
		{
			for (uint8_t i = 0; i < NUM_TOTALINPUTS; i++)
			{
				if (input[i] > (inputZero[i] + CALIBRATE_INPUT_THRESHOLD))
				{
					numSticks += 1;
					if (numSticks == 1)
					{
						configInt2.primaryCh = i;
						configInt2.primaryRev = CH_FWD;
					}
					else if (numSticks == 2)
					{
						configInt2.secondaryCh = i;
						configInt2.secondaryRev = CH_FWD;
					}
				}
				else if (input[i] < (inputZero[i] - CALIBRATE_INPUT_THRESHOLD))
				{
					numSticks += 1;
					if (numSticks == 1)
					{
						configInt2.primaryCh = i;
						configInt2.primaryRev = CH_RVS;
					}
					else if (numSticks == 2)
					{
						configInt2.secondaryCh = i;
						configInt2.secondaryRev = CH_RVS;
					}
				}
			}
			if (numSticks == 2 &&
				((configInt.primaryCh == configInt2.primaryCh) || (configInt.primaryCh == configInt2.secondaryCh)) &&
				((configInt.secondaryCh == configInt2.secondaryCh) || (configInt.secondaryCh == configInt2.primaryCh)) &&
				((configInt.primaryRev == configInt2.primaryRev) || (configInt.secondaryRev == configInt2.secondaryRev)) &&
				((configInt.primaryRev != configInt2.primaryRev) || (configInt.secondaryRev != configInt2.secondaryRev)))
			{
				calibrated = true;
				if (configInt.secondaryRev != configInt2.secondaryRev)
				{
					configInt2.primaryCh = configInt.secondaryCh;
					configInt2.primaryRev = configInt.secondaryRev;
					configInt.secondaryCh = configInt.primaryCh;
					configInt.secondaryRev = configInt.primaryRev;
					configInt.primaryCh = configInt2.primaryCh;
					configInt.primaryRev = configInt2.primaryRev;
				}
			}
		}
	}

	// WAIT FOR INPUTS TO FALL BACK TO ZERO REFERENCE
	SYSTEM_WaitForResetInputs(inputZero);
	CORE_Delay(100);
	LED_RedOFF();
	LED_GreenOFF();

	// 	DELAY BETWEEN TESTS
	CORE_Delay(2000);

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_GreenON();
	LED_RedON();

	// CHECK FOR OPERATOR INPUT
	calibrated = false;
	while (!calibrated)
	{
		CORE_Delay(CALIBRATE_RADIO_POLL_PERIOD);

		uint8_t numSticks = 0;

		for (uint8_t i = 0; i < NUM_TOTALINPUTS; i++)
		{
			if (input[i] > (inputZero[i] + CALIBRATE_INPUT_THRESHOLD))
			{
				numSticks += 1;
				if (numSticks == 1)
				{
					configInt.servoChA = i;
					configInt.servoRevA = CH_FWD;
				}
			}
			else if (input[i] < (inputZero[i] - CALIBRATE_INPUT_THRESHOLD))
			{
				numSticks += 1;
				if (numSticks == 1)
				{
					configInt.servoChA = i;
					configInt.servoRevA = CH_RVS;
				}
			}
		}
		if (numSticks == 1 && (configInt.servoChA != configInt.primaryCh) && (configInt.servoChA != configInt.secondaryCh))
		{
			calibrated = true;
		}
	}

	// WAIT FOR INPUTS TO FALL BACK TO ZERO REFERENCE
	SYSTEM_WaitForResetInputs(inputZero);
	CORE_Delay(100);
	LED_RedOFF();
	LED_GreenOFF();

	// 	DELAY BETWEEN TESTS
	CORE_Delay(2000);

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_GreenON();
	LED_RedON();

	// CHECK FOR OPERATOR INPUT
	calibrated = false;
	while (!calibrated)
	{
		CORE_Delay(CALIBRATE_RADIO_POLL_PERIOD);

		uint8_t numSticks = 0;

		for (uint8_t i = 0; i < NUM_TOTALINPUTS; i++)
		{
			if (input[i] > (inputZero[i] + CALIBRATE_INPUT_THRESHOLD))
			{
				numSticks += 1;
				if (numSticks == 1)
				{
					configInt.servoChB = i;
					configInt.servoRevB = CH_FWD;
				}
			}
			else if (input[i] < (inputZero[i] - CALIBRATE_INPUT_THRESHOLD))
			{
				numSticks += 1;
				if (numSticks == 1)
				{
					configInt.servoChB = i;
					configInt.servoRevB = CH_RVS;
				}
			}
		}
		if (numSticks == 1 && (configInt.servoChB != configInt.primaryCh) && (configInt.servoChB != configInt.secondaryCh))
		{
			calibrated = true;
		}
	}

	// WAIT FOR INPUTS TO FALL BACK TO ZERO REFERENCE
	SYSTEM_WaitForResetInputs(inputZero);
	CORE_Delay(100);
	LED_RedOFF();
	LED_GreenOFF();

	// 	DELAY BETWEEN TESTS
	CORE_Delay(1000);

	// WRITE NEW CONFIG TO EEPROM
	config = configInt;
	EEPROM_Write(EEPROM_OFFSET, &config, sizeof(config));

	// PULSE LED TO LET USER KNOW SUCCESSFUL
	LED_nPulse (5);

	// 	DELAY AT END OF TEST
	CORE_Delay(1000);

	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	SERVO_Init();
}

void SYSTEM_WaitForResetInputs(uint16_t* inputZero)
{
	bool centered = false;
	while (!centered)
	{
		CORE_Delay(CALIBRATE_RADIO_POLL_PERIOD);
		uint8_t numSticks = 0;
		for (uint8_t i = 0; i < NUM_TOTALINPUTS; i++)
		{
			if (input[i] > (inputZero[i] + CALIBRATE_INPUT_THRESHOLD - CALIBRATE_INPUT_HYST) || input[i] < (inputZero[i] - CALIBRATE_INPUT_THRESHOLD + CALIBRATE_INPUT_HYST))
			{
				numSticks += 1;
			}
		}
		if (numSticks == 0)
		{
			centered = true;
		}
	}
}

/*
 * INTERRUPT ROUTINES
 */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
