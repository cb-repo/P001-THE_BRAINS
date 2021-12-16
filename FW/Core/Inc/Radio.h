/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef RADIO_H
#define RADIO_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define RADIO_PERIOD	20
#define RADIO_MIN		1000
#define RADIO_CENTER	1500
#define RADIO_MAX		2000
#define RADIO_FULLSCALE	(RADIO_MAX-RADIO_MIN)
#define RADIO_HALFSCALE	(RADIO_MAX-RADIO_CENTER)

#define SPWM_CH_MAX		4
#define PPM_CH_MAX		8
#define IBUS_CH_MAX		14
#define SBUS_CH_MAX		16

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void);
void RADIO_Update(void);

void SPWM_Init (void);
void SPWM_Deinit (void);
void PPM_Init (void);
void PPM_Deinit (void);
void IBUS_Init (void);
void IBUS_Deinit (void);
void SBUS_Init (void);
void SBUS_Deinit (void);

/*
 * EXTERN DECLARATIONS
 */

extern uint16_t input;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* RADIO_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
