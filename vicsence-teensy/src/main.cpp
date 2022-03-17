#include <Arduino.h>
#include <Wire.h>
#include "read_camera.h"
#include "read_temperature.h"
#include "detect_victim.h"

read_camera camR(&Serial1);
read_temperature d6tR(&Wire);

read_camera camL(&Serial2);
read_temperature d6tL(&Wire1);
detect_victim victim(&Serial1, &Serial2, &Wire, &Wire1);

uint8_t interruptPin=21;
int vicNumR, vicNumL;
float tempR, tempL;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire1.begin();
  pinMode(interruptPin, OUTPUT);
}

void loop() {
  vicNumR=camR.victim_num();
  tempR=d6tR.temp();
  vicNumL=camL.victim_num();
  tempL=d6tL.temp();
  Serial.print(victim.kitNumOneSide(true));  Serial.print("\t");
  Serial.print(tempR);  Serial.print("\t");
  Serial.print(victim.kitNumOneSide(false));  Serial.print("\t");
  Serial.print(tempL);  Serial.print("\t");
  Serial.println("");
  if(tempL>30.0){
    digitalWrite(interruptPin, HIGH);
  }else{
    digitalWrite(interruptPin, LOW);
  }
  delay(50);
}