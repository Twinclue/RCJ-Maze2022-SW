#include "read_camera.h"

read_camera::read_camera(HardwareSerial *_serial){
	serial=_serial;
	// init
	serial->begin(9600);
}
int read_camera::victim_num(){
	switch (this->read())
	{
	case 'H':
		return 3;
	case 'S':
		return 2;
	case 'R':
	case 'Y':
		return 1;
	case 'U':
	case 'G':
		return 0;
	case 'N':
		return -1;
	default:
		return -2;
	}
}
char read_camera::read(){
	if(serial->available()){
		return serial->read();
	}else{
		return -1;
	}
}