#include <Arduino.h>
#include <Encoder.h>
#include <LiquidCrystal.h>
#include <PCA9547.h>
#include <VL53L0X.h>
#include "move_robot.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"
#include "D6Tarduino.h"
#include "detect_victim.h"

#include "read_camera.h"
#include "read_temperature.h"

Encoder enc(8,7);

// read_tof toff(&Wire2);
// read_tof tofb(&Wire);
PCA9547 multiplex;
VL53L0X tof;

// drive_motor leftM(35,36,37,15,16);
// drive_motor rightM(39,38,14,15,16);

// read_imu imu;
Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);
read_light light(&npix);

LiquidCrystal lcd(25,24,12,11,10,9);
// detect_wall wall(&toff,&tofb);

// move_robot move(&leftM,&rightM,&toff,&tofb,&imu,&light,&lcd,&npix);
// node n;
detect_victim victim(&Serial4, &Serial5, &Wire2, &Wire); 
// coordinate debug;

// solver solver(&imu,&light,&move,&wall,&n);

D6Tarduino rightTempRaw;
D6Tarduino leftTempRaw;

read_camera cam(&Serial4);
// read_temperature temp(&Wire2);

const byte lTouch = 21;
const byte rTouch = 17;

char buff[30];
int i=0;
void setup()
{
  pinMode(27, INPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(1, OUTPUT);
  Serial.begin(19200);
  Wire2.begin();
  Wire.begin();

  TwoWire *tof_bus=&Wire;
  multiplex.attatch(*tof_bus);
  tof_bus->begin();
  tof.setBus(tof_bus);
  multiplex.enable(4);
  tof.init();
  tof.setTimeout(1000);
  tof.startContinuous(10);
  multiplex.disable();

  // imu.begin(&Wire2);
  lcd.begin(16,2);
  // rightTempRaw.setBus(&Wire2);
  // leftTempRaw.setBus(&Wire);
  pinMode(rTouch,INPUT);
  pinMode(lTouch,INPUT);

  digitalWrite(5, HIGH);
  delay(100);

}

void loop()
{
  if(digitalRead(27) == true){
    // debug = n.getNowCoor();
    // lcd.clear();
    // lcd.home();
    // lcd.print(debug.x);
    // lcd.print(":");
    // lcd.print(debug.y);
    // Serial.print("X:");
    // Serial.println(debug.x);
    // Serial.print("Y:");
    // Serial.println(debug.y);
    // solver.EXrightHand();
    digitalWrite(6, HIGH);
    lcd.clear();  lcd.print("off");
    delay(500);
  }else{
      // imu.read();
      // lcd.home();
      lcd.clear();
      int encoder_value=enc.read();
      encoder_value*=0.25;
      if(encoder_value==1){
        lcd.print("Press to");
        lcd.setCursor(0,1);
        lcd.print("turn to      <==");
        // if(digitalRead(0)){
        //   move.turn();
        // }
      }
      if(encoder_value==2){
        lcd.print("Press to      ^");
        lcd.setCursor(0,1);
        lcd.print("move to       |");
        // if(digitalRead(0)){
        //   move.rev();
        // }
      }
      if(encoder_value==3){
        lcd.print("Press to");
        lcd.setCursor(0,1);
        lcd.print("turn to      ==>");
        // if(digitalRead(0)){
        //   move.turn(-90);
        // }
      }
      if(encoder_value==4){
        lcd.print("Press to      |");
        lcd.setCursor(0,1);
        lcd.print("move to       V");
        // if(digitalRead(0)){
        //   move.fwd();
        // }
      }
      if(encoder_value==5){
        lcd.print("Press to drop a");
        lcd.setCursor(0,1);
        lcd.print("RescueKit to ==>");
        // if(digitalRead(0)){
        //   move.drop(true);
        // }
      }
      if(encoder_value==6){
        lcd.print("Press to drop a");
        lcd.setCursor(0,1);
        lcd.print("RescueKit to <==");
        // if(digitalRead(0)){
        //   move.drop(false);
        // }
      }
      if(encoder_value==7){
        lcd.print("<== Sensor Check");
        lcd.setCursor(0,1);
        lcd.print("Action Test ==>");
      }
      if(encoder_value==8){
        lcd.print("Light Raw: ");
        lcd.print(light.read());
        lcd.setCursor(0,1);
        lcd.print("Light Col: ");
        switch (light.getFloorColor())
        {
        case 0:
        lcd.print("WHT");
          break;
        case 1:
          lcd.print("BLK");
          break;
        default:
          lcd.print("SIL");
        }
      }
      if(encoder_value==9){
        // lcd.print(toff.read(3));
        // lcd.setCursor(6,0);
        // lcd.print("Front");
        // lcd.setCursor(13,0);
        // lcd.print(toff.read(0));

        // lcd.setCursor(0,1);
        // lcd.print(toff.read(2));
        // lcd.setCursor(6,1);
        // lcd.print(toff.read(4));
        // lcd.setCursor(13,1);
        // lcd.print(toff.read(1));
        multiplex.enable(4);
        lcd.print(tof.readRangeContinuousMillimeters());
        digitalWrite(1, !tof.timeoutOccurred());
        multiplex.disable();
        delay(450);
      }
      if(encoder_value==10){
        // lcd.print(tofb.read(1));
        // lcd.setCursor(6,0);
        // lcd.print(tofb.read(4));
        // lcd.setCursor(13,0);
        // lcd.print(tofb.read(2));

        // lcd.setCursor(0,1);
        // lcd.print(tofb.read(0));
        // lcd.setCursor(6,1);
        // lcd.print("Back");
        // lcd.setCursor(13,1);
        // lcd.print(tofb.read(3));
        // delay(450);
      }
      if(encoder_value==11){
        lcd.print("Touch");
        lcd.setCursor(0,1);
        lcd.print((digitalRead(rTouch)?"True":"False"));
        lcd.setCursor(11,1);
        lcd.print((digitalRead(lTouch)?"True":"False"));
      }
      if(encoder_value==12){
        lcd.print("Pitch: ");
        // lcd.print(imu.getGPitch());
        lcd.setCursor(0,1);
        lcd.print("Yaw  : ");
        // lcd.print(imu.getGYaw());
      }
      if(encoder_value==13){
        lcd.print("Right TEMP: ");
        lcd.print(rightTempRaw.getTEMP());
        // lcd.setCursor(0,1);
        // lcd.print("Left  TEMP: ");
        // lcd.print(leftTempRaw.getTEMP());
      }
      if(encoder_value==14){
       lcd.print("Right PTAT: ");
        lcd.print(rightTempRaw.getPTAT());
        // lcd.setCursor(0,1);
        // lcd.print("Left  PTAT: ");
        // lcd.print(leftTempRaw.getPTAT());
      }
      if(encoder_value==15){
        lcd.print("Right KitNum: ");
        lcd.print(victim.kitNumOneSide(true));
        // lcd.setCursor(0,1);
        // lcd.print("Left  KitNum: ");
        // lcd.print(victim.kitNumOneSide(false));
      }
      if(encoder_value<1||encoder_value>15){
        lcd.print(enc.read());
      }
      digitalWrite(2, HIGH);
      delay(50);
    }
  
}
