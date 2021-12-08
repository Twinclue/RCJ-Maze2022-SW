#ifndef DETECT_WALL_H_
#define DETECT_WALL_H_
#include "read_tof.h"
#include <Arduino.h>

class detect_wall{
    public:
        detect_wall(read_tof *_front,read_tof *_back);
        bool getSingleWall(uint8_t dir);
    private:
        read_tof *front;
        read_tof *back;

        const byte fls = 0;
        const byte flf = 1;
        const byte frf = 2;
        const byte frs = 3;
        const byte fc  = 4;

        const byte brs = 0;
        const byte brf = 1;
        const byte blf = 2;
        const byte bls = 3;
        const byte bc  = 4;
        
        const uint16_t wallThreshold = 150;
};

#endif