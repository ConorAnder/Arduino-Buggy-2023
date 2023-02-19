float prevTime;
float setpoint = 30; //setpoint will be the distance we want to follow at
float cumulError;
float prevError;

void setup() {
  Serial.begin(9600);
  prevTime = 0;
  prevError = 0;
  cumulError = 0;
}

void loop() {
  int motorSpeed = controlPID(distance());
  delay(50);

}

int distance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  int duration = pulseIn(ECHO, HIGH);
  return duration/56;
}

float controlPID(float input) {
  const float kP = 2;
  const float kI = 5;
  const float kD = 1;

  float currTime = millis();
  float timeDiff = currTime - prevTime;
  float currError = setpoint - input;
  float prop = kP * currError;

  cumulError += (currError * timeDiff);
  float integ = kI * cumulError;

  float rateError = (currError - prevError) / timeDiff;
  float der = kD * rateError;

  prevError = currError;
  prevTime = currTime;

  return prop + integ + der;
}