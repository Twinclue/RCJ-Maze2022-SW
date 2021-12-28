#include <GY521.h>


class read_imu : public GY521
{
private:
    const float caxe = -0.086;
    const float caye = -0.014;
    const float caze = 1.080;
    const float cgxe = -0.653;
    const float cgye = 0.344;
    const float cgze = 0.295;//マイナス方向にドリフト→値を大きく

public:
    read_imu();
    bool begin(TwoWire * _bus);
};

