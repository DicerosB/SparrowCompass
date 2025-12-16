#include "SparrowCompass.h"

SparrowCompass sp_compass = SparrowCompass();

void setup() {
  // put your setup code here, to run once:
  sp_compass.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  sp_compass.work();
  digitalWrite(DEBUG_LED_Pin, 1);
  delay(500);
  digitalWrite(DEBUG_LED_Pin, 0);
  delay(500);
}