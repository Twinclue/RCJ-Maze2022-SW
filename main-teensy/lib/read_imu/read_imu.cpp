#include "read_imu.h"

read_imu::read_imu(){
    setThrottleTime(GY521_THROTTLE_TIME);
}

bool read_imu::begin(TwoWire *_bus){
    setBus(_bus);
    bus->begin();
    wakeup();
    setAccelSensitivity(2);  // 8g
    setGyroSensitivity(1);   // 500 degrees/s
    setThrottle();
    axe = caxe;
    aye = caye;
    aze = caze;
    gxe = cgxe;
    gye = cgye;
    gze = cgze;
    return isConnected();
}

float read_imu::getGPitch(){
    t = micros();
    deltat = (t - pret) * 0.000001;
    pret = t;
    this->read();
    cangle += this->getGyroY() * deltat;
    angle = (preangle * 0.90) + (cangle * 0.1);
    preangle = angle;
    return angle;
}

float read_imu::getGYaw(){
    t = micros();
    deltat = (t - pret) * 0.000001;
    pret = t;
    this->read();
    cangle += this->getGyroZ() * deltat;
    angle = (preangle * 0.90) + (cangle * 0.1);
    preangle = angle;
    return angle;
}