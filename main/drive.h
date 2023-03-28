#include <stdint.h>

void setupDrive();

int computeServoInput(int potInput, int ref);

void steerAtRef (int ref);

void steering(int x);

void power(int pwm);

void drive(int steeringVariable, int speedVariable);

int16_t getSteerPwm();

int16_t getThrottlePwm();
