#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"

Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);

void setup()
{
  pinMode(22,INPUT);
  npix.begin();
  npix.setPixelColor(1,255,0,0);
  npix.setBrightness(255);
  npix.show();
  delay(100);
}

void loop()
{
  Serial.println(analogRead(22));
  npix.setPixelColor(0,255,255,255);
  npix.show();
  delay(500);
  npix.setPixelColor(0,0,0,0);
  npix.show();
  delay(500);
}