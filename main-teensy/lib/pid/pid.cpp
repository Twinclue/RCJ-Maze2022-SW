#include "pid.h"

pid::pid(float _kp,float _ki,float _kd){
    kp = _kp;
    ki = _ki;
    kd = _kd;
}

pid::init(){
    initTime = millis();
    integral = 0;
}

float pid::calcP(int feedBack,int target){
    return ((feedBack - target) * kp);
}

float pid::calcPD(int feedBack,int target){
    float p,d;
    prevError = error;
    error = feedBack - target;
    unsigned long deltaT = millis() - initTime;
    p = kp * error;
    d = kd * (error - prevError) / deltaT;
    return (p + d);
}

float pid::calcPI(int feedBack,int target){
    float p,i;
    prevError = error;
    error = feedBack - target;
    unsigned long deltaT = millis() - initTime;
    integral += (error + prevError) / 2.0 * deltaT;
    p = kp * error;
    i = ki * integral;
    return (p + i);
}

float pid::calcPID(int feedBack,int target){
    float p,i,d;

    prevError = error;
    error = feedBack - target;
    unsigned long deltaT = millis() - initTime;

    integral += (error + prevError) / 2.0 * deltaT;

    p = kp * error;
    i = ki * integral;
    d = kd * (error - prevError) / deltaT;
    return (p + i + d);
}