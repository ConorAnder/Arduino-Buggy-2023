#include <NewPing.h>

const int FORWARD = 12;
const int BACKWARD = 21;
const int L_SPEED = 19;
const int R_SPEED = 6;
const int R_HALL = 16;
const int L_HALL = 15;
const int TRIG = 17;
const int ECHO = 18;

double LpulseCount, RpulseCount;
int Lpulse, Rpulse;
int LprevPulse, RprevPulse;
double motorTime, elapsedMTime;
double totalObjDist, prevDist;

NewPing sonar(TRIG, ECHO, 50);

void setup() {
  Serial.begin(9600);

  pinMode(FORWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);
  pinMode(R_HALL, INPUT_PULLUP);
  pinMode(L_HALL, INPUT_PULLUP);
  pinMode (TRIG, OUTPUT);
  pinMode (ECHO, INPUT);

  LpulseCount = 0;
  Lpulse = 0;
  LprevPulse = 0;
  RpulseCount = 0;
  Rpulse = 0;
  RprevPulse = 0;
  motorTime = millis();
  totalObjDist = 0;
  prevDist = 0;
}

void loop() {
  digitalWrite(FORWARD, HIGH);
  digitalWrite(L_SPEED, HIGH);
  digitalWrite(R_SPEED, HIGH);

  double rightMDist = RDist();
  double leftMDist = LDist();
  double dist = sonar.ping() / US_ROUNDTRIP_CM;

  if (dist > 9 && dist < 40) {
    totalObjDist += dist - prevDist;
    prevDist = dist;
  }
  else {
    totalObjDist = 0;
    prevDist = 0;
  }

  double buggySpeed, objectSpeed;
  if (LpulseCount > 100) {
    elapsedMTime = millis() - motorTime;
    buggySpeed = velocity(rightMDist, leftMDist);
    objectSpeed = objectVel(totalObjDist, buggySpeed); 
    totalObjDist = 0;
    motorTime = millis();
    RpulseCount = 0;
    LpulseCount = 0;
  }
}

double RDist() {
  if (digitalRead(R_HALL))
    Rpulse = 1;

  else
    Rpulse = 0;
  
  if (Rpulse % RprevPulse != 0)
    RpulseCount++;
  
  RprevPulse = Rpulse;
  return RpulseCount / (2*3.14159*3.4);
}

double LDist() {
  if (digitalRead(L_HALL))
    Lpulse = 1;

  else
    Lpulse = 0;
  
  if (Lpulse % LprevPulse != 0)
    LpulseCount++;
  
  LprevPulse = Lpulse;
  return LpulseCount / (2*3.14159*3.4);
}

double velocity(double dist1, double dist2) {
  double meanDistance = (dist1 + dist2) / 2.0;
  return meanDistance / (elapsedMTime/1000.0);
}

double objectVel(double distance, double speed) {
  double RelSpeed = distance / (elapsedMTime/1000);
  return RelSpeed + speed;
}