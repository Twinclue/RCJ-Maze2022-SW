#include <Arduino.h>
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"
#include "move_robot.h"

read_tof toff(&Wire2);
read_tof tofb(&Wire);

drive_motor left(35,36,37,15,16);
drive_motor right(38,39,14,15,16);
move_robot move(&left,&right,&toff,&tofb);


void setup()
{
  delay(100);
}

void loop()
{
  move.fwd();
}