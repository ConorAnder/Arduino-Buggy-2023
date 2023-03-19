#include <SPI.h>
#include <WiFiNINA.h>
#include <NewPing.h>

// WiFi
const char SSID[] = "POCO X3 Pro";
const char PASSWORD[] = "buggy2023";
WiFiServer server(1300);
bool start = false;

// Arduino Pins
const int FORWARD = 12;
const int BACKWARD = 21;
const int L_SPEED = 19;
const int R_SPEED = 6;
const int L_EYE = 5;
const int R_EYE = 4;
const int TRIG = 17;
const int ECHO = 18;
const int R_HALL = 2;
const int L_HALL = 3;

// Interrupt Global Variables
double Rstate;
double Lstate;
volatile bool Lbool;
volatile bool Rbool;

// PID Global Variables
double kp = 10;
double ki = 0;
double kd = 0;
double totalTime;
double totalError;
double prevTime;
double prevError;
const int setpoint = 10;
double input;
double out;
double Max, Min;

// Velocity Global Variables
double LpulseCount, RpulseCount;
int Lpulse, Rpulse;
int LprevPulse, RprevPulse;
double motorTime, elapsedMTime;
double totalObjDist, prevDist;
NewPing sonar(TRIG, ECHO, 50);

void setup() {
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  delay(5000);
  IPAddress ip = WiFi.localIP();
  Serial.print(ip);
  server.begin();

  pinMode(FORWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);
  
  pinMode(L_EYE, INPUT);
  pinMode(R_EYE, INPUT);
  Rstate = HIGH;
  Lstate = HIGH;

  // Velocity Measurement
  pinMode (TRIG, OUTPUT);
  pinMode (ECHO, INPUT);
  pinMode(R_HALL, INPUT_PULLUP);
  pinMode(L_HALL, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(R_HALL), Rincrement, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_HALL), Lincrement, CHANGE);

  LpulseCount = 0;
  RpulseCount = 0;
  motorTime = millis();
  totalObjDist = 0;
  prevDist = 0;
}

void loop() {
  WiFiClient client = server.available();  
    
  if (client.connected()) {
    char ch = client.read();
    if (ch == 'w') {
      start = true;
      Serial.println("Go");
    }

    else if (ch == 's')
      start = false;
  }

  int dist = sonar.ping() / US_ROUNDTRIP_CM;
  if (dist == 0)
    dist = 45;
  double rightMDist = RDist();
  double leftMDist = LDist();
  if (dist < 10 && dist > 0 || !start) {
    Lstate = 0;
    Rstate = 0;
  }

  else if (digitalRead(L_EYE) && digitalRead(R_EYE)) {
    Lstate = 150;
    Rstate = 150;
    Lbool = false;
  	Rbool = false;    
  }

  if (!digitalRead(L_EYE))
    Lbool = true;
  
  if (!digitalRead(R_EYE))
    Rbool = true;

  if (Lbool && start && dist > 9) {
    Lstate = 150;
    Rstate = 90;
  }

  if (Rbool && start && dist > 9) {
    Lstate = 90;
    Rstate = 150;
  }

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

  client.print(dist);

  input = dist;
  if (input > 9 && input < 40) {
    out = PID(input);
  } else
    out = 0;
  Max = max(Max, out);
  Min = min(Min, out);

  if (out > 0 && start) {
    digitalWrite(BACKWARD, HIGH);
    digitalWrite(FORWARD, LOW);
    analogWrite(L_SPEED, (Lstate/150)*out);
    analogWrite(R_SPEED, (Rstate/150)*out);
  }
  else if (out < 0 && start) {
    digitalWrite(FORWARD, HIGH);
    digitalWrite(BACKWARD, LOW);
    analogWrite(L_SPEED, (Lstate/150)*(-1*out));
    analogWrite(R_SPEED, (Rstate/150)*(-1*out));
  }
  else {
    digitalWrite(FORWARD, HIGH);
    digitalWrite(BACKWARD, LOW);
    analogWrite(L_SPEED, (Lstate));
    analogWrite(R_SPEED, (Rstate));
  }
  if (Lbool || Rbool)
    delay(100);

    Serial.println(buggySpeed);
}

double PID(int distance) {
  double currTime = millis();
  double time = currTime - prevTime;

  double currError = setpoint - distance;
  totalError += (currError * time);
  double rateError = (currError - prevError) / time;

  double output = kp * currError + ki * totalError + kd * rateError;
  output = map(output, Min, Max, -255, 255);

  prevError = currError;
  prevTime = currTime;

  return output;
}

double RDist() {
  return RpulseCount / (2*3.14159*3.4);
}

double LDist() {
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

void Rincrement() {
  RpulseCount++;
}

void Lincrement() {
  LpulseCount++;
}