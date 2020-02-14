#include <Servo.h>
#include <iarduino_RTC.h>
#include "Ultrasonic.h"

#define LEFT_ROTATE_STATE 0
#define HOLD_STATE 90
#define RIGHT_ROTATE_STATE 180

#define OPEN_TIME "11:00:00"
#define CLOSE_TIME "22:00:00"

#define MAX_HEIGHT 150
#define MIN_HEIGHT 5

Servo myservo;

volatile boolean isButton1Pressed = false;
volatile boolean isButton2Pressed = false;

int zeroInterruptPin = 2;
int firstInterruptPin = 3;

int servoPin = 9;

String data;

iarduino_RTC time(RTC_DS3231);

Ultrasonic ultrasonic(6, 5);

void setup() {
  pinMode(zeroInterruptPin, INPUT_PULLUP);
  pinMode(firstInterruptPin, INPUT_PULLUP);

  pinMode(servoPin, OUTPUT);

  myservo.attach(servoPin);
  myservo.write(HOLD_STATE);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(zeroInterruptPin), zeroInterruptCallback, FALLING);
  attachInterrupt(digitalPinToInterrupt(firstInterruptPin), firstInterruptCallback, FALLING);

  //    time.begin();
  //    time.settime(45,9,19,17,8,19,6);
}

void loop() {
  checkReceivedData();
  //  rtcManagement();
}

int get_distance() {
  Serial.println(ultrasonic.read(CM));
  return ultrasonic.read(CM);
}

void checkReceivedData() {
  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.println(data);
    boolean isOkState = isCurrentStateNormal();
    if (data == "1" && isOkState) {
      myservo.write(LEFT_ROTATE_STATE);
    } else if (data == "2" && isOkState)  {
      myservo.write(RIGHT_ROTATE_STATE);
    } else if (data == "0")  {
      myservo.write(HOLD_STATE);
    }  else if (data[0] == 'h') {
      moveToState(map(data.substring(1, data.length()).toInt(), 0, 100, MIN_HEIGHT, MAX_HEIGHT));
    }
  }
}

void moveToState(int state) {
  if (isStateInNormalRange(state)) {
    int currentState = get_distance();
    if (currentState > state) {
      goDownToState(state);
    } else {
      goUpToState(state);
    }
  }
}

boolean isStateInNormalRange(int state) {
  return state >= MIN_HEIGHT && state <= MAX_HEIGHT;
}

boolean isCurrentStateNormal() {
  return isStateInNormalRange(get_distance());
}

void goDownToState(int state) {
  myservo.write(LEFT_ROTATE_STATE);
  while (get_distance() > state) {
    delay(100);
  }
  myservo.write(HOLD_STATE);
}

void goUpToState(int state) {
  myservo.write(RIGHT_ROTATE_STATE);
  while (get_distance() < state) {
    delay(100);
  }
  myservo.write(HOLD_STATE);
}

void rtcManagement() {
  String currentTime = time.gettime("H:i:s");
  Serial.println(currentTime);
  if (currentTime == OPEN_TIME) {
    moveToState(MAX_HEIGHT);
  } else if (currentTime == CLOSE_TIME) {
    moveToState(MIN_HEIGHT);
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
  if (isCurrentStateNormal()) {
    myservo.write(isButtonPressed ? state : HOLD_STATE);
  } else {
    myservo.write(HOLD_STATE);
  }
}
