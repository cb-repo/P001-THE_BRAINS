/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef LED_H
#define LED_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void LED_Init (void);

bool LED_GreenState (void);
bool LED_RedState (void);

void LED_GreenON (void);
void LED_RedON (void);
void LED_GreenOFF (void);
void LED_RedOFF (void);
void LED_GreenToggle (void);
void LED_RedToggle (void);

void LED_Pulse(void);
void LED_TriPulse(void);
void LED_nPulse (uint8_t n);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* LED_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
