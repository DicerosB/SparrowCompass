#include "SparrowCompass.h"

//SparrowCompass sp_compass = SparrowCompass();

void setup() {
  // put your setup code here, to run once:
  pinMode(DEBUG_LED_Pin, OUTPUT);
  pinMode(PA12, OUTPUT);
  digitalWrite(PA12, LOW);
  delay(100);
  digitalWrite(PA12, HIGH);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("USB Serial Initialised"));
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String buffer = Serial.readString();
    Serial.println(buffer);
  }
  digitalWrite(DEBUG_LED_Pin, 1);
  delay(500);
  digitalWrite(DEBUG_LED_Pin, 0);
  delay(500);
}