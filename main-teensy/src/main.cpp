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
detect_wall wall(&toff, &tofb);
node n;
solver solver(&bno, &light, &move, &wall, &n);

D6Tarduino rightTempRaw;
D6Tarduino leftTempRaw;

uint8_t calib[4];
int Yaw, Pitch;

void setup(){
    Serial.begin(115200);
    lcd.begin(16,2);
    Wire2.begin();
    Wire.begin();
    pinMode(R_COMM_ITR,INPUT);
    pinMode(R_COMM_2,INPUT);
    pinMode(R_COMM_3,INPUT);
    pinMode(L_COMM_ITR,INPUT);
    pinMode(L_COMM_2,INPUT);
    pinMode(L_COMM_3,INPUT);

    pinMode(RE_SW,INPUT);

    pinMode(STST,INPUT);
    pinMode(BUZZER,OUTPUT);
    pinMode(RE_LED_B,OUTPUT);
    pinMode(RE_LED_G,OUTPUT);
    pinMode(RE_LED_R,OUTPUT);
    lcd.home();
    lcd.clear();
    if(!bno.begin()){
        lcd.print("INIT BNO FAILED");
        delay(BNO055_SAMPLERATE_DELAY_MS);
    }
    lcd.print("INIT BNO SUCCESS");
    digitalWrite(RE_LED_B,HIGH);
    delay(1000);
    digitalWrite(RE_LED_B,LOW);
}
void loop(){
  lcd.home();
  lcd.clear();
  if (digitalRead(STST) == HIGH) {
    lcd.print("SCORING RUN MODE");
    if(solver.EXrightHand()){
      //finished!
      lcd.clear();
      lcd.print("I'M HOME NOW!");
      while(true);
    }
  }
  else {
    switch (enc.read()){
      case -28:
        enc.write(0);
        break;

      case -24:
        lcd.print("Press to");
        lcd.setCursor(0, 1);
        lcd.print("turn to      <==");
        if (digitalRead(0)) {
          move.turn();
        }
        break;

      case -20:
        lcd.print("Press to      ^");
        lcd.setCursor(0, 1);
        lcd.print("move to       |");
        if (digitalRead(0)) {
          move.rev();
        }
        break;

      case -16:
        lcd.print("Press to");
        lcd.setCursor(0, 1);
        lcd.print("turn to      ==>");
        if (digitalRead(0)) {
          move.turn(-90);
        }
        break;

      case -12:
        lcd.print("Press to      |");
        lcd.setCursor(0, 1);
        lcd.print("move to       V");
        if (digitalRead(0)) {
          move.fwd();
        }
        break;

      case -8:
        lcd.print("Press to drop a");
        lcd.setCursor(0, 1);
        lcd.print("RescueKit to ==>");
        if (digitalRead(0)) {
          move.drop(true);
        }
        break;

      case -4:
        lcd.print("Press to drop a");
        lcd.setCursor(0, 1);
        lcd.print("RescueKit to <==");
        if (digitalRead(0)) {
          move.drop(false);
        }
        break;

      case 0:
        lcd.print("<== Sensor Check");
        lcd.setCursor(0, 1);
        lcd.print("Action Test ==>");
        break;
      case 4:
        lcd.print("Light Raw: ");
        lcd.print(light.read());
        lcd.setCursor(0, 1);
        lcd.print("Light Col: ");
        switch (light.getFloorColor()) {
          case 0:
            lcd.print("WHT");
            break;
          case 1:
            lcd.print("BLK");
            break;
          default:
            lcd.print("SIL");
            break;
        }
        break;
      case 8:
        lcd.print(toff.read(3));
        lcd.setCursor(6, 0);
        lcd.print("Front");
        lcd.setCursor(13, 0);
        lcd.print(toff.read(0));

        lcd.setCursor(0, 1);
        lcd.print(toff.read(2));
        lcd.setCursor(6, 1);
        lcd.print(toff.read(4));
        lcd.setCursor(13, 1);
        lcd.print(toff.read(1));
        delay(450);
        break;
      case 12:
        lcd.print(tofb.read(1));
        lcd.setCursor(6, 0);
        lcd.print(tofb.read(4));
        lcd.setCursor(13, 0);
        lcd.print(tofb.read(2));

        lcd.setCursor(0, 1);
        lcd.print(tofb.read(0));
        lcd.setCursor(6, 1);
        lcd.print("Back");
        lcd.setCursor(13, 1);
        lcd.print(tofb.read(3));
        delay(450);
        break;
      case 16:
        lcd.print("Touch");
        lcd.setCursor(0, 1);
        lcd.print((digitalRead(SW_R) ? "True" : "False"));
        lcd.setCursor(11, 1);
        lcd.print((digitalRead(SW_L) ? "True" : "False"));
        break;
      case 20:
        lcd.print("Pitch: ");
        lcd.print(bno.getPitch());
        lcd.setCursor(0, 1);
        lcd.print("Yaw  : ");
        lcd.print(bno.getYaw());
        break;
      case 24:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : ALL");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.calibAll(1);
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 28:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : MAG");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.magCalib();
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 32:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : ACC");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.accCalib();
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 36:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : GYR");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.gyroCalib();
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 40:
        lcd.print("Right COMM: ");
        lcd.print(digitalRead(R_COMM_ITR));
        lcd.print(" ");
        lcd.print(digitalRead(R_COMM_2));
        lcd.print(digitalRead(R_COMM_3));
        lcd.setCursor(0, 1);
        lcd.print("Left  COMM: ");
        lcd.print(digitalRead(L_COMM_ITR));
        lcd.print(" ");
        lcd.print(digitalRead(L_COMM_2));
        lcd.print(digitalRead(L_COMM_3));
        break;
      case 44:
        enc.write(0);
        break;
      default:
        lcd.print(enc.read());
        break;
    }
    delay(50);
  }
}