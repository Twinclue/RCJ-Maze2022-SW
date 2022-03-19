#include "read_BNO055.h"

read_BNO055::read_BNO055(){
    setThrottleTime(GY521_THROTTLE_TIME);
}

bool read_BNO055::begin(TwoWire *_bus){
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

float read_BNO055::getGPitch(){
    tP = micros();
    deltatP = (tP - pretP) * 0.000001;
    pretP = tP;
    this->read();
    cangleP += this->getGyroY() * deltatP;
    angleP = (preangleP * 0.90) + (cangleP * 0.1);
    preangleP = angleP;
    return angleP;
}

float read_BNO055::getGYaw(){
    tY = micros();
    deltatY = (tY - pretY) * 0.000001;
    pretY = tY;
    this->read();
    cangleY += this->getGyroZ() * deltatY;
    angleY = (preangleY * 0.90) + (cangleY * 0.1);
    preangleY = angleY;
    return angleY;
}