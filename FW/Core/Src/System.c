/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#include "System.h"


/*
 * PRIVATE DEFINITIONS
 */


#define CALIBRATION_TEST_DELAY		1000

#define CALIBRATE_DRIVEINPUT_PERIOD	200
#define CALIBRATE_MOTORJERK_PERIOD	500

#define CALIBRATE_TIMEOUT			10000

#define FAULT_VOLTAGE_TRIP			100

#define STARTUP_RADIO_TIMEOUT		400

#define CALIBRATION_INPUT_WIGGLE	20


/*
 * PRIVATE TYPES
 */


/*
 * PRIVATE PROTOTYPES
 */


void 		SYSTEM_InitRadio						( void );
void 		SYSTEM_HandleFaultStatus				( void );
void 		SYSTEM_HandleLEDs						( void );
void 		SYSTEM_HandleCalibration 				( void );
void 		SYSTEM_HandleOutputs					( void );
void 		SYSTEM_UpdateServo						( void );
void 		SYSTEM_UpdateMotors						( void );
void 		SYSTEM_VerifyConfig						( void );

uint32_t	SYSTEM_GetBatteryVoltage 				( void );
int32_t		SYSTEM_RadioToMotor 					( uint16_t );
uint16_t 	SYSTEM_ReverseRadio 					( uint16_t );

bool		SYSTEM_InitiateCalibration				( void );
void 		SYSTEM_UpdateCalibration				( void );
void 		SYSTEM_CalibrateMotorSameDirection 		( SYSTEM_Config* );
void 		SYSTEM_CalibrateMotorOppositeDirection	( SYSTEM_Config* );
void 		SYSTEM_CalibrateServoA 					( SYSTEM_Config* );
void 		SYSTEM_CalibrateServoB 					( SYSTEM_Config* );
void 		SYSTEM_WaitForActiveInput 				( void );
void 		SYSTEM_WaitForResetInputs 				( void );
void 		SYSTEM_WaitForResetInputsServo 			( SYSTEM_Config* );
uint32_t 	SYSTEM_CountActiveInputs 				( void );
uint32_t 	SYSTEM_CountActiveInputsServo 			( SYSTEM_Config* );


/*
 * PRIVATE VARIABLES
 */


SYSTEM_FaultStatus fault;
SYSTEM_Config config;

RADIO_Data * ptrRadioData;

bool calibrateWindow = false;

/*
 * PUBLIC FUNCTIONS
 */


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_Init(void)
{
	// INIT ADC, USED FOR BATTERY VOLTAGE AND TEMPERATURE DETECTION
	ADC_Init();

	// INIT BATTERY VOLTAGE DETECT INPUT AND DETERMINE IF 1S OR 2S INPUT
	GPIO_Init(BATTERY_GPIO, BATTERY_PIN, GPIO_Mode_Analog);
	if (SYSTEM_GetBatteryVoltage() >= BATT_2S_LOW) {
		fault.faultVoltage = BATT_2S_LOW;
	} else {
		fault.faultVoltage = BATT_1S_LOW;
	}

	// INIT THE CALIBRATION INPUT
	GPIO_EnableInput(CALIBRATE_GPIO, CALIBRATE_PIN, GPIO_Pull_Up);

	// INIT STATUS LEDs
	LED_Init();

	// READ CONFIGURATION FROM EEPROM
	EEPROM_Read(EEPROM_OFFSET, &config, sizeof(config));

	// CHECK FOR VALID CONFIG
	SYSTEM_VerifyConfig();

	// INIT RADIO BASED ON EXISTING CONFIG
	ptrRadioData = RADIO_GetDataPtr();
	SYSTEM_InitRadio();
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_Update (void)
{
	// Update Inputs
	RADIO_Update();

	// Check For Fault Conditions
	SYSTEM_HandleFaultStatus();

	// Update LED's Based on Current Fault Status
	SYSTEM_HandleLEDs();

	// Handle System Calibration
	SYSTEM_HandleCalibration();

	//
	SYSTEM_HandleOutputs();
}


/*
 * PRIVATE FUNCTIONS
 */


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_InitRadio (void)
{
	// INIT FUNCTION VARIABLES
	uint32_t now = CORE_GetTick();
	uint32_t tick = now;

	// INIT RADIO MODULE
	RADIO_Init(&config.radio);

	// ALLOW TIME FOR DETECTION OF EXISTING RADIO CONFIG
	while ( STARTUP_RADIO_TIMEOUT >= (now - tick) )
	{
		// UPDATE LOOP VARIABLES
		now = CORE_GetTick();
		// UPDATE RADIO DATA
		RADIO_Update();
		// NO NEED TO PROGRESS IF RADIO SUCCESSFULLY CONNECTED
		if ( !ptrRadioData->inputLost )
		{
			break;
		}
		// LOOP PACING
		CORE_Idle();
	}

	// IF NO RADIO WAS DETECTED ON EXISTING CONFIG
	if ( ptrRadioData->inputLost )
	{
		// IF A NEW RADIO IS DETECTED
		if ( RADIO_DetInit(&config.radio) )
		{
			// WRITE NEW CONFIG TO EEPROM
			EEPROM_Write(EEPROM_OFFSET, &config, sizeof(config));
		}
	}

	// CHECK IF VALID RADIO IS WAS DETECTED, EXISTING OR NEW
	if ( !ptrRadioData->inputLost )
	{
		// ENABLE POSSIBILITY OF CALIBRATION AT LATER DATE
		calibrateWindow = true;
	}
}




/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_HandleFaultStatus (void)
{
	static bool faultVoltage = false;
	static uint32_t tickVolt = 0;
	uint32_t SystemVolt = SYSTEM_GetBatteryVoltage();
	int32_t SystemTemp = ADC_ReadDieTemp();

	//
	if (!faultVoltage) {
		if (SystemVolt <= fault.faultVoltage) {
			faultVoltage = true;
			tickVolt = CORE_GetTick();
		}
	} else {
		if (SystemVolt >= (fault.faultVoltage + BATT_HYST)) {
			faultVoltage = false;
			fault.inputVoltage = false;
		} else if (FAULT_VOLTAGE_TRIP <= (CORE_GetTick() - tickVolt)) {
			fault.inputVoltage = true;
		}
	}

	//
	if (!fault.overTemperature)
	{
		if (SystemTemp >= TEMP_HIGH) {
			fault.overTemperature = true;
		}
	}
	else
	{
		if (SystemTemp <= (TEMP_HIGH - TEMP_HYST)) {
			fault.overTemperature = false;
		}
	}

	//
	if (ptrRadioData->inputLost) {
		fault.signalLost = true;
	} else {
		fault.signalLost = false;
	}
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_HandleCalibration (void)
{
	// INIT LOOP VARIABLES
	uint32_t now = CORE_GetTick();

	// IF STILL WITHIN BOOT CALIBRATION WINDOW
	if (calibrateWindow)
	{
		// HAS THE USER INDICATED THEY WANT TO CALIBRATE DEVICE
		if ( SYSTEM_InitiateCalibration() )
		{
			// RUN CALIBRATION SEQUENCE
			SYSTEM_UpdateCalibration();
			calibrateWindow = false;
		}
		// HAS THE WINDOW ELAPSED
		if ( now >= CALIBRATE_TIMEOUT )
		{
			calibrateWindow = false;
		}
	}
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_HandleLEDs (void)
{
	// INIT LOOP VARIABLES
	uint32_t now = CORE_GetTick();
	static uint32_t tick = 0;

	// UPDATE LED BASED ON FAULT STATES
	if (fault.overTemperature)
	{
		LED_GreenOFF();
		if (LED_FAULT_TEMPFLASH <= (now - tick))
		{
			LED_RedToggle();
			tick = now;
		}
	}
	else if (fault.inputVoltage)
	{
		LED_GreenOFF();
		if (LED_FAULT_VOLTAGEFLASH <= (now - tick))
		{
			LED_RedToggle();
			tick = now;
		}
	}
	else if (fault.signalLost)
	{
		LED_RedOFF();
		if (LED_FAULT_SIGNALFLASH <= (now - tick))
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


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_HandleOutputs (void)
{
	// Initialize Loop Variables
	bool f = (fault.overTemperature || fault.inputVoltage || fault.signalLost);
	static bool f_p = true; // Init servo on first loop

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

	//
	f_p = f;
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_UpdateServo (void)
{
	// Extract appropriate data for servo
	uint16_t servoA = ptrRadioData->ch[config.chServoA];
	uint16_t servoB = ptrRadioData->ch[config.chServoB];

	// Check for channel reverse
	if (config.chServoArev) { servoA = SYSTEM_ReverseRadio(servoA); }
	if (config.chServoBrev) { servoB = SYSTEM_ReverseRadio(servoB); }

	// Update Servos
	SERVO_Update(servoA, servoB);
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_UpdateMotors (void)
{
	// Extract appropriate data for motors
	uint16_t driveA = ptrRadioData->ch[config.chDriveA];
	uint16_t driveB = ptrRadioData->ch[config.chDriveB];

	// Check for channel reverse
	if (config.chDriveArev) { driveA = SYSTEM_ReverseRadio(driveA); }
	if (config.chDriveBrev) { driveB = SYSTEM_ReverseRadio(driveB); }

	if (driveA && driveB)
	{
		// Check for channel reverse
		if (config.chDriveArev) { driveA = SYSTEM_ReverseRadio(driveA); }
		if (config.chDriveBrev) { driveB = SYSTEM_ReverseRadio(driveB); }

		// Mix channels for Arcade drive mode
		if (config.mode == ARCADE)
		{
			int32_t dA = driveA - (RADIO_CH_CENTER - driveB);
			int32_t dB = driveA + (RADIO_CH_CENTER - driveB);
			driveA = (uint16_t)dA;
			driveB = (uint16_t)dB;
		}
		MOTOR_Update( SYSTEM_RadioToMotor(driveA), SYSTEM_RadioToMotor(driveB) );
	}
	else
	{
		MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	}
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_VerifyConfig (void)
{
	// CHECK FOR CORRECT CONFIG HASH VALUES - CALIBRATION WRITTEN TO EEPROM
	if ((config.hashA != CONFIG_HASH_A) || (config.hashB != CONFIG_HASH_B))
	{
		// FRESH DEVICE USED SO WRITE DEFAULT CONFIG TO EEPROM
		config.hashA = 				CONFIG_HASH_A;
		config.mode = 				TANK;
		config.chDriveA = 			IP1;
		config.chDriveArev = 		false;
		config.chDriveB = 			IP2;
		config.chDriveBrev = 		false;
		config.chServoA = 			IP3;
		config.chServoArev =		false;
		config.chServoB = 			IP4;
		config.chServoBrev = 		false;
		config.radio.Baud_SBUS = 	SBUS_BAUD;
		config.radio.Protocol =		SBUS;
		config.hashB = 				CONFIG_HASH_B;
		EEPROM_Write(EEPROM_OFFSET, &config, sizeof(config));
	}
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
uint32_t SYSTEM_GetBatteryVoltage (void)
{
	uint32_t ain = ADC_Read(BATTERY_CHANNEL);
	return AIN_AinToDivider(ain, BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
int32_t SYSTEM_RadioToMotor (uint16_t radio)
{
	if (radio > RADIO_CH_MAX) {
		radio = RADIO_CH_MAX;
	} else if (radio < RADIO_CH_MIN) {
		radio = RADIO_CH_MIN;
	}

	return (((int32_t)radio - RADIO_CH_CENTER) * MOTOR_MAX) / RADIO_CH_HALFSCALE;
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
uint16_t SYSTEM_ReverseRadio (uint16_t radio)
{
	return ((2 * RADIO_CH_CENTER) - radio);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
bool SYSTEM_InitiateCalibration (void)
{
	// INIT LOOP VARIABLES
	static RADIO_ChannelActiveFlags	chActive_p[RADIO_NUM_CHANNELS]	= {chActive_False};
	static uint8_t 					chCounter[RADIO_NUM_CHANNELS] 	= {0};
	bool 							retVal 							= false;

	// ONLY PROCEED IF THERE IS A RADIO DETECTED AND CHANNEL NEUTRAL/ZERO POSITION ARRAY SET
	if ( ptrRadioData->chZeroSet && !ptrRadioData->inputLost )
	{
		// DETECT METHOD 1: STICK WIGGLE
		// ITTERATE THROUGH EACH AVALIBLE RADIO CHANNEL
		for (uint8_t ch = 0; ch < ptrRadioData->ch_num; ch++)
		{
			// IF THE CHANNEL IS NOW INACTIVE BUT WAS PREVIOUSLY ACTIVE
			if ( (ptrRadioData->chActive[ch] == chActive_False) &&
			   ( (chActive_p[ch] == chActive_True) ||
				 (chActive_p[ch] == chActive_TrueRev ) ) )
			{
				// INCREMENT THE COUNTER
				chCounter[ch] += 1;
			}

			// CHECK IF ENOUGH INPUT WIGGLES HAVE BEEN DONE
			if (chCounter[ch] > CALIBRATION_INPUT_WIGGLE)
			{
				retVal = true;
				break;
			}

			// UPDATE VARIABLES FOR NEXT LOOP
			chActive_p[ch] = ptrRadioData->chActive[ch];
		}

		// DETECT METHOD 2: CALIBRATE PADS SHORTED ON BACK OF DEVICE
		if (!GPIO_Read(CALIBRATE_GPIO, CALIBRATE_PIN))
		{
			retVal = true;
		}
	}

	return retVal;
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_UpdateCalibration (void)
{
	// DISABLE ALL OUTPUTS
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	SERVO_Deinit();

	// TURN ON LEDS TO TELL OPERATOR CALIBRATION IS STARTING
	LED_nPulse(10);
	LED_GreenON();
	LED_RedON();

	// CREATE LOCAL VARIABLE TO ASSIST IN CALIBRATION
	SYSTEM_Config c;
	c.radio = config.radio;

	// WAIT FOR USER TO PRESS A STICK TO INITATE CALIBRATION SEQUENCE
	SYSTEM_WaitForActiveInput();
	SYSTEM_WaitForResetInputs();
//	CORE_Delay(CALIBRATION_TEST_DELAY);

	// START OF TEST

	LED_GreenOFF();
	LED_RedOFF();
	CORE_Delay(CALIBRATION_TEST_DELAY);
	MOTOR_Update(MOTOR_MAX, MOTOR_MAX);
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	LED_RedON();
	SYSTEM_CalibrateMotorSameDirection(&c);
	LED_GreenON();
	SYSTEM_WaitForResetInputs();
	LED_nPulse(3);

	LED_RedOFF();
	LED_GreenOFF();
	CORE_Delay(CALIBRATION_TEST_DELAY);
	MOTOR_Update(-MOTOR_MAX, MOTOR_MAX);
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	LED_RedON();
	SYSTEM_CalibrateMotorOppositeDirection(&c);
	LED_GreenON();
	SYSTEM_WaitForResetInputs();
	LED_nPulse(3);

	LED_RedOFF();
	LED_GreenOFF();
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	LED_RedON();
	SYSTEM_CalibrateServoA(&c);
	LED_GreenON();
	SYSTEM_WaitForResetInputsServo(&c);
	LED_nPulse(3);
	CORE_Delay(CALIBRATION_TEST_DELAY);

	LED_RedOFF();
	LED_GreenOFF();
	CORE_Delay(CALIBRATE_MOTORJERK_PERIOD);
	LED_RedON();
	SYSTEM_CalibrateServoB(&c);
	LED_GreenON();
	SYSTEM_WaitForResetInputsServo(&c);

	// WRITE NEW CONFIG TO EEPROM
	config.chDriveA 		= c.chDriveA;
	config.chDriveArev 		= c.chDriveArev;
	config.chDriveB 		= c.chDriveB;
	config.chDriveBrev 		= c.chDriveBrev;
	config.chServoA 		= c.chServoA;
	config.chServoArev 		= c.chServoArev;
	config.chServoB 		= c.chServoB;
	config.chServoBrev 		= c.chServoBrev;
	config.mode 			= c.mode;
	config.radio.Protocol 	= c.radio.Protocol;
	config.radio.Baud_SBUS	= c.radio.Baud_SBUS;
	config.hashA 			= CONFIG_HASH_A;
	config.hashB 			= CONFIG_HASH_B;
	EEPROM_Write(EEPROM_OFFSET, &config, sizeof(config));

	// PULSE LED TO LET USER KNOW SUCCESSFUL
	LED_nPulse (10);
	LED_RedOFF();
	LED_GreenOFF();

	// Reinitialize Outputs
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);
	SERVO_Init();
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_CalibrateMotorSameDirection (SYSTEM_Config* c)
{
	// LOOP UNTIL BREAK CONDITION REACHED
	while (1)
	{
		// INIT LOOP VARIABLES
		uint32_t tick = CORE_GetTick();

		// UPDATE RADIO DATA AND COUNT ACTIVE INPUTS
		RADIO_Update();
		uint32_t chActive = SYSTEM_CountActiveInputs();

		// START BREAK SEQUENCE FOR ARCADE INPUT DRIVE STYLE
		if (chActive == 1)
		{
			c->mode = ARCADE;
			break;
		}
		// START BREAK SEQUENCE FOR TANK INPUT DRIVE STYLE
		else if (chActive == 2)
		{
			c->mode = TANK;
			break;
		}

		// LOOP PACING
		while (CALIBRATE_DRIVEINPUT_PERIOD >= (CORE_GetTick() - tick))
		{
			RADIO_Update();
			CORE_Idle();
		}
	}

	// ITTERATE THROUGH EACH CHANNEL TO FIND THE PUSHED STICK
	for (uint8_t ch = 0; ch < ptrRadioData->ch_num; ch++)
	{
		// IF STICK IS ACTIVE IN POSITIVE DIRECTION
		if (ptrRadioData->chActive[ch] == chActive_True)
		{
			c->chDriveA = ch;
			c->chDriveArev = false;
			break;
		}
		// IF STICK IS ACTIVE IN NEGATIVE DIRECTION
		else if (ptrRadioData->chActive[ch] == chActive_TrueRev)
		{
			c->chDriveA = ch;
			c->chDriveArev = true;
			break;
		}
	}

	// IF TANK MODE WAS SELECTED
	if (c->mode == TANK)
	{
		// CONTINUE ITTERATING THROUGH EACH CHANNEL TO FIND THE SECOND PUSHED STICK
		for (uint8_t ch = c->chDriveA; ch < ptrRadioData->ch_num; ch++)
		{
			// IF STICK IS ACTIVE IN POSITIVE DIRECTION
			if (ptrRadioData->chActive[ch] == chActive_True)
			{
				c->chDriveB = ch;
				c->chDriveBrev = false;
				break;
			}
			// IF STICK IS ACTIVE IN NEGATIVE DIRECTION
			else if (ptrRadioData->chActive[ch] == chActive_TrueRev)
			{
				c->chDriveB = ch;
				c->chDriveBrev = true;
				break;
			}
		}
	}
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_CalibrateMotorOppositeDirection (SYSTEM_Config* c)
{
	// LOOP UNTIL BREAK CONDITION REACHED
	while (1)
	{
		// INIT LOOP VARIABLES
		uint32_t tick = CORE_GetTick();

		// UPDATE RADIO DATA AND COUNT ACTIVE INPUTS
		RADIO_Update();
		uint32_t chActive = SYSTEM_CountActiveInputs();

		// DO SOME PRECHECKS BEFORE BREAK SEQUENCE
		bool chRevDriveA = false;
		if (ptrRadioData->chActive[c->chDriveA] == chActive_TrueRev)
		{
			chRevDriveA = true;
		}
		bool chRevDriveB = false;
		if (ptrRadioData->chActive[c->chDriveA] == chActive_TrueRev)
		{
			chRevDriveA = true;
		}

		// START BREAK SEQUENCE FOR ARCADE INPUT DRIVE STYLE
		if (c->mode == ARCADE && 											// Arcade mode chosen from previous detection
			chActive == 1 && 												// Only one channel is active
			ptrRadioData->chActive[c->chDriveA] == chActive_False )			// DriveA channel is not active channel
		{
			break;
		}
		// START BREAK SEQUENCE FOR TANK INPUT DRIVE STYLE
		else if (c->mode == TANK && 													// Tank mode chosen from previous detection
				 chActive == 2 &&														// Only two active channels
				 ptrRadioData->chActive[c->chDriveA] != chActive_False &&				// DriveA channel was one of the two active channels
				 ptrRadioData->chActive[c->chDriveB] != chActive_False &&				// DriveB channel was one of the two active channels
				 ((chRevDriveA == c->chDriveArev) != (chRevDriveB == c->chDriveBrev)) ) // Check only one of the changed directions
		{
			// IF DRIVEB HAS BEEN REVERSED THEN CHANNELS A AND B NEED TO BE SWAPPED
			if (chRevDriveB != c->chDriveBrev) {
				SYSTEM_Config cInt;
				cInt.chDriveA 		= c->chDriveA;
				cInt.chDriveArev 	= c->chDriveArev;
				c->chDriveA 		= c->chDriveB;
				c->chDriveArev 		= c->chDriveBrev;
				c->chDriveB 		= cInt.chDriveA;
				c->chDriveBrev 		= cInt.chDriveArev;
			}
			break;
		}

		// LOOP PACING
		while (CALIBRATE_DRIVEINPUT_PERIOD >= (CORE_GetTick() - tick))
		{
			RADIO_Update();
			CORE_Idle();
		}
	}

	// EXTRACT REMAINING ARCADE DRIVE DATA
	if (c->mode == ARCADE)
	{
		// ITTERATE THROUGH EVERY INPUT
		for (uint8_t ch = 0; ch < ptrRadioData->ch_num; ch++)
		{
			// IF STICK IS ACTIVE IN POSITIVE DIRECTION
			if (ptrRadioData->chActive[ch] == chActive_True)
			{
				c->chDriveB = ch;
				c->chDriveBrev = true;
				break;
			}
			// IF STICK IS ACTIVE IN NEGATIVE DIRECTION
			else if (ptrRadioData->chActive[ch] == chActive_TrueRev)
			{
				c->chDriveB = ch;
				c->chDriveBrev = false;
				break;
			}
		}
	}
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_CalibrateServoA (SYSTEM_Config* c)
{
	// LOOP UNTIL BREAK CONDITION REACHED
	while (1)
	{
		// UPDATE RADIO INPUTS
		RADIO_Update();
		// START BREAK SEQUENCE WHEN ANY STICK IS PUSHED (EXCL DRIVE INPUTS)
		if ( SYSTEM_CountActiveInputsServo(c) == 1)
		{
			break;
		}
		// LOOP PACING
		CORE_Idle();
	}

	// ITTERATE THROUGH EACH CHANNEL TO FIND PUSHED STICK
	for (uint8_t ch = 0; ch < ptrRadioData->ch_num; ch++)
	{
		// IGNROE DRIVE INPUTS
		if (ch != c->chDriveA && ch != c->chDriveB)
		{
			// IF STICK IS ACTIVE IN POSITIVE DIRECTION
			if (ptrRadioData->chActive[ch] == chActive_True)
			{
				c->chServoA = ch;
				c->chServoArev = false;
			}
			// IF STICK IS ACTIVE IN NEGATIVE DIRECTION
			else if (ptrRadioData->chActive[ch] == chActive_TrueRev)
			{
				c->chServoA = ch;
				c->chServoArev = true;
			}
		}
	}
}


/*
 * TEXT
 *
 * INPUTS:
 * OUTPUTS:
 */
void SYSTEM_CalibrateServoB (SYSTEM_Config* c)
{
	// LOOP UNTIL BREAK CONDITION REACHED
	while (1)
	{
		// UPDATE RADIO INPUTS
		RADIO_Update();
		// START BREAK SEQUENCE WHEN ANY STICK IS PUSHED (EXCL DRIVE INPUTS)
		if ( SYSTEM_CountActiveInputsServo(c) == 1)
		{
			break;
		}
		// LOOP PACING
		CORE_Idle();
	}

	// ITTERATE THROUGH EACH CHANNEL TO FIND PUSHED STICK
	for (uint8_t ch = 0; ch < ptrRadioData->ch_num; ch++)
	{
		// IGNROE DRIVE INPUTS
		if (ch != c->chDriveA && ch != c->chDriveB)
		{
			// IF STICK IS ACTIVE IN POSITIVE DIRECTION
			if (ptrRadioData->chActive[ch] == chActive_True)
			{
				c->chServoB = ch;
				c->chServoBrev = false;
			}
			// IF STICK IS ACTIVE IN NEGATIVE DIRECTION
			else if (ptrRadioData->chActive[ch] == chActive_TrueRev)
			{
				c->chServoB = ch;
				c->chServoBrev = true;
			}
		}
	}
}


/*
 * WAITS UNTIL ANY OF THE RADIO INPUTS ARE ACTIVE
 *
 * INPUTS: 	N/A
 * OUTPUTS: N/A
 */
void SYSTEM_WaitForActiveInput (void)
{
	// LOOP UNTIL BREAK CONDITION REACHED
	while (1)
	{
		// UPDATE RADIO DATA
		RADIO_Update();

		// BREAK IF ANY STICKS ARE PUSHED
		if (SYSTEM_CountActiveInputs() > 0) {
			break;
		}

		// LOOP PACING
		CORE_Idle();
	}
}


/*
 * WAITS UNTIL ALL RADIO INPUTS ARE BACK TO NEUTRAL POSITION
 *
 * INPUTS: 	N/A
 * OUTPUTS: N/A
 */
void SYSTEM_WaitForResetInputs (void)
{
	// LOOP UNTIL BREAK CONDITION REACHED
	while (1)
	{
		// UPDATE RADIO DATA
		RADIO_Update();

		// BREAK WHEN ALL STICKS HAVE RETURNED BACK TO ZERO POSITION
		if ( SYSTEM_CountActiveInputs() == 0) {
			break;
		}

		// LOOP PACING
		CORE_Idle();
	}
}

/*
 * WAITS UNTIL ALL RADIO INPUTS ARE BACK TO NEUTRAL POSITION (EXCL DRIVE INPUTS)
 *
 * INPUTS: 	POINTER TO SYSTEM_Config STRUCT CONTAINING DRIVE INPUTS TO IGNORE
 * OUTPUTS: N/A
 */
void SYSTEM_WaitForResetInputsServo (SYSTEM_Config* c)
{
	// LOOP UNTIL BREAK CONDITION REACHED
	while (1)
	{
		// UPDATE RADIO DATA
		RADIO_Update();

		// BREAK WHEN ALL STICKS HAVE RETURNED BACK TO ZERO POSITION (EXCL DRIVE INPUTS)
		if ( SYSTEM_CountActiveInputsServo(c) == 0) {
			break;
		}

		// LOOP PACING
		CORE_Idle();
	}
}

/*
 * COUNTS AND RETURNS NUMBER OF THE RADIO INPUTS ARE ACTIVE
 *
 * INPUTS: 	N/A
 * OUTPUTS: NUMBER OF ACTIVE RADIO CHANNELS
 */
uint32_t SYSTEM_CountActiveInputs (void)
{
	// INIT FUNCTION VARIABLES
	uint32_t retVal = 0;

	// ITTERATE THROUGH EACH RADIO INPUT
	for (uint8_t ch = 0; ch < ptrRadioData->ch_num; ch++)
	{
		if ( (ptrRadioData->chActive[ch] == chActive_True) ||
			 (ptrRadioData->chActive[ch] == chActive_TrueRev) )
		{
			retVal += 1;
		}
	}

	// RETURN NUMBER OF ACTIVE INPUTS
	return retVal;
}

/*
 * COUNTS AND RETURNS NUMBER OF THE RADIO INPUTS ARE ACTIVE (EXCL DRIVE INPUTS)
 *
 * INPUTS: 	POINTER TO SYSTEM_Config STRUCT CONTAINING DRIVE INPUTS TO IGNORE
 * OUTPUTS: NUMBER OF ACTIVE RADIO CHANNELS (EXCL DRIVE INPUTS)
 */
uint32_t SYSTEM_CountActiveInputsServo (SYSTEM_Config* c)
{
	// INIT FUNCTION VARIABLES
	uint32_t retVal = 0;

	// ITTERATE THROUGH EACH RADIO INPUT
	for (uint8_t ch = 0; ch < ptrRadioData->ch_num; ch++)
	{
		if ( ( (ptrRadioData->chActive[ch] == chActive_True) ||
			   (ptrRadioData->chActive[ch] == chActive_TrueRev) ) &&
			 (ch != c->chDriveA && ch != c->chDriveB) )
		{
			retVal += 1;
		}
	}

	// RETURN NUMBER OF ACTIVE INPUTS
	return retVal;
}


/*
 * INTERRUPT ROUTINES
 */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
