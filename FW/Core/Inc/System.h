/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef SYSTEM_H
#define SYSTEM_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define BOOT_TIMEOUT		(2*1000)
#define CONFIG_SET			100
#define CONFIG_RESET		(10*1000)

#define SYSTEM_BATT_WARN	3500
#define SYSTEM_BATT_FAULT	3300
#define SYSTEM_BATT_HYST	100

#define SYSTEM_TEMP_WARN	80
#define	SYSTEM_TEMP_FAULT	100
#define SYSTEM_TEMP_HYST	5

#define SYSTEM_INPUT_TIMEOUT 50


/*
 * PUBLIC TYPES
 */

// Flash Types
// OFF
// ON: LED fully illuminated
// Flash USlow: (0.2s on, 2s period)
// Flash Slow:  (0.5s on, 1s period)
// Flash Fast:	(0.1s on, 0.2 period)

//typedef enum {
//	BOOT = 				0x00, // Init system parameters											Red: OFF,	Green: ON
//	CONFIG_INPUT =		0x01, // Wait for user to input config									Red: OFF,	Green: Flash Fast
//	CONFIG_CONFIRM =	0x02, // Confirm successful set of config								Red: 1 Flash Slow,	Green: OFF
//	CONFIG_COMPLETE = 	0x03, // Confirm config set process iscomplete							Red & Green : 3x Flash Slow
//	RUN = 				0x04, // Normal operation												Red: ON,	Green: OFF
//	WARNING_BATT = 		0x05, // Battery low													Red: ON,	Green: Flashing Slow
//	WARNING_TEMP =  	0x06, // Temperature high (overrides battery warning)					Red: ON,	Green: Flash Fast
//	FAULT_INPUT =		0x07, // No input (radio) detected										Red: Flash USlow,	Green: ON
//	FAULT_BATT = 		0x08, // Battery critically low (Overrides all warnings)				Red: Flash Slow, 	Green: ON
//	FAULT_TEMP = 		0x09, // Temperature critically high (Overrides battery fault)			Red: Flash Fast, 	Green: ON
//} RunStates_t;

typedef enum {
	BOOT = 			0x00, // Init system parameters
	CONFIG =		0x01, // Config run parameter
	RUN = 			0x02, // Normal operation
	WARNING = 		0x03, // Warning Flag is set
	FAULT =			0x04 // Fault Flag is set
} Status_t;

typedef struct {
	uint8_t batt;
	uint8_t temp;
} WarningStates_t;

typedef struct {
	uint8_t input;
	uint8_t batt;
	uint8_t temp;
} FaultStates_t;

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init (void);
void SYSTEM_Update (void);
int32_t CONVERT_RadioToMotor (uint32_t);

/*
 * EXTERN DECLARATIONS
 */

extern Status_t status;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* _H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
