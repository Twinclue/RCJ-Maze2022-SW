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

coordinate debug;

solver solver(&imu,&light,&move,&wall,&n);



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
  lcd.begin(16,2);
}
uint8_t a = 0;
coordinate b;
void loop()
{
  if(digitalRead(0) == HIGH){
    /*
    debug = n.getNowCoor();
    lcd.clear();
    lcd.home();
    lcd.print(debug.x);
    lcd.print(":");
    lcd.print(debug.y);
    solver.EXrightHand();*/
    move.turn();
    delay(500);
  }
  else{
    imu.update();
    lcd.clear();
    lcd.home();
    lcd.print(imu.getFPitch());
    if(abs(imu.getFPitch()) >= 20){
      digitalWrite(6,HIGH);
    }
    else{
      digitalWrite(6,LOW);
    }
    delay(10);
  }
}