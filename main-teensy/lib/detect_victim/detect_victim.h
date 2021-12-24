#ifndef DETECT_VICTIM_
#define DETECT_VICTIM_

#include <Arduino.h>
#include "read_camera.h"
#include "read_temperature.h"

class detect_victim{
    public:
        detect_victim(HardwareSerial *_serialR, HardwareSerial *_serialL, TwoWire *_busR, TwoWire *_busL);
        int8_t kitNumOneSide(bool rightSide);
        int8_t[2] kitNumBothSide(); 
        bool isHeatedVictim();
    private:
        float temp_th = 28.0;
        HardwareSerial* serialR, serialL;
        TwoWire* busR, busL;
        read_camera* camR = new read_camera(serialR);
        read_camera* camL = new read_camera(serialL);
        read_temperature* d6tR = new read_temperature(busR);
        read_temperature* d6tL = new read_temperature(busL);
}
#endif