const int FORWARD = 12;
const int BACKWARD = 21;
const int L_SPEED = 19;
const int R_SPEED = 6;
const int R_HALL = 16;
const int L_HALL = 15;

double LpulseCount, RpulseCount;
int Lpulse, Rpulse;
int LprevPulse, RprevPulse;

void setup() {
  Serial.begin(9600);

  pinMode(FORWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);
  pinMode(R_HALL, INPUT_PULLUP);
  pinMode(L_HALL, INPUT_PULLUP);

  LpulseCount = 0;
  Lpulse = 0;
  LprevPulse = 0;
  RpulseCount = 0;
  Rpulse = 0;
  RprevPulse = 0;
}

void loop() {
  digitalWrite(FORWARD, HIGH);
  digitalWrite(L_SPEED, HIGH);
  digitalWrite(R_SPEED, HIGH);

  LVel();
}

void RVel() {
  if (digitalRead(R_HALL))
    Rpulse = 1;

  else
    Rpulse = 0;
  
  if (Rpulse % RprevPulse != 0)
    RpulseCount += 0.88462;
  
  if (RpulseCount > 480) {
    Serial.println("Revolution");
    RpulseCount -= 480;
  }
  RprevPulse = Rpulse;
}

void LVel() {
  if (digitalRead(L_HALL))
    Lpulse = 1;

  else
    Lpulse = 0;
  
  if (Lpulse % LprevPulse != 0)
    LpulseCount++;
  
  if (LpulseCount > 480) {
    Serial.println("Revolution");
    LpulseCount -= 480;
  }
  LprevPulse = Lpulse;
}