#include <Arduino.h>
#include "read_imu.h"
#include "read_tof.h"


read_imu sensor;
read_tof tof(&Wire2);


const int port = 0;

void setup()
{
  delay(100);
}

void loop()
{
  
  for(int i=0;i<5;i++){
    Serial.print(tof.read(i));
    Serial.print(" : ");
  }
  
  //Serial.print(tof.read(4));
  Serial.println();
  delay(100);
}