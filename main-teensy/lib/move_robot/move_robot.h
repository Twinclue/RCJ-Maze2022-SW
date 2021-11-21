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
    private:
        drive_motor *left;
        drive_motor *right;
        read_tof *front;
        read_tof *back;

};
#endif