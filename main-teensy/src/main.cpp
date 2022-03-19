#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Encoder.h>
#include "read_BNO055.h"
#include "solver.h"
#include "pinmap.h"
#include "move_robot.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"
#include "D6Tarduino.h"
#include "detect_victim.h"
#include "solver.h"

#define BNO055_SAMPLERATE_DELAY_MS (100)

LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7);
read_BNO055 bno = read_BNO055(55, 0X28, &Wire2, &lcd);
Encoder enc(8,7);

read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor leftM(G2_A_DIR,G2_A_PWM,G2_A_FLT);
drive_motor rightM(G2_B_DIR,G2_B_PWM,G2_B_FLT);
Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);
read_light light(&npix);
move_robot move(&leftM,&rightM,&toff,&tofb,&bno,&light,&lcd,&npix);

uint8_t calib[4];
uint8_t trigPin=0, calibTrig1=1, calibTrig2=2;
int Yaw, Pitch;

void setup(){
    Serial.begin(115200);
    lcd.begin(16,2);
    pinMode(R_COMM_ITR,INPUT);
    pinMode(R_COMM_2,INPUT);
    pinMode(R_COMM_3,INPUT);
    pinMode(L_COMM_ITR,INPUT);
    pinMode(L_COMM_2,INPUT);
    pinMode(L_COMM_3,INPUT);

    pinMode(RE_SW,INPUT);

    pinMode(RE_LED_B,OUTPUT);
    pinMode(RE_LED_G,OUTPUT);
    pinMode(RE_LED_R,OUTPUT);
    move.attachInterrupts();
}
void loop(){
    Serial.print(digitalRead(R_COMM_ITR));
    Serial.print(" ");
    Serial.print(digitalRead(R_COMM_2));
    Serial.print(" ");
    Serial.print(digitalRead(R_COMM_3));
    Serial.print(" ");
    Serial.print(digitalRead(L_COMM_ITR));
    Serial.print(" ");
    Serial.print(digitalRead(L_COMM_2));
    Serial.print(" ");
    Serial.print(digitalRead(L_COMM_3));
    Serial.println(" ");
    if(digitalRead(RE_SW)){
      move.fwd();
    }
}