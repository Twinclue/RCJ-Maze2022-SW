#include "solver.h"

solver::solver(read_tof *_front,read_tof *_back,read_imu *_imu,read_light *_light,move_robot *_move){
    front = _front;
    back = _back;
    imu = _imu;
    light = _light;
    move = _move;
}

solver::rightHand(){

}