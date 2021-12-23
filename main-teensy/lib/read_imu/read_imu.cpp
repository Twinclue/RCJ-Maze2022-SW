#include "read_imu.h"

read_imu::read_imu(){
}

bool read_imu::begin(){
    Wire2.begin();
    this->initialize();
    filter->begin(rate);
}
void read_imu::update(){
    if((micros() - premicros >= (1000000 / rate))){
        //Serial.println(micros()-premicros);
        this->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        filter->updateIMU(gx / 131.0, gy / 131.0, gz / 131.0, ax / 16384.0, ay / 16384.0, az / 16384.0);
        premicros = micros() + (1000000 / rate);
    }
}

float read_imu::getFPitch(){
    if((prePitch - filter->getPitch()) <= -180){
        over0countPitch++;
    }
    else if((prePitch - filter->getPitch()) >= 300){
        over0countPitch--;
    }

    if(over0countPitch != 0){
        prePitch = filter->getPitch();
        return -((360*over0countPitch) - filter->getPitch());
    }
    else{
        prePitch = filter->getPitch();
        return filter->getPitch();
    }
}

float read_imu::getFRoll(){
    return filter->getRoll();
}

float read_imu::getFYaw(){
    if((preYaw - filter->getYaw()) <= -300){
        over0countYaw++;
    }
    else if((preYaw - filter->getYaw()) >= 300){
        over0countYaw--;
    }

    if(over0countYaw != 0){
        preYaw = filter->getYaw();
        return -((360*over0countYaw) - filter->getYaw());
    }
    else{
        preYaw = filter->getYaw();
        return filter->getYaw();
    }
}