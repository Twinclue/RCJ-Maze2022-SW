#ifndef _READ_BNO055_
#define _READ_BNO055_
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "vector.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_I2CDevice.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

class read_BNO055{
    public:
    read_BNO055(int32_t _sensorID, uint8_t _address, TwoWire *_bus, LiquidCrystal *_disp); //constructor
    bool begin(); 
    bool calibAll(bool useEEPROM); //return if calibration is done
    bool getCalib_EEPROM(adafruit_bno055_offsets_t *calib);
    bool storeCalib_EEPROM(adafruit_bno055_offsets_t calib);

    bool resetbyWire();
    bool resetbyPulse();    //use when resetbyWire doesn't work

    bool gyroCalib(void);
    bool accCalib(void);
    bool magCalib(void);
    void displayCalStatus(void);

    int getYaw(void);
    int getPitch(void);

    private:
    bool write8(Adafruit_BNO055::adafruit_bno055_reg_t reg, byte value);

    double Yaw, YawContinuous, prevYaw, dYaw, Pitch;
    bool isCalibrated=false, firstCalib=true;
    int32_t sensorID;
    uint8_t address;
    uint8_t bnoRSTpin=40;   //temporarily set to a unused pin
    sensor_t sensor;
    sensors_event_t event;
    adafruit_bno055_offsets_t calibData;
    const Adafruit_BNO055::adafruit_bno055_opmode_t DEFAULT_MODE = bno->OPERATION_MODE_NDOF;

    TwoWire *bus;
    LiquidCrystal *disp;
    Adafruit_BNO055 *bno; 
    Adafruit_I2CDevice *i2c_dev;
};
#endif