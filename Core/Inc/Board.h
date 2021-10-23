/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef BOARD_H
#define BOARD_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// CONFIG INTERRUPT
#define USE_GPIO_IRQS
#define USE_EXTI_1
#define USE_EXTI_2
#define USE_EXTI_6
#define USE_EXTI_7

// CONFIG TIMERS
#define USE_TIM_IRQS 		1
// Motor
#define USE_TIM2
#define TIM_MOTOR TIM_2
#define TIM_MOTOR_AF		GPIO_AF2_TIM2
#define TIM_MOTOR_RELOAD 	255
#define TIM_MOTOR_FREQ		(MOTOR_FREQ*(TIM_MOTOR_RELOAD+1))
// Radio
#define USE_TIM6
#define TIM_RADIO 			TIM_6
#define TIM_RADIO_RELOAD 	0xffff
#define TIM_RADIO_FREQ		1000000
// Servo
#define USE_TIM21
#define TIM_SERVO 			TIM_21
#define TIM_SERVO_RELOAD 	(10*1000)
#define TIM_SERVO_FREQ		1000000
// Wepon
#define USE_TIM22
#define TIM_WEPON 			TIM_22
#define TIM_WEPON_RELOAD 	(10*1000)
#define TIM_WEPON_FREQ		1000000

// CONFIG LED
#define LED_STATUS_GPIO		GPIOB
#define LED_STATUS_PIN		GPIO_PIN_8

// CONFIG RADIO
// Pin16 (PB2) - RadioS3
// Pin15 (PB1) - RadioS4
// Pin14 (PB0) - RadioS2
// Pin13 (PA7) - RadioS2

#define RADIO_S1_GPIO		GPIOA
#define RADIO_S1_PIN		GPIO_PIN_6
#define RADIO_S2_GPIO		GPIOA
#define RADIO_S2_PIN		GPIO_PIN_7
#define RADIO_S3_GPIO		GPIOB
#define RADIO_S3_PIN		GPIO_PIN_1
#define RADIO_S4_GPIO		GPIOB
#define RADIO_S4_PIN		GPIO_PIN_2

// CONFIG DC-MOTOR
#define MOTOR_LPWM1_GPIO	GPIOA
#define MOTOR_LPWM1_PIN		GPIO_PIN_0
#define MOTOR_LPWM1_CH		0
#define MOTOR_LPWM2_GPIO	GPIOA
#define MOTOR_LPWM2_PIN		GPIO_PIN_1
#define MOTOR_LPWM2_CH		1
#define MOTOR_RPWM1_GPIO	GPIOA
#define MOTOR_RPWM1_PIN		GPIO_PIN_2
#define MOTOR_RPWM1_CH		2
#define MOTOR_RPWM2_GPIO	GPIOA
#define MOTOR_RPWM2_PIN		GPIO_PIN_3
#define MOTOR_RPWM2_CH		3
#define MOTOR_ALL_GPIO		GPIOA
#define MOTOR_ALL_PIN		(MOTOR_LPWM1_PIN | MOTOR_LPWM2_PIN | MOTOR_RPWM1_PIN | MOTOR_RPWM2_PIN)

// CONFIG SERVO
#define SERVO_GPIO			GPIOA
#define SERVO_PIN			GPIO_PIN_4
#define WEPON_GPIO			GPIOA
#define WEPON_PIN			GPIO_PIN_5

#endif /* BOARD_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
