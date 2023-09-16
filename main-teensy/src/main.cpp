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
solver solver(&bno, &light, &move, &wall, &n, &lcd);

D6Tarduino rightTempRaw;
D6Tarduino leftTempRaw;

uint8_t calib[4];
int Yaw, Pitch;

bool runningFlag = false;
uint8_t colorState=0;




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

    pinMode(STST, INPUT);
    pinMode(BUZZER, OUTPUT);

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
    if(digitalRead(STST)){
      lcd.clear();
      lcd.print("STST IS ON NOW!");
      lcd.setCursor(0,1);
      lcd.print("PLEASE TURN OFF");
      while(digitalRead(STST)){
        tone(BUZZER,1000);
        digitalWrite(RE_LED_R,HIGH);
        delay(100);
        digitalWrite(RE_LED_R,LOW);
        noTone(BUZZER);
        delay(100);
      }
    }
}
void loop(){


  lcd.home();
  lcd.clear();
  if (digitalRead(STST) == HIGH) {
    runningFlag = true;
    lcd.print("SCORING RUN MODE");
    if(solver.EXrightHand()){
      //finished!
      lcd.clear();
      lcd.print("I'M HOME NOW!");
      while(true);
    }
  }
  else {
    if(runningFlag){
      runningFlag = false;
      //n.lackOfProgress();
    }
    int encoderValue=enc.read()*0.25;
    switch (encoderValue){
      case -11: 
        enc.write(11*4);  //jumping to another end
        break;
      case -10:
        lcd.print("<== Action Test");
        lcd.setCursor(0, 1);
        lcd.print("Sensor Check ==>");
        break;
      case -9:
        if(digitalRead(RE_SW)){
          move.loadRescueKit();
        }
        break;
      case -8:
        lcd.print("CLICK TO BLINK");
        if(digitalRead(RE_SW)){
          move.blink();
        }
        
        break;
      case -7:
        lcd.print("corrDir");
        if(digitalRead(RE_SW)){
          move.corrDir();
        }
        break;

      case -6:
        lcd.print("Press to");
        lcd.setCursor(0, 1);
        lcd.print("turn to      <==");
        if (digitalRead(0)) {
          move.turn();
        }
        break;

      case -5:
        lcd.print("Press to      ^");
        lcd.setCursor(0, 1);
        lcd.print("move to       |");
        if (digitalRead(0)) {
          move.rev();
        }
        break;

      case -4:
        lcd.print("Press to");
        lcd.setCursor(0, 1);
        lcd.print("turn to      ==>");
        if (digitalRead(0)) {
          move.turn(-90);
        }
        break;

      case -3:
        lcd.print("Press to      |");
        lcd.setCursor(0, 1);
        lcd.print("move to       V");
        if (digitalRead(0)) {
          move.fwd();
        }
        break;

      case -2:
        lcd.print("Press to drop a");
        lcd.setCursor(0, 1);
        lcd.print("RescueKit to ==>");
        if (digitalRead(0)) {
          move.drop(true);
        }
        break;

      case -1:
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
      case 1:
        if (digitalRead(0)) {
          colorState++;
          delay(100);
        }
        switch (colorState%3)
        {
        case 0:
          light.setColor(80, 80, 80);
          break;
        case 1:
          light.setColor(150, 0,0);
          break;
        case 2:
          light.setColor(0, 150, 0);
          break;
        default:
          break;
        }
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
      case 2:
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
      case 3:
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
      case 4:
        lcd.print("Touch");
        lcd.setCursor(0, 1);
        lcd.print((digitalRead(SW_R) ? "True" : "False"));
        lcd.setCursor(11, 1);
        lcd.print((digitalRead(SW_L) ? "True" : "False"));
        break;
      case 5:
        lcd.print("Pitch: ");
        lcd.print(bno.getPitch());
        lcd.setCursor(0, 1);
        lcd.print("Yaw  : ");
        lcd.print(bno.getYaw());
        break;
      case 6:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : ALL");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.calibAll(1);
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 7:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : MAG");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.magCalib();
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 8:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : ACC");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.accCalib();
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 9:
        lcd.print("CLICK TO CALIB");
        lcd.setCursor(0, 1);
        lcd.print("CALIB MODE : GYR");
        if(digitalRead(RE_SW)){
          digitalWrite(RE_LED_R,HIGH);
          bno.gyroCalib();
          digitalWrite(RE_LED_R,LOW);
        }
        break;
      case 10:
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
      case 11:
        lcd.print("GAME MODE:");
        if(digitalRead(RE_SW)){
          if(move.currentMode==move.WORLD){move.currentMode=move.ENTRY;}
          else if(move.currentMode==move.ENTRY){move.currentMode=move.WORLD;}
          else{
            lcd.setCursor(0,1);
            lcd.print("EXEPTION");
            while(1);
          }
          delay(100);
        }
        lcd.setCursor(0, 1);
        if(move.currentMode==move.WORLD){lcd.print("WORLD");}
        if(move.currentMode==move.ENTRY){lcd.print("ENTRY");}
        break;
      case 12:
        enc.write(-10*4);   //jumping to another end
        break;
      default:    //should never happen
        lcd.print(encoderValue);
        break;
    }
    delay(50);
  }
}