#include "detect_wall.h"

detect_wall::detect_wall(read_tof *_front,read_tof *_back){
    front = _front;
    back = _back;
}

bool detect_wall::getSingleWall(uint8_t dir){
    switch (dir)
    {
    case 0://front
        if(front->read(fc) < wallThreshold){
            return true;
        }
        break;
    case 1://left
        if(front->read(fls) < wallThreshold || back->read(bls) < wallThreshold){
            return true;
        }
        break;
    case 2://back
        if(back->read(bc) < wallThreshold){
            return true;
        }
        break;
    case 3://right
        if(front->read(frs) < wallThreshold || back->read(brs) < wallThreshold){
            return true;
        }
    default:
        return true;
        break;
    }
    return false;
}