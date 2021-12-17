#ifndef SOLVER_H_
#define SOLVER_H_

#include "move_robot.h"
#include "detect_wall.h"
#include "node.h"

class solver{
    public:
        solver(read_imu *_imu,read_light *_light,move_robot *_move,detect_wall *_wall);
        int rightHand();
        int EXrightHand();
    private:

        read_imu *imu;
        read_light *light;
        move_robot *move;
        detect_wall *wall;


};

#endif
