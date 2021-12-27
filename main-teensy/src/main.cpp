#include <Arduino.h>
#include <Wire.h>
#include "read_camera.h"
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
/*
#include "D6Tarduino.h"
#include "read_temperature.h"
read_temperature d6tR(&Wire);
read_temperature d6tL(&Wire2);
*/
#include "detect_victim.h"
detect_victim victim(&Serial4, &Serial5, &Wire2, &Wire); 
// read_temperature d6tR(&Wire2);
// read_temperature d6tL(&Wire);
//D6Tarduino d6tR;


// read_camera camR(&Serial5);
// read_camera camL(&Serial4);

void setup()
{
  //Serial.begin(9600);
  Wire.begin();
  Wire2.begin();
  //d6tR.setBus(&Wire2);
  //imu.begin();

  pinMode(0,INPUT);
  pinMode(27,INPUT);
  pinMode(1,OUTPUT);
  pinMode(6,OUTPUT);
  delay(100);
}
uint8_t a = 0;
//coordinate b;

void loop()
{
  // Serial.print(camR.read());
  // Serial.println(camL.read());
  // int8_t kitNumR;
  Serial.println("Hello");
  Serial.print(victim.kitNumOneSide(true));
  Serial.print(victim.kitNumOneSide(false));
  // Serial.println(kitNumR);
  //Serial.println(camR.victim_num());
  delay(50);
}