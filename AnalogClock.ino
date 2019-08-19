

// OLED_Analog_Clock 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program was made simply because I was bored.
//
// Hardware requirements:
//  - Any development board supported by the libraries used
//  - OLED display module supported by OLED_I2C
//  - DS3231 RTC
//
// Software requirements:
//  - OLED_I2C library    http://www.RinkyDinkElectronics.com/library.php?id=79 (Arduino)
//                        http://www.RinkyDinkElectronics.com/library.php?id=80 (chipKit)
//  - DS3231 library      http://www.RinkyDinkElectronics.com/library.php?id=73 (Arduino)
//                        http://www.RinkyDinkElectronics.com/library.php?id=74 (chipKit)
//
// Default serial speed for this sketch is 115200 baud.
//
/**************************************************************************/

// Enable (1) or disable (0) VT100 terminal mode
// Enable this if your terminal program supports VT100 control sequences.
// The Serial Monitor in the Arduino IDE does not support VT100 control sequences. 
// If using the Serial Monitor the line ending should be set to 'No line ending'.
#define VT100_MODE  0

#include <OLED_I2C.h>
#include <DS3231.h>

// Declare which fonts we will be using
extern uint8_t Sinclair_M[];

// Init the OLED and DS3231 using the hardware I2C pins
OLED myOLED(SDA, SCL);
DS3231 rtc(SDA, SCL);

// Init a Time-data structure
Time  t;

int clockCenterX=31;
int clockCenterY=31;
int oldsec=0;

void setup()
{
  myOLED.begin();
  myOLED.setFont(Sinclair_M);
  
  rtc.begin();

  Serial.begin(115200);
  Serial.println("Send any character to enter serial mode...");
  Serial.println();
  t = rtc.getTime();
}

void drawDisplay()
{
  // Clear screen
  myOLED.clrScr();
  
  // Draw Clockface
  for (int i=0; i<2; i++)
  {
    myOLED.drawCircle(clockCenterX, clockCenterY, 31-i);
  }
  for (int i=0; i<3; i++)
  {
    myOLED.drawCircle(clockCenterX, clockCenterY, i);
  }
  
  // Draw a small mark for every hour
  for (int i=0; i<12; i++)
  {
    drawMark(i);
  }  
  t = rtc.getTime();
}

void drawMark(int h)
{
  float x1, y1, x2, y2;
  
  h=h*30;
  h=h+270;
  
  x1=29*cos(h*0.0175);
  y1=29*sin(h*0.0175);
  x2=26*cos(h*0.0175);
  y2=26*sin(h*0.0175);
  
  myOLED.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
}

void drawSec(int s)
{
  float x1, y1, x2, y2;

  s=s*6;
  s=s+270;
  
  x1=29*cos(s*0.0175);
  y1=29*sin(s*0.0175);
  x2=26*cos(s*0.0175);
  y2=26*sin(s*0.0175);
  
  if ((s % 5) == 0)
    myOLED.clrLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
  else
    myOLED.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY);
}

void drawMin(int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;

  m=m*6;
  m=m+270;
  
  x1=25*cos(m*0.0175);
  y1=25*sin(m*0.0175);
  x2=3*cos(m*0.0175);
  y2=3*sin(m*0.0175);
  x3=10*cos((m+8)*0.0175);
  y3=10*sin((m+8)*0.0175);
  x4=10*cos((m-8)*0.0175);
  y4=10*sin((m-8)*0.0175);
  
  myOLED.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY);
  myOLED.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY);
  myOLED.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY);
  myOLED.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY);
}

void drawHour(int h, int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;

  h=(h*30)+(m/2);
  h=h+270;
  
  x1=20*cos(h*0.0175);
  y1=20*sin(h*0.0175);
  x2=3*cos(h*0.0175);
  y2=3*sin(h*0.0175);
  x3=8*cos((h+12)*0.0175);
  y3=8*sin((h+12)*0.0175);
  x4=8*cos((h-12)*0.0175);
  y4=8*sin((h-12)*0.0175);
  
  myOLED.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY);
  myOLED.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY);
  myOLED.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY);
  myOLED.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY);
}

void printDate()
{
  Time t_temp;
  
  t_temp = rtc.getTime();
  myOLED.print(rtc.getDOWStr(FORMAT_SHORT), RIGHT, 0);
  if (t_temp.date<10)
    myOLED.printNumI(t_temp.date, 96, 16);
  else
    myOLED.printNumI(t_temp.date, 88, 16);
  myOLED.print(rtc.getMonthStr(FORMAT_SHORT), RIGHT, 32);
  myOLED.printNumI(t_temp.year, RIGHT, 47);
}

void loop()
{
  int x, y;
  int prevSec;
  
  drawDisplay();
  drawSec(t.sec);
  drawMin(t.min);
  drawHour(t.hour, t.min);
  printDate();
  myOLED.update();
  
  prevSec = t.sec;
  while (t.sec == prevSec)
  {
    if (Serial.available()>0)
      //serialMode();
    delay(100);
    t = rtc.getTime();
  }
}
