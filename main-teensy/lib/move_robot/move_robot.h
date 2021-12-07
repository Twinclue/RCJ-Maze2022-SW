#ifndef MOVE_ROBOT_H_
#define MOVE_ROBOT_H_

#include "drive_motor.h"
#include "read_imu.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "pid.h"
#include "read_light.h"

class move_robot{
    public:
        move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back,read_imu *_imu,read_light *_light);
        short fwd(short remDist = 300);
        short rev(short remDist = 300);
        short turn(short remAng = 90);

        void  corrDir();//correct direction
        void  corrDist();
        bool avoidObstacle();

    private:

        drive_motor *left;
        drive_motor *right;
        read_tof *front;
        read_tof *back;

        read_imu *imu;

        read_light *light

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
        const float kp = 20;
        pid *corrDirPid = new pid(5,1,0);
        pid *turnPid = new pid(5,1,0);
        pid *fwdPid = new pid(50,0,0);

        const byte lTouch = 21;
        const byte rTouch = 17;
};
#endif