#include <PCA9547.h>
#include <VL53L0X.h>
#include <VL6180X.h>

#define VL61NUM 4
#define VL53NUM 1

class read_tof{
    public:
        read_tof(TwoWire *_bus);
        int read(uint8_t _direction);
    private:
        const int vl61Num = 4;
        const int vl53Num = 1;
        PCA9547 i2cSelect;
        VL6180X vl61[VL61NUM];
        VL53L0X vl53[VL53NUM];
        TwoWire *bus;
        int dist;
};