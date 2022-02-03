#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#include "move_robot.h"
#include "detect_wall.h"
#include "solver.h"
#include "node.h"
#include "detect_victim.h"


#define DEBUG

read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor leftM(35,36,37,15,16);
drive_motor rightM(39,38,14,15,16);

read_imu imu;
Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);
read_light light(&npix);

LiquidCrystal lcd(25,24,12,11,10,9);
detect_wall wall(&toff,&tofb);

move_robot move(&leftM,&rightM,&toff,&tofb,&imu,&light,&lcd,&npix);
node n;
detect_victim victim(&Serial4, &Serial5, &Wire2, &Wire); 
coordinate debug;

solver solver(&imu,&light,&move,&wall,&n);


void setup()
{
  //Serial.begin(9600);
  Wire.begin();
  Wire2.begin();
  imu.begin(&Wire2);

  pinMode(0,INPUT);
  pinMode(27,INPUT);
  pinMode(1,OUTPUT);
  pinMode(6,OUTPUT);
  delay(100);
  #ifdef DEBUG
  lcd.clear();
  lcd.home();
  lcd.print("DEBUG MODE!");
  Serial.println("DEBUG MODE CLICK TO START");
  while(!digitalRead(0));
  Serial.println("START!");
  #endif
}
uint8_t a = 0;
//coordinate b;
#ifdef DEBUG
unsigned int debugLoopCount = 0;
#endif
void loop()
{
  if(digitalRead(27) == HIGH){
    #ifdef DEBUG
    Serial.println("-----------------");
    Serial.print("SYS TIME : ");
    Serial.println(millis());
    Serial.print("LOOP COUNT : ");
    Serial.println(debugLoopCount);
    Serial.println("-----------------");
    Serial.print("X : ");
    Serial.print(debug.x);
    Serial.print(" | Y : ");
    Serial.print(debug.y);
    Serial.print(" | Z : ");
    Serial.println(debug.z);
    debugLoopCount++;
    Serial.println("-----------------");
    lcd.clear();
    lcd.home();
    lcd.print("DEBUG MODE!");
    lcd.setCursor(0,1);
    lcd.print("CLICK TO MOVE");
    while(!digitalRead(0));
    lcd.clear();
    lcd.home();
    lcd.print("DEBUG MODE!");
    lcd.setCursor(0,1);
    lcd.print("PLZ WAIT:MOVING");
    #endif
    solver.EXrightHand();
    delay(500);
  }
  else{
    if(digitalRead(0)){
      move.blink();
    }
    // debug = n.getNowCoor();
    // lcd.clear();
    // lcd.home();
    // lcd.print(debug.x);
    // lcd.print(":");
    // lcd.print(debug.y);
    lcd.clear();
    lcd.home();
    lcd.print(victim.kitNumOneSide(true));
    lcd.print(":");
    lcd.print(victim.kitNumOneSide(false));
    delay(100);
  }
}