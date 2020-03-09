#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <Adafruit_GFX.h>
#include <gamma.h>
#include <DFRobot_RGBMatrix.h>
#include <Wire.h>
#include <math.h>
#include <Plotter.h>
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
int xArray[50];
int yArray[50];
char xSize = 0;

/*
 * RED, BLUE, GREEN, YELLOW
 */
int cRF = -1;
int cLF = -1;
int cLH = -1;
int cRH = -1;

const int BLACK = matrix.Color333(0,0,0);
const int RED = matrix.Color333(5, 0, 0);
const int YELLOW = matrix.Color333(5, 5, 0);
const int BLUE = matrix.Color333(0, 0, 5);
const int GREEN = matrix.Color333(0, 5, 0);
const int WHITE = matrix.Color333(7, 7, 7);
const int PURPLE = matrix.Color333(5, 0, 5);

char *LH = "LEFT HAND";
char *RH = "RIGHT HAND";
char *LF = "LEFT FOOT";
char *RF = "RIGHT FOOT";

char *BLU = "BLUE";
char *YEL = "YELLOW";
char *GRE = "GREEN";
char *REDS = "RED";

const float RSQ = sq(32.5);
const float PI2 = PI/2;
const float START_THETA = 0;
const float THETA_ACC = -1 * PI / 64;

// Sensors
char OUT[] = {52, 53, 50, 51, 22};
char RIN[] = {49, 45, 41, 37, 33};
char GIN[] = {48, 44, 40, 36, 32};
char BBIN[] = {47, 43, 39, 35, 31};
char YIN[] = {46, 42, 38, 34, 30};



CapacitiveSensor r1 = CapacitiveSensor(OUT[0], RIN[0]); 
CapacitiveSensor r2 = CapacitiveSensor(OUT[1], RIN[1]); 
CapacitiveSensor r3 = CapacitiveSensor(OUT[2], RIN[2]); 
CapacitiveSensor r4 = CapacitiveSensor(OUT[3], RIN[3]); 
CapacitiveSensor r5 = CapacitiveSensor(OUT[4], RIN[4]); 
//
CapacitiveSensor g1 = CapacitiveSensor(OUT[0], GIN[0]);
CapacitiveSensor g2 = CapacitiveSensor(OUT[1], GIN[1]);
CapacitiveSensor g3 = CapacitiveSensor(OUT[2], GIN[2]);
CapacitiveSensor g4 = CapacitiveSensor(OUT[3], GIN[3]);
CapacitiveSensor g5 = CapacitiveSensor(OUT[4], GIN[4]); 
//
CapacitiveSensor b1 = CapacitiveSensor(OUT[0], BBIN[0]);
CapacitiveSensor b2 = CapacitiveSensor(OUT[1], BBIN[1]);
CapacitiveSensor b3 = CapacitiveSensor(OUT[2], BBIN[2]);
CapacitiveSensor b4 = CapacitiveSensor(OUT[3], BBIN[3]);
CapacitiveSensor b5 = CapacitiveSensor(OUT[4], BBIN[4]); 
//
CapacitiveSensor y1 = CapacitiveSensor(OUT[0], YIN[0]);
CapacitiveSensor y2 = CapacitiveSensor(OUT[1], YIN[1]);
CapacitiveSensor y3 = CapacitiveSensor(OUT[2], YIN[2]);
CapacitiveSensor y4 = CapacitiveSensor(OUT[3], YIN[3]);
CapacitiveSensor y5 = CapacitiveSensor(OUT[4], YIN[4]); 

const int NUM_SENSORS = 20;
long values[NUM_SENSORS];
Plotter p;


//DEBUG
char diagnostics = 0;

// Declaring Functions

void displayResult(int option);
void drawSpinnerBack();
void redrawAll();
void spinSpinner();
int getBackgroundPixel(int i, int j, char inCircle);
void addMaybe(int x, int y);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void sensorOut(char loopForever);

void setup() {
  // p.Begin();
  // p.AddTimeGraph("Test", 100, 
  //                "R1", values[0], 
  //                "R2", values[1], 
  //                "R3", values[2],
  //                "R4", values[3],
  //                "G1", values[5],
  //                "G2", values[6],
  //                "G3", values[7],
  //                "G4", values[8], 
  //                "B1", values[10],
  //                "B2", values[11],
  //                "B3", values[12],
  //                "B4", values[13], 
  //                "Y1", values[15],
  //                "Y2", values[16],
  //                "Y3", values[17],
  //                "Y4", values[18]);
//  setPins();
//  if (diagnostics) {
//    sensorOut(1);
//  }
  matrix.begin();
  matrix.setTextColor(WHITE);
  sensorOut(0);
  // matrix.drawCircle(32, 32, 5, RED);
  redrawAll();
  delay(1000);
}

void loop() {
  spinSpinner();
  sensorOut(0);
  delay(100);
}

//void setPins() {
//  int i;
//  for (i = 50; i <= 53; i++) {
//    pinMode(i, OUTPUT);
//    digitalWrite(i, LOW);
//  }
//  pinMode(22, OUTPUT);
//  digitalWrite(22, LOW);
//
//  for (i = 30; i <=49; i++) {
//    pinMode(i, INPUT);
//  }
//}
//
void sensorOut(char loopForever) {
  char reading = 1;
  while(reading) {
    // values[0] = r1.capacitiveSensor(30);
    // matrix.drawCircle(32, 32, 7, WHITE);
    // values[5] = g1.capacitiveSensor(30);
    // matrix.drawCircle(32, 32, 9, WHITE);
    // values[10] = b1.capacitiveSensor(30);
    // matrix.drawCircle(32, 32, 11, WHITE);
    // values[15] = y1.capacitiveSensor(30);
    // matrix.drawCircle(32, 32, 13, WHITE);
    for (int i = 0; i < 10; i++) {
      values[0] = r1.capacitiveSensor(30);
      values[1] = r2.capacitiveSensor(30);
      values[2] = r3.capacitiveSensor(30);
      values[3] = r4.capacitiveSensor(30);
      // values[4] = r5.capacitiveSensor(30);

      values[5] = g1.capacitiveSensor(30);
      values[6] = g2.capacitiveSensor(30);
      values[7] = g3.capacitiveSensor(30);
      values[8] = g4.capacitiveSensor(30);
      // values[9] = g5.capacitiveSensor(30);

      values[10] = b1.capacitiveSensor(30);
      values[11] = b2.capacitiveSensor(30);
      values[12] = b3.capacitiveSensor(30);
      values[13] = b4.capacitiveSensor(30);
      // values[14] = b5.capacitiveSensor(30);

      values[15] = y1.capacitiveSensor(30);
      values[16] = y2.capacitiveSensor(30);
      values[17] = y3.capacitiveSensor(30);
      values[18] = y4.capacitiveSensor(30);
      // values[19] = y5.capacitiveSensor(30);

      p.Plot();
    }
    reading = loopForever;
  }
}

void displayResult(int option) {
  char *body;
  char *color;
  int cNum = (option - 1) % 4;
  if (cNum == 0) {
    color = BLU;
  } else if (cNum == 1) {
    color = YEL;
  } else if (cNum == 2) {
    color = GRE;
  } else {
    color = REDS;
  }
  
  if (option < 5) {
    body = RF;
  } else if (option < 9) {
    body = LF;
  } else if (option < 13) {
    body = LH;
  } else {
    body = RH;
  }
  int startY = 23; // top left
  int startX = 0;  // top left
  
  matrix.drawRect(startX , startY, 64, 16 + 3, WHITE);
  matrix.fillRect(startX + 1, startY + 1, 62, 16 + 1, BLACK);

  matrix.setCursor(startX + 2, startY + 2);
  matrix.print(body);
  matrix.setCursor(startX + 2, startY + 2 + 8);
  matrix.print(color);
  delay(2000);
}

void drawSpinnerBack() {
  int x, y, i;
  for (i = 0; i < xSize; i++) {
    matrix.drawPixel(xArray[i], yArray[i], getBackgroundPixel(xArray[i], yArray[i], 1));
  }
  xSize = 0;
}

void redrawAll() {
  int color;
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      color = getBackgroundPixel(i,j, 0);
      matrix.drawPixel(i, j, color);
    }
  }
}

void spinSpinner() {
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
    drawSpinnerBack();
    endX = radius * cos(theta) + 31.5;
    endY = radius * sin(theta) + 31.5;
    drawLine(31, 31, endX, endY, BLACK);
    drawLine(31, 32, endX, endY, BLACK);
    drawLine(32, 31, endX, endY, BLACK);
    drawLine(32, 32, endX, endY, BLACK);
    delay(50);
    theta += v;
    v += THETA_ACC;
  }
  delay(1000);
  displayResult(result);
  
  redrawAll();
}

int getBackgroundPixel(int i, int j, char inCircle) {
  int color = 0;
  int radius = 0;
  char colored = 0;
  if ((i > 29 && i < 34) || (j > 29 && j < 34)) {
    if ((i > 29 && i < 34) && (j > 29 && j < 34)) {
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
        color = BLUE;
      } else if (theta < PI2 / 2) {
        color = YELLOW;
      } else if (theta < PI2 * 3 / 4) {
        color = GREEN;
      } else {
        color = RED;
      }
    }
  }
  return color;
}

void addMaybe(int x, int y) {
  char present = 0;
  for (int i = 0; i < xSize; i++) {
    if (xArray[i] == x && yArray[i] == y) {
      present = 1;
      break;
    }
  }
  if (!present) {
    xArray[xSize] = x;
    yArray[xSize] = y;
    xSize++;
  }
}

// Adapted from Adafruit_GFX library
void drawLine(int16_t x0, int16_t y0, 
          int16_t x1, int16_t y1, 
          uint16_t color) 
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      // EDITED
      addMaybe(y0, x0);
      // END
      matrix.drawPixel(y0, x0, color);
    } else {
      // EDITED
      addMaybe(x0, y0);
      // END
      matrix.drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
// End adapted from Adafruit_GFX library
