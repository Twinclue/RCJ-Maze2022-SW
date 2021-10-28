#include <Arduino.h>
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"

read_imu sensor;
read_tof toff(&Wire2);
read_tof tofb(&Wire);


const int port = 0;

void setup()
{
  delay(100);
}

void loop()
{/*
  Serial.println();
  Serial.print("Front : ");
  for(int i=0;i<5;i++){
    Serial.print(toff.read(i));
    Serial.print(" : ");
  }

  Serial.println();


Serial.print("Behind : ");
  for(int i=0;i<5;i++){
    Serial.print(tofb.read(i));
    Serial.print(" : ");
  }
  Serial.println();
  */
  Serial.println(readRAngle(toff.read(0),tofb.read(3)));
  delay(50);
}