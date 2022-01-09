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
    short moveResult = 0;
    for(int i =0;i < 4;i++){
        walls[i] = wall->getSingleWall(i);
    }
    n->searchAroundNodes(walls[front],walls[left],walls[back],walls[right]);
    moveto = n->getMinCountDir();
    Serial.print("Move to :");
    Serial.println(moveto);
    switch (moveto){
    case front:
        moveResult =  move->fwd(150);
        //検知
        moveResult = move->fwd(150);
        //検知
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
    }
    else{
        n->setTile(black,moveto);
        n->updateRotation(moveto);
    }
}