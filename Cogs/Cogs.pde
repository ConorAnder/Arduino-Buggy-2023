float AngleStart = 0;
float AngleBump = 0;
float Speed = 1;
color OrangeColor = color(180, 95, 10);
color BlueColor = color(0, 80, 110);
  
void setup() {
  size(800, 800);
  smooth();
}

void draw() {
  background(BlueColor);
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
  angle += Speed*AngleBump;
  AngleStart += Speed*.01;
  AngleBump += Speed*.005;
}
