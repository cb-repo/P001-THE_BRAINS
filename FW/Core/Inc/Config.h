/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef CONFIG_H
#define CONFIG_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define DEFAULT_RADIO		FS2A
#define DEFAULT_MODE		TANK

#define UNASSIGNED			0
#define ARCADE_DRIVE		1
#define ARCADE_STEER		2
#define TANK_MOTOR1			3
#define TANK_MOTOR2			4

/*
 * PUBLIC TYPES
 */


typedef enum {
	FAULT,
	SPWM,
	PPM,
	IBUS,
	SBUS,
} InputType_t;

typedef enum {
	IP1,  IP2,  IP3,  IP4,
	IP5,  IP6,  IP7,  IP8,
	IP9,  IP10, IP11, IP12,
	IP13, IP14, IP15, IP16,
} InputChannel_t;

typedef enum {
	TANK,
	ARCADE,
} DriveType_t;

typedef struct {
	InputType_t radio;
	DriveType_t drive;
	InputChannel_t locoCh1; //Locomotion Channel 1
	InputChannel_t locoCh2;	// Locomotion Channel 2
	InputChannel_t auxCh1;	// Auxiliary Channel 1
	InputChannel_t auxCh2;	// Auxiliary Channel 2
	uint16_t reverseChMask[16];
	uint16_t failsafeChMask[16];
} Config_t;

/*
 * PUBLIC FUNCTIONS
 */

void CONFIG_Init (void);
bool CONFIG_Set (void);
bool CONFIG_CheckForReset (void);

/*
 * EXTERN DECLARATIONS
 */

extern uint16_t input;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* CONFIG_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
