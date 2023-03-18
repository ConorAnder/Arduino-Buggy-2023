const int FORWARD = 12;
const int BACKWARD = 21;
const int L_SPEED = 19;
const int R_SPEED = 6;
const int TRIG = 17;
const int ECHO = 18;

// PID Global Variables
double kp = 4;
double ki = 0;
double kd = 0;
double totalTime;
double totalError;
double prevTime;
double prevError;
const int setpoint = 20;

double input;
double out;

double Max, Min, zero;


void setup() {
  Serial.begin(9600);

  pinMode(FORWARD, OUTPUT);
  pinMode(BACKWARD, OUTPUT);
  pinMode(L_SPEED, OUTPUT);
  pinMode(R_SPEED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  prevTime = 0;
  prevError = 0;
  input = 30;
  Max = 0;
  Min = 0;
  zero = 1;
}

void loop() {
  delay(20);
  input = distance();
  if (input > 10 && input < 40) {
    out = PID(input);
  } else
    out = 0;
  Max = max(Max, out);
  Min = min(Min, out);
  if (input == 20)
    zero = out;

  if (out >= 0) {
    digitalWrite(BACKWARD, HIGH);
    digitalWrite(FORWARD, LOW);
    analogWrite(L_SPEED, out);
    analogWrite(R_SPEED, 1.5*out);
  }
  else {
    digitalWrite(FORWARD, HIGH);
    digitalWrite(BACKWARD, LOW);
    analogWrite(L_SPEED, (-1*out));
    analogWrite(R_SPEED, (-1.5*out));
  }


  Serial.print(input);
  Serial.print(",");
  Serial.println(setpoint);

  bool flag = false;
  if (Serial.read() == 'y')
    flag = true;

  while (flag) {
    Serial.println(zero);
  }
}

int distance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  int duration = pulseIn(ECHO, HIGH);
  return duration / 56;
}

double PID(int distance) {
  double currTime = millis();
  double time = currTime - prevTime;

  double currError = setpoint - distance;
  totalError += (currError * time);
  double rateError = (currError - prevError) / time;

  double output = kp * currError + ki * totalError + kd * rateError;
  output = map(output, -15, 7, -255, 255);

  prevError = currError;
  prevTime = currTime;

  return output;
}
