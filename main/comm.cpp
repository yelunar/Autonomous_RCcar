
#define RED 13 // competitor 1
#define BLUE 12 // competitor 2
#define WHITE 5 // charging LED

#define CHARGING_PIN A2

#include "comm.h"
#include "Arduino.h"
#include "mux_sensors.h"
#include "Timer.h"
#include "drive.h"

Timer t;

String inputString = "";
bool batteryLedStatus = 0;

typedef struct {
  int16_t distanceSensorData[VL_NO];
  int16_t batteryVoltage;
  int16_t velocity;
  int16_t motorPwm;
  int16_t servoPwm;
} SensorData;

typedef union {
  SensorData sensors;
  byte serializedSensor[sizeof(SensorData)];
} SerializedSensorData;

void setupComm() {
  inputString.reserve(200);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(CHARGING_PIN, INPUT);

  //  cli(); // disable the global interrupts system in order to setup the timers
  //  TCCR1A = 0; // SET TCCR1A and B to 0
  //  TCCR1B = 0;
  //  // Set the OCR1A with the desired value:
  //  OCR1A = 15624;
  //  // Active CTC mode:
  //  TCCR1B |= (1 << WGM12);
  //  // Set the prescaler to 1024:
  //  TCCR1B |= (1 << CS10);
  //  TCCR1B |= (1 << CS12);
  //  // Enable the Output Compare interrupt (by setting the mask bit)
  //  TIMSK1 |= (1 << OCIE1A);
  //  sendStatus();
  //  Timer1.initialize(3000000);
  //  Timer1.attachInterrupt(sendStatus);

  //  Serial.begin(19200);
}

void sendStatus() {
  // read sensors + battery
  SerializedSensorData sensorData;
  for (int i = 0; i < VL_NO; i++) {
    sensorData.sensors.distanceSensorData[i] = getMuxDistanceReading(i);
    delay(10);
  }

  sensorData.sensors.batteryVoltage = getBatteryLevel();
  sensorData.sensors.velocity = getVelocity();
  sensorData.sensors.motorPwm = getThrottlePwm();
  sensorData.sensors.servoPwm = getSteerPwm();

  // send to Serial
  Serial.write(sensorData.serializedSensor, sizeof(sensorData));
  //  Serial.println(sensorData.sensors.distanceSensorData[0]);
  //  Serial.println(sensorData.sensors.distanceSensorData[1]);
  //  Serial.println(sensorData.sensors.distanceSensorData[2]);
  //  Serial.println(sensorData.sensors.batteryVoltage);


  // blink charging LED
  if (analogRead(CHARGING_PIN) > 50) {
    batteryLedStatus = ~batteryLedStatus;
  }
  else {
    batteryLedStatus = 0;
  }
  digitalWrite(WHITE, batteryLedStatus);
}


int readSerialData() {
  int data = readData();

  if (data == 29000) {
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
  } else if (data == 29001) {
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH);
  } else if (data == 28000) {
    return 0;
  } else if (data == 28001) {
    return 100;
  }

  return data;
}

int readData() {
  char inChar;
  int no = -1;

  while (Serial.available()) {
    inChar = (char) Serial.read();

    if (inChar != '\n') {
      inputString += inChar;
    }
    else {
      no = format_string();

      inputString = "";
    }
  }

  return no;
}

int format_string() {
  char chars[20];
  int i = 0;
  int j = 0;
  while (inputString.charAt(i) < '0' || inputString.charAt(i) > '9') {
    i++;
  }

  for (int k = i; k < inputString.length(); k++) {
    chars[j++] = inputString.charAt(k);
  }
  chars[j] = '\0';

  return atoi(chars);
}
