#include <Servo.h>

#define LEFT_ROTATE_STATE 0
#define HOLD_STATE 90
#define RIGHT_ROTATE_STATE 180

Servo myservo;

volatile boolean isButton1Pressed = false;
volatile boolean isButton2Pressed = false;

int zeroInterruptPin = 2;
int firstInterruptPin = 3;

int servoPin = 9;

char data;

void setup() {
  pinMode(zeroInterruptPin, INPUT_PULLUP);
  pinMode(firstInterruptPin, INPUT_PULLUP);

  pinMode(servoPin, OUTPUT);

  myservo.attach(servoPin);
  myservo.write(HOLD_STATE);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(zeroInterruptPin), zeroInterruptCallback, FALLING);
  attachInterrupt(digitalPinToInterrupt(firstInterruptPin), firstInterruptCallback, FALLING);
}

void loop() {
  checkReceivedData();
}

void checkReceivedData() {
  if (Serial.available() > 0)
  {
    data = Serial.read();
    Serial.println(data);
    if (data == '1') {
      myservo.write(LEFT_ROTATE_STATE);
    } else if (data == '0')  {
      myservo.write(HOLD_STATE);
    } else if (data == '2')  {
      myservo.write(RIGHT_ROTATE_STATE);
    }
  }
}

void zeroInterruptCallback() {
  isButton1Pressed = !isButton1Pressed;
  detachInterrupt(digitalPinToInterrupt(zeroInterruptPin));
  attachInterrupt(digitalPinToInterrupt(zeroInterruptPin), zeroInterruptCallback, isButton1Pressed ? RISING : FALLING);
  writeDifferentState(LEFT_ROTATE_STATE, isButton1Pressed);
}

void firstInterruptCallback() {
  isButton2Pressed = !isButton2Pressed;
  detachInterrupt(digitalPinToInterrupt(firstInterruptPin));
  attachInterrupt(digitalPinToInterrupt(firstInterruptPin), firstInterruptCallback, isButton2Pressed ? RISING : FALLING);
  writeDifferentState(RIGHT_ROTATE_STATE, isButton2Pressed);
}

void writeDifferentState(int state, boolean isButtonPressed) {
  myservo.write(isButtonPressed ? state : HOLD_STATE);
}









