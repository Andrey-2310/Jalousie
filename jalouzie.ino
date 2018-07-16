#include <Servo.h>

Servo myservo;

int servoPin = 9;

int servoState = 90;

char data;

void setup() {
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  checkReceivedData();
  myservo.write(servoState);
}

void checkReceivedData() {
  if (Serial.available() > 0)
  {
    data = Serial.read();     
    Serial.print(data);          
    Serial.print("\n");
    if (data == '1') {
      servoLeftRotate();
    }
    else if (data == '2')  {
      servoRightRotate();
    }
     else if (data == '0')  {
      servoStop();
    }
  }
}

void servoLeftRotate() {
  servoState = 180;
}

void servoRightRotate() {
  servoState = 0;
}

void servoStop() {
  servoState = 90;
}


