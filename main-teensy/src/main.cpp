#include <Arduino.h>
#include <Encoder.h>
#include <LiquidCrystal.h>

#include "D6Tarduino.h"
#include "advanced_tof.h"
#include "detect_victim.h"
#include "drive_motor.h"
#include "move_robot.h"
#include "read_tof.h"
#include "solver.h"

Encoder enc(8, 7);

read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor leftM(35, 36, 37, 15, 16);
drive_motor rightM(39, 38, 14, 15, 16);

read_imu imu;
Adafruit_NeoPixel npix = Adafruit_NeoPixel(2, 26, NEO_GRB + NEO_KHZ800);
read_light light(&npix);

LiquidCrystal lcd(25, 24, 12, 11, 10, 9);
detect_wall wall(&toff, &tofb);
node n;
move_robot move(&leftM, &rightM, &toff, &tofb, &imu, &light, &lcd, &npix,&n);

detect_victim victim(&Serial4, &Serial5, &Wire2, &Wire);
coordinate debug;

solver solver(&imu, &light, &move, &wall, &n);

D6Tarduino rightTempRaw;
D6Tarduino leftTempRaw;

const byte lTouch = 21;
const byte rTouch = 17;

char buff[30];
int i = 0;
void setup() {
  Serial.begin(19200);
  Wire2.begin();
  Wire.begin();
  imu.begin(&Wire2);
  lcd.begin(16, 2);
  rightTempRaw.setBus(&Wire2);
  leftTempRaw.setBus(&Wire);
  pinMode(rTouch, INPUT);
  pinMode(lTouch, INPUT);
  pinMode(27,INPUT);
  pinMode(0,INPUT);
  delay(100);
}

void loop() {
  if (digitalRead(27) == HIGH) {
    debug = n.getNowCoor();
    lcd.clear();
    lcd.home();
    lcd.print(debug.x);
    lcd.print(":");
    lcd.print(debug.y);
    Serial.print("X:");
    Serial.println(debug.x);
    Serial.print("Y:");
    Serial.println(debug.y);
    solver.EXrightHand();
    delay(50);
  }else {
    imu.read();
    lcd.home();
    lcd.clear();

    switch (enc.read()) {
      case -28:
        enc.write(32);
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
        lcd.print((digitalRead(rTouch) ? "True" : "False"));
        lcd.setCursor(11, 1);
        lcd.print((digitalRead(lTouch) ? "True" : "False"));
        break;
      case 20:
        lcd.print("Pitch: ");
        lcd.print(imu.getGPitch());
        lcd.setCursor(0, 1);
        lcd.print("Yaw  : ");
        lcd.print(imu.getGYaw());
        break;
      case 24:
        lcd.print("Right TEMP: ");
        lcd.print(rightTempRaw.getTEMP());
        lcd.setCursor(0, 1);
        lcd.print("Left  TEMP: ");
        lcd.print(leftTempRaw.getTEMP());
        break;
      case 28:
        lcd.print("Right PTAT: ");
        lcd.print(rightTempRaw.getPTAT());
        lcd.setCursor(0, 1);
        lcd.print("Left  PTAT: ");
        lcd.print(leftTempRaw.getPTAT());
        break;
      case 32:
        lcd.print("Right KitNum: ");
        lcd.print(victim.kitNumOneSideMode(true));
        lcd.setCursor(0, 1);
        lcd.print("Left  KitNum: ");
        lcd.print(victim.kitNumOneSideMode(false));
        break;
      case 36:
        enc.write(-24);
      default:
        lcd.print(enc.read());
        break;
    }
    delay(50);
  }
}
