#include <Wifiduino.h>
#include <Wifiduino.cpp>

float testingvar = 0.6225313770302807;
double testingvar2 = 0.6225313770302807;
char x[] = "test";

void setup() {
  createWifiduino();
  Serial.begin(9600);

  //Wifi.variable("test", &testingvar, FLOAT);
  // put your setup code here, to run once:
}

void loop() {
  Serial.println(charray(&testingvar, FLOAT));
  Serial.println(charray(&testingvar2, DOUBLE));
  Serial.println(charray(&x, CHAR));
  // put your main code here, to run repeatedly:
  //Serial.println(isNegative(5));
  //total_foo = 12;
  //Serial.println(Wifi.varDict->findVarNode(x)->name);
  delay(1000);
}

void serialEvent(){
  Wifi.readMessage();
}
