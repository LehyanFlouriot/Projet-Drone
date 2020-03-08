/*
    Arduino and MPU6050 IMU - 3D Visualization Example 
     by Dejan, https://howtomechatronics.com
*/
import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;
Serial myPort;
String data="";
float roll, pitch,yaw;
void setup() {
  size(600,600,P3D);
  background(0);
  lights();
  myPort = new Serial(this, "COM8", 57600); // starts the serial communication
  myPort.bufferUntil('\n');
}
void draw() {
  translate(width/2, height/2, 0);
  background(0);
  
  ambientLight(0,0,255);

  textSize(22);
  text("Roll: " + int(roll) + "     Pitch: " + int(pitch), -100, 265);
  
  
  // Rotate the object
  
  rotateX(radians(roll));
  rotateZ(radians(pitch));
  rotateY(radians(-yaw));
  
  // 3D 0bject
  
  noFill();
  stroke(255);
  DessinDrone();
  
  
}
// Read data from the Serial Port

void serialEvent (Serial myPort) { 
  // reads the data from the Serial Port up to the character '.' and puts it into the String variable "data".
  data = myPort.readStringUntil('\n');
  // if you got any bytes other than the linefeed:
  if (data != null) {
    data = trim(data);
    // split the string at "/"
    String items[] = split(data, '/');
    if (items.length > 1) {
      //--- Roll,Pitch in degrees
      yaw = float(items[0]);
      roll = float(items[1]);
      pitch = float(items[2]);
    }
  }
}


void DessinDrone(){
  beginShape();
  box(500,50,50);
  box(50,50,500);
  
  
  translate(0,25,0);
  rotateY(radians(45));
  box(150,5,150);
  
  translate(0,-50,0);
  
  //box(150,-5,150);
 
 
  endShape();
}
