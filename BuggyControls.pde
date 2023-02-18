import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
PFont font;

void setup() {
  size (600, 700);
  
  cp5 = new ControlP5(this);
  font = createFont("helvetica", 40);
  
  cp5.addButton("START")
    .setPosition(200, 100)
    .setSize (200, 200)
    .setFont(font)
    .setColorBackground(color(30,158,0))
   ;
   
  cp5.addButton("STOP")
    .setPosition(200, 400)
    .setSize (200, 200)
    .setFont(font)
    .setColorBackground(color(190,0,0))
   ;
}

void draw(){
  textSize (40);                            // text size
  background (0, 50, 150);                  // background colour (r, g, b)
  fill (255, 255, 255);                     // text colour (r, g, b)
  text ("BUGGY CONTROLS", 150, 60);         // ("text", x coordinate, y coordinate)
}
