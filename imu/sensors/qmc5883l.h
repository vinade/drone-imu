#ifndef QMC5883L_SENSOR_H
#define QMC5883L_SENSOR_H

// https://www.electronoobs.com/eng_arduino_tut77.php

#include "i2c.h"
#include "../essential/eeprom.h"

#define QMC5883L_MODE_STANDBY 0b00000000
#define QMC5883L_MODE_CONTINUOUS 0b00000001

#define QMC5883L_ODR_10HZ 0b00000000
#define QMC5883L_ODR_50HZ 0b00000100
#define QMC5883L_ODR_100HZ 0b00001000
#define QMC5883L_ODR_200HZ 0b00001100

#define QMC5883L_RNG_2G 0b00000000
#define QMC5883L_RNG_8G 0b00010000

#define QMC5883L_OSR_512 0b00000000
#define QMC5883L_OSR_256 0b01000000
#define QMC5883L_OSR_128 0b10000000
#define QMC5883L_OSR_64 0b11000000

class QMC5883LSensor : public I2CSensor
{
private:
    void soft_reset();
    void set_mode(uint16_t mode, uint16_t odr, uint16_t rng, uint16_t osr);

public:
    QMC5883LSensor::QMC5883LSensor();
    vec3float offset;
    vec3float direction;

    int address = I2C_ADDRESS_QMC5883L;

    void setup_instance();
    void update();
    int get_address() { return this->address; };

#ifdef CALIBRATION_SCRIPT
    void check_non_zero(float fabs_value, boolean *zero_flag, uint8_t *counter);
    void set_min_max(float value, float *min, float *max);
    void calibrate();
#endif
};

#endif
