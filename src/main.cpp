#include <Wifiduino.h>
#include <Wifiduino.cpp>

float testingvar;
char x[] = "test";

void setup() {
  createWifiduino();
  Serial.begin(9600);

  wifi.variable("test", &testingvar, FLOAT);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(isNegative(5));
  //total_foo = 12;
  Serial.println(wifi.varDict->findVarNode(x)->name);
  delay(1000);
}
