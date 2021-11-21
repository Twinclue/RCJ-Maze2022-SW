#ifndef MOVE_ROBOT_H_
#define MOVE_ROBOT_H_

#include "drive_motor.h"
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"

class move_robot{
    public:
        move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back);
        short fwd(short remDist = 300);
        short rev(short remDist = 300);
        void  corrDir();//correct direction

    private:
        drive_motor *left;
        drive_motor *right;
        read_tof *front;
        read_tof *back;

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
        const float kp = 2;
};
#endif