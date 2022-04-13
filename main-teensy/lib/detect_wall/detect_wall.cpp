#include "detect_wall.h"

detect_wall::detect_wall(read_tof *_toff,read_tof *_tofb){
    toff = _toff;
    tofb = _tofb;
}

bool detect_wall::getSingleWall(uint8_t dir){
    switch (dir)
    {
    case front://front
        if(toff->read(fc) < wallThreshold){
            return true;
        }
        break;
    case left://left
        if(toff->read(fls) < wallThreshold && tofb->read(bls) < wallThreshold){
            return true;
        }
        break;
    case back://back
        if(tofb->read(bc) < wallThreshold){
            return true;
        }
        break;
    case right://right
        if(toff->read(frs) < wallThreshold && tofb->read(brs) < wallThreshold){
            return true;
        }
        break;
    default:
        return true;
        break;
    }
    return false;
}