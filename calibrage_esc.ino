/**
 * Usage, according to documentation(https://www.firediy.fr/files/drone/HW-01-V4.pdf) :
 *     1. Plug your Arduino to your computer with USB cable, open terminal, then type 1 to send max throttle to every ESC to enter programming mode
 *     2. Power up your ESCs. You must hear "beep1 beep2 beep3" tones meaning the power supply is OK
 *     3. After 2sec, "beep beep" tone emits, meaning the throttle highest point has been correctly confirmed
 *     4. Type 0 to send 0 throttle
 *     5. Several "beep" tones emits, wich means the quantity of the lithium battery cells (3 beeps for a 3 cells LiPo)
 *     6. A long beep tone emits meaning the throttle lowest point has been correctly confirmed
 *     7. Type 2 to launch test function. This will send 0 to 180 throttle to ESCs to test them
 */

#include <Servo.h>

Servo motA, motB, motC, motD;
char data;

/**
 * Initialisation routine
 */
void setup() {
    Serial.begin(9600);

    motA.attach(3, 1000, 2000); // 1000 and 2000 are very important ! Values can be different with other ESCs.
    motB.attach(5, 1000, 2000);
    motC.attach(6, 1000, 2000);
    motD.attach(9, 1000, 2000);

    displayInstructions();
}

/**
 * Main function
 */
void loop() {
    if (Serial.available()) {
        data = Serial.read();

        switch (data) {
            // 0
            case 48 : Serial.println("Sending 0 throttle");
                      motA.write(0);
                      motB.write(0);
                      motC.write(0);
                      motD.write(0);
            break;

            // 1
            case 49 : Serial.println("Sending 180 throttle");
                      motA.write(180);
                      motB.write(180);
                      motC.write(180);
                      motD.write(180);
            break;

            // 2
            case 50 : Serial.print("Running test in 3");
                      delay(1000);
                      Serial.print(" 2");
                      delay(1000);
                      Serial.println(" 1...");
                      delay(1000);
                      test();
            break;

            //3
            case 51 : Serial.print("power 20");
                      motA.write(20);
                      motB.write(20);
                      motC.write(20);
                      motD.write(20);
            break;


            //4
            case 52 : Serial.print("power 40");
                      motA.write(40);
                      motB.write(40);
                      motC.write(40);
                      motD.write(40);
            break;

            case 53 : Serial.print("Indpendance test");
                      motA.write(0);
                      motB.write(0);
                      motC.write(0);
                      motD.write(0);

                      delay(1000);

                      motA.write(20);
                      delay(2000);
                      motA.write(0);
                      delay(2000);

                      motB.write(20);
                      delay(2000);
                      motB.write(0);
                      delay(2000);

                      motC.write(20);
                      delay(2000);
                      motC.write(0);
                      delay(2000);

                      motD.write(20);
                      delay(2000);
                      motD.write(0);
                      delay(2000);

                      delay(4000);

                      motA.write(20);
                      delay(1000);
                      motB.write(20);
                      delay(1000);
                      motC.write(20);
                      delay(1000);
                      motD.write(20);
                      delay(1000);

                      delay(4000);

             
                      motA.write(0);
                      motB.write(0);
                      motC.write(0);
                      motD.write(0);

                      
                      
            break;

            case 54: Serial.println("Test de vol"); 
                    vol();

            break;

            case 55: Serial.println("Calibrage esc");
                     calibrage();

            break;
        }
    }
}

/**
 * Test function sending angle to the ESCs from 0 to 180 degrees
 */
void test()
{
    for (int i=0; i<=180; i++) {
        Serial.print("Speed = ");
        Serial.println(i);

        motA.write(i);
        motB.write(i);
        motC.write(i);
        motD.write(i);

        delay(200);
    }

    Serial.println("STOP");
    motA.write(0);
    motB.write(0);
    motC.write(0);
    motD.write(0);
}
void vol()
{
  Serial.println("Bienvenu dans le module de test de vol, entrer 1 pour augmenter la puissance ou 0 pour tout stopper");
  int puissance=0;
  Serial.println("La puissance actuelle est de 0");
  char saisi=0;
  while (saisi != 48)
  {
    motA.write(puissance);
    motB.write(puissance);
    motC.write(puissance);
    motD.write(puissance);

    if(Serial.available())
    {
      saisi = Serial.read();
      if (saisi == 49)
      {
        Serial.print("Vous avez saisi : ");
        Serial.println(saisi);
        puissance = puissance + 1;
        
        if( puissance == 180)
        {
          puissance = 0;
        }
        Serial.print("La nouvelle puissance des moteurs est : ");
        Serial.println(puissance);
      }
    }
  }
  motA.write(0);
  motB.write(0);
  motC.write(0);
  motD.write(0);
  Serial.println("Moteur eteins");
}

void calibrage()
{
  Serial.println("Valeur maximal 180");
  motA.write(180);
  motB.write(180);
  motC.write(180);
  motD.write(180);

  delay(3000);

  Serial.println("Valeur minimale 0");
  motA.write(0);
  motA.write(0);
  motA.write(0);
  motA.write(0);

  delay(4000);
}

/**
 * Displays instructions to user
 */
void displayInstructions()
{
    Serial.println("READY - PLEASE SEND INSTRUCTIONS AS FOLLOWING :");
    Serial.println("\t0 : Sends 0 throttle");
    Serial.println("\t1 : Sends 180 throttle");
    Serial.println("\t2 : Runs test function");
    Serial.println("\t3 : Sends 20 throttle");
    Serial.println("\t4 : Sends 40 throttle");
    Serial.println("\t5 : Runs independant test");
    Serial.println("\t6 : Test de vol");
    Serial.println("\t7 : Calibrage\n");
}
