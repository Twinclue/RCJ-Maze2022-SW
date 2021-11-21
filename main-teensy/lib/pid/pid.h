#ifndef PID_H_
#define PID_H_
#include <Arduino.h>

class pid{
    public:
        pid(float _kp,float _ki,float _kd);
        init();
        float calcP(int feedBack,int target);
        float calcPI(int feedBack,int target);
        float calcPD(int feedBack,int target);
        float calcPID(int feedBack,int target);
    private:
        float kp;
        float ki;
        float kd;
        float error;
        float prevError;
        unsigned long initTime;
        float integral;
};


#endif