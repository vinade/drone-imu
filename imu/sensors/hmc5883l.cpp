#ifndef HMC5883L_SENSOR_CPP
#define HMC5883L_SENSOR_CPP

#include "hmc5883l.h"

void HMC5883LSensor::setup_instance()
{
    Wire.begin();
    Wire.beginTransmission(this->address);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.endTransmission();
}

void HMC5883LSensor::update()
{
    int x, y, z;

    Wire.beginTransmission(this->address);
    Wire.write(0x03);
    Wire.endTransmission();

    Wire.requestFrom(address, 6);
    if (Wire.available() >= 6)
    {
        x = Wire.read() << 8;
        x |= Wire.read();

        z = Wire.read() << 8;
        z |= Wire.read();

        y = Wire.read() << 8;
        y |= Wire.read();
    }

    this->direction.x = x;
    this->direction.y = y;
    this->direction.z = z;
}

#endif
