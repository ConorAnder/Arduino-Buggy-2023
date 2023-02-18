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
  
}

void loop() {
  if (!digitalRead(L_EYE) && !digitalRead(R_EYE)) {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(BACKWARD, LOW);
    digitalWrite(FORWARD, HIGH);
    digitalWrite(L_SPEED, HIGH);
    digitalWrite(R_SPEED, HIGH);
  }
  
  else if (digitalRead(L_EYE) && !digitalRead(R_EYE)) {
    analogWrite(L_SPEED, 200);
    analogWrite(R_SPEED, 100);
  }
  
  else if (digitalRead(R_EYE) && !digitalRead(L_EYE)) {
    analogWrite(L_SPEED, 100);
    analogWrite(R_SPEED, 200);
  }
  else {
    digitalWrite(L_SPEED, LOW); 
    digitalWrite(R_SPEED, LOW);
  }
  
}