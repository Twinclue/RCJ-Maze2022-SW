#include "advanced_tof.h"

float readRAngle(int distA,int distB,byte sensorDist=175){
    return atan2f(float(distA)-float(distB),sensorDist)*360/(2*PI);
}