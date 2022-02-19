#ifndef MPU6050_SENSOR_CPP
#define MPU6050_SENSOR_CPP

#include "mpu6050.h"

MPU6050Sensor::MPU6050Sensor(int instance_address)
{
    this->address = instance_address;

    this->gyro_offset.x = 0;
    this->gyro_offset.y = 0;
    this->gyro_offset.z = 0;
}

void MPU6050Sensor::setup_instance()
{
    this->write(MPU6050_SMPLRT_DIV, 0x00);
    this->write(MPU6050_CONFIG, 0x00);
    this->write(MPU6050_GYRO_CONFIG, 0x08);
    this->write(MPU6050_ACCEL_CONFIG, 0x00);
    this->write(MPU6050_PWR_MGMT_1, 0x01);

#ifndef CALIBRATION_SCRIPT
    int offset_address = this->address == I2C_ADDRESS_MPU6050_A ? EEPROM_GYRO_A_OFFSET : EEPROM_GYRO_B_OFFSET;
    read_from_eeprom(offset_address, this->gyro_offset);

    if (isnan(this->gyro_offset.x))
    {
        this->gyro_offset.x = 0;
        this->gyro_offset.y = 0;
        this->gyro_offset.z = 0;
    }
#endif
}

void MPU6050Sensor::update()
{

    Wire.beginTransmission(this->address);
    Wire.write(0x3B);
    Wire.endTransmission();
    Wire.requestFrom(this->address, 14);

    if (Wire.available() == 14)
    {
        int16_t ax, ay, az;
        int16_t gx, gy, gz;
        int16_t t;

        /*
            A troca dos eixos x e y, e inversão do eixo x, é ncessária para a conversão do sistema
            de coordenadas do sensor para o sistema de coordenadas do drone
        */

        ay = Wire.read() << 8 | Wire.read();
        ax = Wire.read() << 8 | Wire.read();
        az = Wire.read() << 8 | Wire.read();

        t = Wire.read() << 8 | Wire.read();

        gy = Wire.read() << 8 | Wire.read();
        gx = Wire.read() << 8 | Wire.read();
        gz = Wire.read() << 8 | Wire.read();

        this->temperature = (t + 12412.0) / 340.0;

        this->accel.x = ((float)ax) / 16384.0;
        this->accel.y = -((float)ay) / 16384.0;
        this->accel.z = -((float)az) / 16384.0;

        this->gyro.x = ((float)gx) / 65.5;
        this->gyro.y = -((float)gy) / 65.5;
        this->gyro.z = -((float)gz) / 65.5;

        this->gyro.x -= this->gyro_offset.x;
        this->gyro.y -= this->gyro_offset.y;
        this->gyro.z -= this->gyro_offset.z;
    }
}

#ifdef CALIBRATION_SCRIPT

void MPU6050Sensor::calibrate(unsigned int sample_size)
{
    float x = 0, y = 0, z = 0;
    int16_t rx, ry, rz;

    for (int i = 0; i < sample_size; i++)
    {
        Wire.beginTransmission(this->address);
        Wire.write(0x43);
        Wire.endTransmission(false);
        Wire.requestFrom(this->address, 6);

        ry = Wire.read() << 8 | Wire.read();
        rx = Wire.read() << 8 | Wire.read();
        rz = Wire.read() << 8 | Wire.read();

        x += ((float)rx) / 65.5;
        y -= ((float)ry) / 65.5;
        z += ((float)rz) / 65.5;
    }

    this->gyro_offset.x = x / 3000;
    this->gyro_offset.y = y / 3000;
    this->gyro_offset.z = z / 3000;
}

#endif

#endif