#include <Arduino.h>
#include "GY521.h"

GY521 sensor(0x68);

uint32_t counter = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  sensor.setBus(&Wire2);
  Wire2.begin();

  delay(100);
  while (sensor.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
  sensor.setAccelSensitivity(2);  // 8g
  sensor.setGyroSensitivity(1);   // 500 degrees/s

  sensor.setThrottle();
  Serial.println("start...");
  
  // set callibration values from calibration sketch.
  sensor.axe = -0.071;
  sensor.aye = -0.008;
  sensor.aze = 1.085;
  sensor.gxe = -0.709;
  sensor.gye = 0.479;
  sensor.gze = 0.087;
}

void loop()
{
  sensor.read();
  
  float pitch = sensor.getPitch();
  float roll  = sensor.getRoll();
  float yaw   = sensor.getYaw();
  if (counter % 10 == 0)
  {
    Serial.println("\nCNT\tPITCH\tROLL\tYAW");
  }

  Serial.print(counter);
  Serial.print('\t');
  Serial.print(pitch, 3);
  Serial.print('\t');
  Serial.print(roll, 3);
  Serial.print('\t');
  Serial.print(yaw, 3);
  Serial.println();
  
 /*
  Serial.print(sensor.getGyroX());Serial.print(" , ");
  Serial.print(sensor.getGyroX());Serial.print(" , ");
  Serial.println(sensor.getGyroX());
  */
  delay(200);
  counter++;
}