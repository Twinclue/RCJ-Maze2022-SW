#ifndef READ_TEMPERATURE_H_
#define READ_TEMPERATURE_H_

#include <Arduino.h>
#include "D6Tarduino.h"

class read_temperature{
    public:
    read_temperature(TwoWire* _bus);
    float temp();

    private:
    TwoWire* bus;
    D6Tarduino* sensor = new D6Tarduino();

};
#endif