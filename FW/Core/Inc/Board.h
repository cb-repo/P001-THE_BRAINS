/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef BOARD_H
#define BOARD_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#define STM32L0

// ADC CONFIG
#define ADC_VREF	      	3300


// TIM CONFIG
#define TIM_USE_IRQS
// 		Motor O/P
#define TIM2_ENABLE
#define TIM_MOTOR 			TIM_2
#define TIM_MOTOR_AF2		GPIO_AF2_TIM2
#define TIM_MOTOR_AF5		GPIO_AF5_TIM2
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
// 		USART1_Tx - Pin30 (PA9)
//		USART1_Rx - Pin31	(PA10)
#define UART1_GPIO			GPIOA
#define UART1_PINS			(GPIO_PIN_9 | GPIO_PIN_10)
#define UART1_AF		    GPIO_AF4_USART1
#define UART_BFR_SIZE     	128


// IRQ CONFIG
#define GPIO_USE_IRQS
#define USE_EXTI_3
#define USE_EXTI_8
#define USE_EXTI_9
#define USE_EXTI_10
#define USE_EXTI_15
// 		Calibrate I/P - Pin13 (PA3)
#define CALIBRATE_GPIO		GPIOA
#define	CALIBRATE_PIN		GPIO_PIN_3
// 		RadioS1 - Pin31 (PA10)
#define RADIO_S1_GPIO		GPIOA
#define RADIO_S1_PIN		GPIO_PIN_10
// 		RadioS2 - Pin30 (PA9)
#define RADIO_S2_GPIO		GPIOA
#define RADIO_S2_PIN		GPIO_PIN_9
// 		RadioS3 - Pin29 (PA8)
#define RADIO_S3_GPIO		GPIOA
#define RADIO_S3_PIN		GPIO_PIN_8
// 		RadioS4 - Pin28 (PB15)
#define RADIO_S4_GPIO		GPIOB
#define RADIO_S4_PIN		GPIO_PIN_15


// DC-MOTOR CONFIG
#define USE_DRV8212
#define MOTOR_LPWM1_CH		0
#define MOTOR_LPWM2_CH		1
#define MOTOR_RPWM1_CH		2
#define MOTOR_RPWM2_CH		3
// 		MotorL_PWM1 - Pin38 (PA15)
#define MOTOR_LPWM1_GPIO	GPIOA
#define MOTOR_LPWM1_PIN		GPIO_PIN_15
// 		MotorL_PWM2 - Pin39 (PB3)
#define MOTOR_LPWM2_GPIO	GPIOB
#define MOTOR_LPWM2_PIN		GPIO_PIN_3
// 		MotorL_MODE - Pin45 (PB8)
#define MOTOR_LMODE_GPIO	GPIOB
#define MOTOR_LMODE_PIN		GPIO_PIN_8
// 		MotorR_PWM1 - Pin21 (PB10)
#define MOTOR_RPWM1_GPIO	GPIOB
#define MOTOR_RPWM1_PIN		GPIO_PIN_10
// 		MotorR_PWM2 - Pin22 (PB11)
#define MOTOR_RPWM2_GPIO	GPIOB
#define MOTOR_RPWM2_PIN		GPIO_PIN_11
// 		MotorR_MODE - Pin10 (PB2)
#define MOTOR_RMODE_GPIO	GPIOB
#define MOTOR_RMODE_PIN		GPIO_PIN_2


// IO CONFIG
// 		Servo1 O/P - Pin11 (PA1)
#define SERVO1_GPIO			GPIOA
#define SERVO1_PIN			GPIO_PIN_1
// 		Servo2 O/P - Pin2 (PC13)
#define SERVO2_GPIO			GPIOC
#define SERVO2_PIN			GPIO_PIN_13
// 		Battery Detect - Pin14 (PA4)
#define BATTERY_GPIO		GPIOA
#define	BATTERY_PIN			GPIO_PIN_4
#define BATTERY_CHANNEL		ADC_CHANNEL_4
#define BATTERY_DET_RLOW	10*1000
#define BATTERY_DET_RHIGH	10*1000
// 		Status/Green LED - Pin3 (PC14)
#define LED_STATUS_GPIO		GPIOC
#define LED_STATUS_PIN		GPIO_PIN_14
// 		Fault/Red LED - Pin10 (PA0)
#define LED_FAULT_GPIO		GPIOA
#define LED_FAULT_PIN		GPIO_PIN_0


#endif /* BOARD_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
