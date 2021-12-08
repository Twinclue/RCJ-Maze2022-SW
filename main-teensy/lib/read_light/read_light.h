#ifndef READ_LIGHT_H_
#define READ_LIGHT_H_
#include <Adafruit_NeoPixel.h>

class read_light{
    public:
        read_light(Adafruit_NeoPixel *_led);
        int read();
        int getFloorColor(); // returns 0 on W,1 on B, 2 on S
    private:
        Adafruit_NeoPixel *led;
        const uint8_t lednum = 1;
        const uint8_t cr = 255;
        const uint8_t cg = 0;
        const uint8_t cb = 0;

        const uint8_t emitter = 22;

        const uint16_t thresholdBW = 50;
        const uint16_t thresholdWS = 500;
};

#endif