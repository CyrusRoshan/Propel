#include <Wifiduino.h>

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(isNegative(5));
  //total_foo = 12;
  Serial.println(foo(millis()));
  Serial.println("TEST");
  delay(1000);
}
