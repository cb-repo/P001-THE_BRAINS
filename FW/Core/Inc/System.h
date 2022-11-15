/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef SYSTEM_H
#define SYSTEM_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"
#include "Core.h"
#include "GPIO.h"
#include "ADC.h"
#include "EEPROM.h"
#include "UART.h"

#include "Radio.h"
#include "Servo.h"
#include "Motor.h"
#include "LED.h"

/*
 * PUBLIC DEFINITIONS
 */

#define EEPROM_OFFSET				0
#define CONFIG_HASH_A				0x7ae12336
#define CONFIG_HASH_B				0x8771c50d

#define LED_FAULTFLASH	500

#define BATT_1S_LOW		3200
#define BATT_2S_LOW		6200
#define BATT_HYST		100

#define TEMP_HIGH		100
#define TEMP_HYST		10

/*
 * PUBLIC TYPES
 */

typedef enum {
	IP1,
	IP2,
	IP3,
	IP4,
} SYSTEM_InputNumber;

typedef enum {
	TANK,
	ARCADE,
} SYSTEM_DriveMode;

typedef struct {
	bool signalLost;
	bool inputVoltage;
	bool overTemperature;
	uint32_t faultVoltage;
} SYSTEM_FaultStatus;

typedef struct _SYSTEM_Config {
	uint32_t hashA;
	SYSTEM_DriveMode mode;
	uint8_t chDriveA;
	uint8_t chDriveB;
	uint8_t chServoA;
	uint8_t chServoB;
	int8_t chRevMask[RADIO_NUM_CHANNELS];
	RADIO_Properties radio;
	uint32_t hashB;
} SYSTEM_Config;

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init (void);
void SYSTEM_Update (void);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* SYSTEM_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
