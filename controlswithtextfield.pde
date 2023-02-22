import controlP5.*;
import processing.net.*;

ControlP5 cp5;
//Client arduino;
Textfield output;
PFont font1, font2;

float AngleStart = 0;
float AngleBump = 0;
float Speed = 1;
color OrangeColor = color(180, 95, 10);
color BlueColor = color(0, 80, 110);
color RedColor = color(200, 0, 0);
color GreenColor = color(0, 200, 0);
boolean spin = false;
  
void setup() {
  size(800, 1000);
  smooth();
  
  cp5 = new ControlP5(this);
  //arduino = new Client(this, "192.168.222.232", 80);
  font1 = createFont("helvetica", 40);
  font2 = createFont("helvetica", 20);
  
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
    
  output = cp5.addTextfield("Obstacle Detection")
             .setPosition(100, 800)
             .setSize(400, 100)
             .setAutoClear(false)
             .setColor(color(255, 255, 255))
             .setFont(font2)
             .setColorBackground(color(0,0,0))
             ;
}

void draw() {
  background(0, 50, 150);
  drawCog(200, 200);
  drawCog(600, 600);
}

void drawCog(int pos1, int pos2) {
  int diam = 400;
  float angle = AngleStart;
  
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

//public void Go() {
 // if (arduino.active()) {
 //   arduino.write("w");
 //   println("Button pressed!");
 //   spin = true;
  //}
//}

//public void Stop() {
  //if (arduino.active()) {
    //arduino.write("s");
    //println("Button pressed!");
    //spin = false;
  //}
//}
