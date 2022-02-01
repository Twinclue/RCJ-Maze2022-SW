#ifndef SOLVER_H_
#define SOLVER_H_

#include "move_robot.h"
#include "detect_wall.h"
#include "node.h"

#define DEBUG_NODE_

class solver{
    public:
        solver(read_imu *_imu,read_light *_light,move_robot *_move,detect_wall *_wall,node *_node);
        int rightHand();
        int EXrightHand();
        bool walls[4];
        uint8_t moveto;
    private:

        read_imu *imu;
        read_light *light;
        move_robot *move;
        detect_wall *wall;
        node *n;

        

        bool slopeFlag = false;

};

#endif
