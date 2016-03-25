extern "C" {
    #include "Wifiduino.h"   
}
#include <string>

int setPosition(std::string);
//Morse morse(13);

int pos = 0;

void setup()
{
    //myservo.attach(A0);
//    Wifiduino wifiduino = *new Wifiduino;
//    wifiduino.function("setpos", setPosition);
//    wifiduino.variable("getpos", &pos, INT);
}

void loop()
{
}

int setPosition(std::string posValue) {
    pos = std::stoi(posValue);
    printf("%d", pos); //myservo.write(pos);
    return 0;
}

int main() {
    //setup();
    /*while(1) {
        loop();
    }*/
    
    Wifiduino wifiduino = *(createWifiduino());
    wifiduino.varDict->appendVarNode(wifiduino.varDict, "first");
    wifiduino.varDict->appendVarNode(wifiduino.varDict, "second");
    printf("%s\n", wifiduino.varDict->findVarNode(wifiduino.varDict, "first")->name);
    printf("%s\n", wifiduino.varDict->findVarNode(wifiduino.varDict, "second")->name);
    return 0;
}