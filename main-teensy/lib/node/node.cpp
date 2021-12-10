#include "node.h"

node::node(){
}

void node::updatePosition(uint8_t moveto){
    short tempNodeNum;
    switch (moveto){
    case front:
        break;

    case left:
        rotate = rotateToLeft(rotate);
        break;

    case back:
        rotate = reverseR(rotate);
        break;

    case right:
        rotate = rotateToRight(rotate);
        break;

    default:
        break;
    }
}

uint16_t node::makeNewNode(coordinate _p, uint8_t side){
    lastNodeNum++;
    nodes[lastNodeNum].count = 0;
    nodes[lastNodeNum].p = _p;
    nodes[lastNodeNum].conn[convRtoArrnum(reverseR(kagome(rotate,side)))] = nowNodeNum;
    nodes[nowNodeNum].conn[convRtoArrnum(kagome(rotate,side))] = lastNodeNum;
    return lastNodeNum;
}

short node::searchNode(coordinate _p){
    for(int i = 0;i<nodeNum;i++){
        if((nodes[i].p.x == _p.x) && (nodes[i].p.y == _p.y) && (nodes[i].p.z == _p.z)){
            return i;
        }
    }
    return -1;
}

short node::checkNodeCount(coordinate _p){
    for(int i = 0;i<nodeNum;i++){
        if((nodes[i].p.x == _p.x) && (nodes[i].p.y == _p.y) && (nodes[i].p.z == _p.z)){
            return nodes[i].count;
        }
    }
    return -1;
}

uint8_t node::getMinCountDir(){
    
}

void node::searchAroundNodes(bool rWall,bool fWall,bool lWall){
    if(rWall == false){
        tempNode[right] = searchNode(convRXYZtoCoorAddLengh(rotateToRight(rotate)),now.p.x,now.p.y,now.p.z);
        if(tempNode[right] == -1){
            tempNode[right] = makeNewNode(convRXYZtoCoorAddLengh(rotateToRight(rotate),now.p.x,now.p.y,now.p.z),rotateToRight(rotate));
        }
    }
    else{
        tempNode[right] = -1;
    }

    if(fwall == false){
        tempNode[front] = searchNode(convRXYZtoCoorAddLengh(rotate,now.p.x,now.p.y,now.p.z));
        if(tempNode[front] == -1){
            tempNode[front] = makeNewNode(convRXYZtoCoorAddLengh(rotate,now.p.x,now.p.y,now.p.z),rotate);
        }
    }
    else{
        tempNode[front] = -1;
    }

    if(lWall == false){
        tempNode[left] = searchNode(convRXYZtoCoorAddLengh(rotateToLeft(rotate)),now.p.x,now.p.y,now.p.z);
        if(tempNode[left] == -1){
            tempNode[left] = makeNewNode(convRXYZtoCoorAddLengh(rotateToLeft(rotate),now.p.x,now.p.y,now.p.z),rotateToLeft(rotate));
        }
    }
    else{
        tempNode[left] = -1;
    }

    tempNode[back] = -1;
}

uint8_t node::convRtoArrnum(uint8_t _r){
    switch (_r)
    {
    case 1:
        return 0;
        break;
    case 2:
        return 1;
        break;
    case 4:
        return 2;
        break;
    case 8:
        return 3;
        break;
    default:
        break;
    }
    return 0;
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
    }
    return temp;
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

uint8_t node::reverseR(uint8_t _r){
    uint8_t temp = _r;
    for(int i = 0;i < 2;i++){
        if(temp==8){
            temp = 1;
        }
        else{
            temp = temp<<1;
        }
    }
    return temp;
}

uint8_t node::kagome(uint8_t _r,uint8_t _side){
    switch (_r){
    case 1:
        switch (_side){
        case front:
            return 1;
            break;
        case left:
            return 2;
            break;
        case back:
            return 4;
            break;
        case right:
            return 8;
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (_side){
        case front:
            return 2;
            break;
        case left:
            return 4;
            break;
        case back:
            return 8;
            break;
        case right:
            return 1;
            break;
        default:
            break;
        }
        break;
    case 4:
        switch (_side){
        case front:
            return 4;
            break;
        case left:
            return 8;
            break;
        case back:
            return 1;
            break;
        case right:
            return 2;
            break;
        default:
            break;
        }
        break;
    case 8:
        switch (_side){
        case front:
            return 8;
            break;
        case left:
            return 1;
            break;
        case back:
            return 2;
            break;
        case right:
            return 4;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

