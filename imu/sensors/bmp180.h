#ifndef BMP180_SENSOR_H
#define BMP180_SENSOR_H

/*
* Referência: https://osoyoo.com/2018/01/17/arduino-lesson-barometric-pressure-sensor-bmp-180/
* The BMP180 must be in contact with the ambient air to carry out the measurements. If you need to integrate the sensor into a housing, do not forget to provide holes for air circulation.
* Do not expose the BMP180 excessively to the airflow of a fan, as this may result in erroneous or very fluctuating measurements.
* The measurement of the atmospheric pressure depends on the temperature. Avoid placing the BMP180 in front of a source of heat, still less in front of a source producing rapid changes (heating, window in full sun …).
* The BMP180 is sensitive to moisture and is not intended for direct contact with water.
* It is also sensitive to light. It should be protected as much as possible from ambient light. Do not place the sensor in front of the ventilation hole of your case, for example.
* The BMP180 accepts a supply voltage between 1.8 and 3.6 Volts. The best way is to use the 3.3V output of your Arduino without ever exceeding 3.6V (according to Sparkfun).
*/

#define BMP180_OSS 3
#define BMP180_OSD 26

#include "i2c.h"

#define BMP180_SEA_LEVEL_PRESSURE 1030

class BMP180Sensor : public I2CSensor
{
private:
    int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
    uint16_t ac4, ac5, ac6;
    // int32_t b5;

    // float temperature;
    // float pressure;
    boolean has_first_altitude = false;
    float first_altitude = 0;

public:
    int address = I2C_ADDRESS_BMP180;
    float height;
    float altitude;

    int32_t read_pressure();

    float get_temperature(int32_t *b5);
    float get_pressure(int32_t b5);

    void update_altitude(float temperature, float pressure);
    void update();
    // void set_sea_level_pressure();
    // void *get_data();
    void setup_instance();
    int get_address() { return this->address; }
};

#endif
