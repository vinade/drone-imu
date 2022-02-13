#ifndef GPS_SENSOR_CPP
#define GPS_SENSOR_CPP

#include "gps.h"

void GPSSensor::setup()
{
    gps_serial.begin(9600);
}

void GPSSensor::update()
{

    while (gps_serial.available())
    {
        this->gps.encode(gps_serial.read());
    }

    if (this->gps.location.isUpdated())
    {
        this->coords.x = this->gps.location.lat();
        this->coords.y = this->gps.location.lng();
        this->coords.z = this->gps.altitude.meters();
        this->satellites = this->gps.satellites.value();

        if (!this->ready)
        {
            if (abs(this->coords.x) > 1.0)
            {
                this->ready = true;
            }
        }
    }
}
#endif
