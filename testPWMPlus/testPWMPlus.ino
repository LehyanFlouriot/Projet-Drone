#include <Wire.h>

#define FREQ        250   // Sampling frequency
// ----------------------- Variables for servo signal generation -------------
unsigned int  period; // Sampling period
unsigned long loop_timer;
unsigned long now, difference;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  // Start I2C communication
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
  // Set pins #4 #5 #6 #7 as outputs
    DDRD |= B11110000;

    period = (1000000/FREQ) ; // Sampling period in µs

    // Initialize loop_timer
    loop_timer = micros();

    
}

void loop() {
  // put your main code here, to run repeatedly:
  applyMotorSpeed();
}

void applyMotorSpeed() {
    // Fs = 250Hz : on envoie les impulsions toutes les 4000µs
    while ((now = micros()) - loop_timer < 4000);

    // Update loop timer
    loop_timer = now;

    // On active les broches #4 #5 #6 #7 (état HIGH)
    PORTD |= B11110000;
    Serial.println("HIGH");

    // On boucle tant que toutes les sorties ne sont pas retournées à l'état LOW
    while (PORTD >= 16) {
        now = micros();
        difference = now - loop_timer;

        if (difference >= 1500) {PORTD &= B11101111;
          Serial.println("LOW");
        } // Passe la broche #4 à LOW
        if (difference >= 1500) PORTD &= B11011111; // Passe la broche #5 à LOW
        if (difference >= 1500) PORTD &= B10111111; // Passe la broche #6 à LOW
        if (difference >= 1500) PORTD &= B01111111; // Passe la broche #7 à LOW
    }
}
