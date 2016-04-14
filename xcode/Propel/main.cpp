//#include <Arduino.h>
#include "Propel.hpp"

int ledStatus = 0;

void setup() {
  Propel.variable("led", &ledStatus, INT); //bind variable ledStatus to Propel

  //Serial.begin(9600); //begin serial at 9600 baud
  //pinMode(13, OUTPUT); //set pinmode 13 (for the built in LED pin) to output
}

void loop() {
  //digitalWrite(13, ledStatus); //refresh ledStatus, which will be changed by POST requests through wifiduino or manual CLI input
  //delay(100);
}

//void serialEvent(){ // this needs to be added for wifiduino to be able to read in data
  //Wifi.readMessage();
//}

int main() {
    setup();
    
}