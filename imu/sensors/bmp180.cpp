#ifndef BMP180_SENSOR_CPP
#define BMP180_SENSOR_CPP

#include "bmp180.h"

void BMP180Sensor::setup_instance()
{
    this->ac1 = this->read_2_bytes(0xAA);
    this->ac2 = this->read_2_bytes(0xAC);
    this->ac3 = this->read_2_bytes(0xAE);
    this->ac4 = this->read_2_bytes(0xB0);
    this->ac5 = this->read_2_bytes(0xB2);
    this->ac6 = this->read_2_bytes(0xB4);
    this->b1 = this->read_2_bytes(0xB6);
    this->b2 = this->read_2_bytes(0xB8);
    this->mb = this->read_2_bytes(0xBA);
    this->mc = this->read_2_bytes(0xBC);
    this->md = this->read_2_bytes(0xBE);

    delay(200);
}

// int32_t BMP180Sensor::update_temperature()
float BMP180Sensor::get_temperature(int32_t *b5)
{
    int32_t x1, x2, UT;

    Wire.beginTransmission(this->address);
    Wire.write(0xf4);
    Wire.write(0x2e);
    Wire.endTransmission();

    delay(50);

    UT = read_2_bytes(0xf6);

    x1 = (UT - (int32_t)this->ac6) * (int32_t)this->ac5 >> 15;
    x2 = ((int32_t)this->mc << 11) / (x1 + (int32_t)this->md);

    *b5 = x1 + x2;

    float temperature = (*b5 + 8) >> 4;
    return temperature / 10.0;
}

int32_t BMP180Sensor::read_pressure()
{
    int32_t value;
    Wire.beginTransmission(this->address);
    Wire.write(0xf4);
    Wire.write(0x34 + (BMP180_OSS << 6));
    Wire.endTransmission();

    delay(BMP180_OSD);

    Wire.beginTransmission(this->address);
    Wire.write(0xf6);
    Wire.endTransmission();
    Wire.requestFrom(this->address, 3);

    if (Wire.available() >= 3)
    {
        value = (((int32_t)Wire.read() << 16) | ((int32_t)Wire.read() << 8) | ((int32_t)Wire.read())) >> (8 - BMP180_OSS);
    }

    return value;
}

float BMP180Sensor::get_pressure(int32_t b5)
{
    int32_t x1, x2, x3, b3, b6, p, UP;
    uint32_t b4, b7;

    UP = read_pressure(); // dado cru do sensor

    b6 = b5 - 4000;

    x1 = (this->b2 * (b6 * b6) >> 12) >> 11;
    x2 = (this->ac2 * b6) >> 11;
    x3 = x1 + x2;
    b3 = (((((long)this->ac1) * 4 + x3) << BMP180_OSS) + 2) >> 2;

    x1 = (this->ac3 * b6) >> 13;
    x2 = (this->b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (this->ac4 * (unsigned long)(x3 + 32768)) >> 15;

    b7 = ((unsigned long)(UP - b3) * (50000 >> BMP180_OSS));

    p = (b7 < 0x80000000) ? (b7 << 1) / b4 : p = (b7 / b4) << 1;

    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p += (x1 + x2 + 3791) >> 4;

    return p / 100.0f;
}

/*
    P0 = pressao nível do mar em hPa
    P = pressao em hPa
    T = temperatura em Celsius

    N = (P0/P)^(1/5.257) - 1
        N = (P0/P)^0.19022256039 - 1
    K = T + 273.15
    altitude = (N * K)/0.0065

    altitude em metros

*/
void BMP180Sensor::update_altitude(float temperature, float pressure)
{
    float k = temperature + 273.15;
    float n = BMP180_SEA_LEVEL_PRESSURE / pressure;
    n = pow(n, 0.19022256039) - 1;
    this->altitude = (n * k) / 0.0065;

    if (!this->has_first_altitude)
    {
        this->first_altitude = this->altitude;
        this->has_first_altitude = true;
    }

    this->height = this->altitude - this->first_altitude;
}

void BMP180Sensor::update()
{
    int32_t component_b5;

    float temperature = this->get_temperature(&component_b5);
    float pressure = this->get_pressure(component_b5);

    this->update_altitude(temperature, pressure);
}

#endif
