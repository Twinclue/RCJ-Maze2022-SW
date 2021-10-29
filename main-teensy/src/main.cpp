#include <Arduino.h>
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"

read_imu imu;
read_tof toff(&Wire2);
read_tof tofb(&Wire);

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