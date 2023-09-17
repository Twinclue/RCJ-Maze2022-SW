#ifndef MOVE_ROBOT_H_
#define MOVE_ROBOT_H_

#include <LiquidCrystal.h>
#include "drive_motor.h"
#include "read_BNO055.h"
#include "read_tof.h"
#include "advanced_tof.h"
#include "pid.h"
#include "read_light.h"
#include <Servo.h>
#include "detect_victim.h"
#include "detect_wall.h"
#include "pinmap.h"

enum class _gameMode : uint8_t{
  WORLD,
  ENTRY,
};

class move_robot{
    public:
        move_robot(drive_motor *_left,drive_motor *_right,read_tof *_front,read_tof *_back,read_BNO055 *_imu,read_light *_light,LiquidCrystal *_disp,Adafruit_NeoPixel *_led);
        short fwd(short remDist = 300);
        int rev(int remDist = 300);
        short turn(short remAng = 90);
        short goUp();
        short goDown();

        void  corrDir();//correct direction
        void  corrDist();
        bool avoidObstacle();

        void drop(bool dir);
        uint8_t getRescueKitNum(){return rescueKitNum;}
        void attachInterrupts();
        void detachInterrups();
        void blink(bool color=0);
        void loadRescueKit(){flipper->write(0);}

        enum gameMode : uint8_t{
            WORLD,
            ENTRY,
        };
        gameMode currentMode=gameMode::WORLD;
        
    private:

        drive_motor *left;
        drive_motor *right;
        read_tof *front;
        read_tof *back;

        read_BNO055 *imu;

        read_light *light;
        LiquidCrystal *disp;
        Adafruit_NeoPixel *led;

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
        pid *corrDirPid = new pid(25,0.001,0);
        pid *turnPid = new pid(5,0.00005,0);
        pid *fwdPid = new pid(10,0,0);//50,0,0

        const byte lTouch = 21;
        const byte rTouch = 17;

        const uint8_t servo = 30;
        Servo *flipper = new Servo();

        detect_victim *vic = new detect_victim(&Serial4, &Serial5, &Wire2, &Wire);
        detect_wall *mwall;
        uint8_t rescueKitNum = 12;
        short prePitch = 0;

        int red;
        int green;
        const int r_th=200;
        const int g_th=450;

    

        bool victim();

};
#endif