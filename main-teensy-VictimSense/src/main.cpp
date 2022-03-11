#include <Arduino.h>
#include <Wire.h>
#include "read_camera.h"
#include "read_temperature.h"
#include "detect_victim.h"

read_camera camR(&Serial1);
read_camera camL(&Serial2);
read_temperature d6tR(&Wire);
read_temperature d6tL(&Wire1);
// detect_victim victim(&Serial1, &Serial2, &Wire, &Wire1);

int vicNumR, vicNumL;
float tempR, tempL;

void setup() {
  Serial.beign(115200);
  Wire.begin();
  Wire1.begin();
}

void loop() {
  vicNumR=camR.victim_num();
  vicNumL=camL.victim_num();
  tempR=d6tR.temp();
  tempL=d6tL.temp();
  Serial.print(vicNumR);  Serial.print("\t");
  Serial.print(vicNumL);  Serial.print("\t");
  Serial.print(tempR);  Serial.print("\t");
  Serial.print(tempL);  Serial.print("\t");
  Serial.println("");
  delay(50);
}