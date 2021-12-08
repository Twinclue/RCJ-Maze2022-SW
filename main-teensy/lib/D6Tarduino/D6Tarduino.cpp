#include "D6Tarduino.h"
#include <Wire.h>

D6Tarduino::D6Tarduino(){

}

void D6Tarduino::setBus(TwoWire *_bus) {
   bus = _bus;
}

uint16_t D6Tarduino::conv8us_s16_le(uint8_t* buf, int n) {
    int ret;
    ret = buf[n];
    ret += buf[n + 1] << 8;
    return (int16_t)ret;   // and convert negative.
}

uint8_t D6Tarduino::calc_crc(uint8_t data) {
    int index;
    uint8_t temp;
    for (index = 0; index < 8; index++) {
        temp = data;
        data <<= 1;
        if (temp & 0x80) {data ^= 0x07;}
    }
    return data;
}


float D6Tarduino::getTEMP(){
    int i;
    memset(rbuf,0,N_READ);
    bus->beginTransmission(D6T_ADDR);  // I2C client address
    bus->write(D6T_CMD);               // D6T register
    bus->endTransmission();            // I2C repeated start for read
    bus->requestFrom(D6T_ADDR, N_READ);
    i = 0;
    while (bus->available()) {
        rbuf[i++] = bus->read();
    }

    if (D6T_checkPEC(rbuf, N_READ - 1)) {
        return -1;
    }
    return conv8us_s16_le(rbuf, 2)/10.0;
}


float D6Tarduino::getPTAT(){
    int i;
    memset(rbuf,0,N_READ);
    bus->beginTransmission(D6T_ADDR);  // I2C client address
    bus->write(D6T_CMD);               // D6T register
    bus->endTransmission();            // I2C repeated start for read
    bus->requestFrom(D6T_ADDR, N_READ);
    i = 0;
    while (bus->available()) {
        rbuf[i++] = bus->read();
    }

    if (D6T_checkPEC(rbuf, N_READ - 1)) {
        return -1;
    }
    return conv8us_s16_le(rbuf, 0)/10.0;
}

bool D6Tarduino::D6T_checkPEC(uint8_t buf[], int n) {
    int i;
    uint8_t crc = calc_crc((D6T_ADDR << 1) | 1);  // I2C Read address (8bit)
    for (i = 0; i < n; i++) {
        crc = calc_crc(buf[i] ^ crc);
    }
    bool ret = crc != buf[n];
    return ret;
}
