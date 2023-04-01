#define RED 1
#define GREEN 1
#define BLUE 1

int red = 9;
int green = 6;
int blue = 5;

void setup() {

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

}

void loop() {
//
//  for (int i = 0; i < 256; i++) {
//    setColor(255, 0, i);
//    delay(10);
//  }
//
//  for (int i = 255; i >= 0; i--) {
//    setColor(i, 0, 255);
//    delay(10);
//  }
//
//  //////////////////////////////////
//
//  for (int i = 0; i < 256; i++) {
//    setColor(0, i, 255);
//    delay(10);
//  }
//
//  for (int i = 255; i >= 0; i--) {
//    setColor(0, 255, i);
//    delay(10);
//  }
//
//  ///////////////////////////////////
//
//  for (int i = 0; i < 256; i++) {
//    setColor(i, 255, 0);
//    delay(10);
//  }
//
//  for (int i = 255; i >= 0; i--) {
//    setColor(255, i, 0);
//    delay(10);
//  }

digitalWrite(red, HIGH);

}

void setColor(int redVal, int greenVal, int blueVal) {

  analogWrite(red, redVal);
  analogWrite(green, greenVal);
  analogWrite(blue, blueVal);
  
}

void incrementColor(int redFlag, int greenFlag, int blueFlag) {

  for (int i = 0; i < 256; i++) {
    setColor(i * redFlag, i * greenFlag, i * blueFlag);

    delay(10);
  }
  
}

void decrementColor(int redFlag, int greenFlag, int blueFlag) {

  for (int i = 255; i >= 0; i--) {
    setColor(i * redFlag, i * greenFlag, i * blueFlag);

    delay(10);
  }
  
}
