#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


float x, y, z;          // conditions initiales aléatoires
float sigma = 10.0;
float rho   = 28.0;
float beta  = 8.0 / 3.0;
float dt    = 0.01;


int prev_px, prev_py;


float xMin = 1e6, xMax = -1e6;
float zMin = 1e6, zMax = -1e6;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0)); 

  
  x = random(-100, 100) / 10.0;  // entre -10 et 10
  y = random(-100, 100) / 10.0;
  z = random(-100, 100) / 10.0;

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while(true);

  display.clearDisplay();
  display.display();

  prev_px = SCREEN_WIDTH / 2;
  prev_py = SCREEN_HEIGHT / 2;
}

void loop() {

  //équations différentielles de l'attracteur de Lorenz
  float dx = sigma * (y - x);
  float dy = x * (rho - z) - y;
  float dz = x * y - beta * z;

  x += dx * dt;
  y += dy * dt;
  z += dz * dt;

 
  if(x < xMin) xMin = x;
  if(x > xMax) xMax = x;
  if(z < zMin) zMin = z;
  if(z > zMax) zMax = z;

  
  float xCenter = (xMin + xMax)/2.0;
  float zCenter = (zMin + zMax)/2.0;

 
  float scaleX = 3.0;
  float scaleZ = 2.0;
  int px = constrain((x - xCenter) * scaleX + SCREEN_WIDTH/2, 0, SCREEN_WIDTH-1);
  int py = constrain(SCREEN_HEIGHT/2 - (z - zCenter) * scaleZ, 0, SCREEN_HEIGHT-1);

  
  static int frame = 0;
  if(frame % 3000 == 0) display.clearDisplay();

 
  display.drawLine(prev_px, prev_py, px, py, SSD1306_WHITE);
  display.display();

  prev_px = px;
  prev_py = py;

  frame++;
  delay(10);
}
