#include "node.h"

node::node(){
}

void node::updatePosition(uint8_t moveto){
    short tempNodeNum;
    switch (moveto){
    case front:
        break;

    case left:
        
        break;

    case back:
        
        break;

    case right:
        
        break;

    default:
        break;
    }
}

short node::searchNode(coordinate _p){
    for(int i = 0;i<nodeNum;i++){
        if((nodes[i].x == _p.x) && (nodes[i].y == _p.y) && (nodes[i].z == _p.z)){
            return i;
        }
    }
    return -1;
}

coordinate node::convRXYZtoCoorAddLengh(uint8_t r,short x,short y,short z,short lengh){
    switch (r)
    {
    case 1:
        break;
    case 2:
        break;
    case 4:
        break;
    case 8:
        break;
    default:
        break;
    }
}

uint8_t node::rotateToRight(uint8_t _r){
    if(_r==1){
        return 8;
    }
    else{
        return _r>>1;
    }
}

uint8_t node::rotateToLeft(uint8_t _r){
    if(_r==8){
        return 1;
    }
    else{
        return _r<<1;
    }
}