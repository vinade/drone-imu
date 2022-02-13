/*
https://students.iitk.ac.in/roboclub/2017/12/21/Beginners-Guide-to-IMU.html
https://engineering.stackexchange.com/questions/3348/calculating-pitch-yaw-and-roll-from-mag-acc-and-gyro-data



*/

#ifndef IMU_SENSOR_CPP
#define IMU_SENSOR_CPP
#define TO_DEGREES 57.2957795131

#include "imu.h"

IMUSensor::IMUSensor(MPU6050Sensor *a, MPU6050Sensor *b, MagnetometerSensor *m)
{
    this->gyro_a = a;
    this->gyro_b = b;
    this->magneto = m;
    this->last_sample = millis();
}

void IMUSensor::setup_instance()
{
    if (this->gyro_a == NULL)
    {
        this->gyro_a = new MPU6050Sensor(I2C_ADDRESS_MPU6050_A);
        this->gyro_a->setup_instance();
    }

    if (this->gyro_b == NULL)
    {
        this->gyro_b = new MPU6050Sensor(I2C_ADDRESS_MPU6050_B);
        this->gyro_b->setup_instance();
    }

    if (this->magneto == NULL)
    {
        this->magneto = new MagnetometerSensor();
        this->magneto->setup_instance();
    }

    this->last_sample = millis();
}

float geometric_module(float a, int b)
{
    float frac = a - (int)a;
    float m = ((int)a) % b;
    m = m + frac;

    if (a < 0)
    {
        return b + m;
    }

    return m;
}

void IMUSensor::update()
{
    float gx, gy, gz;
    float ax, ay, az;
    MPU6050Sensor *a = this->gyro_a;
    MPU6050Sensor *b = this->gyro_b;
    MagnetometerSensor *m = this->magneto;

    a->try_update();
    b->try_update();
    m->try_update();

    ax = (a->accel.x + b->accel.x) / 2.0;
    ay = (a->accel.y + b->accel.y) / 2.0;
    az = (a->accel.z + b->accel.z) / 2.0;

    device_state.accel.x = low_pass_filter(ax, &this->old_acc.x, ACC_MEASURED_WEIGHT);
    device_state.accel.y = low_pass_filter(ay, &this->old_acc.y, ACC_MEASURED_WEIGHT);
    device_state.accel.z = low_pass_filter(az, &this->old_acc.z, ACC_MEASURED_WEIGHT);

    gx = (a->gyro.x + b->gyro.x) / 2.0;
    gy = (a->gyro.y + b->gyro.y) / 2.0;
    gz = (a->gyro.z + b->gyro.z) / 2.0;

    if (this->gyr_initial_counter <= GYR_OFFSET_SAMPLE){
        this->offset_gyr.x += gx/float(GYR_OFFSET_SAMPLE);
        this->offset_gyr.y += gy/float(GYR_OFFSET_SAMPLE);
        this->offset_gyr.z += gz/float(GYR_OFFSET_SAMPLE);
        this->gyr_initial_counter++;
    } else {
        gx = low_pass_filter(gx - this->offset_gyr.x, &this->old_gyr.x, GYR_MEASURED_WEIGHT);
        gy = low_pass_filter(gy - this->offset_gyr.y, &this->old_gyr.y, GYR_MEASURED_WEIGHT);
        gz = low_pass_filter(gz - this->offset_gyr.z, &this->old_gyr.z, GYR_MEASURED_WEIGHT);
    }

    // float angleAccX = atan2(device_state.accel.y, device_state.accel.z + abs(device_state.accel.x));
    // float angleAccY = atan2(device_state.accel.x, sqrt(device_state.accel.z * device_state.accel.z + device_state.accel.y * device_state.accel.y));

    float angleAccY = atan2(device_state.accel.y, device_state.accel.z + abs(device_state.accel.x));
    float angleAccX = atan2(device_state.accel.x, sqrt(device_state.accel.z * device_state.accel.z + device_state.accel.y * device_state.accel.y));

    // float angleAccX = atan2(device_state.accel.x, sqrt(device_state.accel.z * device_state.accel.z + device_state.accel.y * device_state.accel.y));
    // float angleAccY = atan2(device_state.accel.y, sqrt(device_state.accel.z * device_state.accel.z + device_state.accel.x * device_state.accel.x));

    ax = device_state.accel.x;
    ay = device_state.accel.y;
    az = device_state.accel.z;

    // float roll_acc = atan(ay / az);                        // * 360 / 2.0 / PI
    // float pitch_acc = atan(-ax / sqrt(ay * ay + az * az)); // * 360 / -2.0 / PI
    float pitch_acc = angleAccY; // * 360 / -2.0 / PI
    float roll_acc = angleAccX; // * 360 / 2.0 / PI
    angleAccX = angleAccX * 360 / 2.0 / PI;
    angleAccY = angleAccY * 360 / 2.0 / PI;

    float mx, my;
    ax = device_state.magneto.x;
    ay = device_state.magneto.y;
    az = device_state.magneto.z;

    // versao 1
    // mx = ax * cos(pitch_acc) + az * sin(pitch_acc);
    // my = ax * sin(roll_acc) * sin(pitch_acc) + ay * cos(roll_acc) - az * sin(roll_acc) * cos(pitch_acc);
    // float yaw_mag = atan(my, mx) * TO_DEGREES;

    // versao 2
    mx = ax * cos(pitch_acc) + ay * sin(roll_acc) * sin(pitch_acc) + az * sin(pitch_acc) * cos(roll_acc);
    my = ay * cos(roll_acc) - az * sin(roll_acc);
    float yaw_mag = atan2(-my, mx) * TO_DEGREES;

    // versao 3
    // mx = ax * cos(pitch_acc) - ay * sin(roll_acc) * sin(pitch_acc) + az * sin(pitch_acc) * cos(roll_acc);
    // my = ay * cos(roll_acc) + az * sin(roll_acc);
    // float yaw_mag = atan2(my, mx) * TO_DEGREES;

    // float yaw_mag = atan(my / mx) * TO_DEGREES * 2.0;
    // float yaw_mag = atan(my / mx) * TO_DEGREES;
    // roll_acc = roll_acc * TO_DEGREES;
    // pitch_acc = pitch_acc * -TO_DEGREES;

    // float normalized_yaw_mag = geometric_module(yaw_mag, 360);
    // float normalized_yaw = geometric_module(device_state.angle.z, 360);
    // float diff = normalized_yaw_mag - normalized_yaw;
    // float inv_diff;

    // if (normalized_yaw < normalized_yaw_mag)
    // {
    //     inv_diff = normalized_yaw + (360 - normalized_yaw_mag);
    //     diff = fabs(diff) < fabs(inv_diff) ? diff : -inv_diff;
    // }
    // else
    // {
    //     inv_diff = normalized_yaw_mag + (360 - normalized_yaw);
    //     diff = fabs(diff) < fabs(inv_diff) ? diff : inv_diff;
    // }

    // yaw_mag = device_state.angle.z + diff;

    float interval = (millis() - this->last_sample) * 0.001;

    // device_state.angle.x = (GYR_ACC_K * (device_state.angle.x + gx * interval)) + ((1.0 - GYR_ACC_K) * roll_acc);
    // device_state.angle.y = (GYR_ACC_K * (device_state.angle.y + gy * interval)) + ((1.0 - GYR_ACC_K) * pitch_acc);
    // device_state.angle.z = (GYR_MAG_K * (device_state.angle.z + gz * interval)) + ((1.0 - GYR_MAG_K) * yaw_mag);


    device_state.angle.x = (GYR_ACC_K * (device_state.angle.x + gx * interval)) + ((1.0 - GYR_ACC_K) * angleAccX);
    device_state.angle.y = (GYR_ACC_K * (device_state.angle.y + gy * interval)) + ((1.0 - GYR_ACC_K) * angleAccY);
    device_state.angle.z = (GYR_MAG_K * (device_state.angle.z + gz * interval)) + ((1.0 - GYR_MAG_K) * yaw_mag);

    // device_state.angle.x = 0;
    // device_state.angle.y = 0;
    // device_state.angle.z = 0;


/*

    float angleAccY = atan2(device_state.accel.y, device_state.accel.z + abs(device_state.accel.x));
    float angleAccX = atan2(device_state.accel.x, sqrt(device_state.accel.z * device_state.accel.z + device_state.accel.y * device_state.accel.y));
    float pitch_acc = angleAccY; // * 360 / -2.0 / PI
    float roll_acc = angleAccX; // * 360 / 2.0 / PI
    angleAccX = angleAccX * TO_DEGREES;
    angleAccY = angleAccY * TO_DEGREES;

    float interval = (millis() - this->last_sample) * 0.001;

    device_state.angle.x = (GYR_ACC_K * (device_state.angle.x + gx * interval)) + ((1.0 - GYR_ACC_K) * angleAccX);
    device_state.angle.y = (GYR_ACC_K * (device_state.angle.y + gy * interval)) + ((1.0 - GYR_ACC_K) * angleAccY);

    //temp
    ax = device_state.angle.x;
    device_state.angle.x = device_state.angle.y;
    device_state.angle.y = ax;

    ax = device_state.magneto.x;
    ay = device_state.magneto.y;
    az = device_state.magneto.z;
    roll = device_state.angle.x * TO_RAD;
    pitch = device_state.angle.y * TO_RAD;
    yaw = atan2((-ay * cos(roll) + az * sin(roll)), (ax * cos(pitch) + ay * sin(pitch) * sin(roll) + az * sin(pitch) * cos(roll)));
    device_state.angle.z = (GYR_MAG_K * (device_state.angle.z + gz * interval)) + ((1.0 - GYR_MAG_K) * yaw * TO_DEGREES);

*/


    this->last_sample = millis();
}

#endif