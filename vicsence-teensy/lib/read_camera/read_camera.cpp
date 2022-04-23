#include "read_camera.h"

read_camera::read_camera(HardwareSerial *_serial){
	serial=_serial;
	// init
	serial->begin(115200);
}
int read_camera::victim_num(){
	while(serial->available() == -1);
	switch (this->read())
	{
	case 'S':
		return 2;
	case 'H':
		return 3;
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