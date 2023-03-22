#include <WiFiNINA.h>

// WiFi
const char SSID[] = "POCO X3 Pro";
const char PASSWORD[] = "buggy2023";
WiFiServer server(1300);
bool start = false;
bool close = false;
char ch;

// Arduino Pins
const int FORWARD = 12;
const int BACKWARD = 21;
const int LEN= 19;
const int REN = 6;
const int L_EYE = 5;
const int R_EYE = 4;
const int TRIG = 17;
const int ECHO = 18;
const int R_HALL = 2;
const int L_HALL = 3;

// Speed Global Variables
int rSpeed;
int lSpeed;

// PID Global Variables
float kp = 10;
float ki = 0;
float kd = 0;
double totalTime;
double totalError;
double prevTime;
double prevError;
const int SETPOINT = 10;
double out;

// Velocity Global Variables
volatile int LpulseCount, RpulseCount;
int buggySpeed, objectSpeed;
double motorTime, elapsedMTime;
double totalObjDist, prevDist;

void setup() {
  // WiFi
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  delay(5000);
  IPAddress ip = WiFi.localIP();
  Serial.print(ip);
  server.begin();

  // H-Bridge Data Pins and Enable Pins
  pinMode(FORWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(LEN, OUTPUT);
  pinMode(REN, OUTPUT);

  //IR Sensors
  pinMode(L_EYE, INPUT);
  pinMode(R_EYE, INPUT);

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
  // WiFi
  WiFiClient client = server.available();

  // Debugging
  ch = Serial.read();
  if (ch == 'w') {
    start = true;
    Serial.println("Go");
  }

  if (ch == 's') {
    start = false;
    Serial.println("Stop");
  }

  if (client.connected()) {
    ch = client.read();
    if (ch == 'w') {
      start = true;
      //Serial.println("Go");
    }

    else if (ch == 's')
      start = false;
    //Serial.println("Stop");
  }

  String message = String(buggySpeed) + "," + String(objectSpeed);
  server.println(message);

  // Ultrasonic
  int dist = distance();
  if (dist == 0)
    dist = 45;

  if (dist < 10) {
    close = true;
  }

  else {
    close = false;
  }

  if (dist > 9 && dist < 40) {
    totalObjDist += dist - prevDist;
    prevDist = dist;
    out = PID(dist);
  }
  
  else {
    totalObjDist = 0;
    prevDist = 0;
    out = 150;
  }

  // Encoder and Ultrasonic
  calcVelocity();

  // Infrared Sensors
  lSpeed = 150;
  rSpeed = 150;

  if (!digitalRead(L_EYE)) {
    lSpeed = 150;
    rSpeed = 90;
  }

  else if (!digitalRead(R_EYE)) {
    rSpeed = 150;
    lSpeed = 90;
  }

  // Update Motor State
  if (start && !close) {
    digitalWrite(FORWARD, HIGH);
  }

  else if (!start || close) {
    digitalWrite(FORWARD, LOW);
  }

  // Update Motor Speeds
  analogWrite(LEN, out * (lSpeed / 150));
  analogWrite(REN, out * (rSpeed / 150));
}

int distance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, 2900) / 58.0;
}

double PID(int distance) {
  double currTime = millis();
  double time = currTime - prevTime;

  double currError = SETPOINT - distance;
  totalError += (currError * time);
  double rateError = (currError - prevError) / time;

  double output = kp * currError + ki * totalError + kd * rateError;

  prevError = currError;
  prevTime = currTime;

  return -1 * output;
}

void calcVelocity() {
  if (LpulseCount > 100) {
    elapsedMTime = millis() - motorTime;
    buggySpeed = ((((2*3.14159*3.4) / RpulseCount) + ((2*3.14159*3.4) / LpulseCount)) / 2.0) / (elapsedMTime/1000.0);
    objectSpeed = (totalObjDist / (elapsedMTime/1000)) + buggySpeed;
    totalObjDist = 0;
    motorTime = millis();
    RpulseCount = 0;
    LpulseCount = 0;
  }
}

void Rincrement() {
  RpulseCount++;
}

void Lincrement() {
  LpulseCount++;
}
