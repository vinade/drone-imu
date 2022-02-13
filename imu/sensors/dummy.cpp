#ifndef DUMMY_SENSOR_CPP
#define DUMMY_SENSOR_CPP

#include "dummy.h"

void DummySensor::setup_instance()
{
    this->max = 20;
    this->min = 15;
}

void DummySensor::update()
{
    this->value = random(this->min, this->max);
}

void *DummySensor::get_data()
{
    return (void *)&this->value;
}

uint8_t DummySensor::minimum_update_interval = 10;
uint8_t DummySensor::static_setup_ready = FALSE;

#endif
