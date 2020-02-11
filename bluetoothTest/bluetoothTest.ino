#include <SoftwareSerial.h>
#define RX 10
#define TX 11

SoftwareSerial BlueT(RX,TX);    //définition du serial bluetooth

void setup() {
  // put your setup code here, to run once:
  BlueT.begin(9600);
  delay(100);

}

void loop() {
  // put your main code here, to run repeatedly:
  BlueT.println("Le bluetooth marche");
}
