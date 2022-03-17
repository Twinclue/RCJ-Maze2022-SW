#include "read_BNO055.h"
read_BNO055::read_BNO055(int32_t _sensorID, uint8_t _address, TwoWire *_bus, LiquidCrystal *_disp){
    sensorID=_sensorID;
    address=_address;
    bus=_bus;
    disp=_disp;
    bno = new Adafruit_BNO055(sensorID, address, bus);
    i2c_dev = new Adafruit_I2CDevice(address, bus);
}
bool read_BNO055::begin(){
    int begun=bno->begin();
    delay(1000);
    bno->setExtCrystalUse(true);
    bno->setAxisRemap(bno->REMAP_CONFIG_P3);
    //bno->setMode(bno->OPERATION_MODE_NDOF); //test if it stops calibrations after this
    return(begun);
}
bool read_BNO055::getCalib_EEPROM(adafruit_bno055_offsets_t *calib){
    int eeAddress = 0;
    long bnoID;
    EEPROM.get(eeAddress, bnoID);
    bno->getSensor(&sensor);
    if(bnoID!=sensor.sensor_id){
        return false;
    }else{
        eeAddress += sizeof(long);  //length of bnoID 
        EEPROM.get(eeAddress, calibData);
        *calib=calibData;
        return true;
    }
}
bool read_BNO055::storeCalib_EEPROM(adafruit_bno055_offsets_t calib){
    int eeAddress = 0;
    long bnoID;
    bno->getSensor(&sensor);
    bnoID=sensor.sensor_id;
    EEPROM.put(eeAddress, bnoID);
    eeAddress += sizeof(long);  //length of bnoID 
    EEPROM.put(eeAddress, calib);
    // if possible, get data from EEPROM and compare with calib
    return true;
}
bool read_BNO055::calibAll(bool igEEPROM=0){ //select if ignore EEPROM, default 0
    isCalibrated=false;
    if(firstCalib){
        firstCalib=false;
    }else if(!resetbyWire()){
        return false;
    }
    if(this->getCalib_EEPROM(&calibData)&&!igEEPROM){
        bno->setSensorOffsets(calibData);
        bno->setMode(DEFAULT_MODE);
        return true;
    }else{
        uint8_t system, gyro, accel, mag;
        system = gyro = accel = mag = 0;
        bno->getEvent(&event);
        while(!isCalibrated){
            bno->getCalibration(&system, &gyro, &accel, &mag);
            this->displayCalStatus();
            disp->clear();
            disp->print("Sys:Gyr:Acc:Mag:");    
            disp->setCursor(0,1);   disp->print(system);
            disp->setCursor(4,1);   disp->print(gyro);
            disp->setCursor(8,1);   disp->print(accel);
            disp->setCursor(12,1);  disp->print(mag);
            if(system==3&&gyro==3&&accel==3&&mag==3){
                isCalibrated=true;
                bno->getSensorOffsets(calibData);
                this->storeCalib_EEPROM(calibData);
                bno->setMode(DEFAULT_MODE);
            }
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }
        return true;
    }
}

//manually calibrate
bool read_BNO055::gyroCalib(void){
    isCalibrated=false;
    disp->clear();
    disp->print("KEEP STABLE");
    if(!resetbyWire()){
        return false;
    }
    uint8_t gyro=0, other=0;
    adafruit_bno055_offsets_t newCalib=calibData;   //for comparison
    bno->getEvent(&event);
    while(!isCalibrated){
        bno->getCalibration(&other, &gyro, &other, &other);
        disp->setCursor(0,1);   disp->print("Gyro:");   disp->setCursor(5,1);   disp->print(gyro);
        if(gyro==3){
            isCalibrated=true;
            bno->getSensorOffsets(newCalib);
            calibData.gyro_offset_x=newCalib.gyro_offset_x;
            calibData.gyro_offset_y=newCalib.gyro_offset_y;
            calibData.gyro_offset_z=newCalib.gyro_offset_z;
            this->storeCalib_EEPROM(calibData);
            bno->setMode(DEFAULT_MODE);
        }
        delay(BNO055_SAMPLERATE_DELAY_MS);
    }
    return true;
}
bool read_BNO055::accCalib(void){
    isCalibrated=false;
    disp->clear();  disp->print("PLACE in 6 pos");  //autoscroll may be better
    if(!resetbyWire()){
        return false;
    }
    uint8_t acc=0;
    adafruit_bno055_offsets_t newCalib=calibData;   //for comparison
    while(!isCalibrated){
        bno->getCalibration(NULL, NULL, &acc, NULL);

        disp->setCursor(0,1);   disp->print("Acc:");   disp->setCursor(4,1);   disp->print(acc);
        if(acc==3){
            isCalibrated=true;
            bno->getSensorOffsets(newCalib);
            /*reflesh only accel*/
            calibData.accel_offset_x=newCalib.accel_offset_x;
            calibData.accel_offset_y=newCalib.accel_offset_y;
            calibData.accel_offset_z=newCalib.accel_offset_z;
            calibData.accel_radius=newCalib.accel_radius;
            this->storeCalib_EEPROM(calibData);
            bno->setMode(DEFAULT_MODE);
        }
        delay(BNO055_SAMPLERATE_DELAY_MS);
    }
    return true;
}
bool read_BNO055::magCalib(void){
    isCalibrated=false;
    disp->clear();  disp->print("SLOWLY ROTATE");  
    if(!resetbyWire()){
        return false;
    }
    //bno->setMode(bno->OPERATION_MODE_CONFIG);   //this seems to have stopped calibration 
    uint8_t mag=0;
    adafruit_bno055_offsets_t newCalib=calibData;   //for comparison
    while(!isCalibrated){
        bno->getCalibration(NULL, NULL, NULL, &mag);
        disp->setCursor(0,1);   disp->print("Mag:");   disp->setCursor(4,1);   disp->print(mag);
        if(mag==3){
            isCalibrated=true;
            bno->getSensorOffsets(newCalib);
            /*reflese only accel*/
            calibData.mag_offset_x=newCalib.mag_offset_x;
            calibData.mag_offset_y=newCalib.mag_offset_y;
            calibData.mag_offset_z=newCalib.mag_offset_z;
            calibData.mag_radius=newCalib.mag_radius;
            this->storeCalib_EEPROM(calibData);
            bno->setMode(DEFAULT_MODE);
        }
        delay(BNO055_SAMPLERATE_DELAY_MS);
    }
    return true;
}

int read_BNO055::getYaw(void){
    imu::Quaternion quat = bno->getQuat(); 
    imu::Vector<3> euler= quat.toEuler();
    Yaw = euler.x()*180.0/M_PI; //Yaw in degree,
    /*convert to data specialized to limited usage*/
    Yaw = -Yaw;
    if(Yaw*prevYaw<=0.0 && abs(Yaw)>90.0){
      if(Yaw<0.0){              //0 degree
        dYaw=360.0-abs(prevYaw-Yaw);
      }else{                    //180 degree
        dYaw=abs(Yaw-prevYaw)-360.0;
      }
    }else{
      dYaw=Yaw-prevYaw;
    }
    YawContinuous+=dYaw;
    prevYaw=Yaw;

    return int(YawContinuous);
}
int read_BNO055::getPitch(void){
    imu::Quaternion quat = bno->getQuat(); 
    imu::Vector<3> euler= quat.toEuler();
    Pitch=euler.z()*180.0/M_PI; //Pitch in degree
    if(Pitch<0){
        Pitch+=180.0;
    }else{
        Pitch-=180.0;
    }
    return int(Pitch);
}

void read_BNO055::displayCalStatus(void)
{
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0 ;
    bno->getCalibration(&system, &gyro, &accel, &mag);
    /* The data should be ignored until the system calibration is > 0 */
    Serial.print("\t");
    if (!system)
    {
        Serial.print("! ");
    }
    /* Display the individual values */
    Serial.print("Sys:");
    Serial.print(system, DEC);
    Serial.print(" G:");
    Serial.print(gyro, DEC);
    Serial.print(" A:");
    Serial.print(accel, DEC);
    Serial.print(" M:");
    Serial.print(mag, DEC);
    Serial.println("");
}

/* this function is inspired from Adafruit_BNO055::setExtCrystalUse function */
bool read_BNO055::resetbyWire(void){
    bool success;
    bno->setMode(bno->OPERATION_MODE_CONFIG);
    delay(20);
    success=this->write8(bno->BNO055_PAGE_ID_ADDR, 0)           //choose page 0 of the register map
            &&this->write8(bno->BNO055_SYS_TRIGGER_ADDR, 0X20); //RST_SYS(@bit 5)
    delay(1000);
    bno->begin();   //should do begin after reset
    return success;
}

bool read_BNO055::write8(Adafruit_BNO055::adafruit_bno055_reg_t reg, byte value){
    uint8_t buffer[2] = {(uint8_t)reg, (uint8_t)value};
    return i2c_dev->write(buffer, 2);
}

bool read_BNO055::resetbyPulse(){
    digitalWrite(bnoRSTpin, LOW);
    delayMicroseconds(2);
    digitalWrite(bnoRSTpin, HIGH);
    return true;
}