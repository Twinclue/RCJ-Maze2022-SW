#include "solver.h"

solver::solver(read_imu *_imu,read_light *_light,move_robot *_move,detect_wall *_wall){
    imu = _imu;
    light = _light;
    move = _move;
    wall = _wall;
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