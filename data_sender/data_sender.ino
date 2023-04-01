#include "TimerOne.h"
#define RED 7
#define BLUE 6

String inputString = "";
int y = 0;
int x = 0;

unsigned char sensorData[] = {100, 200, 13, 98};

void setup() {
  Serial.begin(19200);
  inputString.reserve(200);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(sendStatus);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
}

void loop() {
//      char str[] = {'c', 'e', 'v', 'a', '\n'};
//      char *str = "ceva\n";
//      Serial.print(str);

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar != '\n') {
      inputString += inChar;
    }
    else {
      //Serial.println("received");
      //mySerial.println("received");
      int no = format_string();
      y = no & 0x00FF;
      x = no >> 8;

      if (no == 29000) {
        digitalWrite(RED, HIGH);
        digitalWrite(BLUE, LOW);
      } else if (no == 29001) {
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, HIGH);        
      }
      
      char returnChar = '\n';
      Serial.print(no); Serial.print(returnChar);

      inputString = "";
    }
  }
}

void sendStatus() {
    for (int i = 0; i < sizeof(sensorData); i++) Serial.print(sensorData[i]);
    Serial.println();
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
