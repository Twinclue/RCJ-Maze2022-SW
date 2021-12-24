#include "detect_victim.h"

detect_victim::detect_victim(HardwareSerial *_serialR, HardwareSerial *_serialL, TwoWire *_busR, TwoWire *_busL){
    serialR = _serialR;
    serialL = _serialL;
    busR = _busR;
    busL = _busL;
}

detect_victim::bool isHeatedVictim(){
    if(d6tR.temp()>temp_th || d6tL.temp()>temp_th){
        return true;
    }else{
        return false;
    }
}

detect_victim:: int8_t kitNumOneSide(bool rightSide){
    int8_t kitNum=0;
    if(){
        kitNum += 1;
    }
    if(rightSide){
        camR -> victim_num();
    }else{

    }
}
detect_victim:: int8_t[2] kitNumBothSide(){

}