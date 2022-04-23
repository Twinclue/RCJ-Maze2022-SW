#include "detect_victim.h"

detect_victim::detect_victim(uint8_t _lowR,uint8_t _highR , uint8_t _lowL, uint8_t _highL , TwoWire *_busR, TwoWire *_busL){
    busR = _busR;
    busL = _busL;
    d6tL = new read_temperature(busL);
    d6tR = new read_temperature(busR);
    camR = new read_camera(_lowR,_highR);
    camL = new read_camera(_lowL,_highL);
}

int8_t detect_victim::isHeatedVictim(bool rightSide){
    if(rightSide){
        return d6tR->temp()>=temp_th;
    }else{
        return d6tL->temp()>=temp_th;
    }
}

int8_t detect_victim::kitNumOneSide(bool rightSide){
    int8_t kitNum=-1;
    if(rightSide){
        if(camR->victim_num()==3){kitNum=3;}
        else if(camR->victim_num()==2){kitNum=2;}
        else if(camR->victim_num()==1||this->isHeatedVictim(true)){kitNum = 1;}
        else if(camR->victim_num()==0){kitNum=0;}
        else if(camR->victim_num()==-1){kitNum=-1;}
    }else{
        if(camL->victim_num()==3){kitNum=3;}
        else if(camL->victim_num()==2){kitNum=2;}
        else if(camL->victim_num()==1||this->isHeatedVictim(false)){kitNum = 1;}
        else if(camL->victim_num()==0){kitNum=0;}
        else if(camL->victim_num()==-1){kitNum=-1;}
    }
    return kitNum;
}
/*
int8_t[2] detect_victim:: kitNumBothSide(){
    int8_t kitNum[2]={-1,-1};
    if(camR->victim_num()==3){kitNum[1]=3;}
    if(camR->victim_num()==2){kitNum[1]=2;}
    if(camR->victim_num()==1||this->isHeatedVictim(true)){kitNum[1] = 1;}
    if(camR->victim_num()==0){kitNum[1]=0;}
    if(camR->victim_num()==-1){kitNum[1]=-1;}

    if(camL->victim_num()==3){kitNum[0]=3;}
    if(camL->victim_num()==2){kitNum[0]=2;}
    if(camL->victim_num()==1||this->isHeatedVictim(false)){kitNum[0] = 1;}
    if(camL->victim_num()==0){kitNum[0]=0;}
    if(camL->victim_num()==-1){kitNum[0]=-1;}

    return &kitNum;
}
*/