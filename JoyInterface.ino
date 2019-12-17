#include "UnoJoy.h"

const byte button0Pin = 2;
const byte button1Pin = 3;
const byte button2Pin = 4;
const byte button3Pin = 5;
const byte button4Pin = 6;
const byte button5Pin = 7;

const byte toggle0Pin = 8;
const byte toggle1Pin = 9;
const byte toggle2Pin = 10;
const byte toggle3Pin = 11;
const byte toggle4Pin = 12;

const byte pot0Pin = 0;
const byte pot1Pin = 1;
const byte pot2Pin = 2;
const byte pot3Pin = 3;

struct UnoOut {
  uint8_t button0: 1;
  uint8_t button1: 1;
  uint8_t button2: 1;
  uint8_t button3: 1;
  uint8_t button4: 1;
  uint8_t button5: 1;
  
  uint8_t toggle0: 1;
  uint8_t toggle1: 1;
  uint8_t toggle2: 1;
  uint8_t toggle3: 1;
  uint8_t toggle4: 1;

  uint8_t pot0 : 8;
  uint8_t pot1 : 8;
  uint8_t pot2 : 8;
  uint8_t pot3 : 8;
};
struct UnoOut getBlankData() {
  UnoOut out;
  out.button0 = 0;
  out.button1 = 0;
  out.button2 = 0;
  out.button3 = 0;
  out.button4 = 0;
  out.button5 = 0;
  
  out.toggle0 = 0;
  out.toggle1 = 0;
  out.toggle2 = 0;
  out.toggle3 = 0;
  out.toggle4 = 0;

  out.pot0 = 1024/2;
  out.pot1 = out.pot0;
  out.pot2 = out.pot0;
  out.pot3 = out.pot0;
  
  return out;
}
struct UnoOut getControllerData() {
  UnoOut out = getBlankData();
  if(digitalRead(button0Pin)) {
    out.button0 = 1;
  }
  if(digitalRead(button1Pin)) {
    out.button1 = 1;
  }
  if(digitalRead(button2Pin)) {
    out.button2 = 1;
  }
  if(digitalRead(button3Pin)) {
    out.button3 = 1;
  }
  if(digitalRead(button4Pin)) {
    out.button4 = 1;
  }
  if(digitalRead(button5Pin)) {
    out.button5 = 1;
  }
  if(digitalRead(toggle0Pin)) {
    out.toggle0 = 1;
  }
  if(digitalRead(toggle1Pin)) {
    out.toggle1 = 1;
  }
  if(digitalRead(toggle2Pin)) {
    out.toggle2 = 1;
  }
  if(digitalRead(toggle3Pin)) {
    out.toggle3 = 1;
  }
  if(digitalRead(toggle4Pin)) {
    out.toggle4 = 1;
  }
  out.pot0 = analogRead(pot0Pin);
  out.pot1 = analogRead(pot1Pin);
  out.pot2 = analogRead(pot2Pin);
  out.pot3 = analogRead(pot3Pin);
  return out;
}
dataForController_t translate(UnoOut u) {
  dataForController_t d = getBlankDataForController();
  
  d.triangleOn = u.button0;
  d.circleOn = u.button1;
  d.squareOn = u.button2;
  d.crossOn = u.button3;
  d.l1On = u.button4;
  d.l2On = u.button5;
  
  d.l3On = u.toggle0;
  d.r1On = u.toggle1;
  d.r2On = u.toggle2;
  d.r3On = u.toggle3;
  d.selectOn = u.toggle4;
   
  //several other digital inputs are not used here. They are set to 0 by getBlankDataForController() above.
  d.leftStickX = map(u.pot0, 0, 1023, 0, 255);
  d.leftStickY = map(u.pot1, 0, 1023, 0, 255);
  d.rightStickX = map(u.pot2, 0, 1023, 0, 255);
  d.rightStickY = map(u.pot3, 0, 1023, 0, 255);
  return d;
}

void setup() 
{
  Serial.begin(9600);
  for(int i = 2; i < 13; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  setupUnoJoy();
}

void loop() 
{
  setControllerData(translate(getControllerData()));
}
