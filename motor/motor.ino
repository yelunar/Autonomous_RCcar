int forward = 5;
int backward = 4;
int enable = 6;

int hallA = 3;
int hallB = 2;

int dir = 8;
int dirState = 0;

volatile int count;

bool allowPrint = false;
long int currentTime = 0;
bool isForward = true;

void setup() {

  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);
  pinMode(enable, OUTPUT);

  pinMode(hallA, INPUT_PULLUP);
  pinMode(hallB, INPUT_PULLUP);

  pinMode(dir, INPUT_PULLUP);

  Serial.begin(9600);

}

void loop() {
  int dirStatus = digitalRead(dir);
  
  if (dirStatus == HIGH) {
    digitalWrite(forward, LOW);
    digitalWrite(backward, HIGH);
  } else {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, LOW);
  }

//  Serial.print("dirStatus: ");
//  Serial.println(dirStatus);
  
  analogWrite(enable, 100);
  //  accelerate();
  //  decelerate();
  attachInterrupt(digitalPinToInterrupt(hallB), readHallB, RISING);

  long int currentMillis = millis();
  if (currentMillis - currentTime >= 1000) {
    currentTime = millis();
    if (isForward) {
      Serial.print("Forward: ");
    } else {
      Serial.print("Backward: ");
    }
    Serial.println(count);
    count = 0;
  }

}

void accelerate() {

  for (int i = 0; i < 100; i++) {
    analogWrite(enable, i);
    delay(10);
  }

}

void decelerate() {

  for (int i = 100; i >= 0; i--) {
    analogWrite(enable, i);
    delay(10);
  }
}

void readHallB() {

  count++;

  if (digitalRead(hallA) == 1) {
    isForward = true;
  } else {
    isForward = false;
  }

}
