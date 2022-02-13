#ifndef BASIC_SENSOR_CPP
#define BASIC_SENSOR_CPP

#include "basicSensor.h"

void BasicSensor::set_fallback(BasicSensor *fallback_sensor)
{
    this->fallback = fallback_sensor;
}

void BasicSensor::set_fail_state(unsigned int *sensor_fail_state)
{
    this->fail_state = sensor_fail_state;
}

void BasicSensor::set_warning_state(unsigned int *sensor_warning_state)
{
    this->warning_state = sensor_warning_state;
}

void BasicSensor::warn()
{
    *this->warning_state |= this->family;
}

void *BasicSensor::read()
{
    this->try_update();
    if (this->available)
    {
        return this->get_data();
    }

    return NULL;
}

void BasicSensor::try_update()
{
    if (!this->available)
    {
        return;
    }

    unsigned int interval;
    interval = millis() - this->last_update;
    if (interval >= this->minimum_update_interval)
    {
        this->update();
        this->last_update = millis();
    }
}

uint8_t BasicSensor::minimum_update_interval = SENSOR_DEFAULT_UPDATE_INTERVAL;
boolean BasicSensor::static_setup_ready = FALSE;

#endif
