#ifndef DETECT_VICTIM_
#define DETECT_VICTIM_

#include <Arduino.h>
#include "read_camera.h"
#include "read_temperature.h"

class detect_victim{
    public:
        detect_victim(HardwareSerial *_serialR, HardwareSerial *_serialL, TwoWire *_busR, TwoWire *_busL);
        int8_t kitNumOneSide(bool rightSide);
        int8_t isHeatedVictim(bool rightSide);
        //int8_t kitNumOneSideMode(bool rightSide);
        //int8_t[2] kitNumBothSide(); 
    private:
        const float temp_th = 25.0;
        const bool camFirst = true;
        HardwareSerial* serialR, *serialL;
        TwoWire* busR, *busL;
        read_camera* camR;
        read_camera* camL;
        read_temperature* d6tR;
        read_temperature* d6tL;
};
#endif