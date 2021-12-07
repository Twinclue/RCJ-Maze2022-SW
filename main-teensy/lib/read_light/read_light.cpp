#include "read_light.h"

read_light::read_light(Adafruit_NeoPixel *_led){
    led = _led;
    pinMode(emitter,INPUT);
    led->begin();
    led->setPixelColor(lednum,cr,cg,cb);
    led->setBrightness(255);
    led->show();
}

int read_light::getFloorColor(){
    if(this->read() < thresholdBW){
        return 1;
    }
    else if(this->read() < thresholdWS){
        return 0;
    }
    else{
        return 2;
    }
}

int read_light::read(){
    return analogRead(emitter);
}