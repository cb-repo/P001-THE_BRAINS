#include <Servo.h> 

Servo radioS1;
Servo radioS2;
Servo radioS3;
Servo radioS4;
Servo servoS1;
Servo servoS2;

// RADIO PINS
#define RADIO1              1
#define RADIO2              2
#define RADIO3              3
#define RADIO4              4

// SERVO PINs
#define SERVO1              5
#define SERVO2              6
int32_t servo1 = 0;
int32_t servo2 = 0;

// SERVO POSITIONS
#define SERVO_LOW           0
#define SERVO_MID           90
#define SERVO_MAX           180

// STATE MACHINE
#define STATE_IDLE          0
#define STATE_POWERSUPPLY   1
#define STATE_RADIO         2
#define STATE_MOTOR         3
int32_t state = STATE_IDLE;


void setup() {
    //INIT RADIO
    radioS1.attach(RADIO1);
    radioS2.attach(RADIO2);
    radioS3.attach(RADIO3);
    radioS4.attach(RADIO4);
    radioS1.write(90);
    radioS2.write(90);
    radioS3.write(90);
    radioS4.write(90);
    servoS1.attach(SERVO1);
    servoS2.attach(SERVO2);
}

void loop() {
    readRadio();

    switch(state) {
      case STATE_IDLE:
        state = STATE_POWERSUPPLY;
        break;
      case STATE_POWERSUPPLY:
        testPS();
        state = STATE_RADIO;
        break;
      case STATE_RADIO:
        testRADIO();
        state = STATE_MOTOR;
        break;
      case STATE_MOTOR:
        testMOTOR();
        state = STATE_IDLE;
        break;
      default:
        state = STATE_IDLE;
        break;
    }
}

void testPS() {
    // SOMETHING
}

void testRADIO() {
    // SOMETHING
}

void testMOTOR() {
    // SOMETHING
}

void readRadio() {
    spareServo1 = servoS1.read();
    spareServo2 = servoS2.read();
}
