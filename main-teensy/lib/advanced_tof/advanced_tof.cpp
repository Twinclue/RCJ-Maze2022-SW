#include "advanced_tof.h"

float readRAngle(int distA,int distB){
    return atan2f(float(distA)-float(distB),175)*360/(2*PI);
}