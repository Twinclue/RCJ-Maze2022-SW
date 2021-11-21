#include <Arduino.h>
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"

read_imu imu;
read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor left(35,36,37,15,16);
drive_motor right(38,39,14,15,16);



char buff[30];
int i=0;
void setup()
{
  Serial.begin(19200);
  Wire2.begin();
  Wire.begin();
  imu.begin(&Wire2);
  delay(100);
}

void loop()
{

  if (Serial.available() > 0) {
    buff[i] = Serial.read();
    if (buff[i] == '\n') {
      buff[i] = '\0';
      right.on(atoi(strtok(buff, ":")));
      left.on(atoi(strtok(NULL, ":")));
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

  Serial.println();

  delay(50);
}

