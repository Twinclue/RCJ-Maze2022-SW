#include "read_tof.h"

read_tof::read_tof(TwoWire *_bus){
    bus = _bus;

    i2cSelect.attatch(*bus);
    bus->begin();
    for(int i=0;i < VL61NUM;i++){
        vl61[i].setBus(bus);
        i2cSelect.enable(i);
        vl61[i].init();
        vl61[i].configureDefault();
        vl61[i].setScaling(1);
        vl61[i].setTimeout(500);
        i2cSelect.disable();
    }
    for(int i = 0;i<VL53NUM;i++){
        vl53[i].setBus(bus);
        i2cSelect.enable(i+VL61NUM);
        vl53[i].init();
        vl53[i].setTimeout(1000);
        //vl53[i].setMeasurementTimingBudget(20000);
        vl53[i].startContinuous(10);
        i2cSelect.disable();
    }
}

int read_tof::read(uint8_t _direction){
    i2cSelect.enable(_direction);
    if(_direction>=VL61NUM){
        dist = vl53[_direction-VL61NUM].readRangeContinuousMillimeters();
        if(vl53[_direction-VL61NUM].timeoutOccurred()){
            return -1;
        }
    }
    else{
        dist = vl61[_direction].readRangeSingleMillimeters();
        if (vl61[_direction].timeoutOccurred()){
             return -1;
        }
    }
    i2cSelect.disable();
    return dist;
}