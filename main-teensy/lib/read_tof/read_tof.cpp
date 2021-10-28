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
        i2cSelect.enable(i);
        vl53[i].setTimeout(500);
        vl53[i].init();
        vl53[i].setMeasurementTimingBudget(20000);
        i2cSelect.disable();
    }
}