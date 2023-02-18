const int FORWARD = 12;
const int BACKWARD = 21;
const int L_SPEED = 19;
const int R_SPEED = 6;
const int L_EYE = 3;
const int R_EYE = 2;

void setup() {
  Serial.begin(9600);

  pinMode(FORWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);
  
  pinMode(L_EYE, INPUT);
  pinMode(R_EYE, INPUT);
  attachInterrupt(digitalPinToInterrupt(L_EYE), LirSensor, RISING);
  attachInterrupt(digitalPinToInterrupt(R_EYE), RirSensor, RISING);
  
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(BACKWARD, LOW);
  digitalWrite(FORWARD, HIGH);
  digitalWrite(L_SPEED, HIGH);
  digitalWrite(R_SPEED, HIGH);
  
}

void LirSensor() {
  while(digitalRead(L_EYE && !digitalRead(R_EYE))) {
    analogWrite(R_SPEED, 100);
    analogWrite(L_SPEED, 200);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  digitalWrite(L_SPEED, HIGH);
  digitalWrite(R_SPEED, HIGH);
}

void RirSensor() {
  while(digitalRead(R_EYE) && !digitalRead(L_EYE)) {
    analogWrite(L_SPEED, 100);
    analogWrite(R_SPEED, 200);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  digitalWrite(L_SPEED, HIGH);
  digitalWrite(R_SPEED, HIGH);
}