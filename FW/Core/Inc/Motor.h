/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef MOTOR_H
#define MOTOR_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define MOTOR_FREQ			(20*1000) 	// 20kHz
#define MOTOR_RES			255
#define MOTOR_STALL			50
#define MOTOR_OFF			0
#define MOTOR_MIN			1
#define MOTOR_MAX			256
#define MOTOR_BRAKE			1

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void MOTOR_Init (void);
void MOTOR_Deinit (void);
void MOTOR_Stop (void);
void MOTOR_Update (int32_t, int32_t);
void MOTOR_M1_Update (int32_t);
void MOTOR_M2_Update (int32_t);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* MOTOR_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
