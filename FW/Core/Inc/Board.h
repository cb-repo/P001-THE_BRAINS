/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef BOARD_H
#define BOARD_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#define STM32L0

// ADC CONFIG
#define ADC_VREF	      	3300

//#define DATA_EEPROM_BASE

// TIM CONFIG
#define TIM_USE_IRQS
// 		Motor O/P
#define TIM2_ENABLE
#define TIM_MOTOR 			TIM_2
#define TIM_MOTOR_AF2		GPIO_AF2_TIM2
#define TIM_MOTOR_RELOAD 	255
#define TIM_MOTOR_FREQ		(MOTOR_FREQ*(TIM_MOTOR_RELOAD+1))
// 		Radio I/P
#define TIM6_ENABLE
#define TIM_RADIO 			TIM_6
#define TIM_RADIO_RELOAD 	0xffff
#define TIM_RADIO_FREQ		1000000
// 		Servo1 O/P
#define TIM21_ENABLE
#define TIM_SERVO1 			TIM_21
#define TIM_SERVO1_RELOAD 	(10*1000)
#define TIM_SERVO1_FREQ		1000000
// 		Servo2 O/P
#define TIM22_ENABLE
#define TIM_SERVO2 			TIM_22
#define TIM_SERVO2_RELOAD 	(10*1000)
#define TIM_SERVO2_FREQ		1000000


// UART CONFIG
//		LPUART1_Tx - Pin21 (PB10) -- Not Using Tx Function
//		LPUART1_Rx - Pin22 (PB11)
#define RADIO_UART			UART_LP
#define RADIO_UART_GPIO		UARTLP_GPIO
#define RADIO_UART_PIN		UARTLP_PINS
#define UARTLP_GPIO			GPIOB
#define UARTLP_PINS			GPIO_PIN_11 // (GPIO_PIN_10 | GPIO_PIN_11)
#define UARTLP_AF		    GPIO_AF4_LPUART1
#define UART_BFR_SIZE     	128


// IRQ CONFIG
#define GPIO_USE_IRQS
#define GPIO_IRQ2_ENABLE
#define GPIO_IRQ5_ENABLE
#define GPIO_IRQ6_ENABLE
#define GPIO_IRQ10_ENABLE
#define GPIO_IRQ11_ENABLE
// 		Calibrate I/P - Pin21 (PB10)
#define CALIBRATE_GPIO		GPIOB
#define	CALIBRATE_PIN		GPIO_PIN_10
// 		RadioS1 - Pin22 (PB11)
#define PWM_S1_GPIO		GPIOB
#define PWM_S1_PIN		GPIO_PIN_11
// 		RadioS2 - Pin20 (PB2)
#define PWM_S2_GPIO		GPIOB
#define PWM_S2_PIN		GPIO_PIN_2
// 		RadioS3 - Pin16 (PA6)
#define PWM_S3_GPIO		GPIOA
#define PWM_S3_PIN		GPIO_PIN_6
// 		RadioS4 - Pin15 (PA5)
#define PWM_S4_GPIO		GPIOA
#define PWM_S4_PIN		GPIO_PIN_5


// DC-MOTOR CONFIG
#define USE_DRV8212
#define MOTOR_LPWM1_CH		0
#define MOTOR_LPWM2_CH		1
#define MOTOR_RPWM1_CH		2
#define MOTOR_RPWM2_CH		3
// 		MotorL_PWM1 - Pin10 (PA0)
#define MOTOR_LPWM1_GPIO	GPIOA
#define MOTOR_LPWM1_PIN		GPIO_PIN_0
// 		MotorL_PWM2 - Pin11 (PA1)
#define MOTOR_LPWM2_GPIO	GPIOA
#define MOTOR_LPWM2_PIN		GPIO_PIN_1
// 		MotorL_MODE - Pin4 (PC15)
#define MOTOR_LMODE_GPIO	GPIOC
#define MOTOR_LMODE_PIN		GPIO_PIN_15
// 		MotorR_PWM1 - Pin12 (PA2)
#define MOTOR_RPWM1_GPIO	GPIOA
#define MOTOR_RPWM1_PIN		GPIO_PIN_2
// 		MotorR_PWM2 - Pin13 (PA3)
#define MOTOR_RPWM2_GPIO	GPIOA
#define MOTOR_RPWM2_PIN		GPIO_PIN_3
// 		MotorR_MODE - Pin14 (PA4)
#define MOTOR_RMODE_GPIO	GPIOA
#define MOTOR_RMODE_PIN		GPIO_PIN_4


// IO CONFIG
// 		Servo1 O/P - Pin25 (PB12)
#define SERVO1_GPIO			GPIOB
#define SERVO1_PIN			GPIO_PIN_12
// 		Servo2 O/P - Pin13 (PA15)
#define SERVO2_GPIO			GPIOA
#define SERVO2_PIN			GPIO_PIN_15
// 		Battery Detect - Pin17 (PA7)
#define BATTERY_GPIO		GPIOA
#define	BATTERY_PIN			GPIO_PIN_7
#define BATTERY_CHANNEL		ADC_CHANNEL_7
#define BATTERY_DET_RLOW	18
#define BATTERY_DET_RHIGH	100
// 		Status/Green LED - Pin19 (PB1)
#define LED_STATUS_GPIO		GPIOB
#define LED_STATUS_PIN		GPIO_PIN_1
// 		Fault/Red LED - Pin18 (PB0)
#define LED_FAULT_GPIO		GPIOB
#define LED_FAULT_PIN		GPIO_PIN_0


#endif /* BOARD_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
