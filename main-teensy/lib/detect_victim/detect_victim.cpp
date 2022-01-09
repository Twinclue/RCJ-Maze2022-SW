#include "detect_victim.h"

detect_victim::detect_victim(HardwareSerial *_serialR, HardwareSerial *_serialL, TwoWire *_busR, TwoWire *_busL){
    serialR = _serialR;
    serialL = _serialL;
    busR = _busR;
    busL = _busL;
    d6tL = new read_temperature(busL);
    d6tR = new read_temperature(busR);
    camR = new read_camera(serialR);
    camL = new read_camera(serialL);
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
        if(camR->victim_num()==2){kitNum=2;}
        if(camR->victim_num()==1||this->isHeatedVictim(true)){kitNum = 1;}
        if(camR->victim_num()==0){kitNum=0;}
        if(camR->victim_num()==-1){kitNum=-1;}
    }else{
        if(camL->victim_num()==3){kitNum=3;}
        if(camL->victim_num()==2){kitNum=2;}
        if(camL->victim_num()==1||this->isHeatedVictim(false)){kitNum = 1;}
        if(camL->victim_num()==0){kitNum=0;}
        if(camL->victim_num()==-1){kitNum=-1;}
    }
    return kitNum;
}

// int8_t detect_victim::kitNumOneSideMode(bool rightSide){
//     const uint8_t sampleNum = 10;
//     int8_t kitNum[sampleNum];
//     for(int c=0;c<sampleNum;c++){
//         kitNum[c] = this->kitNumOneSide(rightSide);
//         Serial.print(kitNum[c]);
//         Serial.print(" , ");
//         delay(10);
//     }
//     Serial.println();
//     uint8_t count[4] = {0,0,0,0};
//     for(int k =0;k<sampleNum;k++){
//         switch (kitNum[k])
//         {
//         case -1:
//             count[4]++;
//             break;
//         case 0:
//             count[0]++;
//             break;
//         case 1:
//             count[1]++;
//             break;
//         case 2:
//             count[2]++;
//             break;
//         case 3:
//             count[3]++;
//             break;
//         default:
//             break;
//         }
//     }
//     int8_t mode = -1;
//     int8_t max = 0;
//     for(int i=0;i<4;i++){
//         if(max<count[i]){
//             max = count[i];
//             mode = i;
//         }
//     }
//     return mode==4?-1:mode;

// }
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