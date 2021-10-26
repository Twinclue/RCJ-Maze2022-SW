#include <GY521.h>


class read_imu : public GY521
{
private:
    const float caxe = -0.071;
    const float caye = -0.008;
    const float caze = 1.085;
    const float cgxe = -0.709;
    const float cgye = 0.479;
    const float cgze = 0.2;//マイナス方向にドリフト→値を大きく

public:
    read_imu();
    bool begin(TwoWire * _bus);
};

