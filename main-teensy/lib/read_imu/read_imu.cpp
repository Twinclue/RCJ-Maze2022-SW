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