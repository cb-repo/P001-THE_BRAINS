/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef SYSTEM_H
#define SYSTEM_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define FAULT_LED_FLASH 		500

#define BATT_1S_LOW				3400
#define BATT_2S_LOW				6600
#define BATT_HYST				100

#define TEMP_HIGH				100
#define TEMP_HYST				50

#define INPUT_TIMEOUT 			50

#define CH_FWD						1
#define CH_RVS						(-1)

/*
 * PUBLIC TYPES
 */

typedef struct _SYSTEM_Status {
	bool faultInput;
	bool faultBatt;
	bool faultTemp;
} SYSTEM_Status;

typedef struct _SYSTEM_Config {
	uint32_t hashA;
	uint8_t mode;
	uint8_t primaryCh;
	int8_t primaryRev;
	uint8_t secondaryCh;
	int8_t secondaryRev;
	uint8_t servoChA;
	int8_t servoRevA;
	uint8_t servoChB;
	int8_t servoRevB;
	uint32_t hashB;
} SYSTEM_Config;

typedef enum {
	TANK = 1,
	ARCADE = 2,
} SYSTEM_DrivingModeIndex;

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init (void);
void SYSTEM_Update (void);
int32_t SYSTEM_RadioToMotor (uint16_t);
uint16_t SYSTEM_ReverseRadio(uint16_t);
uint16_t SYSTEM_RadioTruncate (uint16_t);

/*
 * EXTERN DECLARATIONS
 */

extern SYSTEM_Status status;
extern SYSTEM_Status status_p;
extern SYSTEM_Config config;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* SYSTEM_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
