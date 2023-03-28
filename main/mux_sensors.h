#define TCAADDR 0x70
#define VL_NO 3
#include <stdint.h>

void encoderTrigger();

void tcaselect(int i);

void setupSensors();

int16_t getBatteryLevel();

int16_t getMuxDistanceReading(int sensorIndex); 

int16_t getVelocity();
