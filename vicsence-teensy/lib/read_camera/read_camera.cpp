#include "read_camera.h"

read_camera::read_camera(uint8_t _low,uint8_t _high){
	low=_low;
	high=_high;
	pinMode(low,INPUT_PULLDOWN);
	pinMode(high,INPUT_PULLDOWN);
}
int read_camera::victim_num(){
	bool lowbit = digitalRead(low);
	bool highbit = digitalRead(high);
	int res = (highbit<<1 | lowbit) - 1;
	return res;
}