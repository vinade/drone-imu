#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H

#include <AltSoftSerial.h>
#include "TinyGPS++.h"
#include "basicSensor.h"

AltSoftSerial gps_serial;

class GPSSensor : public BasicSensor
{
private:
    TinyGPSPlus gps;

public:
    boolean ready = false;
    vec3float coords;
    int satellites;

    static void
    setup();
    void update();
};

#endif
