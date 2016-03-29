#include <Wifiduino.h>
#include <Wifiduino.cpp>

int i = 0;
int test = 12345;

void setup() {
  createWifiduino();
  Serial.begin(9600);
  Wifi.variable("test", &test, INT);
  // put your setup code here, to run once:
}

void loop() {
  //Serial.println(test);
  // put your main code here, to run repeatedly:
  //Serial.println(isNegative(5));
  //total_foo = 12;
  //Serial.println(Wifi.varDict->findVarNode(x)->name);
  delay(100);
}

void serialEvent(){
  Wifi.readMessage();
}
