#ifndef HCSR04_SENSOR_H
#define HCSR04_SENSOR_H

#include "basicSensor.h"

class HCSR04Sensor : public BasicSensor
{
private:
    uint8_t ping_pin;
    uint8_t echo_pin;

public:
    float height = 0;

    HCSR04Sensor(uint8_t echo, uint8_t ping);
    void update();
    void setup_instance();
};

#endif
