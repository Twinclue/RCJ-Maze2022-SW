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

coordinate node::convRXYZtoCoorAddLengh(uint8_t r,short x,short y,short z,short lengh = 1){
    coordinate temp;
    switch (r)
    {
    case 1:
        temp.x = x;
        temp.y = y + lengh;
        temp.z = z;
        break;
    case 2:
        temp.x = x - lengh;
        temp.y = y;
        temp.z = z;
        break;
    case 4:
        temp.x = x;
        temp.y = y - lengh;
        temp.z = z;
        break;
    case 8:
        temp.x = x + lengh;
        temp.y = y;
        temp.z = z;
        break;
    default:
        break;
    return temp;
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