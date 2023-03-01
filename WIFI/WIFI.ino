#include <SPI.h>
#include <WiFiNINA.h>

const char SSID[] = "POCO X3 Pro";  //Hotspot name and password
const char PASSWORD[] = "buggy2023";
WiFiServer server(1300);  //Server port
bool start = false;       //Updates via wifi to start and stop the buggy

const int FORWARD = 12;   //1A and 3A
const int BACKWARD = 21;  //2A and 4A
const int L_SPEED = 19;   //12EN
const int R_SPEED = 6;    //34EN
const int L_EYE = 3;      //Left IR sensor
const int R_EYE = 2;      //Right IR sensor
const int TRIG = 17;      //Ultrasonic trigger
const int ECHO = 18;      //Ultrasonic echo

int Rstate;  //global variables used to update speed via PWM on EN pins
int Lstate;
volatile bool Lbool;  //global variables corresponding to IR sensors updated via interrupts
volatile bool Rbool;  //Must be volatile if changed within interrupts

void setup() {
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  delay(5000);  //Make sure wifi is connected
  IPAddress ip = WiFi.localIP();
  Serial.print(ip);  //In case IP address changes
  server.begin();

  pinMode(FORWARD, OUTPUT);  //output because sending data out to h-bridge
  pinMode(BACKWARD, OUTPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);

  pinMode(L_EYE, INPUT);  //input because receiving data to arduino
  pinMode(R_EYE, INPUT);
  attachInterrupt(digitalPinToInterrupt(L_EYE), LirSensor, FALLING);
  attachInterrupt(digitalPinToInterrupt(R_EYE), RirSensor, FALLING);
  //creates an interrupt function L/RirSensor which is run when the value from
  //L/R_EYE falls from HIGH to LOW
  //interrupts only found at pins 3 and 4

  Rstate = HIGH;  //initialise speed
  Lstate = HIGH;

  Lbool = false;  //initialise IR booleans
  Rbool = false;

  pinMode(TRIG, OUTPUT);  //Ultrasonic pins
  pinMode(ECHO, INPUT);
}

void loop() {
  WiFiClient client = server.available();  //create client object for processing

  if (client.connected()) {  //read a character from processing
    char ch = client.read();
    if (ch == 'w') {  //determine if buggy should start or stop
      start = true;
    }

    else if (ch == 's')
      start = false;
  }

  int dist = distance();      //read distance from function
  if (dist < 10 || !start) {  //if object seen or told to stop
    Lstate = 0;
    Rstate = 0;  //stop the buggy
  }

  else if (digitalRead(L_EYE) && digitalRead(R_EYE)) {  //if IR detects nothing
    Lstate = 150;
    Rstate = 150;
    Lbool = false;
    Rbool = false;
  }

  if (Lbool && start && dist > 9) {  //if left IR triggers
    Lstate = 150; //differential steering
    Rstate = 70;
  }

  if (Rbool && start && dist > 9) {  //if right IR triggers
    Lstate = 70;
    Rstate = 150;
  }

  server.write(dist);  //send distance data to processing

  digitalWrite(FORWARD, HIGH);  //move forward
  digitalWrite(BACKWARD, LOW);
  analogWrite(L_SPEED, Lstate);  //use PWM to vary speed
  analogWrite(R_SPEED, Rstate);
  delay(200);  //delay for interrupt recovery
}

int distance() {  //return distance using Ultrasonic
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);  //send ping
  delayMicroseconds(10);
  int duration = pulseIn(ECHO, HIGH);  //listen for response
  return duration / 56; //convert time to distance
}

void LirSensor() {  //Left sensor interrupt function
  Lbool = true;
}

void RirSensor() {  // sensor interrupt function
  Rbool = true;
}
void LirSensor() { //Left sensor interrupt function
  Lbool = true;
}

void RirSensor() { //Right sensor interrupt function
  Rbool = true;
}
