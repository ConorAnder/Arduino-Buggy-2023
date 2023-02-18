import processing.serial.*;
import controlP5.*;

Serial arduino;
String feedback;
ControlP5 cp5;
Textfield output;
PFont font1, font2;

void setup() {
  size(800, 600);

  // initialize serial communication with Arduino
//  arduino = new Serial(this, "COM3", 9600); // replace with your own port and baud rate

  // initialize ControlP5 library
  cp5 = new ControlP5(this);

  font1 = createFont("helvetica", 40);
  font2 = createFont("helvetica", 20);
  
  // create start button
  cp5.addButton("START")
     .setPosition(175, 150)
     .setSize(200, 200)
     .setColorBackground(color(30,158,0))
     .setFont(font1)
     ;

  // create stop button
  cp5.addButton("STOP")
     .setPosition(425, 150)
     .setSize(200, 200)
     .setColorBackground(color(190,0,0))
     .setFont(font1)
     ;

  // create textfield for output from Arduino
  output = cp5.addTextfield("Obstacle Detection")
             .setPosition(200, 400)
             .setSize(400, 100)
             .setAutoClear(false)
             .setColor(color(255, 255, 255))
             .setFont(font2)
             .setColorBackground(color(0,0,0))
             ;
}

void draw() {
  textSize (50);                            // text size
  background (0, 50, 150);                  // background colour (r, g, b)
  fill (255, 255, 255);                     // text colour (r, g, b)
  text ("BUGGY CONTROLS", 200, 80);         // ("text", x coordinate, y coordinate)
}
