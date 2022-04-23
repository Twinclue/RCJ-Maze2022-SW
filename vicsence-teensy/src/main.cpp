#include <Arduino.h>
#include <Wire.h>
#include "read_camera.h"
#include "read_temperature.h"
#include "detect_victim.h"
#include "pinmap_vic.h"

read_camera camR(&Serial1);
read_temperature d6tR(&Wire);

read_camera camL(&Serial2);
read_temperature d6tL(&Wire1);
detect_victim victim(&Serial1, &Serial2, &Wire, &Wire1);

uint8_t interruptPin=21;
int vicNumR, vicNumL;
float tempR, tempL;

bool rLowBit, rHighBit, lLowBit, lHighBit;
bool rIntr, lIntr;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire1.begin();
  pinMode(interruptPin, OUTPUT);
  pinMode(R_COMM_ITR,OUTPUT);
  pinMode(R_COMM_2,OUTPUT);
  pinMode(R_COMM_3,OUTPUT);
  pinMode(L_COMM_ITR,OUTPUT);
  pinMode(L_COMM_2,OUTPUT);
  pinMode(L_COMM_3,OUTPUT);

  digitalWrite(R_COMM_ITR,LOW);
  digitalWrite(R_COMM_2,LOW);
  digitalWrite(R_COMM_3,LOW);
  digitalWrite(L_COMM_ITR,LOW);
  digitalWrite(L_COMM_2,LOW);
  digitalWrite(L_COMM_3,LOW);
}

void loop() {
  switch (victim.kitNumOneSide(true)){
    case -1:
      rIntr=false;
      rLowBit=false;
      rHighBit=false;
      break;
    case 0:
      rIntr=true;
      rLowBit=false;
      rHighBit=false;
      break;
    case 1:
      rIntr=true;
      rLowBit=true;
      rHighBit=false;
      break;
    case 2:
      rIntr=true;
      rLowBit=false;
      rHighBit=true;
      break;
    case 3:
      rIntr=true;
      rLowBit=true;
      rHighBit=true;
      break;
    default:
      rIntr=false;
      rLowBit=false;
      rHighBit=false;
      break;
  }
  switch(victim.kitNumOneSide(false)){
    case -1:
      lIntr=false;
      lLowBit=false;
      lHighBit=false;
      break;
    case 0:
      lIntr=true;
      lLowBit=false;
      lHighBit=false;
      break;
    case 1:
      lIntr=true;
      lLowBit=true;
      lHighBit=false;
      break;
    case 2:
      lIntr=true;
      lLowBit=false;
      lHighBit=true;
      break;
    case 3:
      lIntr=true;
      lLowBit=true;
      lHighBit=true;
      break;
    default:
      lIntr=false;
      lLowBit=false;
      lHighBit=false;
      break;
  }

  digitalWrite(R_COMM_ITR,rIntr);
  digitalWrite(R_COMM_2,rLowBit);
  digitalWrite(R_COMM_3,rHighBit);
  digitalWrite(L_COMM_ITR,lIntr);
  digitalWrite(L_COMM_2,lLowBit);
  digitalWrite(L_COMM_3,lHighBit);
  
  Serial.print(rIntr);
  Serial.print(" ");
  Serial.print(rLowBit);
  Serial.print(" ");
  Serial.print(rHighBit);
  Serial.print(" ");
  Serial.print(" ");
  Serial.print(lIntr);
  Serial.print(" ");
  Serial.print(lLowBit);
  Serial.print(" ");
  Serial.println(lHighBit);
  
  //Serial.print((camR.read()));
  //Serial.println(camL.read());
  tempR=d6tR.temp();
  tempL=d6tL.temp();
  //Serial.print(tempR);  Serial.print("\t");
  //Serial.print(tempL);  Serial.print("\t");
  //Serial.println("");
  delay(50);

}