/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Config.h"
#include "System.h"
#include "EEPROM.h"
#include "Core.h"
#include "string.h"

/*
 * PRIVATE DEFINITIONS
 */

#define CONFIG_OFFSET 			0

#define CONFIG_SET 				100
#define CONFIG_RESET			(10*1000)
#define CONFIG_TIMEOUT			(10*1000)
#define TRIGGER_TIMEOUT			200
#define SETSUCCESS_TIMEOUT 		(2*1000)
#define RESETSUCCESS_TIMEOUT	(5*1000)


#define RADIO_DET_P				1700
#define RADIO_DET_N				1300

#define MOTOR_TWITCH			200

/*
 * PRIVATE TYPES
 */

typedef enum {
	OFF,
	NORMAL,
	REVERSE,
} InputDir_t;

Config_t config;

/*
 * PRIVATE PROTOTYPES
 */

void CONFIG_DetRadio(void);
void CONFIG_DetMode(void);
void CONFIG_Calibrate_IRQ(void);

/*
 * PRIVATE VARIABLES
 */

static uint8_t configSetFlag = 0;
static uint8_t configResetFlag = 0;
static uint32_t config_tick = 0;

/*
 * PUBLIC FUNCTIONS
 */


void CONFIG_Init(void)
{
	EEPROM_Read(CONFIG_OFFSET, &config, sizeof(config));
	GPIO_OnChange(CALIBRATE_GPIO, CALIBRATE_PIN, GPIO_IT_Both, CONFIG_Calibrate_IRQ);
}


bool CONFIG_Set(void)
{
	if (configSetFlag)
	{
		CONFIG_DetRadio();
		CONFIG_DetMode();
		EEPROM_Write(CONFIG_OFFSET, &config, sizeof(config));
		LED_TriPulse();
		CONFIG_SetSuccessDelay();
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CONFIG_CheckForReset(void)
{
	if (configFlag && (CONFIG_RESET <= (CORE_GetTick() - config_tick)))
	{
		LED_Pulse();
		config.radio = 			SPWM;
		config.drive = 			TANK;
		config.locoCh1 = 		IP3;
		config.locoCh2 = 		IP2;
		config.auxCh1 = 		IP1;
		config.auxCh2 = 		IP4;
		memset(config.reverseChMask, 0, sizeof(config.reverseChMask));
		memset(config.failsafeChMask, 0, sizeof(config.failsafeChMask));
		EEPROM_Write(CONFIG_OFFSET, &config, sizeof(config));
		LED_TriPulse();
		CONFIG_ResetSuccessDelay();
	}
}


/*
 * PRIVATE FUNCTIONS
 */

void CONFIG_DetRadio(void)
{
	config.radio = SPWM;
}

void CONFIG_DetMode(void)
{
	uint32_t tick = CORE_GetTick();
	uint32_t now = tick;
	uint8_t timeout = 0;
	uint8_t trigger = 0;
	uint32_t count = 0;
	InputDir_t flags[16] = {OFF};

	MOTOR_Update(MOTOR_MAX, MOTOR_MAX);
	CORE_Delay(MOTOR_TWITCH);
	MOTOR_Update(MOTOR_OFF, MOTOR_OFF);

	while (CONFIG_TIMEOUT >= now - tick)
	{
		if 		(radioS1 > RADIO_DET_P) 	{flags[IP1] = NORMAL;  trigger = 1;}
		else if (radioS1 < RADIO_DET_N) 	{flags[IP1] = REVERSE; trigger = 1;}
		if 		(radioS2 > RADIO_DET_P) 	{flags[IP2] = NORMAL;  trigger = 1;}
		else if (radioS2 < RADIO_DET_N) 	{flags[IP2] = REVERSE; trigger = 1;}
		if 		(radioS3 > RADIO_DET_P) 	{flags[IP3] = NORMAL;  trigger = 1;}
		else if (radioS3 < RADIO_DET_N) 	{flags[IP3] = REVERSE; trigger = 1;}
		if 		(radioS4 > RADIO_DET_P) 	{flags[IP4] = NORMAL;  trigger = 1;}
		else if (radioS4 < RADIO_DET_N) 	{flags[IP4] = REVERSE; trigger = 1;}
		if (trigger && !count) { count = CORE_GetTick(); }
		if (trigger && (TRIGGER_TIMEOUT <= now - count)) { break; }
		CORE_Idle();
		now = CORE_GetTick();
	}

	if (flags[IP1] && !(flags[IP2] || flags[IP3] || flags[IP4]))
	{
		config.drive = ARCADE;
		if 		(flags[IP1] == NORMAL)  { config.locoCh1 = IP1; config.reverseChMask[IP1] = 0}
		else if (flags[IP1] == REVERSE) {}
	}
	else if (flags[IP2] && !(flags[IP1] || flags[IP3] || flags[IP4])) 	{config.mode = ARCADE;}
	else if (flags[IP3] && !(flags[IP2] || flags[IP1] || flags[IP4])) 	{config.mode = ARCADE;}
	else if (flags[IP4] && !(flags[IP2] || flags[IP3] || flags[IP1])) 	{config.mode = ARCADE;}
	else if ((flags[IP1] && flags[IP2]) & !(flags[IP3] || flags[IP4])) 	{config.mode = TANK;}
	else if ((flags[IP1] && flags[IP3]) & !(flags[IP2] || flags[IP4])) 	{config.mode = TANK;}
	else if ((flags[IP1] && flags[IP4]) & !(flags[IP3] || flags[IP2])) 	{config.mode = TANK;}
	else if ((flags[IP2] && flags[IP3]) & !(flags[IP1] || flags[IP4])) 	{config.mode = TANK;}
	else if ((flags[IP2] && flags[IP4]) & !(flags[IP1] || flags[IP3])) 	{config.mode = TANK;}
	else if ((flags[IP3] && flags[IP4]) & !(flags[IP1] || flags[IP2])) 	{config.mode = TANK;}
	else 												{config.mode = TANK;}
//
//	if (config.mode = TANK)
//	{
//		// set channels to
//	}

}

void CONFIG_SetSuccessDelay(void)
{
	static uint32_t tick = CORE_GetTick();
	while (SETSUCCESS_TIMEOUT > (CORE_GetTick() - tick)) { CORE_Idle(); }
}


void CONFIG_ResetSuccessDelay(void)
{
	static uint32_t tick = CORE_GetTick();
	while (RESETSUCCESS_TIMEOUT > (CORE_GetTick() - tick)) { CORE_Idle(); }
}


/*
 * INTERRUPT ROUTINES
 */


void CONFIG_Calibrate_IRQ (void)
{
	if (GPIO_Read(CALIBRATE_GPIO, CALIBRATE_PIN))
	{
		configFlag = 1;
		config_tick = CORE_GetTick();
	}
	else if (status == BOOT && (CONFIG_SET <= CORE_GetTick() - config_tick))
	{
		configSetFlag = 1;
	}
	else
	{
		configFlag = 0;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
