#ifndef READ_LIGHT_H_
#define READ_LIGHT_H_
#include <Adafruit_NeoPixel.h>

class read_light{
    public:
        read_light(Adafruit_NeoPixel *_led);
    private:
        Adafruit_NeoPixel *led;
};

#endif