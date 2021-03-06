#ifndef GPIO_H
#define GPIO_H

#include "STM32X.h"
#include "Board.h"


/*
 * PUBLIC DEFINITIONS
 */

/*
 * PUBLIC TYPES
 */

typedef GPIO_TypeDef GPIO_t;


typedef enum {
	GPIO_IT_RISING 	= GPIO_MODE_IT_RISING,
	GPIO_IT_FALLING = GPIO_MODE_IT_FALLING,
	GPIO_IT_BOTH 	= GPIO_MODE_IT_RISING_FALLING,
} GPIO_IT_Dir_t;


/*
 * PUBLIC FUNCTIONS
 */

// Initialisation
void GPIO_EnableOutput(GPIO_t * gpio, uint32_t pin, GPIO_PinState state);
void GPIO_EnableInput(GPIO_t * gpio, uint32_t pin, uint32_t pullup);
void GPIO_Disable(GPIO_t * gpio, uint32_t pin);
#ifdef USE_GPIO_IRQS
void GPIO_EnableIRQ(GPIO_t * gpio, uint32_t pin, uint32_t pullup, GPIO_IT_Dir_t dir, VoidFunction_t callback);
#endif //USE_GPIO_IRQS

// Outputs
void GPIO_Write(GPIO_t * gpio, uint32_t pin, GPIO_PinState state);
static inline void GPIO_Set(GPIO_t * gpio, uint32_t pin);
static inline void GPIO_Reset(GPIO_t * gpio, uint32_t pin);

// Inputs
static inline bool GPIO_Read(GPIO_t * gpio, uint32_t pin);
static inline uint32_t GPIO_ReadPort(GPIO_t * gpio, uint32_t pins);


/*
 * EXTERN DECLARATIONS
 */

#include "GPIO.inl"

#endif //GPIO_H
