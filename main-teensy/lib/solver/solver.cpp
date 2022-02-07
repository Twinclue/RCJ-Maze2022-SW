#include "solver.h"
solver::solver(read_imu *_imu,read_light *_light,move_robot *_move,detect_wall *_wall,node *_node){
    imu = _imu;
    light = _light;
    move = _move;
    wall = _wall;
    n = _node;
}

int solver::rightHand(){
    if(!wall->getSingleWall(right)){
        move->turn();
        move->fwd();
    }
    else if(!wall->getSingleWall(front)){
        move->fwd();
    }
    else if(!wall->getSingleWall(left)){
        move->turn(-90);
        move->fwd();
    }
    else{
        move->turn(180);
        move->fwd();
    }
    return 0;
}

int solver::EXrightHand(){
    short moveResult = 0;;
    for(int i =0;i < 4;i++){
        walls[i] = wall->getSingleWall(i);
    }
    n->searchAroundNodes(walls[front],walls[left],walls[back],walls[right]);
    moveto = n->getMinCountDir();
    switch (moveto){
    case front:
        if(slopeFlag){
            moveResult = move->goUp();
        }
        else{
            moveResult =  move->fwd();
        }
        break;
    case left:
        move->turn(-90);
        moveResult =  move->fwd();
        break;
    case back:
        move->turn(180);
        moveResult =  move->fwd();
        break;
    case right:
        move->turn();
        moveResult =  move->fwd();
        break;
    default:
        break;
    }
    if(moveResult == 0){
        n->updatePosition(moveto);
        n->setTile(light->getFloorColor());
        slopeFlag = false;
    }
    else if(moveResult == -2){
        n->updatePosition(moveto);
        //n->setTile(3);
        slopeFlag = true;
    }
    else{
        n->setTile(black,moveto);
        n->updateRotation(moveto);
    }

    #ifdef DEBUG_NODE_

    Serial.print("WALLS :: ");
    Serial.print("RIGHT : ");
    Serial.print(walls[right]);
    Serial.print(" | FRONT : ");
    Serial.print(walls[front]);
    Serial.print(" | LEFT : ");
    Serial.print(walls[left]);
    Serial.print(" | BACK : ");
    Serial.println(walls[back]);
    Serial.print("TEMP NODE :: ");
    Serial.print("RIGHT : ");
    Serial.print(n->getTempNode(right));
    Serial.print(" | FRONT : ");
    Serial.print(n->getTempNode(front));
    Serial.print(" | LEFT : ");
    Serial.print(n->getTempNode(left));
    Serial.print(" | BACK : ");
    Serial.println(n->getTempNode(back));

    Serial.print("MOVE TO : ");
    switch (moveto)
    {
    case right:
        Serial.print("RIGHT");
        break;
    case front:
        Serial.print("FRONT");
        break;
    case left:
        Serial.print("LEFT");
        break;
    case back:
        Serial.print("BACK");
        break;
    default:
        Serial.print("ERROR");
        break;
    }
    Serial.print(" | MOVE RESULT : ");
    Serial.print(moveResult);
    Serial.print(" | SLOPE FLAG : ");
    Serial.println(slopeFlag);
    Serial.println("-----------------");
    Serial.println("");
    #endif
}