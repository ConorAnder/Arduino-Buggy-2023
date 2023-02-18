const int FORWARD = 12;
const int BACKWARD = 21;
const int L_SPEED = 19;
const int R_SPEED = 6;
const int L_EYE = 3;
const int R_EYE = 2;
const int TRIG = 17;
const int ECHO = 18;

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
  attachInterrupt(digitalPinToInterrupt(L_EYE), LirSensor, FALLING);
  attachInterrupt(digitalPinToInterrupt(R_EYE), RirSensor, FALLING);
  Rstate = HIGH;
  Lstate = HIGH;

  pinMode (TRIG, OUTPUT);
  pinMode (ECHO, INPUT);
}

void loop() {
  int dist = distance();
  if (dist < 10) {
    Lstate = 0;
    Rstate = 0;
    Serial.print(dist);
    Serial.print(" ");
  }
  else if (digitalRead(L_EYE) && digitalRead(R_EYE)) {
    Lstate = 150;
    Rstate = 150;
  }
  
  digitalWrite(FORWARD, HIGH);
  digitalWrite(BACKWARD, LOW);
  analogWrite(L_SPEED, Lstate);
  analogWrite(R_SPEED, Rstate);
  delay(200);
}

int distance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  int duration = pulseIn(ECHO, HIGH);
  return duration/56;
}

void LirSensor() {
  Lstate = 150;
  Rstate = 90;
}

void RirSensor() {
  Lstate = 90;
  Rstate = 150;
}