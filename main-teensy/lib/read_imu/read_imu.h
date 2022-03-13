#include <GY521.h>


class read_BNO055 : public GY521
{
private:
    const float caxe = -0.086;
    const float caye = -0.014;
    const float caze = 1.080;
    const float cgxe = -0.653;
    const float cgye = 0.344;
    const float cgze = 0.295;//マイナス方向にドリフト→値を大きく

    float preangleY = 0;
    float angleY;
    unsigned long tY;
    unsigned long pretY;
    float deltatY;
    float cangleY = 0;

    
    float preangleP = 0;
    float angleP;
    unsigned long tP;
    unsigned long pretP;
    float deltatP;
    float cangleP = 0;

public:
    read_BNO055();
    bool begin(TwoWire * _bus);
    float getGPitch();
    float getGYaw();
};

