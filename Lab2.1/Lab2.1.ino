
// CHANGE THESE TO MATCH YOUR WIRING, THEN DELETE THE PREVIOUS "#error" LINE
const int LEYE = 19;
const int REYE = 15;
int reye_prevstate = 0;
int leye_prevstate = 0;

bool isStateChange (const int eye, int& prevstate){
  int currstate;

  if (digitalRead(eye) == HIGH)
    currstate = 1;
  else
    currstate = 0;

  if (currstate - prevstate != 0){
    prevstate = currstate;
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);

  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );
}

void loop() {
  if(isStateChange(LEYE, leye_prevstate)){
    if(leye_prevstate == 0)
      Serial.print("l_eye low ");
    else
      Serial.print("l_eye high ");
  }

   if(isStateChange(REYE, reye_prevstate)){
    if(reye_prevstate == 0)
      Serial.print("r_eye low ");
    else
      Serial.print("r_eye high ");
  }
  
  delay(1000);
}
