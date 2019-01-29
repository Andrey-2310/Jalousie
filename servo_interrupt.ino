#include <Servo.h>

#define LEFT_ROTATE_STATE 0
#define HOLD_STATE 90
#define RIGHT_ROTATE_STATE 180

Servo myservo;

volatile int servoState = HOLD_STATE;

volatile boolean isButton1Pressed = false;
volatile boolean isButton2Pressed = false;

int zeroInterruptPin = 2;
int firstInterruptPin = 3;

int servoPin = 9;

void setup() {
  pinMode(zeroInterruptPin, INPUT_PULLUP);
  pinMode(firstInterruptPin, INPUT_PULLUP);

  pinMode(servoPin, OUTPUT);

  myservo.attach(servoPin);

  attachInterrupt(digitalPinToInterrupt(zeroInterruptPin), zeroInterruptCallback, FALLING);
  attachInterrupt(digitalPinToInterrupt(firstInterruptPin), firstInterruptCallback, FALLING);
}

void loop() {
  myservo.write(servoState);
  delay(500);
}

void zeroInterruptCallback() {
  isButton1Pressed = !isButton1Pressed;
  detachInterrupt(digitalPinToInterrupt(zeroInterruptPin));
  attachInterrupt(digitalPinToInterrupt(zeroInterruptPin), zeroInterruptCallback, isButton1Pressed ? RISING : FALLING);
  servoState = getDifferentState(LEFT_ROTATE_STATE);
}

void firstInterruptCallback() {
  isButton2Pressed = !isButton2Pressed;
  detachInterrupt(digitalPinToInterrupt(firstInterruptPin));
  attachInterrupt(digitalPinToInterrupt(firstInterruptPin), firstInterruptCallback, isButton2Pressed ? RISING : FALLING);
  servoState = getDifferentState(RIGHT_ROTATE_STATE);
}

int getDifferentState(int state) {
  return servoState == state ? HOLD_STATE : state;
}








