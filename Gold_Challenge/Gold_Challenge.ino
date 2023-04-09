#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>

const char SSID[] = "POCO X3 Pro";
const char PASSWORD[] = "buggy2023";
WiFiServer server(1300);
char command;

const int DATA1 = 15;
const int DATA2 = 16;
const int DATA3 = 6;
const int DATA4 = 4;
const int LEN = 19;
const int REN = 5;
const int TRIG = 7;
const int ECHO = 10;
const int R_HALL = 3;
const int L_HALL = 2;

volatile int LpulseCount, RpulseCount;

Madgwick filter;
float prevYaw;
float accX, accY, accZ;
float gyrX, gyrY, gyrZ;
float roll, pitch, yaw;

void setup() {
  // WiFi
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  delay(5000);
  IPAddress ip = WiFi.localIP();
  Serial.print(ip);
  server.begin();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  pinMode(DATA1, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(DATA3, OUTPUT);
  pinMode(DATA4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(R_HALL), Rincrement, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_HALL), Lincrement, CHANGE);

  command = 'q';
  yaw = 0;
}

void loop() {
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyrX, gyrY, gyrZ);
  }

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
  }

  filter.updateIMU(gyrX, gyrY, gyrZ, accX, accY, accZ);
  roll = filter.getRoll();
  pitch = filter.getPitch();
  //yaw = filter.getYaw();
  yaw += (gyrZ + 3.11) * 0.001;

  if (Serial.available() > 0) {
    command = Serial.read();
    while (Serial.available() > 0)
      Serial.read();
  }

  if (command == 'w') {
    digitalWrite(DATA1, HIGH);
    digitalWrite(DATA2, LOW);
    digitalWrite(DATA3, HIGH);
    digitalWrite(DATA4, LOW);
  }

  else if (command == 'd') {
    digitalWrite(DATA1, HIGH);
    digitalWrite(DATA2, LOW);
    digitalWrite(DATA3, LOW);
    digitalWrite(DATA4, HIGH);
  }

  else if (command == 'a') {
    digitalWrite(DATA1, LOW);
    digitalWrite(DATA2, HIGH);
    digitalWrite(DATA3, HIGH);
    digitalWrite(DATA4, LOW);
  }

  else if (command == 's') {
    digitalWrite(DATA1, LOW);
    digitalWrite(DATA2, HIGH);
    digitalWrite(DATA3, LOW);
    digitalWrite(DATA4, HIGH);
  }

  else {
    digitalWrite(DATA1, LOW);
    digitalWrite(DATA2, LOW);
    digitalWrite(DATA3, LOW);
    digitalWrite(DATA4, LOW);
  }

  if (abs(yaw) > 360)
    yaw = 0;

  if (yaw - prevYaw != 0)
    Serial.println(yaw);
  prevYaw = yaw;
}

void Rincrement() {
  RpulseCount++;
}

void Lincrement() {
  LpulseCount++;
}
