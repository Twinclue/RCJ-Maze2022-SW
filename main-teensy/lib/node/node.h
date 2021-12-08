#ifndef NODE_H_
#define NODE_H_

#include <Arduino.h>

const uint8_t front = 0;
const uint8_t left = 1;
const uint8_t back = 2;
const uint8_t right = 3;
const uint8_t nodeNum = 128;

struct _coordinate{
    short x;
    short y;
    short z;
};
typedef struct _coordinate coordinate;

struct _block{
    coordinate p;
    short count = -1;
    uint8_t tile = 0;
    unsigned short conn[4];//connected to
};

typedef struct _block block;

class node{
    public:
        node(/* args */);
        void updatePosition(uint8_t moveto);
        coordinate convRXYZ_coor_lengh(uint8_t r,short x,short y,short z);
        uint8_t rotateToRight(uint8_t _r);
        uint8_t rotateToLeft(uint8_t _r);
    private:
        short searchNode(coordinate _p);
        block nodes[nodeNum];
        block now;
        uint8_t rotate = 1;
        uint16_t nowNodeNum = 0;

};




#endif