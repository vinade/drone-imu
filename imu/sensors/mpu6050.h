#ifndef MPU6050_SENSOR_H
#define MPU6050_SENSOR_H

#include "i2c.h"
#include "../essential/eeprom.h"

#define MPU6050_SMPLRT_DIV 0x19
#define MPU6050_CONFIG 0x1a
#define MPU6050_GYRO_CONFIG 0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I 0x75
#define MPU6050_PWR_MGMT_1 0x6b
#define MPU6050_TEMP_H 0x41
#define MPU6050_TEMP_L 0x42
#define MPU6050_STATIC_SAMPLE 3000

#define MPU6050_A 2
#define MPU6050_B 3
#define MPU6050_NONE 255

class MPU6050Sensor : public I2CSensor
{

public:
    float temperature;
    vec3float accel;
    vec3float gyro;
    vec3float gyro_offset;

    int address = I2C_ADDRESS_MPU6050_A;

    MPU6050Sensor(int instance_address);
    void setup_instance();
    void update();
    int get_address() { return this->address; }

#ifdef CALIBRATION_SCRIPT
    void calibrate(unsigned int sample_size);
#endif
};

#endif