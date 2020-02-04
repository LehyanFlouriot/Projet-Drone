#include <Adafruit_NeoPixel.h> // Charge la librairie Neo Pixel d'Adafruit utilisé pour piloter le ruban de LED

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
 
void setup() {
   strip.begin(); // Lance la connection
   strip.show(); // Initialise toute les led à 'off'
   

   progressiveUp(20, RED);
   progressiveDown(20);
   progressiveUp(20, YELLOW);
   progressiveDown(20);
   progressiveUp(20, GREEN);
   progressiveDown(20);
   
}


void loop() {


  strip.setBrightness(100); // Règle la luminosité à 100 % de la luminosité maximale
  chenillardUp(50,GREEN);
  chenillardUp(50,OFF);
  

  

   
}

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
