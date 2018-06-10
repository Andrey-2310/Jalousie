int clockwisePin = 6;
int counterClockwisePin = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(clockwisePin, OUTPUT);
  pinMode(counterClockwisePin, OUTPUT);
}

void loop() {
  iteration(clockwisePin, counterClockwisePin);
  iteration(counterClockwisePin, clockwisePin);
}

void iteration(int kek, int lol) {
  digitalWrite(lol, HIGH);
  digitalWrite(kek, LOW);
  delay(3000);
}

