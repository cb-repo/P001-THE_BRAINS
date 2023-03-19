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
#include "US.h"

#include "Radio.h"
#include "Servo.h"
#include "Motor.h"
#include "LED.h"

/*
 * PUBLIC DEFINITIONS
 */

#define SYSTEM_GETBIT(var, bit) (((var) >> (bit)) & 1)
#define SYSTEM_SETBIT(var, bit) (var |= (1 << (bit)))
#define SYSTEM_RSTBIT(var, bit)	(var &= (~(1 << (bit))))

#define EEPROM_OFFSET			0
#define CONFIG_HASH_A			0x8ce74359
#define CONFIG_HASH_B			0x8761d60e

#define LED_FAULT_SIGNALFLASH	500
#define LED_FAULT_VOLTAGEFLASH	500
#define LED_FAULT_TEMPFLASH		100


#define BATT_1S_LOW				3200
#define BATT_2S_LOW				6200
#define BATT_HYST				100

#define TEMP_HIGH				100
#define TEMP_HYST				10

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
	bool chDriveArev;
	bool chDriveBrev;
	bool chServoArev;
	bool chServoBrev;
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
