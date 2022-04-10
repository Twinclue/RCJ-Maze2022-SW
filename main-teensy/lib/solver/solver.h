#ifndef SOLVER_H_
#define SOLVER_H_

#include <LiquidCrystal.h>
#include "move_robot.h"
#include "detect_wall.h"
#include "node.h"

#define FLAT 0
#define GOUP 1
#define GODOWN 2


class solver{
    public:
        solver(read_BNO055 *_imu,read_light *_light,move_robot *_move,detect_wall *_wall,node *_node,LiquidCrystal *_lcd);
        int rightHand();
        int EXrightHand();
        bool walls[4];
        uint8_t moveto;
    private:

        read_BNO055 *imu;
        read_light *light;
        move_robot *move;
        detect_wall *wall;
        node *n;
        LiquidCrystal *disp;

        byte slopeState = FLAT;//0:not slope, 1:go up slope, 2:go down slope

};

#endif
