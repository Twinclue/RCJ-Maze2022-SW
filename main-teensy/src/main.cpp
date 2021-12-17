#include <Arduino.h>
#include "move_robot.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"

read_imu imu;
read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor leftM(35,36,37,15,16);
drive_motor rightM(38,39,14,15,16);

Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);

read_light light(&npix);

move_robot move(&leftM,&rightM,&toff,&tofb,&imu,&light);

const byte lTouch = 21;
const byte rTouch = 17;

char buff[30];
int i=0;
void setup()
{
  Serial.begin(19200);
  Wire2.begin();
  Wire.begin();
  imu.begin(&Wire2);
  pinMode(rTouch,INPUT);
  pinMode(lTouch,INPUT);
  delay(100);
}

void loop()
{

  if (Serial.available() > 0) {
    buff[i] = Serial.read();
    if (buff[i] == '\n') {
      buff[i] = '\0';
      rightM.on(atoi(strtok(buff, ":")));
      leftM.on(atoi(strtok(buff, ":")));
      if(atoi(strtok(buff,":"))){
        move.drop(true);
      }
      else if(atoi(strtok(NULL,":"))){
        move.drop(false);
      }
      i = 0;
    } else {
      i += 1;
    }
  }


  imu.read();
  for(int i=0;i<5;i++){// fls : flf : frf : frs : fc
    Serial.print(toff.read(i));
    Serial.print(":");
  }

  for(int i=0;i<5;i++){// brs : brf : bl : bls : bc
    Serial.print(tofb.read(i));
    Serial.print(":");
  }

  Serial.print(imu.getRoll());//Roll : Pitch : Yaw
  Serial.print(":");
  Serial.print(imu.getPitch());
  Serial.print(":");
  Serial.print(imu.getYaw());
  Serial.print(":");
  Serial.print(light.read());
  Serial.print(":");
  Serial.print(light.getFloorColor());
  Serial.print(":");
  Serial.print(digitalRead(rTouch));
  Serial.print(":");
  Serial.print(digitalRead(lTouch));
  Serial.print(":");

  Serial.println();

  delay(50);
}

