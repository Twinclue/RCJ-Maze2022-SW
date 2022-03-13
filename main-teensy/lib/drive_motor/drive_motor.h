#ifndef DRIVE_MOTOR_H_
#define DRIVE_MOTOR_H_

#include <Arduino.h>

class drive_motor
{
public:
    drive_motor(uint8_t _dir, uint8_t _pwm, uint8_t _flt);
    int on(int power);
private:
    uint8_t dir;
    uint8_t pwm;
    uint8_t flt;
    const uint32_t PWMfreq=80000;
};

#endif