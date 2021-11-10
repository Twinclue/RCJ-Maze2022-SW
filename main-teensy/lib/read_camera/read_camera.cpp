#include "read_camera.h"

read_camera::read_camera(uint8_t _txPin, uint8_t _rxPin){
	txPin=_txPin;
	rxPin=_rxPin;
	pinMode(txPin, OUTPUT);
	pinMode(rxPin, OUTPUT);
}

int read_camera::read(){
	bool txIn, rxIn;
	txIn=digitalRead(txPin);
	rxIn=digitalRead(rxPin);
	if(txIn==0&&rxIn==0){return 0;} //no victim
	if(txIn==1&&rxIn==0){return 1;} //alert
	if(txIn==0&&rxIn==1){return 2;} //1 kit
	if(txIn==1&&rxIn==1){return 3;} //2 kits
}