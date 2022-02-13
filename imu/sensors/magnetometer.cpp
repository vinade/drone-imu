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

    device_state.raw_magneto.x = this->magneto->direction.x;
    device_state.raw_magneto.y = this->magneto->direction.y;
    device_state.raw_magneto.z = this->magneto->direction.z;

    if (this->initial_counter <= MAG_OFFSET_SAMPLE){
        device_state.initial_mag.x += this->magneto->direction.x/float(MAG_OFFSET_SAMPLE);
        device_state.initial_mag.y += this->magneto->direction.y/float(MAG_OFFSET_SAMPLE);

        if (this->initial_counter == MAG_OFFSET_SAMPLE){
            float size_initial = sqrt(device_state.initial_mag.x*device_state.initial_mag.x + device_state.initial_mag.y*device_state.initial_mag.y);
            device_state.initial_mag.x = device_state.initial_mag.x/size_initial;
            device_state.initial_mag.y = device_state.initial_mag.y/size_initial;
        }

        this->initial_counter++;
    } else {
        device_state.magneto.x = low_pass_filter(this->magneto->direction.x, &this->old_mag.x, MAG_MEASURED_WEIGHT);
        device_state.magneto.y = low_pass_filter(this->magneto->direction.y, &this->old_mag.y, MAG_MEASURED_WEIGHT);
        device_state.magneto.z = low_pass_filter(this->magneto->direction.z, &this->old_mag.z, MAG_MEASURED_WEIGHT);
    }


    this->azimuth = this->calc_azimuth(device_state.magneto.y, device_state.magneto.x);
}

float MagnetometerSensor::calc_azimuth(float a, float b)
{
    float az = atan2(a, b) * 180.0 / PI;
    return az < 0 ? 360 + az : az;
}

#endif