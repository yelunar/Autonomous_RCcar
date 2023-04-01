#define MOTOR_B_ENABLE 6
#define MOTOR_B_BWD 7
#define MOTOR_B_FWD 8
#define SERVO_A_BWD 9
#define SERVO_A_FWD 10
#define SERVO_A_ENABLE 11
#define SERVO_POT 0

String inputString = "";
char drive_string[10] = {'\0'};
int drive_string_pos = 0;
boolean stringComplete = false;

boolean should_init = true;

// servo

int ref;
float P_CONSTANT = .48;
int it = 400;
int schreechingThreshold = 12;

int x_final = 0;
int y_final = 0;

int throttle = 0;
int steer = 0;

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

int format_string();
void drive(int y);
void power(int pwm);
void steerAtRef(int x);

void setup() {

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

  digitalWrite(motor.fwd, 1);
  digitalWrite(motor.bwd, 0);

  dir = FWD;

  // UART

  Serial.begin(19200);
  inputString.reserve(200);
}

void loop() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar != '\n') {
      inputString += inChar;
    }
    else {
      int no = format_string();
      int y = no & 0x00FF;
      int x = no >> 8;

      inputString = "";
    }
  }
  drive(x, y);
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

void drive(int x, int y) {

    if (y > 51) {
      y = y - 51;
      dir = FWD;
    }
    else {
      dir = BWD;
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
      break;
    case BWD :
      digitalWrite(motor.fwd, 0);
      digitalWrite(motor.bwd, 1);
      break;
  }
  analogWrite(motor.enable, pwm); 
}

void steering(int x) {
  if (x < 51) {
    steerAtRef(map(x, 0, 50, 480, 670));
  } else {
    steerAtRef(map(x, 51, 101, 480, 380));
  }
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

  digitalWrite(serv.fwd, command > 0);
  digitalWrite(serv.bwd, command < 0);
  analogWrite(serv.enable, abs(command));
}
