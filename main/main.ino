#include "drive.h"
#include "comm.h"
#include "mux_sensors.h"

int steeringCommand;
int tractionCommand;

long currentTime;

void setup() {

  currentTime = millis();
  Serial.begin(19200);

  Serial.println("sensors");
  setupSensors();

  delay(1000);
  setupComm();

  Serial.println("drive");
  setupDrive();
}

void loop() {
  // 1. Read commands from RPi (Serial)
  // 2. Update steering and power variables (x, y)
  // 3. drive(x,y);
  // 4. Read sensors
  //  for (int t = 0; t<VL_NO; t++){
  //    getMuxDistanceReading(t); -> update output buffer byte[]
  //  }
  // * ranging sensors
  // * battery sensor
  // 5. Write buffer on Serial
  // 6. delay(50);

  /*
    for (int i = 5; i < 100; i += 10) {
    for (int j = 0; j < 50; j++) {
      steering(i);
    }
    }
  */

  long aux = millis();
  if (aux - currentTime > 1000) {
    sendStatus();
    currentTime = aux;
  }
  
  int data = readSerialData();
  if (
    data != 29000 && data != 29001 && // switch RED / BLUE
    data != 28000 && data != 28001 && // switch MANUAL / AUTONOM
    data != -1
    ) {
    tractionCommand = data & 0x00FF;
    steeringCommand = data >> 8;
  }
  drive(steeringCommand, tractionCommand);
  delay(50);
}
