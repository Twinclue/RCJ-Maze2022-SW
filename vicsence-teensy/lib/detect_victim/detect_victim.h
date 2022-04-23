#ifndef DETECT_VICTIM_
#define DETECT_VICTIM_

#include <Arduino.h>
#include "read_camera.h"
#include "read_temperature.h"

class detect_victim{
    public:
        detect_victim(uint8_t _lowR,uint8_t _highR , uint8_t _lowL, uint8_t _highL , TwoWire *_busR, TwoWire *_busL);
        int8_t kitNumOneSide(bool rightSide);
        int8_t isHeatedVictim(bool rightSide);
        //int8_t[2] kitNumBothSide(); 
    private:
        const float temp_th = 28.0;
        const bool camFirst = true;
        TwoWire* busR, *busL;
        read_camera* camR;
        read_camera* camL;
        read_temperature* d6tR;
        read_temperature* d6tL;
};
#endif