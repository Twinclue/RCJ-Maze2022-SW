#include "move_robot.h"

move_robot::move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back){
    left = _left;
    right = _right;
    front = _front;
    back = _back;
}

short move_robot::fwd(short remDist = 300){
    int startDist = front->read(fc);
    while(startDist - front->read(fc) < remDist){
        left->on(255);
        right->on(255);
    }
    left->on(0);
    right->on(0);
    return 1;
}

void move_robot::corrDir(){
    float relativeAng = 0;
    if(front->read(fls) <= 200 && back->read(bls) <= 200){
        relativeAng = (front->read(fls),back->read(bls));
        while(-3 <= relativeAng && relativeAng <= 3){
            left->on(kp*relativeAng);
            right->on(-kp*relativeAng);
        }
    }
}
