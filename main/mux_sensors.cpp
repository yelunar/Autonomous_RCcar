#define ENCA 2
#define ENCB 3
#define NB_TEETH    12.0
#define REDUCTION   4.0/9/34*17/39 //differential reduction, motor reduction and motor gear
#define WHEEL_RAD   55/2 //mm
#define ANG2VEL     2.0*PI/NB_TEETH*REDUCTION*WHEEL_RAD

#define BATTERY_PIN A1

#include "mux_sensors.h"
#include "Wire.h"
#include <VL53L0X.h>

uint8_t VL53[] = {2, 4, 5 };
volatile int16_t velocity = 0; // mm/s
int counterEnc = micros();

void tcaselect(int i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setupSensors() {
  Wire.begin();

  for (int index = 0; index < VL_NO; index++) {
    tcaselect(VL53[index]);
    delay(30);
    VL53L0X sensor;
    sensor.init();
    sensor.setTimeout(500);
    sensor.startContinuous();
  }

  pinMode(BATTERY_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), encoderTrigger, FALLING);
  Serial.println("sensors setup success");
}

//void loop() {
//  for (uint8_t t = 0; t < VL_NO; t++){
//    Serial.println(t);
//    Serial.println(getDistanceReading(t));
//  }
//  delay(100);
//}

int16_t getVelocity() {
  return velocity;
}

int16_t getBatteryLevel() {
  return map(analogRead(BATTERY_PIN), 0, 1023, 0, 100);
}

void encoderTrigger()
{
  velocity = (int16_t)1000000 * ANG2VEL / (micros() - counterEnc);
  if (digitalRead(ENCB) == LOW)
  {
    velocity = -velocity;
  }
  counterEnc = micros();
}

int16_t getMuxDistanceReading(int sensorIndex) {
  tcaselect(VL53[sensorIndex]);
  VL53L0X sensor;
  return sensor.readRangeContinuousMillimeters();
}
