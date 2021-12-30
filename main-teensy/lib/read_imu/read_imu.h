#include <MadgwickAHRS.h>
#include "MPU6050.h"


class read_imu : public MPU6050
{
private:
    Madgwick *filter = new Madgwick;
    int16_t ax, ay, az;//加速度 int16_tは2バイトの符号付き整数
    int16_t gx, gy, gz;//角速度 同上

    float preYaw=0,prePitch=0,preRoll=0;
    int over0countYaw = 0,over0countPitch = 0,over0countRoll = 0;
    unsigned long premicros;
    const uint16_t rate = 55;

public:
    read_imu();
    bool begin();
    void update();
    float getFPitch();
    float getFRoll();
    float getFYaw();
    int getO0cY(){return over0countYaw;}
};

