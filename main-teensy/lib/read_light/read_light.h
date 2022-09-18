#ifndef READ_LIGHT_H_
#define READ_LIGHT_H_
#include <Adafruit_NeoPixel.h>

const uint8_t silver = 2;
const uint8_t black = 1;
const uint8_t whilte = 0;

class read_light{
    public:
        read_light(Adafruit_NeoPixel *_led);
        int read();
        int getFloorColor(); // returns 0 on W,1 on B, 2 on S
        int getColorRaw();
    private:
        Adafruit_NeoPixel *led;
        const uint8_t lednum = 1;
        const uint8_t cr = 0;
        const uint8_t cg = 0;
        const uint8_t cb = 80;

        const uint8_t emitter = 22;

        const uint16_t thresholdBS = 50;
        const uint16_t thresholdSW = 200;
};

#endif