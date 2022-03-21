/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Config.h"
#include "System.h"
#include "EEPROM.h"
#include "Core.h"
#include "string.h"
#include "GPIO.h"
#include "LED.h"

/*
 * PRIVATE DEFINITIONS
 */

#define CONFIG_OFFSET 			0

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void CONFIG_DetRadio(void);
void CONFIG_DetMode(void);

/*
 * PRIVATE VARIABLES
 */

uint32_t config;

bool configSetFlag = false;

/*
 * PUBLIC FUNCTIONS
 */

void CONFIG_Init(void)
{
	EEPROM_Read(CONFIG_OFFSET, &config, sizeof(config));

	GPIO_EnableInput(CALIBRATE_GPIO, CALIBRATE_PIN, GPIO_Pull_Up);
	CORE_Delay(10);
	if (GPIO_Read(CALIBRATE_GPIO, CALIBRATE_PIN) == false)
	{
		configSetFlag = true;
	}
	GPIO_Deinit(CALIBRATE_GPIO, CALIBRATE_PIN);
}


void CONFIG_Set(void)
{

}


/*
 * PRIVATE FUNCTIONS
 */


/*
 * INTERRUPT ROUTINES
 */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
