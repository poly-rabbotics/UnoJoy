/*  UnoJoy.h
 *  Borrowed from Alan Chatham - 2012
 *  RMIT Exertion Games Lab
  */

#ifndef UNOJOY
#define UNOJOY

#include <stdint.h>
#include <util/atomic.h>
#include <Arduino.h>
typedef struct dataForController_t
  {
    uint8_t triangleOn : 1;  // Each of these member variables
    uint8_t circleOn : 1;    //  control if a button is off or on
    uint8_t squareOn : 1;    // For the buttons, 
    uint8_t crossOn : 1;     //  0 is off
    uint8_t l1On : 1;        //  1 is on
    uint8_t l2On : 1;        
    uint8_t l3On : 1;        // The : 1 here just tells the compiler
    uint8_t r1On : 1;        //  to only have 1 bit for each variable.
                                 //  This saves a lot of space for our type!
    uint8_t r2On : 1;
    uint8_t r3On : 1;
    uint8_t selectOn : 1;
    uint8_t startOn : 1;
    uint8_t homeOn : 1;
    uint8_t dpadLeftOn : 1;
    uint8_t dpadUpOn : 1;
    uint8_t dpadRightOn : 1;

    uint8_t dpadDownOn : 1;
        uint8_t padding : 7;     // We end with 7 bytes of padding to make sure we get our data aligned in bytes
                                 
    uint8_t leftStickX : 8;  // Each of the analog stick values can range from 0 to 255
    uint8_t leftStickY : 8;  //  0 is fully left or up
    uint8_t rightStickX : 8; //  255 is fully right or down 
    uint8_t rightStickY : 8; //  128 is centered.
                                 // Important - analogRead(pin) returns a 10 bit value, so if you're getting strange
                                 //  results from analogRead, you may need to do (analogRead(pin) >> 2) to get good data
  } dataForController_t;

dataForController_t getBlankDataForController(void){
    // Create a dataForController_t
    dataForController_t controllerData;
    // Make the buttons zero
    controllerData.triangleOn = 0;
    controllerData.circleOn = 0;
    controllerData.squareOn = 0;
    controllerData.crossOn = 0;
    controllerData.l1On = 0;
    controllerData.l2On = 0;
    controllerData.l3On = 0;
    controllerData.r1On = 0;
    controllerData.r2On = 0;
    controllerData.r3On = 0;
    controllerData.dpadLeftOn = 0;
    controllerData.dpadUpOn = 0;
    controllerData.dpadRightOn = 0;
    controllerData.dpadDownOn = 0;  
    controllerData.selectOn = 0;
    controllerData.startOn = 0;
    controllerData.homeOn = 0;
    //Set the sticks to 128 - centered
    controllerData.leftStickX = 128;
    controllerData.leftStickY = 128;
    controllerData.rightStickX = 128;
    controllerData.rightStickY = 128;
    // And return the data!
    return controllerData;
}

dataForController_t controllerBuffer;
void setupUnoJoy(void){
    controllerBuffer = getBlankDataForController();
    Serial.begin(38400);
    OCR0A = 128;
    TIMSK0 |= (1 << OCIE0A);
}
ISR(TIMER0_COMPA_vect){
    // If there is incoming data stored in the Arduino serial buffer
    while (Serial.available() > 0) {
      pinMode(13, OUTPUT);
      //digitalWrite(13, HIGH);
      // Get incoming byte from the ATmega8u2
      byte inByte = Serial.read();
      // That number tells us which byte of the dataForController_t struct
      //  to send out.
      Serial.write(((uint8_t*)&controllerBuffer)[inByte]);
      //digitalWrite(13, LOW);
    }
}
int setControllerData(dataForController_t controllerData){
    ATOMIC_BLOCK(ATOMIC_FORCEON){
      controllerBuffer = controllerData;
    }
    return 0;
}

#endif
