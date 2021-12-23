#include <Arduino.h>

/*
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"
#include "read_temperature.h"

drive_motor left(35,36,37,15,16);
drive_motor right(38,39,14,15,16);
read_temperature d6tR(&Wire);
*/
//#include "D6Tarduino.h"
#include "read_temperature.h"
read_temperature d6tR(&Wire);
read_temperature d6tL(&Wire2);


void setup()
{
  Wire.begin();
  Wire2.begin();
  
  imu.begin();

  pinMode(0,INPUT);
  pinMode(27,INPUT);
  pinMode(1,OUTPUT);
  pinMode(6,OUTPUT);
  delay(100);
}
uint8_t a = 0;
coordinate b;
void loop()
{
  float temp=d6tL.temp();
  Serial.println(temp);
  delay(50);

}