#include <Arduino.h>
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"

drive_motor left(35,36,37,15,16);
drive_motor right(38,39,14,15,16);

void setup()
{
  delay(100);
}

void loop()
{
  left.on(0);
  right.on(0);
}