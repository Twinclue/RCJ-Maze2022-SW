#include <Arduino.h>
#include <LiquidCrystal.h>
#include "move_robot.h"


read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor left(35,36,37,15,16);
drive_motor right(39,38,14,15,16);

read_imu imu;

move_robot move(&left,&right,&toff,&tofb,&imu);

LiquidCrystal lcd(25,24,12,11,10,9);


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

void loop()
{
  if(digitalRead(0) == HIGH){
    if(digitalRead(27) == HIGH){
        move.fwd();
        move.turn(-90);
        move.corrDir();
        move.fwd();
        move.turn(-90);
        move.corrDir();
        move.fwd();
        move.turn(-90);
        move.corrDir();
        move.fwd();
        move.turn(-90);
        move.corrDir();
    }
    else{
      digitalWrite(6,HIGH);
      move.turn(-90);
    }
    
  }
  else{
    digitalWrite(1,LOW);
    digitalWrite(6,LOW);
    /*
    lcd.print(readRAngle(toff.read(0),tofb.read(3)));
    delay(100);
    lcd.clear();
    */
  }
}