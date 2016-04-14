//#include <Arduino.h>
#include <iostream>
#include "Propel.hpp"

int ledStatus = 123;
std::string testString = "asdfqwer";
float testFloat = 2.5829324;

void setup() {
    Propel.variable("led", &ledStatus, INT); //bind variable ledStatus to Propel
    Propel.variable("testString", &testString, STRING);
    Propel.variable("testFloat", &testFloat, FLOAT);
    

    //Serial.begin(9600); //begin serial at 9600 baud
    //pinMode(13, OUTPUT); //set pinmode 13 (for the built in LED pin) to output
}

void loop() {
    //digitalWrite(13, ledStatus); //refresh ledStatus, which will be changed by POST requests through wifiduino or manual CLI input
    //delay(100);
}

void serialEvent(){ // this needs to be added for wifiduino to be able to read in data
    Propel.readMessage();
}

int main() {
    setup();
    std::cout << Propel.getVariable("led") << std::endl; //123
    std::cout << Propel.getVariable("testString") << std::endl; //asdfqwer
    std::cout << Propel.getVariable("testFloat") << std::endl; //2.582932
    int testInt = 54;
    Propel.variable("led", &testInt, INT);
    std::cout << Propel.getVariable("led") << std::endl;
    
}