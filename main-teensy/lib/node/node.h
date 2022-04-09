#ifndef NODE_H_
#define NODE_H_

#include <Arduino.h>
#include "read_light.h"

const uint8_t front = 1;
const uint8_t left = 2;
const uint8_t back = 3;
const uint8_t right = 0;
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
    uint8_t tile = 0;// returns 0 on W,1 on B, 2 on S, 3 on slope
    unsigned short conn[4];//connected to //North West South East
};

typedef struct _block block;

class node{
    public:
        node(/* args */);
        void updatePosition(uint8_t moveto);
        void updateRotation(uint8_t moveto);
        uint8_t getMinCountDir();
        void searchAroundNodes(bool fWall,bool lWall,bool bWall,bool rWall);
        void setTile(uint8_t color);
        void setTile(uint8_t color,uint8_t dir);
        coordinate getNowCoor(){return now.p;}
        uint8_t getTile(uint8_t dir);
        void lackOfProgress();
        short getTempNodeNum(uint8_t dir){return tempNode[dir];}
        block getNowNode(){return now;}
        uint16_t getNowNodeNum(){return nowNodeNum;}
        short getRelatedNodeCount(uint8_t dir){return nodes[tempNode[dir]].count;}
    private:
        uint16_t makeNewNode(coordinate _p,uint8_t side);
        short searchNode(coordinate _p);
        short checkNodeCount(coordinate _p);
        coordinate convRXYZtoCoorAddLengh(uint8_t r,short x,short y,short z,short lengh = 1);
        uint8_t kagome(uint8_t _r,uint8_t _side);//ロボットが_rをむいているとき,_sideはNESWで言うとどっちか
        uint8_t convRtoArrnum(uint8_t _r);
        uint8_t rotateToRight(uint8_t _r);
        uint8_t rotateToLeft(uint8_t _r);
        uint8_t reverseR(uint8_t _r);
        block nodes[nodeNum];
        block now;
        uint8_t rotate = 1;
        uint16_t nowNodeNum = 0;
        uint16_t lastNodeNum = 0;
        uint16_t lastCheckPoint = 0;
        short tempNode[4];
};




#endif