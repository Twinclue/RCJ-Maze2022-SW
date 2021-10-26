#include <Arduino.h>
#include "read_imu.h"

read_imu sensor;

void setup()
{
  Serial.begin(115200);
  Wire2.begin();

  delay(100);
  sensor.begin(&Wire2);
}

void loop()
{
  sensor.read();
  float yaw   = sensor.getYaw();

  Serial.print(yaw, 3);
  Serial.println();
  delay(100);
}