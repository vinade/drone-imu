#ifndef MAGNETOMETER_SENSOR_H
#define MAGNETOMETER_SENSOR_H

#include "qmc5883l.cpp"

#define MAG_MEASURED_WEIGHT 0.3
#define MAG_OFFSET_SAMPLE 100

class MagnetometerSensor : public BasicSensor
{
private:
    QMC5883LSensor *magneto = NULL;
    vec3float old_mag = {0, 0, 0};
    int initial_counter = 0;

    float calc_azimuth(float a, float b);

public:
    float azimuth;
    MagnetometerSensor();
    MagnetometerSensor(QMC5883LSensor *m);
    void update();
};

#endif