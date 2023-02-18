import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Client arduino;
String data;

void setup() {
  size(200, 100);
  cp5 = new ControlP5(this);
  arduino = new Client(this, "192.168.222.232", 80);

  cp5.addButton("Go")
     .setValue(0)
     .setPosition(0, 50)
     .setSize(200, 19);
     
   cp5.addButton("Stop")
       .setValue(0)
       .setPosition(0, 0)
       .setSize(200, 19);
}

void draw() {}

public void Go(int theValue) {
  if (arduino.active()) {
    arduino.write("w");
    println("Button pressed!");
  }
}

public void Stop(int theValue) {
  if (arduino.active()) {
    arduino.write("s");
    println("Button pressed!");
  }
}
