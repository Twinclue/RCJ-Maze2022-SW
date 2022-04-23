#ifndef READ_CAMERA_H_
#define READ_CAMERA_H_

#include <Arduino.h>

class read_camera{
	public:
		read_camera(uint8_t _low,uint8_t _high);
		int victim_num();
	private:
		uint8_t low,high;
};

#endif