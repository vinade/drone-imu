#ifndef HCSR04_SENSOR_CPP
#define HCSR04_SENSOR_CPP

#include "hcsr04.h"

HCSR04Sensor::HCSR04Sensor(uint8_t echo, uint8_t ping)
{
    this->ping_pin = ping;
    this->echo_pin = echo;
};

void HCSR04Sensor::setup_instance()
{
    pinMode(this->ping_pin, OUTPUT);
    pinMode(this->echo_pin, INPUT);
}

void HCSR04Sensor::update()
{
    digitalWrite(this->ping_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(this->ping_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->ping_pin, LOW);

    long microseconds = pulseIn(this->echo_pin, HIGH);
    //    this->height = (float)(microseconds / 29 / 2) / 100.0;
    this->height = (float)(microseconds / 58) / 100.0;

    this->warning = 0;
    // if (DroneSensors->gyroscope->available)
    // {
    //     vec3float *gyro = DroneSensors->gyroscope->read();
    //     this->height = this->height * (cos(gyro->x) + sen(gyro->y) - sen(gyro->z) * cos(gyro->z));
    // }
    // else
    // {
    //     this->warning = SENSOR_NOT_HEALTH;
    // }

    if (this->height > 10.0)
    {
        this->warning |= SENSOR_NOT_USEFUL;
    }
}

#endif
