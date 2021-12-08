#ifndef _D6TARDUINO_
#define _D6TARDUINO_
#include <arduino.h>
#include <Wire.h>

#define D6T_ADDR 0x0A  // for I2C 7bit address
#define D6T_CMD 0x4C  // for D6T-44L-06/06H, D6T-8L-09/09H, for D6T-1A-01/02

#define N_ROW 1
#define N_PIXEL 1

#define N_READ ((N_PIXEL + 1) * 2 + 1)

class D6Tarduino{
    public:
        D6Tarduino();
        void setBus(TwoWire * _bus);
        float getPTAT();
        float getTEMP();
    private:
    TwoWire * bus;
        uint8_t rbuf[N_READ];
        uint8_t calc_crc(uint8_t deta);
        uint16_t conv8us_s16_le(uint8_t *buf, int n);
        bool D6T_checkPEC(uint8_t buf[], int n);
};

#endif