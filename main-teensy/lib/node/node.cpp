#include "node.h"

node::node(){
    now.p.x = 0;
    now.p.y = 0;
    now.p.z = 0;
    nodes[0].count = 1;
}

void node::updatePosition(uint8_t moveto){
    now.p = convRXYZtoCoorAddLengh(kagome(rotate,moveto),now.p.x,now.p.y,now.p.z);
    nowNodeNum = searchNode(now.p);
    nodes[nowNodeNum].count++;
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

void node::updateRotation(uint8_t moveto){
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
    for(int i = 0;i < nodeNum;i++){
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
    short compare[4];
    for(int n = 0;n < 4;n++){
        compare[n] = ((tempNode[n]==-1) ? 255 : nodes[tempNode[n]].count);
        Serial.println(compare[n]);
    }
    if(compare[right] <= compare[front] && compare[right] <= compare[left]){
        return right;
    }
    else if(compare[front] <= compare[left]){
        return front;
    }
    else if(compare[left] <= compare[back]){
        return left;
    }
    else{
        return back;
    }
}

void node::searchAroundNodes(bool fWall,bool lWall,bool bWall,bool rWall){
    if(rWall == false){
        tempNode[right] = searchNode(convRXYZtoCoorAddLengh(rotateToRight(rotate),now.p.x,now.p.y,now.p.z));
        if(tempNode[right] == -1){
            tempNode[right] = makeNewNode(convRXYZtoCoorAddLengh(rotateToRight(rotate),now.p.x,now.p.y,now.p.z),rotateToRight(rotate));
        }
        if(nodes[tempNode[right]].tile == black){
            tempNode[right] = -1;
        }
    }
    else{
        tempNode[right] = -1;
    }

    if(fWall == false){
        tempNode[front] = searchNode(convRXYZtoCoorAddLengh(rotate,now.p.x,now.p.y,now.p.z));
        if(tempNode[front] == -1){
            tempNode[front] = makeNewNode(convRXYZtoCoorAddLengh(rotate,now.p.x,now.p.y,now.p.z),rotate);
        }
        if(nodes[tempNode[front]].tile == black){
            tempNode[front] = -1;
        }
    }
    else{
        tempNode[front] = -1;
    }

    if(lWall == false){
        tempNode[left] = searchNode(convRXYZtoCoorAddLengh(rotateToLeft(rotate),now.p.x,now.p.y,now.p.z));
        if(tempNode[left] == -1){
            tempNode[left] = makeNewNode(convRXYZtoCoorAddLengh(rotateToLeft(rotate),now.p.x,now.p.y,now.p.z),rotateToLeft(rotate));
        }
        if(nodes[tempNode[left]].tile == black){
            tempNode[left] = -1;
        }
    }
    else{
        tempNode[left] = -1;
    }

    if(bWall == false){
        tempNode[back] = searchNode(convRXYZtoCoorAddLengh(reverseR(rotate),now.p.x,now.p.y,now.p.z));
        if(tempNode[back] == -1){
            tempNode[back] = makeNewNode(convRXYZtoCoorAddLengh(reverseR(rotate),now.p.x,now.p.y,now.p.z),reverseR(rotate));
        }
        if(nodes[tempNode[back]].tile == black){
            tempNode[back] = -1;
        }
    }
    else{
        tempNode[back] = -1;
    }

}

void node::setTile(uint8_t color){
    if(color == silver){
        lastCheckPoint = nowNodeNum;
    }
    nodes[nowNodeNum].tile = color;
}

void node::setTile(uint8_t color,uint8_t dir){
    nodes[tempNode[dir]].tile = color;
}

uint8_t node::getTile(uint8_t dir){
    return nodes[tempNode[dir]].tile;
}

uint8_t node::convRtoArrnum(uint8_t _r){
    switch (_r)
    {
    case 1:
        return front;
        break;
    case 2:
        return left;
        break;
    case 4:
        return back;
        break;
    case 8:
        return right;
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
    if(_r<=1){
        return 8;
    }
    else{
        return _r>>1;
    }
}

uint8_t node::rotateToLeft(uint8_t _r){
    if(_r>=8){
        return 1;
    }
    else{
        return _r<<1;
    }
}

uint8_t node::reverseR(uint8_t _r){
    uint8_t temp = _r;
    for(int i = 0;i < 2;i++){
        if(temp>=8){
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
    return 1;
}

