const int FORWARD = 12;
const int BACKWARD = 21;
const int L_SPEED = 19;
const int R_SPEED = 6;
const int L_EYE = 3;
const int R_EYE = 2;

volatile int Rstate;
volatile int Lstate;

void setup() {
  Serial.begin(9600);

  pinMode(FORWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);
  
  pinMode(L_EYE, INPUT);
  pinMode(R_EYE, INPUT);
  attachInterrupt(digitalPinToInterrupt(L_EYE), LirSensor, LOW);
  attachInterrupt(digitalPinToInterrupt(R_EYE), RirSensor, LOW);
  Rstate = HIGH;
  Lstate = HIGH;
}

void loop() {
  if (digitalRead(L_EYE) && digitalRead(R_EYE)) {
    Lstate = 200;
    Rstate = 200;
  }
  
  digitalWrite(FORWARD, HIGH);
  digitalWrite(BACKWARD, LOW);
  analogWrite(L_SPEED, Lstate);
  analogWrite(R_SPEED, Rstate);
}

void LirSensor() {
  Lstate = 100;
  Rstate = 200;
}

void RirSensor() {
  Lstate = 200;
  Rstate = 100;
}