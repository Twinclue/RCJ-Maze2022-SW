#include "move_robot.h"

move_robot::move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back){
    left = _left;
    right = _right;
    front = _front;
    back = _back;
}

short move_robot::fwd(short remDist = 300){
    return 1;
}
