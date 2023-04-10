#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include <vector>

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

float prevYaw;
float accX, accY, accZ;
float gyrX, gyrY, gyrZ;
float roll, pitch, yaw;
float revolution = 80;
std::vector<float> angles, distances;
int max1, max2;

void setup() {
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
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
  while (abs(yaw) < revolution) {
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gyrX, gyrY, gyrZ);
    }

    yaw += (gyrZ + 3.11) * 0.001;

    digitalWrite(DATA1, LOW);
    analogWrite(DATA2, 200);
    analogWrite(DATA3, 200);
    digitalWrite(DATA4, LOW);
    int length = distance();
    if (!isinf(1.0 / length)) {
      angles.push_back(yaw);
      distances.push_back(1.0 / length);
    }
    //Serial.println(abs(yaw));
  }
    yaw = 0;

  digitalWrite(DATA1, LOW);
  analogWrite(DATA2, 0);
  analogWrite(DATA3, 0);
  digitalWrite(DATA4, LOW);

  getPeaks(distances, max1, max2);
  Serial.println(String(1 / distances[max1]) + " " + String(angles[max1]));
  Serial.println(String(1 / distances[max2]) + " " + String(angles[max2]));

  delay(5000);
  float parkAngle = (angles[max1] + angles[max2]) / 2.0;
  Serial.println("Park Angle: " + String(parkAngle));
  distances.clear();
  angles.clear();

  while (abs(yaw) < parkAngle) {
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gyrX, gyrY, gyrZ);
    }

    yaw += (gyrZ + 3.11) * 0.001;

    digitalWrite(DATA1, LOW);
    analogWrite(DATA2, 200);
    analogWrite(DATA3, 200);
    digitalWrite(DATA4, LOW);
    int length = distance();
    if (!isinf(1.0 / length)) {
      angles.push_back(yaw);
      distances.push_back(1.0 / length);
    }
    //Serial.println(abs(yaw));
  }
  
  digitalWrite(DATA1, LOW);
  analogWrite(DATA2, 0);
  analogWrite(DATA3, 0);
  digitalWrite(DATA4, LOW);
  while (1)
    ;
}

int distance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, 2900) / 58.0;
}

void getPeaks(std::vector<float> distances, int& max1, int& max2) {
  float max = 0;
  bool ascending = true;
  for (int i = 1; i < distances.size() - 1; i++) {
    if (distances[i] > distances[i - 1])
      ascending = true;

    else if (distances[i] < distances[i - 1])
      ascending = false;

    if (ascending && distances[i + 1] < distances[i]) {
      if (distances[i] > max) {
        max2 = max1;
        max1 = i;
        max = distances[i];
      }
    }
  }
}

void Rincrement() {
  RpulseCount++;
}

void Lincrement() {
  LpulseCount++;
}
