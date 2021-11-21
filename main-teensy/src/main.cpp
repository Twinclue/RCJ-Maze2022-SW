#include <Arduino.h>
#include <LiquidCrystal.h>
#include "move_robot.h"
#include "pid.h"


read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor left(35,36,37,15,16);
drive_motor right(39,38,14,15,16);
move_robot move(&left,&right,&toff,&tofb);

LiquidCrystal lcd(25,24,12,11,10,9);

pid movePID(1,1,1);

void setup()
{
  pinMode(27,INPUT);
  delay(100);
  lcd.begin(16,2);
}

void loop()
{
  if(digitalRead(27) == HIGH){
    move.corrDir();
  }
  else{/*
    lcd.print(readRAngle(toff.read(0),tofb.read(3)));
    delay(100);
    lcd.clear();
    */
  }
}