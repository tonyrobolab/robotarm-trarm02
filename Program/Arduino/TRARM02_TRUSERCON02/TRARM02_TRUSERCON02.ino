//--------------------------------------------------------------------------------
//  File        TRARM02_TRUSERCON02.ino
//
//  Version     v0.1  2025.12.02  Tony Kwon
//                  Initial revision
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  Configuration
//--------------------------------------------------------------------------------
int adc_min[5] = {336, 367, 341, 346, 710};
int adc_max[5] = {670, 702, 692, 687, 800};
int tick_init[5] = {402, 362, 402, 394, 357};
int tick_min[5] = {294, 262, 294, 292, 340};
int tick_max[5] = {510, 463, 510, 497, 446};
int angle_init[5] = {0, 0, 0, 0, 0};
int angle_min[5] = {-45, -45, -45, -45, -45};
int angle_max[5] = {45, 45, 45, 45, 45};

//--------------------------------------------------------------------------------
//  Serial communication
//--------------------------------------------------------------------------------
#define SERIAL_BAUD 115200

void setupSerial() {
  Serial.begin(SERIAL_BAUD);
  while (! Serial);
}

//--------------------------------------------------------------------------------
//  Servo motor driver for PCA9685
//--------------------------------------------------------------------------------
#include <Adafruit_PWMServoDriver.h>

#define MOTOR_PWM_FREQ  50  // 50[Hz] frequency = 20[ms] period
#define MOTOR_CH_MAX    16
 
Adafruit_PWMServoDriver motorServoDriver = Adafruit_PWMServoDriver(0x40);

void setupMotor() {
  motorServoDriver.begin();
  motorServoDriver.setPWMFreq(MOTOR_PWM_FREQ);
  for(int i = 0; i < 5; i++) {
    motorServoDriver.setPWM(i, 0, tick_init[5]);
  }  
}

//--------------------------------------------------------------------------------
//  Setup
//--------------------------------------------------------------------------------
void setup() {
  setupSerial();
  setupMotor();
}

//--------------------------------------------------------------------------------
//  Loop
//--------------------------------------------------------------------------------
void loop() {
  //----------------------------------------
  //  Read ADC
  //----------------------------------------
  int adc_raw[5];
  int adc[5];  
  
  adc_raw[0] = analogRead(A0); 
  adc_raw[1] = analogRead(A1);
  adc_raw[2] = analogRead(A2);
  adc_raw[3] = analogRead(A3);
  adc_raw[4] = analogRead(A4);

  for(int i = 0; i < 5; i++) {
    adc[i] = max(adc_min[i], min(adc_raw[i], adc_max[i]));
  } 

  //----------------------------------------
  //  Rotate TRARM02 motors
  //----------------------------------------
  int tick[5];
  for(int i = 0; i < 5; i++) {
    tick[i] = map(adc[i], adc_min[i], adc_max[i], tick_min[i], tick_max[i]);
  }
  tick[0] = (tick_max[0] - (tick[0] - tick_min[0]));
  tick[1] = (tick_max[1] - (tick[1] - tick_min[1]));
  tick[2] = (tick_max[2] - (tick[2] - tick_min[2]));

  for(int i = 0; i < 5; i++) {
    motorServoDriver.setPWM(i, 0, tick[i]);
  }

  delay(10);  
}