#ifndef DRIVE_MOTOR_H_
#define DRIVE_MOTOR_H_

#include <Arduino.h>

class drive_motor
{
public:
    drive_motor(uint8_t _in1, uint8_t _in2, uint8_t _pwm, uint8_t _lo1, uint8_t _lo2);
    int on(int power);
private:
    uint8_t in1;
    uint8_t in2;
    uint8_t pwm;
    uint8_t lo1;
    uint8_t lo2;
};




#endif