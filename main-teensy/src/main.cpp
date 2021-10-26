#include <Arduino.h>
#include <PCA9547.h>
#include <VL53L0X.h>
#include <VL6180X.h>
#include "read_imu.h"

read_imu sensor;
PCA9547 i2cSelect;
VL6180X vl61a;

const int port = 0;

void setup()
{
  Serial.begin(115200);
  i2cSelect.attatch(Wire2);
  Wire2.begin();
  vl61a.setBus(&Wire2);
  i2cSelect.enable(port);
  vl61a.init();
  vl61a.configureDefault();
  vl61a.setScaling(1);
  vl61a.setTimeout(500);
  i2cSelect.disable();
  
  delay(100);
}

void loop()
{
  i2cSelect.enable(port);
  int dist = vl61a.readRangeSingleMillimeters();
  if (vl61a.timeoutOccurred()){
      Serial.println("failed!");
  }
  i2cSelect.disable();
  Serial.println(dist);
  delay(100);
}