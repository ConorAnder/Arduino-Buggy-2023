import controlP5.*; //buttons
import processing.net.*; //wifi connection

ControlP5 cp5;
Client arduino;

float AngleStart = 0; //global variables for spinning cog graphic
float AngleBump = 0;
float Speed = 1;
color OrangeColor = color(180, 95, 10);
color BlueColor = color(0, 80, 110);
color RedColor = color(200, 0, 0);
color GreenColor = color(0, 200, 0);
boolean spin;

boolean obstacle; //bool to prevent excessive messages in serial

void setup() {
  size(800, 800);
  smooth(); //for smooth animation
  spin = false;
  obstacle = false;

  cp5 = new ControlP5(this); //new button object
  arduino = new Client(this, "192.168.252.232", 1300); //connect client to IP and port

  cp5.addButton("Go") //button setup
    .setColorBackground(GreenColor) //set colour to green
    .setValue(0) //ensure button is off by default
    .setPosition(500, 100)
    .setSize(200, 200);

  cp5.addButton("Stop")
    .setColorBackground(RedColor)
    .setValue(0)
    .setPosition(100, 500)
    .setSize(200, 200);
}

void draw() {
  background(BlueColor);
  drawCog(200, 200); //create cog graphic
  drawCog(600, 600);

  if (arduino.available() > 0) { //if data is available to read
    int message = arduino.read();
    if (message < 10 && !obstacle) { //if object spotted
      print("Stopping for obstacle at ");
      print(message);
      println(" cm");
      obstacle = true;
    }
    if (message > 9 && obstacle) { //if object lost
      println("Obstacle removed");
      obstacle = false;
    }
  }
}

void drawCog(int pos1, int pos2) {
  int diam = 400;
  float angle = AngleStart;

  fill(OrangeColor);
  ellipse(pos1, pos2, 4*diam/5, 4*diam/5); //orange outer circle
  arc(pos1, pos2, diam, diam, angle, angle + (PI/5)); //draw 5 evenly spaced secants
  arc(pos1, pos2, diam, diam, angle + 2*(PI/5), angle + 3*(PI/5));
  arc(pos1, pos2, diam, diam, angle + 4*(PI/5), angle + 5*(PI/5));
  arc(pos1, pos2, diam, diam, angle + 6*(PI/5), angle + 7*(PI/5));
  arc(pos1, pos2, diam, diam, angle + 8*(PI/5), angle + 9*(PI/5));

  fill(0);
  ellipse(pos1, pos2, diam/4, diam/4); //inner black circle

  if (spin) { //update angles to turn cogs
    angle += Speed*AngleBump;
    AngleStart += Speed*.01;
    AngleBump += Speed*.005;
  }
}


public void Go() { //send 'w' when go is pressed
  if (arduino.active()) {
    arduino.write("w");
    spin = true; //turn the cogs
    println("Go");
  }
}

public void Stop() { //send 's' when stop is pressed
  if (arduino.active()) {
    arduino.write("s");
    spin = false; //stop the cogs
    println("Stop");
  }
}

public void Stop() {
  if (arduino.active()) {
    arduino.write("s");
    spin = false;
    println("Stop");
  }
}
