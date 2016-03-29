#include <Wifiduino.h>
#include <Wifiduino.cpp>

int testingvar;

void setup() {
  createWifiduino();
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(isNegative(5));
  //total_foo = 12;
  Serial.println(wifi.test);
  delay(1000);
}
