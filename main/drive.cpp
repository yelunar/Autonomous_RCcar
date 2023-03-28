#include "drive.h"
#include "Arduino.h"

#define MOTOR_B_ENABLE 6
#define MOTOR_B_BWD 7
#define MOTOR_B_FWD 8
#define SERVO_A_BWD 9
#define SERVO_A_FWD 10
#define SERVO_A_ENABLE 11
#define SERVO_POT A0

int ref;
float P_CONSTANT = .55;
int it = 400;
int schreechingThreshold = 12;

int x_final = 0;
int y_final = 0;

volatile int16_t throttlePwm = 0;
volatile int16_t steerPwm = 0;

enum {FWD, BWD} dir;
//enum {LEFT, RIGHT} steer;

struct HBridge {
  int fwd;
  int bwd;
  int enable;
};

struct HBridge motor = {MOTOR_B_FWD, MOTOR_B_BWD, MOTOR_B_ENABLE};
struct HBridge serv = {SERVO_A_FWD, SERVO_A_BWD, SERVO_A_ENABLE};

int counter = 0;

//int format_string();
//void power(int pwm);
//void steerAtRef(int x);

//int y = 0;
//int x = 0;

void setupDrive() {

  digitalWrite(MOTOR_B_FWD, LOW);
  digitalWrite(MOTOR_B_BWD, LOW);
  digitalWrite(SERVO_A_FWD, LOW);
  digitalWrite(SERVO_A_BWD, LOW);

  pinMode(MOTOR_B_FWD, OUTPUT);
  pinMode(MOTOR_B_BWD, OUTPUT);
  pinMode(MOTOR_B_ENABLE, OUTPUT);
  pinMode(SERVO_A_FWD, OUTPUT);
  pinMode(SERVO_A_BWD, OUTPUT);
  pinMode(SERVO_A_ENABLE, OUTPUT);

  pinMode(serv.fwd, OUTPUT);
  pinMode(serv.bwd, OUTPUT);
  pinMode(serv.enable, OUTPUT);
  pinMode(SERVO_POT, INPUT);

  digitalWrite(MOTOR_B_FWD, 1);
  digitalWrite(MOTOR_B_BWD, 0);

  dir = FWD;
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void drive(int x, int y) {

  if (y >= 51) {
    y = y - 51;
    dir = BWD;
  }
  else {
    dir = FWD;
  }

  int pwm = map(y, 0, 50, 0, 255);
  power(pwm);
  steering(x);
}

void power(int pwm) {

  switch (dir) {
    case FWD :
      digitalWrite(motor.fwd, 1);
      digitalWrite(motor.bwd, 0);
      throttlePwm = pwm;
      break;
    case BWD :
      digitalWrite(motor.fwd, 0);
      digitalWrite(motor.bwd, 1);
      throttlePwm = -pwm;
      break;
  }
  analogWrite(motor.enable, pwm);
  }

int computeServoInput(int potInput, int ref) {
  int result = (int)(P_CONSTANT * (potInput - ref));
  if (result > 255) {
    return 255;
  }
  if (result < -255) {
    return -255;
  }
  if (abs(result) < schreechingThreshold) {
    return 0;
  }
  return result;
}

void steerAtRef (int ref) {
  int potInput = analogRead(SERVO_POT);
  int command = computeServoInput(potInput, ref);

  digitalWrite(serv.fwd, command < 0);
  digitalWrite(serv.bwd, command > 0);
  analogWrite(serv.enable, abs(command));
  steerPwm = command < 0? command: -command;
}

void steering(int x) {
  if (x < 51) {
    steerAtRef(map(x, 0, 50, 500, 620)); // left
  } else {
    steerAtRef(map(x, 51, 101, 500, 400)); // right
  }
}

int16_t getSteerPwm(){
  return steerPwm;
}

int16_t getThrottlePwm(){
  return throttlePwm;
}
