#ifndef QMC5883L_SENSOR_CPP
#define QMC5883L_SENSOR_CPP

#include "qmc5883l.h"

QMC5883LSensor::QMC5883LSensor()
{
    this->offset.x = 0;
    this->offset.y = 0;
    this->offset.z = 0;
}

void QMC5883LSensor::setup_instance()
{
#ifndef CALIBRATION_SCRIPT
    read_from_eeprom(EEPROM_MAGNETO_OFFSET, this->offset);
    if (isnan(this->offset.x))
    {
        this->offset.x = 0;
        this->offset.y = 0;
        this->offset.z = 0;
    }

    Serial.print("\toffsetX:");
    Serial.print(this->offset.x);
    Serial.print("\toffsetY:");
    Serial.print(this->offset.y);
    Serial.print("\toffsetZ:");
    Serial.print(this->offset.z);
#endif

    this->write(0x0B, 0x01);
    this->set_mode(QMC5883L_MODE_CONTINUOUS, QMC5883L_ODR_200HZ, QMC5883L_RNG_8G, QMC5883L_OSR_512);
}

void QMC5883LSensor::set_mode(uint16_t mode, uint16_t odr, uint16_t rng, uint16_t osr)
{
    this->write(0x09, mode | odr | rng | osr);
}

void QMC5883LSensor::soft_reset()
{
    this->write(0x0A, 0x80);
}

void QMC5883LSensor::update()
{
    int16_t x, y, z;

    #ifndef CALIBRATION_SCRIPT
    float fx ,fy, fz, size;
    #endif

    Wire.beginTransmission(this->address);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(this->address, 6);

    x = Wire.read();
    x |= Wire.read() << 8;

    y = Wire.read();
    y |= Wire.read() << 8;

    z = Wire.read();
    z |= Wire.read() << 8;

    #ifndef CALIBRATION_SCRIPT
        fx = float(x) - this->offset.x;
        fy = float(y) - this->offset.y;
        fz = float(z) - this->offset.z;

        size = sqrt(fx*fx + fy*fy + fz*fz);
        this->direction.x = fx / size;
        this->direction.y = fy / size;
        this->direction.z = fz / size;
    #else
        this->direction.x = float(x);
        this->direction.y = float(y);
        this->direction.z = float(z);
    #endif
}

#ifdef CALIBRATION_SCRIPT

void QMC5883LSensor::set_min_max(float value, float *min, float *max)
{
    if (*min > value)
    {
        *min = value;
    }
    else if (*max < value)
    {
        *max = value;
    }
}

void QMC5883LSensor::check_non_zero(float fabs_value, boolean *zero_flag, uint8_t *counter)
{
    if (*zero_flag)
    {
        if (fabs_value > 200)
        {
            *zero_flag = false;
            (*counter)++;
        }
    }
    else
    {
        *zero_flag = (fabs_value < 40);
    }
}

void QMC5883LSensor::calibrate()
{

    vec3float value_max = {0, 0, 0};
    vec3float value_min = {0, 0, 0};

    float x_fabs;
    float y_fabs;
    float z_fabs;
    uint8_t x_counter = 0;
    uint8_t y_counter = 0;
    uint8_t z_counter = 0;

    boolean x_zero = false;
    boolean y_zero = false;
    boolean z_zero = false;

    uint8_t min_counter = 5;
    while (x_counter < min_counter || y_counter < min_counter || z_counter < min_counter)
    {

        this->update();

        x_fabs = fabs(this->direction.x);
        y_fabs = fabs(this->direction.y);
        z_fabs = fabs(this->direction.z);


        if ((x_fabs > 600) || (y_fabs > 600) || (z_fabs > 600))
        {
            continue;
        }

        this->set_min_max(this->direction.x, &value_min.x, &value_max.x);
        this->set_min_max(this->direction.y, &value_min.y, &value_max.y);
        this->set_min_max(this->direction.z, &value_min.z, &value_max.z);

        this->check_non_zero(x_fabs, &x_zero, &x_counter);
        this->check_non_zero(y_fabs, &y_zero, &y_counter);
        this->check_non_zero(z_fabs, &z_zero, &z_counter);

        delay(30);
    }

    this->offset.x = (value_max.x + value_min.x) / 2;
    this->offset.y = (value_max.y + value_min.y) / 2;
    this->offset.z = (value_max.z + value_min.z) / 2;
}
#endif

#endif
