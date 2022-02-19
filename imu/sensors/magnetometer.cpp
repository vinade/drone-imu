#ifndef MAGNETOMETER_SENSOR_CPP
#define MAGNETOMETER_SENSOR_CPP

#include "magnetometer.h"

MagnetometerSensor::MagnetometerSensor()
{
    if (this->magneto == NULL)
    {
        this->magneto = new QMC5883LSensor();
        this->magneto->setup_instance();
    }
}

MagnetometerSensor::MagnetometerSensor(QMC5883LSensor *m)
{
    this->magneto = m;
}

void MagnetometerSensor::update()
{
    float magnitude, x, y, z;

    this->magneto->try_update();
    device_state.magneto = low_pass_filter(this->magneto->direction, this->old_mag, MAG_MEASURED_WEIGHT);
    this->azimuth = this->calc_azimuth(device_state.magneto.y, device_state.magneto.x);
}

float MagnetometerSensor::calc_azimuth(float a, float b)
{
    float az = atan2(a, b) * 180.0 / PI;
    return az < 0 ? 360 + az : az;
}

#endif