const int TRIG = 17;
const int ECHO = 18;

void setup() {
  Serial.begin(9600);

  pinMode (TRIG, OUTPUT);
  pinMode (ECHO, INPUT);
}

void loop() {
  int dist = distance();
  if (dist < 10) {
    Serial.print(dist);
    Serial.print(" ");
    delay(1000);
  }
}

int distance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  int duration = pulseIn(ECHO, HIGH);
  return duration/56;
}