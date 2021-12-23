#include "read_temperature.h"

read_temperature::read_temperature(TwoWire* _bus){
    bus=_bus;
    sensor->setBus(bus);
}
float read_temperature:: temp(){
    return sensor->getTEMP();
}