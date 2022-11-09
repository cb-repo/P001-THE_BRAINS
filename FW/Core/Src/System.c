/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "System.h"

/*
 * PRIVATE DEFINITIONS
 */

#define CALIBRATION_TEST_DELAY	2000

#define CALIBRATE_DRIVEINPUT_PERIOD	100
#define CALIBRATE_MOTORJERK_PERIOD	500

#define CALIBRATE_TIMEOUT			5000
#define CALIBRATE_ZERO_THRESHOLD	50
#define CALIBRATE_INPUT_THRESHOLD	200

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void SYSTEM_HandleFaultStatus(void);
void SYSTEM_HandleLEDs(void);
void SYSTEM_HandleCalibration (void);
void SYSTEM_HandleOutputs(void);
void SYSTEM_UpdateServo(void);
void SYSTEM_UpdateMotors(void);
void SYSTEM_VerifyConfig(void);

uint32_t SYSTEM_GetBatteryVoltage(void);
int32_t SYSTEM_RadioToMotor (uint16_t);
void SYSTEM_ReverseRadio(uint16_t*);

void SYSTEM_UpdateCalibration(void);
void SYSTEM_CalibrateSampleChannelZero (void);
void SYSTEM_CalibrateMotorSameDirection (SYSTEM_Config*);
void SYSTEM_CalibrateMotorOppositeDirection (SYSTEM_Config*);
void SYSTEM_CalibrateServoA (SYSTEM_Config*);
void SYSTEM_CalibrateServoB (SYSTEM_Config*);
void SYSTEM_WaitForResetInputs (void);


/*
 * PRIVATE VARIABLES
 */

SYSTEM_FaultStatus fault;
SYSTEM_Config config;

uint16_t channelZero[RADIO_NUM_CHANNELS];

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init(void)
{
	//
	ADC_Init();
	GPIO_Init(BATTERY_GPIO, BATTERY_PIN, GPIO_Mode_Analog);
	if (SYSTEM_GetBatteryVoltage() >= BATT_2S_LOW) {
		fault.faultVoltage = BATT_2S_LOW;
	} else {
		fault.faultVoltage = BATT_1S_LOW;
	}
	// INIT THE CALIBRATION INPUT
	GPIO_EnableInput(CALIBRATE_GPIO, CALIBRATE_PIN, GPIO_Pull_Up);

	// INIT LEDs
	LED_Init();

	// READ CONFIGURATION FROM EEPROM
	EEPROM_Read(EEPROM_OFFSET, &config, sizeof(config));

	// CHECK FOR VALID CONFIG
	SYSTEM_VerifyConfig();

	//
	RADIO_Init(&config.radio);
}

void SYSTEM_Update (void)
{
	// Update Inputs
	RADIO_Update();

	// Check For Fault Conditions
	SYSTEM_HandleFaultStatus();

	// Update LED's Based on Current Fault Status
	SYSTEM_HandleLEDs();

	// Handle System Calibration
	SYSTEM_UpdateCalibration();

	//
	SYSTEM_HandleOutputs();
}

/*
 * PRIVATE FUNCTIONS
 */

void SYSTEM_HandleFaultStatus (void)
{
	// Initialize Loop Variables
	uint32_t SystemVolt = SYSTEM_GetBatteryVoltage();
	int32_t SystemTemp = ADC_ReadDieTemp();
	RADIO_Data* ptrDataRadio = RADIO_GetDataPtr();

	//
	if (!fault.inputVoltage) {
		if (SystemVolt <= fault.faultVoltage) { fault.inputVoltage = true; }
	} else {
		if (SystemVolt >= (fault.faultVoltage + BATT_HYST)) { fault.inputVoltage = false; }
	}

	//
	if (!fault.overTemperature) {
		if (SystemTemp >= TEMP_HIGH) { fault.overTemperature = true; }
	} else {
		if (SystemTemp < (TEMP_HIGH + TEMP_HYST)) { fault.overTemperature = false; }
	}

	//
	if (ptrDataRadio->inputLost) {
		fault.signalLost = true;
	} else {
		fault.signalLost = false;
	}
}


void SYSTEM_HandleCalibration (void)
{
	// Initialize Loop Variables
	uint32_t now = CORE_GetTick();
	static bool calibrateWindow = true;

	//
	if (calibrateWindow && !fault.signalLost) {
		if (now > CALIBRATE_TIMEOUT) {
			calibrateWindow = false;
		} else if (now < CALIBRATE_TIMEOUT && !GPIO_Read(CALIBRATE_GPIO, CALIBRATE_PIN)) {
			SYSTEM_HandleCalibration();
			calibrateWindow = false;
		}
	}
}

void SYSTEM_HandleLEDs (void)
{
	// Initialize Loop Variables
	uint32_t now = CORE_GetTick();
	static uint32_t tick = 0;

	//
	if (fault.overTemperature)
	{
		LED_RedON();
		LED_GreenOFF();
	}
	else if (fault.inputVoltage)
	{
		LED_RedON();
		LED_GreenON();
	}
	else if (fault.signalLost)
	{
		LED_RedOFF();
		if (LED_FAULTFLASH <= (now - tick))
		{
			LED_GreenToggle();
			tick = now;
		}
	}
	else
	{
		LED_RedOFF();
		LED_GreenON();
	}
}

void SYSTEM_HandleOutputs (void)
{
	// Initialize Loop Variables
	static SYSTEM_FaultStatus fault_p;

	bool f = (fault.overTemperature || fault.inputVoltage || fault.signalLost);
	bool f_p = (fault_p.overTemperature || fault_p.inputVoltage || fault_p.signalLost);

	// FAULT CONDITION - RISING EDGE
	if (f && !f_p)
	{
		MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
		SERVO_Deinit();
	}

	// FAULT CONDITION - FALLING EDGE
	if (!f && f_p )
	{
		SERVO_Init();
	}

	// NO FAULT
	if (!f)
	{
		SYSTEM_UpdateServo();
		SYSTEM_UpdateMotors();
	}

	fault_p = fault;
//	memcpy(&fault_p, fault, sizeof(fault_p));
}

void SYSTEM_UpdateServo (void)
{
	// Extract appropriate data for servo
	RADIO_Data* ptrDataRadio = RADIO_GetDataPtr();
	uint16_t servoA = ptrDataRadio->ch[config.chServoA];
	uint16_t servoB = ptrDataRadio->ch[config.chServoB];

	// Check for channel reverse
	if (config.chRevMask[config.chServoA]) { SYSTEM_ReverseRadio(&servoA); }
	if (config.chRevMask[config.chServoB]) { SYSTEM_ReverseRadio(&servoB); }

	// Update Servos
	SERVO_Update(servoA, servoB);
}

void SYSTEM_UpdateMotors (void)
{
	// Extract appropriate data for motors
	RADIO_Data* ptrDataRadio = RADIO_GetDataPtr();
	uint16_t driveA = ptrDataRadio->ch[config.chDriveA];
	uint16_t driveB = ptrDataRadio->ch[config.chDriveB];

	// Check for channel reverse
	if (config.chRevMask[config.chDriveA]) { SYSTEM_ReverseRadio(&driveA); }
	if (config.chRevMask[config.chDriveB]) { SYSTEM_ReverseRadio(&driveB); }

	// Mix channels for Arcade drive mode
	if (config.mode == ARCADE)
	{
		uint16_t a = SYSTEM_RadioToMotor(driveA - (RADIO_CENTER - driveB));
		uint16_t b = SYSTEM_RadioToMotor(driveA + (RADIO_CENTER - driveB));
		driveA = a;
		driveB = b;
	}

	MOTOR_Update( SYSTEM_RadioToMotor(driveA), SYSTEM_RadioToMotor(driveB) );
}

void SYSTEM_VerifyConfig (void)
{
	if ((config.hashA != CONFIG_HASH_A) || (config.hashB != CONFIG_HASH_A))
	{
		config.hashA = 				CONFIG_HASH_A;

		config.mode = 				TANK;
		config.chDriveA = 			IP1;
		config.chDriveB = 			IP2;
		config.chServoA = 			IP3;
		config.chServoB = 			IP4;
		memset(config.chRevMask, 0, sizeof(config.chRevMask));

		config.radio.GPIO_PWM[IP1] = PWM_S1_GPIO;
		config.radio.GPIO_PWM[IP2] = PWM_S2_GPIO;
		config.radio.GPIO_PWM[IP3] = PWM_S3_GPIO;
		config.radio.GPIO_PWM[IP4] = PWM_S4_GPIO;
		config.radio.Pin_PWM[IP1] =	PWM_S1_PIN;
		config.radio.Pin_PWM[IP2] =	PWM_S2_PIN;
		config.radio.Pin_PWM[IP3] =	PWM_S3_PIN;
		config.radio.Pin_PWM[IP4] =	PWM_S4_PIN;
		config.radio.UART = 		RADIO_UART;
		config.radio.Baud_SBUS = 	SBUS_BAUD;
		config.radio.GPIO_UART = 	RADIO_UART_GPIO;
		config.radio.Pin_UART = 	RADIO_UART_PIN;
		config.radio.Timer = 		TIM_RADIO;
		config.radio.TimerFreq = 	TIM_RADIO_FREQ;
		config.radio.TimerReload =	TIM_RADIO_RELOAD;
		config.radio.Protocol =		PWM;
		config.radio.ptrDataRadio = RADIO_GetDataPtr();

		config.hashB = 			CONFIG_HASH_B;

		EEPROM_Write(EEPROM_OFFSET, &config, sizeof(config));
		LED_nPulse(3);
	}
}

uint32_t SYSTEM_GetBatteryVoltage (void)
{
	uint32_t ain = ADC_Read(BATTERY_CHANNEL);
	uint32_t mv = AIN_AinToMv(ain);
	return AIN_AinToDivider(mv, BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
}

int32_t SYSTEM_RadioToMotor (uint16_t radio)
{
	if (radio > RADIO_MAX)
	{
		radio = RADIO_MAX;
	}
	else if (radio < RADIO_MIN)
	{
		radio = RADIO_MIN;
	}

	return (((int32_t)radio - RADIO_CENTER) * MOTOR_MAX) / RADIO_HALFSCALE;
}

void SYSTEM_ReverseRadio (uint16_t* radio)
{
	uint16_t r = *radio;
	*radio = ((2 * RADIO_CENTER) - r);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SYSTEM_UpdateCalibration (void)
{
	// DISABLE ALL OUTPUTS
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	SERVO_Deinit();

	// CREATE LOCAL VARIABLE TO ASSIST IN CALIBRATION
	SYSTEM_Config c;

	//
	RADIO_Detect(&config.radio);
	RADIO_Init(&config.radio);

	//
	SYSTEM_CalibrateSampleChannelZero();

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_nPulse(5);
	LED_GreenON();
	LED_RedON();
	SYSTEM_CalibrateMotorSameDirection(&c);
	LED_RedOFF();
	LED_GreenOFF();

	//
	SYSTEM_WaitForResetInputs();
	CORE_Delay(CALIBRATION_TEST_DELAY);

	//
	LED_GreenON();
	LED_RedON();
	SYSTEM_CalibrateMotorOppositeDirection(&c);
	LED_RedOFF();
	LED_GreenOFF();

	//
	SYSTEM_WaitForResetInputs();
	CORE_Delay(CALIBRATION_TEST_DELAY);

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_GreenON();
	LED_RedON();
	SYSTEM_CalibrateServoA(&c);
	LED_RedOFF();
	LED_GreenOFF();

	// WAIT FOR INPUTS TO FALL BACK TO ZERO REFERENCE
	SYSTEM_WaitForResetInputs();
	CORE_Delay(CALIBRATION_TEST_DELAY);

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_GreenON();
	LED_RedON();
	SYSTEM_CalibrateServoB(&c);
	LED_RedOFF();
	LED_GreenOFF();

	// WAIT FOR INPUTS TO FALL BACK TO ZERO REFERENCE
	SYSTEM_WaitForResetInputs();
	CORE_Delay(CALIBRATION_TEST_DELAY);

	// WRITE NEW CONFIG TO EEPROM
	config.mode = c.mode;
	config.chDriveA = c.chDriveA;
	config.chDriveB = c.chDriveB;
	config.chServoA = c.chServoA;
	config.chServoB = c.chServoB;
	memcpy(config.chRevMask, c.chRevMask, sizeof(config.chRevMask));
	EEPROM_Write(EEPROM_OFFSET, &config, sizeof(config));

	// PULSE LED TO LET USER KNOW SUCCESSFUL
	LED_nPulse (5);
	LED_RedOFF();
	LED_GreenOFF();

	// Reinitialize Outputs
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	SERVO_Init();
}

void SYSTEM_CalibrateSampleChannelZero (void)
{
	// Initialize Function Variables
	RADIO_Data * ptrDataRadio = RADIO_GetDataPtr();

	// Update Radio Inputs
	RADIO_Update();

	// Sample All Radio Inputs
	for (uint8_t i = 0; i < ptrDataRadio->ch_num; i++)
	{
		channelZero[i] = ptrDataRadio->ch[i];
	}
}

void SYSTEM_CalibrateMotorSameDirection (SYSTEM_Config* c)
{
	// Initialize Function Variables
	RADIO_Data * ptrDataRadio = RADIO_GetDataPtr();

	// JERK THE MOTORS
	MOTOR_Update(MOTOR_MAX, MOTOR_MAX);
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);

	// CHECK FOR OPERATOR INPUT
	while (1)
	{
		// Initialize Loop Variables
		uint8_t numSticks = 0;
		uint32_t tick = CORE_GetTick();

		// Update Radio Inputs
		RADIO_Update();

		// Check All Inputs
		for (uint8_t i = 0; i < ptrDataRadio->ch_num; i++)
		{
			// Detect Drive Channel A
			if (numSticks == 0)
			{
				if (ptrDataRadio->ch[i] > (channelZero[i] + CALIBRATE_INPUT_THRESHOLD))
				{
					c->chDriveA = i;
					c->chRevMask[i] = false;
					numSticks += 1;
				}
				else if (ptrDataRadio->ch[i] < (channelZero[i] - CALIBRATE_INPUT_THRESHOLD))
				{
					c->chDriveA = i;
					c->chRevMask[i] = true;
					numSticks += 1;
				}
			}
			// Detect Drive Channel B
			else // if (numSticks >= 1)
			{
				if (ptrDataRadio->ch[i] > (channelZero[i] + CALIBRATE_INPUT_THRESHOLD))
				{
					c->chDriveB = i;
					c->chRevMask[i] = false;
					numSticks += 1;
				}
				else if (ptrDataRadio->ch[i] < (channelZero[i] - CALIBRATE_INPUT_THRESHOLD))
				{
					c->chDriveB = i;
					c->chRevMask[i] = true;
					numSticks += 1;
				}
			}
		}

		// Check For Break Condition
		if (numSticks == 1) {
			c->mode = ARCADE;
			break;
		} else if (numSticks == 2) 	{
			c->mode = TANK;
			break;
		}

		while (CALIBRATE_DRIVEINPUT_PERIOD >= CORE_GetTick() - tick)
		{
			RADIO_Update();
			CORE_Idle();
		}
	}
}


void SYSTEM_CalibrateMotorOppositeDirection (SYSTEM_Config* c)
{
	// Initialize Function Variables
	RADIO_Data * ptrDataRadio = RADIO_GetDataPtr();

	// JERK THE MOTORS
	MOTOR_Update(-MOTOR_MAX, MOTOR_MAX);
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);


	// CHECK FOR OPERATOR INPUT
	while (1)
	{
		// Initialize Loop Variables
		uint8_t numSticks = 0;
		uint32_t tick = CORE_GetTick();

		// Update Radio Inputs
		RADIO_Update();

		// Check All Inputs

		switch (c->mode) {
		case TANK:;
			bool driveA = false;
			bool driveRevA = false;
			if (ptrDataRadio->ch[c->chDriveA] > (channelZero[c->chDriveA] + CALIBRATE_INPUT_THRESHOLD)) {
				driveA = true;
			} else if (ptrDataRadio->ch[c->chDriveA] < (channelZero[c->chDriveA] - CALIBRATE_INPUT_THRESHOLD)) {
				driveA = true;
				driveRevA = true;
			}

			bool driveB = false;
			bool driveRevB = false;
			if (ptrDataRadio->ch[c->chDriveB] > (channelZero[c->chDriveB] + CALIBRATE_INPUT_THRESHOLD)) {
				driveB = true;
			} else if (ptrDataRadio->ch[c->chDriveB] < (channelZero[c->chDriveB] - CALIBRATE_INPUT_THRESHOLD)) {
				driveB = true;
				driveRevB = true;
			}
			// Check if both sticks are pressed and one channel has been reversed
			if (  driveA &&
				  driveB &&
				((driveRevA == c->chRevMask[c->chDriveA]) != (driveRevB == c->chRevMask[c->chDriveB])) )
			{
				// if driveB has been reversed then channels A and B need to be swapped
				if (driveRevB != c->chRevMask[c->chDriveB]) {
					SYSTEM_Config cInt;
					cInt.chDriveA = c->chDriveA;
					c->chDriveA = c->chDriveB;
					c->chDriveB = cInt.chDriveA;
				}
				numSticks = 1;
			}
			break; //break from case

		case ARCADE:;
			for (uint8_t i = 0; i < ptrDataRadio->ch_num; i++)
			{
				if (numSticks == 0 && i != c->chDriveA)
				{
					if (ptrDataRadio->ch[i] > (channelZero[i] + CALIBRATE_INPUT_THRESHOLD))
					{
						c->chDriveB = i;
						c->chRevMask[i] = true;
						numSticks += 1;
					}
					else if (ptrDataRadio->ch[i] < (channelZero[i] - CALIBRATE_INPUT_THRESHOLD))
					{
						c->chDriveB = i;
						c->chRevMask[i] = false;
						numSticks += 1;
					}

				}
			}
			break; //break from case
		}

		// Check For Break Condition
		if (numSticks == 1) {
			break;
		}

		// Loop Pacing
		while (CALIBRATE_DRIVEINPUT_PERIOD >= CORE_GetTick() - tick)
		{
			RADIO_Update();
			CORE_Idle();
		}
	}
}


void SYSTEM_CalibrateServoA (SYSTEM_Config* c)
{
	// Initialize Function Variables
	RADIO_Data * ptrDataRadio = RADIO_GetDataPtr();

	//
	while (1)
	{
		// Initialize Loop Variables
		uint8_t numSticks = 0;

		// Update Radio Inputs
		RADIO_Update();

		// Check All Inputs
		for (uint8_t i = 0; i < ptrDataRadio->ch_num; i++)
		{
			if ( (i != config.chDriveA) && (i != config.chDriveB) )
			{
				if (ptrDataRadio->ch[i] > (channelZero[i] + CALIBRATE_ZERO_THRESHOLD))
				{
					config.chServoA = i;
					config.chRevMask[i] = false;
					numSticks += 1;
				} else if (ptrDataRadio->ch[i] < (channelZero[i] - CALIBRATE_ZERO_THRESHOLD)) {
					config.chServoA = i;
					config.chRevMask[i] = true;
					numSticks += 1;
				}
			}
		}

		// Check For Break Condition
		if (numSticks == 1) { break; }

		// Loop Pacing
		CORE_Idle();
	}
}


void SYSTEM_CalibrateServoB (SYSTEM_Config* c)
{
	// Initialize Function Variables
	RADIO_Data * ptrDataRadio = RADIO_GetDataPtr();

	//
	while (1)
	{
		// Initialize Loop Variables
		uint8_t numSticks = 0;

		// Update Radio Inputs
		RADIO_Update();

		// Check All Inputs
		for (uint8_t i = 0; i < ptrDataRadio->ch_num; i++)
		{
			if ( (i != config.chDriveA) && (i != config.chDriveB) )
			{
				if (ptrDataRadio->ch[i] > (channelZero[i] + CALIBRATE_ZERO_THRESHOLD))
				{
					config.chServoB = i;
					config.chRevMask[i] = false;
					numSticks += 1;
				} else if (ptrDataRadio->ch[i] < (channelZero[i] - CALIBRATE_ZERO_THRESHOLD)) {
					config.chServoB = i;
					config.chRevMask[i] = true;
					numSticks += 1;
				}
			}
		}

		// Check For Break Condition
		if (numSticks == 1) { break; }

		// Loop Pacing
		CORE_Idle();
	}
}


void SYSTEM_WaitForResetInputs (void)
{
	// Initialize Function Variables
	RADIO_Data * ptrDataRadio = RADIO_GetDataPtr();

	//
	while (1)
	{
		// Initialize Loop Variables
		uint8_t numSticks = 0;

		// Update Radio Inputs
		RADIO_Update();

		// Check All Inputs
		for (uint8_t i = 0; i < ptrDataRadio->ch_num; i++)
		{
			if ((ptrDataRadio->ch[i] > (channelZero[i] + CALIBRATE_ZERO_THRESHOLD)) || (ptrDataRadio->ch[i] < (channelZero[i] - CALIBRATE_ZERO_THRESHOLD)))
			{
				numSticks += 1;
			}
		}

		// Check For Break Condition
		if (numSticks == 0) { break; }

		// Loop Pacing
		CORE_Idle();
	}
}


/*
 * INTERRUPT ROUTINES
 */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
