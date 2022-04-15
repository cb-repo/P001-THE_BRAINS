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

#define NUM_TOTALINPUTS	4

typedef enum {
	IP1,
	IP2,
	IP3,
	IP4,
} RADIO_InputDataIndex;

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void);
void RADIO_Update(void);

/*
 * EXTERN DECLARATIONS
 */
extern volatile uint16_t input[NUM_TOTALINPUTS];
extern volatile uint32_t inputHeartbeat;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* RADIO_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
