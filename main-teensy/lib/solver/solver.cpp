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
    bool blackFlag;
    for(int i =0;i < 0;i++){
        walls[i] = wall->getSingleWall(i);
    }
    n->searchAroundNodes(walls[front],walls[left],walls[back],walls[right]);
    moveto = n->getMinCountDir();
    switch (moveto){
    case front:
        blackFlag = (move->fwd() == -1) ? true : false;
        break;
    case left:
        move->turn(-90);
        blackFlag = (move->fwd() == -1) ? true : false;
        break;
    case back:
        move->turn(180);
        blackFlag = (move->fwd() == -1) ? true : false;
        break;
    case right:
        move->turn();
        blackFlag = (move->fwd() == -1) ? true : false;
        break;
    default:
        break;
    }
    if(blackFlag == false){
        n->updatePosition(moveto);
        n->setTile(light->getFloorColor());
    }
    else{
        n->setTile(light->getFloorColor(),moveto);
        n->updateRotation(moveto);
    }
}