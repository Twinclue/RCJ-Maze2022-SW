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

#define BNO055_SAMPLERATE_DELAY_MS (100)

LiquidCrystal lcd (25,24,12,11,10,9);
read_BNO055 bno = read_BNO055(55, 0X28, &Wire2, &lcd);
Encoder enc(8,7);

uint8_t calib[4];
uint8_t trigPin=0, calibTrig1=1, calibTrig2=2;
int Yaw, Pitch;

void setup(){
    Serial.begin(115200);
    lcd.begin(16,2);
    pinMode(trigPin, INPUT);
    pinMode(calibTrig1,INPUT);
    pinMode(calibTrig2,INPUT);

    if(!bno.begin()){
        Serial.println("failed to begin BNO055");
        delay(BNO055_SAMPLERATE_DELAY_MS);
    }
    Serial.println("successfully begun");
    while(!bno.calibAll(0)){
        Serial.println("Failed to calibrate");
    }
    lcd.clear();    lcd.print("1st calib done");
}
void loop(){
    Serial.println(enc.read());
    switch (int8_t(enc.read()*0.25))
    {
    case -1:
        Yaw=bno.getYaw();
        lcd.clear();
        lcd.print("Yaw: ");    lcd.setCursor(4,0);     lcd.print(Yaw);
        break;
    case 0:
        Pitch=bno.getPitch();
        lcd.clear();
        lcd.print("Pit: ");    lcd.setCursor(4,0);     lcd.print(Pitch);
        break;
    case 1:
        lcd.clear();
        lcd.print("ACC CALIB");
        if(digitalRead(trigPin)){
            bno.accCalib();
        }
        break;
    case 2:
        lcd.clear();
        lcd.print("GYRO CALIB:");
        if(digitalRead(trigPin)){
            bno.gyroCalib();
        }
        break;
    case 3:
        lcd.clear();
        lcd.print("MAG CALIB:");
        if(digitalRead(trigPin)){
            bno.magCalib();
        }
        break;
    case 4:
        lcd.clear();
        lcd.print("MAN ALL CALIB");
        if(digitalRead(trigPin)){
            bno.calibAll(1);
        }
        break;
    default:
        lcd.clear();
        lcd.print("ROTATE ENCODER");
        break;
    }
    Serial.println("working");
    delay(BNO055_SAMPLERATE_DELAY_MS);
}