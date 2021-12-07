#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "drive_motor.h"
#include "read_light.h"

Adafruit_NeoPixel npix = Adafruit_NeoPixel(2,26, NEO_GRB + NEO_KHZ800);
read_light light(&npix);

void setup()
{

}

void loop()
{
  Serial.println(light.read());
  delay(100);
}