#ifndef I2C_SENSOR_H
#define I2C_SENSOR_H

#include <Wire.h>
#include "basicSensor.cpp"

#define I2C_ADDRESS_UNKNOWN 0x00   // NAO INICIALIZADO
#define I2C_ADDRESS_BMP180 0x77    // barometro
#define I2C_ADDRESS_MPU6050_A 0x68 // giroscopio
#define I2C_ADDRESS_MPU6050_B 0x69 // giroscopio
#define I2C_ADDRESS_AT24C256 0x50  // eeprom
#define I2C_ADDRESS_HMC5883L 0x1E  // magnetometro
#define I2C_ADDRESS_QMC5883L 0x0D  // magnetometro

class I2CSensor : public BasicSensor
{
public:
    // int address = I2C_ADDRESS_UNKNOWN;
    uint8_t read_1_byte(uint8_t code);
    uint16_t read_2_bytes(uint8_t code);
    void write(uint8_t reg, uint8_t data);
    virtual int get_address();
};

uint8_t I2CSensor::read_1_byte(uint8_t code)
{
    int address = this->get_address();
    uint8_t value;
    Wire.beginTransmission(address);
    Wire.write(code);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);
    if (Wire.available() >= 1)
    {
        value = Wire.read();
    }
    return value;
}

uint16_t I2CSensor::read_2_bytes(uint8_t code)
{
    int address = this->get_address();
    uint16_t value;
    Wire.beginTransmission(address);
    Wire.write(code);
    Wire.endTransmission();
    Wire.requestFrom(address, 2);

    if (Wire.available() >= 2)
    {
        value = (Wire.read() << 8) | Wire.read();
    }
    return value;
}

void I2CSensor::write(uint8_t reg, uint8_t data)
{
    int address = this->get_address();
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

#endif
