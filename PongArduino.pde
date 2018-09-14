import processing.serial.*;

Serial myPort1;
Serial myPort2;

String message1;
float yaw;
float pitch;
float roll;
String[] ypr = new String [3];
String message2;
float yaw2;
float pitch2;
float roll2;
String[] ypr2 = new String [3];

PImage bg;

int x = 20;
int y = 10;

float yPos1 = height/2;
float yPos2 = height/2;

int p1 = 0;
int p2 = 0;

int sx = 2;
int sy = 2;

float vP1 = 0;
float vP2 = 0;

int r1 = 100;
int r2 = 100;
int incR = 5;

boolean aP1 = false;
boolean aP2 = true;

PFont f;

void setup() {
  //println(Serial.list());
  if(aP1 == false){
    myPort1 = new Serial(this, Serial.list()[1], 9600); // Select Port for Player 1
    myPort1.bufferUntil('\n');
  }
  if(aP2 == false){
    myPort2 = new Serial(this,Serial.list()[3], 9600); // Select Port for Player 2
    myPort2.bufferUntil('\n');
  }
  
  bg = loadImage("Images/bg.png");

  //field
  size(400, 300);
  background(bg);
  stroke(255);
  fill(255);
  
  f = createFont("Arial", 24);
  textFont(f);
}

void draw() {
  background(bg);
  x = x + sx;
  y = y + sy;
  
  if(aP1 == false){
    vP1 = serialEvent(myPort1);
  } else {
    if (y < height/2) vP1 = -2;
    else vP1 = 2;
  }
  
  if (yPos1 > height - 70) {
      yPos1 = height - 70;
    } else if (yPos1 < 10) {
      yPos1 = 10;
    } else {
      yPos1 += vP1;
    }
  
  if(aP2 == false){
    vP2 = serialEvent(myPort2);
  } else {
    if(sx > 0){
      if (y < height/2) vP2 = -4;
      else vP2 = 4;
    } else {
      vP2 = 0;
    }
  }
  
  if (yPos2 > height - 70) {
      yPos2 = height - 70;
    } else if (yPos2 < 10) {
      yPos2 = 10;
    } else {
      yPos2 += vP2;
    }

  if ((y < 0) || (y > height)) {
    sy = -1 * sy;
  }

  checkScore();

  //colision
  if ((x <= 20)&&(x >= 10)&&(y >= yPos1)&&(y <= (yPos1 + 60))) {
    sx = -1 * sx;
  }

  if (((x<= width - 10)) && (x >=(width - 30)) && (y >= yPos2)&&(y <= (yPos2 + 60))) {
    sx = -1 * sx;
  }

  //field and ball
  line(200, 0, 200, 300);
  fill(0);
  ellipse(x, y, 10, 10);

  //raqueta and score
  fill(242,168,0);
  stroke(25,120,225);
  strokeWeight(2);
  rect(10, yPos1, 10, 60);
  fill(255);
  noStroke();
  text(p1, (width/2) - 30, 30);
  
  fill(242,168,0);
  stroke(25,120,225);
  strokeWeight(2);
  rect(width - 20, yPos2, 10, 60);
  fill(255);
  noStroke();
  text(p2, (width/2) + 17.5, 30);
}

float serialEvent(Serial myPort)
{
  float vel;
  myPort.write("s");
  message1 = myPort.readStringUntil(13); 
  println(message1);
  if (message1 != null) {
    ypr = split(message1, ","); 
    yaw = -float(ypr[0]); 
    pitch = -float(ypr[1]); // convert to float pitch
    roll = float(ypr[2]); // convert to float roll
  }
  
  if (yaw > 4200) yaw = 4200;
  else if (yaw < -3500) yaw = -3500;
  
  if (yaw > 2700){
    vel = map(yaw, 2700, 4200, 0.75, 10.5);
  } else if (yaw < -2000) {
    vel = map(yaw, -2000, -3500, -0.75, -10.5);
  } else {
    if (yaw > 0) {
      vel = 0.75;
    } else {
      vel = -0.75;
    }
  }
  
  //println(vel);
  return vel;
  
}

void checkScore(){
  if (x > width) {
    p1 += 1; 
    x = width / 2;
    y = height / 2;
  }

  if (x < 0) {
    p2 += 1; 
    x = width / 2;
    y = height / 2;
  }
}