import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Client arduino;

float AngleStart = 0;
float AngleBump = 0;
float Speed = 1;
color OrangeColor = color(180, 95, 10);
color BlueColor = color(0, 80, 110);
color RedColor = color(200, 0, 0);
color GreenColor = color(0, 200, 0);
boolean spin = false;
int prevt; //previous time
int currt; //current time
PFont font1;
  
void setup() {
  size(800, 800);
  smooth();
  prevt = 1;
  currt = 1;
  
  cp5 = new ControlP5(this);
  arduino = new Client(this, "192.168.203.232", 1300);
  
  font1 = createFont("helvetica", 40);

  cp5.addButton("Go")
    .setPosition(500, 100)
    .setSize(200, 200)
    .setColorBackground(color(30,158,0))
    .setFont(font1)
     ;
     
  cp5.addButton("Stop")
    .setPosition(100, 500)
    .setSize(200, 200)
    .setColorBackground(color(190,0,0))
    .setFont(font1)
    ;
}

void draw() {
  background(BlueColor);
  drawCog(200, 200);
  drawCog(600, 600);
}

void drawCog(int pos1, int pos2) {
  int diam = 400;
  float angle = AngleStart;
  
 currt = second();
  if (currt != 0 && prevt != 0 && currt % prevt != 0) {
    arduino.write('h');
    if (arduino.available() > 0) {
      String message = arduino.readString();
      String[] match = match(message, "^[1-9]$");
      if (match != null) {
        print("Stopping for obstacle at ");
        print(message);
        println(" cm");
      }
    }
  }
  prevt = currt;
  
  fill(OrangeColor);
  ellipse(pos1, pos2, 4*diam/5, 4*diam/5);
  arc(pos1, pos2, diam, diam, angle, angle + (PI/5));
  arc(pos1, pos2, diam, diam, angle + 2*(PI/5), angle + 3*(PI/5));
  arc(pos1, pos2, diam, diam, angle + 4*(PI/5), angle + 5*(PI/5));
  arc(pos1, pos2, diam, diam, angle + 6*(PI/5), angle + 7*(PI/5));
  arc(pos1, pos2, diam, diam, angle + 8*(PI/5), angle + 9*(PI/5));
  
  fill(0);
  ellipse(pos1, pos2, diam/4, diam/4);
  
  if (spin) {
    angle += Speed*AngleBump;
    AngleStart += Speed*.01;
    AngleBump += Speed*.005;
  }
}


public void Go() {
  if (arduino.active()) {
    arduino.write("w");
    spin = true;
    println("Go");
  }
}

public void Stop() {
  if (arduino.active()) {
    arduino.write("s");
    spin = false;
    println("Stop");
  }
}
