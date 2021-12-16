/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef SERVO_H
#define SERVO_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define SERVO_PERIOD	20
#define SERVO_MIN		1000
#define SERVO_CENTER	1500
#define SERVO_MAX		2000
#define SERVO_FULLSCALE	SERVO_MAX-SERVO_MIN
#define SERVO_HALFSCALE	SERVO_MAX-SERVO_CENTER

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void SERVO_Init(void);
void SERVO_Deinit(void);
void SERVO_S1_Update(int16_t offset);
void SERVO_S2_Update(int16_t offset);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* SERVO_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
