#ifndef READ_CAMERA_H_
#define READ_CAMERA_H_

#include <Arduino.h>

class read_camera{
	public:
		read_camera(uint8_t _txPin, uint8_t _rxPin);
		int read();
	private:
		uint8_t txPin //Comm2
		uint8_t rxPin //Comm1
};

#endif