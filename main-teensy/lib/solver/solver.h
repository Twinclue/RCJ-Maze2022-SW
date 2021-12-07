#ifndef SOLVER_H_
#define SOLVER_H_

#include "move_robot.h"



class solver{
    public:
        solver(read_tof *_front,read_tof *_back,read_imu *_imu,read_light *_light,move_robot *_move);
        rightHand();
    private:
        read_tof *front;
        read_tof *back;

        read_imu *imu;

        read_light *light;

        move_robot *move;

        const byte fls = 0;
        const byte flf = 1;
        const byte frf = 2;
        const byte frs = 3;
        const byte fc  = 4;

        const byte brs = 0;
        const byte brf = 1;
        const byte blf = 2;
        const byte bls = 3;
        const byte bc  = 4;
};

#endif
