#ifndef SOLVER_H_
#define SOLVER_H_

#include "move_robot.h"
#include "detect_wall.h"


class solver{
    public:
        solver(read_imu *_imu,read_light *_light,move_robot *_move,detect_wall *_wall);
        int rightHand();
    private:

        read_imu *imu;
        read_light *light;
        move_robot *move;
        detect_wall *wall;

        const uint8_t front = 0;
        const uint8_t left = 1;
        const uint8_t back = 2;
        const uint8_t right = 3;
};

#endif
