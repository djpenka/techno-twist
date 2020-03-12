#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <Adafruit_GFX.h>
#include <gamma.h>
#include <DFRobot_RGBMatrix.h>
#include <Wire.h>
#include <math.h>
#include "spinner.h"
#define OE    9
#define LAT   10
#define CLK   11
#define A     A0
#define B     A1
#define C     A2
#define D     A3
#define E     A4
#define WIDTH 64
#define HEIGHT  64
DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, HEIGHT);

const int BLACK = matrix.Color333(0,0,0);
const int RED = matrix.Color333(5, 0, 0);
const int YELLOW = matrix.Color333(5, 5, 0);
const int BLUE = matrix.Color333(0, 0, 5);
const int GREEN = matrix.Color333(0, 5, 0);
const int WHITE = matrix.Color333(5, 5, 5);
const int PURPLE = matrix.Color333(5, 0, 5);
const int LIGHT_GREEN = matrix.Color333(3, 6, 1);

const String LH_STRING PROGMEM = "LEFT HAND";
const String RH_STRING PROGMEM = "RIGHT HAND";
const String LF_STRING PROGMEM = "LEFT FOOT";
const String RF_STRING PROGMEM = "RIGHT FOOT";

const String BLUE_STRING  PROGMEM = "BLUE";
const String YELLOW_STRING  PROGMEM = "YELLOW";
const String GREEN_STRING  PROGMEM = "GREEN";
const String RED_STRING PROGMEM = "RED";

const float RSQ = sq(32.5);
const float PI2 = PI/2;
const float START_THETA = 0;
const float THETA_ACC = -1 * PI / 100;

// Sensors
char OUT[] = {52, 53, 50, 51};
char RIN[] = {48, 49, 46, 47, 44};
char BIN_NEW[] = {45, 42, 43, 40, 41};
char YIN_NEW[] = {38, 39, 36, 37, 34};
char GIN_NEW[] = {35, 32, 33, 30, 31};
int threshold = 470;

CapacitiveSensor r1 = CapacitiveSensor(OUT[0], RIN[0]); 
CapacitiveSensor r2 = CapacitiveSensor(OUT[0], RIN[1]); 
CapacitiveSensor r3 = CapacitiveSensor(OUT[0], RIN[2]); 
CapacitiveSensor r4 = CapacitiveSensor(OUT[0], RIN[3]); 
CapacitiveSensor r5 = CapacitiveSensor(OUT[0], RIN[4]); 

CapacitiveSensor b1 = CapacitiveSensor(OUT[1], BIN_NEW[0]);
CapacitiveSensor b2 = CapacitiveSensor(OUT[1], BIN_NEW[1]);
CapacitiveSensor b3 = CapacitiveSensor(OUT[1], BIN_NEW[2]);
CapacitiveSensor b4 = CapacitiveSensor(OUT[1], BIN_NEW[3]);
CapacitiveSensor b5 = CapacitiveSensor(OUT[1], BIN_NEW[4]); 

CapacitiveSensor y1 = CapacitiveSensor(OUT[2], YIN_NEW[0]);
CapacitiveSensor y2 = CapacitiveSensor(OUT[2], YIN_NEW[1]);
CapacitiveSensor y3 = CapacitiveSensor(OUT[2], YIN_NEW[2]);
CapacitiveSensor y4 = CapacitiveSensor(OUT[2], YIN_NEW[3]);
CapacitiveSensor y5 = CapacitiveSensor(OUT[2], YIN_NEW[4]); 

CapacitiveSensor g1 = CapacitiveSensor(OUT[3], GIN_NEW[0]);
CapacitiveSensor g2 = CapacitiveSensor(OUT[3], GIN_NEW[1]);
CapacitiveSensor g3 = CapacitiveSensor(OUT[3], GIN_NEW[2]);
CapacitiveSensor g4 = CapacitiveSensor(OUT[3], GIN_NEW[3]);
CapacitiveSensor g5 = CapacitiveSensor(OUT[3], GIN_NEW[4]); 

const int NUM_SENSORS = 20;
long values[NUM_SENSORS];
int isTriggered[NUM_SENSORS];

//DEBUG
char debugMode = 0;

// Declaring Functions
void(* funckyFail)(void) = 0;
bool displayResult(int option);
void drawBackground();
int  spinSpinner();
int  getBackgroundPixel(int i, int j, char inCircle);
void sensorOut(char loopForever);
void debugRectangles();
void debugRedraw();
void updateIsTriggered();
char colorIsPressed(char color);
void slowRedrawAll();
void printTimer(int start, int color);

void setup() {
  randomSeed(analogRead(22));
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextColor(WHITE);
  if (debugMode) {
    sensorOut(1);
  }
  drawBackground();
  delay(1000);
}

void loop() {
  sensorOut(0);
  int result = spinSpinner();
  bool fail = displayResult(result);
  if (fail) {
    funckyFail();
  }
  delay(100);
}

void sensorOut(char loopForever) {
  char reading = 1;
  if (debugMode) {
    debugRectangles();
  }
  while(reading) {
    for (int i = 0; i < 3; i++) {
      values[0] = r1.capacitiveSensor(30);
      values[1] = r2.capacitiveSensor(30);
      values[2] = r3.capacitiveSensor(30);
      values[3] = r4.capacitiveSensor(30);
      values[4] = r5.capacitiveSensor(30);

      values[5] = b1.capacitiveSensor(30);
      values[6] = b2.capacitiveSensor(30);
      values[7] = b3.capacitiveSensor(30);
      values[8] = b4.capacitiveSensor(30);
      values[9] = b5.capacitiveSensor(30);

      values[10] = y1.capacitiveSensor(30);
      values[11] = y2.capacitiveSensor(30);
      values[12] = y3.capacitiveSensor(30);
      values[13] = y4.capacitiveSensor(30);
      values[14] = y5.capacitiveSensor(30);

      values[15] = g1.capacitiveSensor(30);
      values[16] = g2.capacitiveSensor(30);
      values[17] = g3.capacitiveSensor(30);
      values[18] = g4.capacitiveSensor(30);
      values[19] = g5.capacitiveSensor(30);
    }

    if (debugMode) {
      debugRedraw();
    } else {
      updateIsTriggered();
    }
    
    reading = loopForever;
  }
}

void updateIsTriggered() {
  for (int k = 0; k < NUM_SENSORS; k++) {
    isTriggered[k] = values[k] > threshold;
    if (isTriggered[k]) {
      Serial.println("IsTriggered");
      Serial.println("k");
      Serial.println(k);
      Serial.println("values[k]");
      Serial.println(values[k]);
    }
  }
}

void debugRedraw() {
  for (int k = 0; k < NUM_SENSORS; k++) {
    if (isTriggered[k] ^ (values[k] > threshold)) {
      Serial.println(k);
      int j = k / 5;
      int i = k % 5;
      int c = PURPLE;

      if (values[k] > threshold) {
        if (j == 0) {
          c = RED;
        } else if (j == 1) {
          c = BLUE;
        } else if (j == 2) {
          c = YELLOW;
        } else if (j == 3) {
          c = GREEN;
        }
      }
      matrix.fillRect(10 + 8 * j, 0 + 10 * i, 5, 5, c);
      isTriggered[k] = values[k] > threshold;
    }
  }
}

void debugRectangles() {
  matrix.setCursor(0,0);

  for (int i = 0; i < 5; i++) {
    matrix.print(i + 1);
    matrix.setCursor(0, 10 * i + 10);
  }

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 4; j++) { 
      int c = PURPLE;
      matrix.fillRect(10 + 8 * j, 0 + 10 * i, 5, 5, c);
    }
  }
}

// 0 = Red
// 1 = Blue
// 2 = Yellow
// 3 = Green
char colorIsPressed(char color) {
  sensorOut(0);
  int i = color * 5;
  for (; i < color * 5 + 5; i++) {
    if (isTriggered[i]) {
      return 1;
    }
  }
  return 0;
}

bool displayResult(int option) {
  // Option is int between 1 and 16
  String body;
  String color;
  bool fail = false;
  int textColor;
  int cNum = (option - 1) % 4;

  // reassign cNum as workaround for proper array indexing
  if (cNum == 0) {
    color = RED_STRING;
    textColor = RED;
  } else if (cNum == 1) {
    color = BLUE_STRING;
    textColor = BLUE;
  } else if (cNum == 2) {
    color = YELLOW_STRING;
    textColor = YELLOW;
  } else {
    color = GREEN_STRING;
    textColor = GREEN;
  }
  
  if (option < 5) {
    body = RF_STRING;
  } else if (option < 9) {
    body = LF_STRING;
  } else if (option < 13) {
    body = LH_STRING;
  } else {
    body = RH_STRING;
  }
  int startY = 23; // top left
  int startX = 0;  // top left
  
  matrix.drawRect(startX , startY, 64, 16 + 3, WHITE);
  matrix.fillRect(startX + 1, startY + 1, 62, 16 + 1, BLACK);

  matrix.setCursor(startX + 2, startY + 2);
  matrix.print(body);
  matrix.setCursor(startX + 2, startY + 2 + 8);
  matrix.setTextColor(textColor);
  matrix.print(color);
  matrix.setTextColor(WHITE);

  printTimer(3, textColor);

  char isPressed = colorIsPressed(cNum);

  matrix.drawRect(startX , startY, 64, 16 + 3, WHITE);
  matrix.fillRect(startX + 1, startY + 1, 62, 16 + 1, BLACK);

  matrix.setCursor(startX + 2, startY + 2);
  matrix.setTextSize(2);

  if (isPressed) {
    matrix.setTextColor(LIGHT_GREEN);
    matrix.print(F("PASS"));
    matrix.setTextColor(WHITE);
  } else {
    matrix.fillScreen(BLACK);
    matrix.drawRect(0, 0, 64, 64, WHITE);
    matrix.drawRect(1, 1, 62, 62, WHITE);
    matrix.setTextSize(8);
    matrix.setTextColor(RED);
    matrix.setCursor(12, -4);
    matrix.print(F("x"));
    delay(3000);
    fail = true;
  }

  matrix.setTextSize(1);

  delay(2000);
  return fail;
}

void printTimer(int start, int color) {
  matrix.drawRect(24, 43, 16, 18, WHITE);
  matrix.setTextSize(2);
  matrix.setTextColor(color);
  for (int i = start; i >= 0; i--) {
    matrix.fillRect(25, 44, 14, 16, BLACK);
    matrix.setCursor(27, 45);
    matrix.print(i);
    delay(1000);
  }
  matrix.setTextSize(1);
  matrix.setTextColor(WHITE);
}


void slowRedrawAll() {
  int color;
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      color = getBackgroundPixel(i,j, 0);
      matrix.drawPixel(i, j, color);
    }
  }
}

void drawBackground() {
  uint8_t *backBuffer = matrix.backBuffer();
  for (int i = 0; i < 6144; i++) {
    backBuffer[i] = pgm_read_byte(spinner + i);
  }
}

int spinSpinner() {
  double v, finalTheta, fTCopy, theta;
  const double radius = 25;
  int endX, endY, result, finalT, t;

  result = random(1, 17);
  finalTheta = result * PI / 8 + 4 * PI - PI / 16;
  fTCopy = finalTheta;
  v = 0;
  finalT = 0;

  while (fTCopy > START_THETA) {
    v += THETA_ACC;
    fTCopy += v;
    finalT++;
  }
  theta = fTCopy;
  v *= -1;
  for (t = 0; t < finalT; t++) {
    endX = radius * cos(theta) + 31.5;
    endY = radius * sin(theta) + 31.5;
    drawBackground();
    matrix.drawLine(31, 31, endX, endY, BLACK);
    matrix.drawLine(31, 32, endX, endY, BLACK);
    matrix.drawLine(32, 31, endX, endY, BLACK);
    matrix.drawLine(32, 32, endX, endY, BLACK);
    delay(50);
    theta += v;
    v += THETA_ACC;
  }
  delay(1000);

  return result;
}

int getBackgroundPixel(int i, int j, char inCircle) {
  int color = 0;
  int radius = 0;
  char colored = 0;
  if ((i > 29 && i < 34) || (j > 29 && j < 34)) {
    if ((i > 29 && i < 34) && (j > 29 && j < 34) && !((i == 30 && j == 30) || (i == 33 && j == 33) || (i == 30 && j == 33) || (i == 33 && j == 30))) {
      color = BLACK;
      colored = true;
    } else if (i == 31 || j == 31 || i == 32 || j == 32) {
      color = WHITE;
      colored = true;
    }
  }
  if (!colored) {
    if (!inCircle) {
      radius = sq(i - 31.5) + sq(j - 31.5);
    }
    double theta = atan2(j-31.5, i-31.5); // arctan(y/x)
    while (theta > PI2) {
      theta -= PI2;
    }
    while (theta < 0) {
      theta += PI2;
    }
    if (inCircle || radius < RSQ) {
      if (theta < PI2 / 4) {
        color = RED;
      } else if (theta < PI2 / 2) {
        color = BLUE;
      } else if (theta < PI2 * 3 / 4) {
        color = YELLOW;
      } else {
        color = GREEN;
      }
    }
  }
  return color;
}