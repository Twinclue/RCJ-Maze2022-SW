#include <Arduino.h>
#include <LiquidCrystal.h>
#include "move_robot.h"
#include "detect_wall.h"

read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor left(35,36,37,15,16);
drive_motor right(39,38,14,15,16);

read_imu imu;

Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);

read_light light(&npix);

move_robot move(&left,&right,&toff,&tofb,&imu,&light);

LiquidCrystal lcd(25,24,12,11,10,9);

detect_wall wall(&toff,&tofb);

void setup()
{
  Wire.begin();
  Wire2.begin();
  
  imu.begin(&Wire2);

  pinMode(0,INPUT);
  pinMode(27,INPUT);
  pinMode(1,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(21,INPUT);
  pinMode(17,INPUT);
  delay(100);
  lcd.begin(16,2);
}

void loop()
{
  if(digitalRead(0) == HIGH){
    if(digitalRead(27) == HIGH){
      Serial.print(wall.getSingleWall(0));
      Serial.print(" : ");
      Serial.print(wall.getSingleWall(1));
      Serial.print(" : ");
      Serial.print(wall.getSingleWall(2));
      Serial.print(" : ");
      Serial.println(wall.getSingleWall(3));
    }
    else{
      
    }
    
  }
  else{

  }
}