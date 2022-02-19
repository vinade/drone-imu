/*
Referências
https://students.iitk.ac.in/roboclub/2017/12/21/Beginners-Guide-to-IMU.html
https://engineering.stackexchange.com/questions/3348/calculating-pitch-yaw-and-roll-from-mag-acc-and-gyro-data
*/

#ifndef IMU_SENSOR_CPP
#define IMU_SENSOR_CPP
#define TO_DEGREES 57.2957795131
#define TO_RAD 0.01745277777

#include "imu.h"

IMUSensor::IMUSensor(MPU6050Sensor *a, MPU6050Sensor *b, MagnetometerSensor *m)
{
    this->mpu_a = a;
    this->mpu_b = b;
    this->magneto = m;
    this->last_sample = millis();
}

void IMUSensor::setup_instance()
{
    if (this->mpu_a == NULL)
    {
        this->mpu_a = new MPU6050Sensor(I2C_ADDRESS_MPU6050_A);
        this->mpu_a->setup_instance();
    }

    if (this->mpu_b == NULL)
    {
        this->mpu_b = new MPU6050Sensor(I2C_ADDRESS_MPU6050_B);
        this->mpu_b->setup_instance();
    }

    if (this->magneto == NULL)
    {
        this->magneto = new MagnetometerSensor();
        this->magneto->setup_instance();
    }

    this->reset_reference();
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
    vec3float g, a;
    float mx, my, interval;
    float angleAccX, angleAccY;
    float roll, pitch, yaw;
    MPU6050Sensor *ga = this->mpu_a;
    MPU6050Sensor *b = this->mpu_b;
    MagnetometerSensor *m = this->magneto;

    this->mpu_a->try_update();
    this->mpu_b->try_update();
    this->magneto->try_update();

    a = (this->mpu_a->accel + this->mpu_b->accel) / 2.0;
    g = (this->mpu_a->gyro + this->mpu_b->gyro) / 2.0;

    device_state.accel = low_pass_filter(a, this->old_acc, ACC_MEASURED_WEIGHT);

    if (this->gyr_initial_counter <= GYR_OFFSET_SAMPLE){
        this->offset_gyr += g/float(GYR_OFFSET_SAMPLE);
        this->offset_accel += a/float(GYR_OFFSET_SAMPLE);
        this->gyr_initial_counter++;
    } else {
        g = low_pass_filter(g - this->offset_gyr, this->old_gyr, GYR_MEASURED_WEIGHT);
    }

    angleAccY = atan2(device_state.accel.y, - device_state.accel.z + abs(device_state.accel.x)/2.0);
    angleAccX = atan2(device_state.accel.x, sqrt(device_state.accel.z * device_state.accel.z + device_state.accel.y * device_state.accel.y) - abs(device_state.accel.y)/3.0);

    angleAccX = angleAccX * TO_DEGREES;
    angleAccY = angleAccY * TO_DEGREES;

    interval = this->dt();
    device_state.angle.x = (GYR_ACC_K * (device_state.angle.x + g.x * interval)) + ((1.0 - GYR_ACC_K) * angleAccX);
    device_state.angle.y = (GYR_ACC_K * (device_state.angle.y + g.y * interval)) + ((1.0 - GYR_ACC_K) * angleAccY);

    a = device_state.magneto;
    roll = device_state.angle.x * TO_RAD;
    pitch = device_state.angle.y * TO_RAD;

    mx = a.x * cos(pitch) + a.y * sin(pitch) * sin(roll) + a.z * sin(pitch) * cos(roll);
    my = -a.y * cos(roll) + a.z * sin(roll);
    yaw = atan2(my, mx);
    device_state.angle.z = (GYR_MAG_K * (device_state.angle.z + g.z * interval)) + ((1.0 - GYR_MAG_K) * yaw * TO_DEGREES);

    if (this->gyr_initial_counter > GYR_OFFSET_SAMPLE){
        this->update_reference();
    }

    this->last_sample = millis();
}


void IMUSensor::reset_reference(){
    device_state.accel_estimative_position = 0;
    device_state.accel_estimative_velocity = 0;
    this->last_sample = millis();
}

void IMUSensor::update_reference(){
    float half_interval;
    vec3float gravity = this->offset_accel; // inverter o vetor de aceleração
    vec3float angle = device_state.angle * TO_RAD;
    vec3float reference_accel = device_state.accel;
    vec3float last_velocity = device_state.accel_estimative_velocity;

    gravity.rotate_X(angle.y);
    gravity.rotate_Y(-angle.x);

    reference_accel = reference_accel * -1;
    reference_accel += gravity;

    if (this->gyr_initial_counter <= 2 * GYR_OFFSET_SAMPLE){
        this->offset_reference_accel += reference_accel/float(GYR_OFFSET_SAMPLE);
        this->gyr_initial_counter++;
        return;
    }

    reference_accel -= this->offset_reference_accel;
    reference_accel = low_pass_filter(reference_accel, this->old_acc_ref, REF_MEASURED_WEIGHT);

    reference_accel.rotate_X(-angle.y);
    reference_accel.rotate_Y(angle.x);
    // reference_accel.rotate_Z(angle.z);

    half_interval = this->dt()/2.0;
    device_state.accel_estimative_velocity += (reference_accel + this->last_accel) * half_interval;
    device_state.accel_estimative_position += (last_velocity + device_state.accel_estimative_velocity) * half_interval;
    this->last_accel = reference_accel;

}

float IMUSensor::dt(){
    return (millis() - this->last_sample) * 0.001;
}

#endif