#ifndef READ_CAMERA_H_
#define READ_CAMERA_H_

#include <Arduino.h>

class read_camera{
	public:
		read_camera(HardwareSerial *_serial);
		char read();
		int victim_num();
	private:
		HardwareSerial *serial;
};

#endif