#ifndef HMC5883L_SENSOR_H
#define HMC5883L_SENSOR_H

// https://www.electronoobs.com/eng_arduino_tut77.php

#include "i2c.h"

class HMC5883LSensor : public I2CSensor
{
public:
    vec3float direction;

    int address = I2C_ADDRESS_HMC5883L;

    void setup_instance();
    void update();
    int get_address() { return this->address; }
};

#endif
