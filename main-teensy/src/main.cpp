#include <Arduino.h>
#include <LiquidCrystal.h>
#include "move_robot.h"
#include "detect_wall.h"
#include "solver.h"
#include "node.h"

read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor leftM(35,36,37,15,16);
drive_motor rightM(39,38,14,15,16);

read_imu imu;
Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);
read_light light(&npix);

move_robot move(&leftM,&rightM,&toff,&tofb,&imu,&light);
LiquidCrystal lcd(25,24,12,11,10,9);
detect_wall wall(&toff,&tofb);

node n;

solver solver(&imu,&light,&move,&wall,&n);



void setup()
{
  Wire.begin();
  Wire2.begin();
  
  imu.begin(&Wire2);

  pinMode(0,INPUT);
  pinMode(27,INPUT);
  pinMode(1,OUTPUT);
  pinMode(6,OUTPUT);
  delay(100);
  lcd.begin(16,2);
}
uint8_t a = 0;
coordinate b;
void loop()
{
  if(digitalRead(27) == HIGH){
    if(digitalRead(0) == HIGH){
    }
  }
  else{
  }
  delay(500);
}