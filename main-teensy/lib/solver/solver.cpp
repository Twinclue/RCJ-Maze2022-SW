#include "solver.h"

solver::solver(read_imu *_imu,read_light *_light,move_robot *_move,detect_wall *_wall){
    imu = _imu;
    light = _light;
    move = _move;
    wall = _wall;
    node = _node;
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
        walls[i] = wall->getSingleWall[i];
    }
    node->searchAroundNodes(walls[front],walls[left],walls[back],walls[right]);
    moveto = node->getMinCountDir();
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
        node->updatePosition(moveto);
        node->setTile(light->getFloorColor());
    }
}