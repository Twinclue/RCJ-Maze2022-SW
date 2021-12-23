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
  delay(100);
  Serial.begin(9600);
  Wire.begin();
  Wire2.begin();
}

void loop()
{
  float temp=d6tL.temp();
  Serial.println(temp);
  delay(50);
}