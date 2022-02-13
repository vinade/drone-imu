#ifndef DUMMY_SENSOR_H
#define DUMMY_SENSOR_H

#include "basicSensor.cpp"

class DummySensor : public BasicSensor
{
private:
    unsigned int max = 1000;
    unsigned int min = 0;
    long value = 0;

public:
    static uint8_t minimum_update_interval;
    static uint8_t static_setup_ready;

    DummySensor(){};

    void update();
    void *get_data();
    void setup_instance();
};

#endif
