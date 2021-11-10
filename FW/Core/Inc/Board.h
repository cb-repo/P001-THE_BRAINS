/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef BOARD_H
#define BOARD_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// CONFIG INTERRUPT
#define USE_GPIO_IRQS
#define USE_EXTI_8
#define USE_EXTI_9
#define USE_EXTI_10
#define USE_EXTI_15

// CONFIG TIMERS
#define USE_TIM_IRQS
// Motor
#define USE_TIM2
#define TIM_MOTOR TIM_2
#define TIM_MOTOR_AF2		GPIO_AF2_TIM2
#define TIM_MOTOR_AF5		GPIO_AF5_TIM2
#define TIM_MOTOR_RELOAD 	255
#define TIM_MOTOR_FREQ		(MOTOR_FREQ*(TIM_MOTOR_RELOAD+1))
// Radio
#define USE_TIM6
#define TIM_RADIO 			TIM_6
#define TIM_RADIO_RELOAD 	0xffff
#define TIM_RADIO_FREQ		1000000
// Servo
#define USE_TIM21
#define TIM_SERVO1 			TIM_21
#define TIM_SERVO1_RELOAD 	(10*1000)
#define TIM_SERVO1_FREQ		1000000
// Wepon
#define USE_TIM22
#define TIM_SERVO2 			TIM_22
#define TIM_SERVO2_RELOAD 	(10*1000)
#define TIM_SERVO2_FREQ		1000000

// CONFIG LED
// Pin3 (PC14) - Status/Green
// Pin10 (PA0) - Fault/Red
#define LED_STATUS_GPIO		GPIOC
#define LED_STATUS_PIN		GPIO_PIN_14
#define LED_FAULT_GPIO		GPIOA
#define LED_FAULT_PIN		GPIO_PIN_0

// CONFIG RADIO
// Pin31 (PA10) - RadioS1
// Pin30 (PA9) - RadioS2
// Pin29 (PA8) - RadioS3
// Pin28 (PB15) - RadioS4

#define RADIO_S1_GPIO		GPIOA
#define RADIO_S1_PIN		GPIO_PIN_10
#define RADIO_S2_GPIO		GPIOA
#define RADIO_S2_PIN		GPIO_PIN_9
#define RADIO_S3_GPIO		GPIOA
#define RADIO_S3_PIN		GPIO_PIN_8
#define RADIO_S4_GPIO		GPIOB
#define RADIO_S4_PIN		GPIO_PIN_15

// CONFIG DC-MOTOR
// Pin38 (PA15) - MotorL_PWM1
// Pin39 (PB3) - MotorL_PWM2
// Pin45 (PB8) - MotorL_nSLEEP
// Pin21 (PB10) - MotorR_PWM1
// Pin22 (PB11) - MotorR_PWM2
// Pin10 (PB2) - MotorR_nSLEEP
#define USE_DRV8212
#define MOTOR_LPWM1_GPIO	GPIOA
#define MOTOR_LPWM1_PIN		GPIO_PIN_15
#define MOTOR_LPWM1_CH		0
#define MOTOR_LPWM2_GPIO	GPIOB
#define MOTOR_LPWM2_PIN		GPIO_PIN_3
#define MOTOR_LPWM2_CH		1
#define MOTOR_LnSLEEP_GPIO	GPIOB
#define MOTOR_LnSLEEP_PIN	GPIO_PIN_8

#define MOTOR_RPWM1_GPIO	GPIOB
#define MOTOR_RPWM1_PIN		GPIO_PIN_10
#define MOTOR_RPWM1_CH		2
#define MOTOR_RPWM2_GPIO	GPIOB
#define MOTOR_RPWM2_PIN		GPIO_PIN_11
#define MOTOR_RPWM2_CH		3
#define MOTOR_RnSLEEP_GPIO	GPIOB
#define MOTOR_RnSLEEP_PIN	GPIO_PIN_2

// CONFIG SERVO
// Pin11 (PA1) - Servo1
// Pin2 (PC13) - Servo2
#define SERVO1_GPIO			GPIOA
#define SERVO1_PIN			GPIO_PIN_1
#define SERVO2_GPIO			GPIOC
#define SERVO2_PIN			GPIO_PIN_13

// CONFIG CALIBRATE
// Pin13 (PA3) - Calibrate
#define CALIBRATE_GPIO		GPIOA
#define	CALIBRATE_PIN		GPIO_PIN_3

// CONFIG BATTERY DETECT
// Pin14 (PA4) - Detect
#define BATTERY_GPIO		GPIOA
#define	BATTERY_PIN			GPIO_PIN_4

#endif /* BOARD_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
