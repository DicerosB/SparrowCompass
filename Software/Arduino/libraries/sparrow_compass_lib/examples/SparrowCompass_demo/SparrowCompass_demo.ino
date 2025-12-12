#include "SparrowCompass.h"

SparrowCompass sp_compass = SparrowCompass();

void setup() {
  // put your setup code here, to run once:
  pinMode(DEBUG_LED_Pin ,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(DEBUG_LED_Pin, 1);
  delay(500);
  digitalWrite(DEBUG_LED_Pin, 0);
  delay(500);
}
