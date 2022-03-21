/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef SYSTEM_H
#define SYSTEM_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

/*
 * PUBLIC TYPES
 */

//	RUN = 				0x04, // Normal operation												Red: ON,	Green: OFF
//	WARNING_BATT = 		0x05, // Battery low													Red: ON,	Green: ON
//	WARNING_TEMP =  	0x06, // Temperature high (overrides battery warning)					Red: ON,	Green: Flashing Fast
//	FAULT_INPUT =		0x07, // No input (radio) detected										Red: OFF,	Green: Flashing USlow
//	FAULT_BATT = 		0x08, // Battery critically low (Overrides all warnings)				Red: OFF, 	Green: Flashing Slow
//	FAULT_TEMP = 		0x09, // Temperature critically high (Overrides battery fault)			Red: OFF, 	Green: Flashing Fast

// Flash USlow: (2s)
// Flash Slow:  (0.5s)
// Flash Fast:	(0.1s)

typedef struct {
	bool faultInput;
	bool faultBatt;
	bool warnBatt;
	bool faultTemp;
	bool warnTemp;
} Status_t;

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init (void);
void SYSTEM_Update (void);

/*
 * EXTERN DECLARATIONS
 */

extern Status_t status;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* SYSTEM_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
