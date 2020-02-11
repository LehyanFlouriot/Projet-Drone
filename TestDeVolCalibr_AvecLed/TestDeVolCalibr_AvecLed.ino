//Inclusion des biblioteques

#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <I2Cdev.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

//-----------------------Definitions des moteurs---------------------------------------------------------------


Servo motA, motB, motC, motD;

// ---------------------------------------------------------------------------
#define YAW      0
#define PITCH    1
#define ROLL     2
// --------------------- MPU650 variables ------------------------------------
// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;

// MPU control/status vars

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// Orientation/motion vars
Quaternion q;        // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // Indicates whether MPU interrupt pin has gone high


//defintion des variables de puissance pour les moteurs et des angles du drone (yaw pitch roll)

const int puissanceMini = 20;
const int puissanceMaxi = 40;

int puissanceA = puissanceMini;
int puissanceB = puissanceMini;
int puissanceC = puissanceMini;
int puissanceD = puissanceMini;



float angleYaw;
float anglePitch;
float angleRoll;

//---------------------Definitions des LED----------------------------------------------------



#define PIXEL_PIN 10 // On définit le pin où est connecté la patte DATA du bandeau
#define PIXEL_COUNT 110 // On définit le nombre de LED compris sur le Ruban de LED soit 150 pour le ruban de 5m50

/* Définition des couleurs */
int RED[3] = {255, 0, 0}; // Couleur Rouge
int GREEN[3] = {0, 255, 0}; // Couleur Verte
int CYAN[3] = {0, 255, 255}; // Couleur Cyan
int YELLOW[3] = {255, 125, 0}; // Couleur Jaune
int ORANGE[3] = {255, 40, 0}; // Couleur Orange
int PURPLE[3] = {255, 0 , 255}; // Couleur Violette
int PINK[3] = {255, 0, 100}; // Couleur Rose
int BLUE[3] = {0, 0, 255}; // Couleur Bleu
int WHITE[3] = {255, 255, 255}; // Couleur Blanche
int OFF[3] = {0, 0, 0}; // Eteint

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800); // Paramètre l'objet strip qui correspond à toute les LED du ruban

//-------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------
/**
 * Interrup détection routine
 */
void dmpDataReady() {
    mpuInterrupt = true;
}

//-----------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

  Serial.begin(57600);
  strip.begin();
  strip.show(); //Leds initialisés comme eteintes

  motA.attach(3, 1000, 2000); // 1000 and 2000 are very important ! Values can be different with other ESCs.
  motB.attach(5, 1000, 2000);
  motC.attach(6, 1000, 2000);
  motD.attach(9, 1000, 2000);

  //Début du setup, Leds rouges
  allLeds(RED);
  Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // Verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // Load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // MPU calibration: set YOUR offsets here.
    mpu.setXAccelOffset(-3961);
    mpu.setYAccelOffset(-927);
    mpu.setZAccelOffset(580);
    mpu.setXGyroOffset(179);
    mpu.setYGyroOffset(-59);
    mpu.setZGyroOffset(7);

    // Returns 0 if it worked
    if (devStatus == 0) {
        // Turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // Enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0 : #pin2)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // Set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // Get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

  
  
  motA.write(0);
  motB.write(0);
  motC.write(0);
  motD.write(0);

  delay(7000);
  
  allLeds(BLUE);
  
  

  delay(2000);
  
  allLeds(GREEN);

}

//-------------------------------------------------------------------------------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:
  testStationnaire();
}

//---------------Fonctions LEDs--------------------------------------

void allLeds(int COLOR[])
{
for(int i = 0 ; i < PIXEL_COUNT ; i++)
{
strip.setPixelColor(i, COLOR[0], COLOR[1], COLOR[2]);
}
strip.show();
}

void chenillardUp(int temps, int color[3])
{
for(int i = 0 ; i < PIXEL_COUNT ; i++)
{
strip.setPixelColor(i, color[0], color[1], color[2]);
strip.show();
delay(temps);
}
}
void chenillardDown(int temps, int color[3])
{
for(int i = 0 ; i < PIXEL_COUNT ; i++)
{
strip.setPixelColor(PIXEL_COUNT-i, color[0], color[1], color[2]);
strip.show();
delay(temps);
}
}

void chenillardProgressif(int temps, int color[3])
{
for(int i = 0 ; i < PIXEL_COUNT ; i++)
{
strip.setPixelColor(i, color[0], color[1], color[2]);
strip.show();
delay(temps);
}
}

void progressiveUp(int temps, int COLOR[])
{
for(int lumi = 0 ; lumi < 100 ; lumi++)
{
for(int i = 0 ; i < 150 ; i++)
{
strip.setPixelColor(i, COLOR[0], COLOR[1], COLOR[2]);
}
strip.setBrightness(lumi);
strip.show();
delay(temps);
}
}


void progressiveDown(int temps)
{
for(int lumi = 0 ; lumi < 100 ; lumi++)
{

strip.setBrightness(100-lumi);
strip.show();
delay(temps);
}
for(int i = 0 ; i < 150 ; i++)
{
strip.setPixelColor(i, OFF[0], OFF[1], OFF[2]);
}
}


void changeColor(int temps, int colorDebut[3], int colorFin[3])
{

int color[3] = {0};

color[0] = colorDebut[0];
color[1] = colorDebut[1];
color[2] = colorDebut[2];

while(color[0] != colorFin[0] || color[1] != colorFin[1] || color[2] != colorFin[2])
{

if(color[0] > colorFin[0])
{
color[0]--;
}
else if(color[0] < colorFin[0])
{
color[0]++;
}

if(color[1] > colorFin[1])
{
color[1]--;
}
else if(color[1] < colorFin[1])
{
color[1]++;
}

if(color[2] > colorFin[2])
{
color[2]--;
}
else if(color[2] < colorFin[2])
{
color[2]++;
}

delay(temps);
allLeds(color);
}
}

//-----------------------------------Fontions test staionnaire----------------------------
void testStationnaire()
{
  // If programming failed, don't try to do anything
    if (!dmpReady) {
        return;
    }

    // Wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // Do nothing...
    }

    // Reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // Get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // Check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

        // Otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // Wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) {
            fifoCount = mpu.getFIFOCount();
        }

        // Read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // Track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // Convert Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        // Print angle values in degrees.
        angleYaw = ypr[YAW] * (180 / M_PI);
        anglePitch = ypr[PITCH] * (180 / M_PI);
        angleRoll = ypr[ROLL] * (180 / M_PI);

        /*
        Serial.print(angleYaw);
        Serial.print("\t");
        Serial.print(anglePitch);
        Serial.print("\t");
        Serial.println(angleRoll);
        */

        //Correction des moteurs
        /*
        if (angleRoll > 0){
          if (anglePitch > 0){
            if (angleYaw > 0){
              puissanceA = puissanceMaxi + 10;
              puissanceB = puissanceMaxi;
              puissanceC = puissanceMini + 10;
              puissanceD = puissanceMaxi;
              }
            else {
              puissanceA = puissanceMaxi;
              puissanceB = puissanceMaxi + 10;
              puissanceC = puissanceMini;
              puissanceD = puissanceMaxi + 10;
              }
            }
          else {
            if (angleYaw > 0){
              puissanceA = puissanceMaxi + 10;
              puissanceB = puissanceMaxi;
              puissanceC = puissanceMaxi + 10;
              puissanceD = puissanceMini;
            }
            else{
              puissanceA = puissanceMaxi;
              puissanceB = puissanceMaxi + 10;
              puissanceC = puissanceMaxi;
              puissanceD = puissanceMini + 10;
            }
          }
        }  
        else{
          if (anglePitch > 0){
            if (angleYaw > 0){
              puissanceA = puissanceMaxi + 10;
              puissanceB = puissanceMini;
              puissanceC = puissanceMaxi + 10;
              puissanceD = puissanceMaxi;
            }
            else{
              puissanceA = puissanceMaxi;
              puissanceB = puissanceMini + 10;
              puissanceC = puissanceMaxi;
              puissanceD = puissanceMaxi + 10;
            }
          }
          else {
            if (angleYaw > 0){
              puissanceA = puissanceMini + 10;
              puissanceB = puissanceMaxi;
              puissanceC = puissanceMaxi + 10;
              puissanceD = puissanceMaxi;
            }
            else{
              puissanceA = puissanceMini;
              puissanceB = puissanceMaxi + 10;
              puissanceC = puissanceMaxi;
              puissanceD = puissanceMaxi + 10;
            }
          }
        }

        */
          
        /*  
        if (angleRoll > 0){
          puissanceD = puissanceMini;
          puissanceC = puissanceMini;
          puissanceA = puissanceMaxi;
          puissanceB = puissanceMaxi;
          }
        if (anglePitch > 0){
          puissanceD = puissanceMaxi;
          puissanceA = puissanceMaxi;
          puissanceC = puissanceMini;
          puissanceB = puissanceMini;
          }
        if (anglePitch < 0){
          puissanceD = puissanceMini;
          puissanceA = puissanceMini;
          puissanceC = puissanceMaxi;
          puissanceB = puissanceMaxi;
          }
        if (angleYaw > 0){
          puissanceD = puissanceMini;
          puissanceC = puissanceMaxi;
          puissanceA = puissanceMaxi;
          puissanceB = puissanceMini;
          }
        if (angleYaw < 0){
          puissanceD = puissanceMaxi;
          puissanceC = puissanceMini;
          puissanceA = puissanceMini;
          puissanceB = puissanceMaxi;
          }
        */    

        if(anglePitch > 0){
          puissanceC = puissanceMaxi;
          puissanceD = puissanceMaxi;
          puissanceA = puissanceMini;
          puissanceB = puissanceMini;
          }
        else{
          puissanceA = puissanceMaxi;
          puissanceB = puissanceMaxi;
          puissanceC = puissanceMini;
          puissanceD = puissanceMini;
          }
        
        
        
    }
  motA.write(puissanceA);
  motB.write(puissanceB);
  motC.write(puissanceC);
  motD.write(puissanceD);
  
  Serial.print(puissanceC);
  Serial.print(" / ");
  Serial.println(puissanceB);
  
  Serial.print(puissanceD);
  Serial.print(" / ");
  Serial.println(puissanceA);
}
