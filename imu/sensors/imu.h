#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#include "mpu6050.cpp"
#include "magnetometer.cpp"
#define GYR_ACC_K 0.5
#define GYR_MAG_K 0.5

#define REF_MEASURED_WEIGHT 0.5
#define ACC_MEASURED_WEIGHT 1
#define GYR_MEASURED_WEIGHT 1
#define GYR_OFFSET_SAMPLE 100
#define REF_MUL_SAMPLE 2

class IMUSensor : public BasicSensor
{
private:
    MPU6050Sensor *mpu_a = NULL;
    MPU6050Sensor *mpu_b = NULL;
    MagnetometerSensor *magneto = NULL;
    int gyr_initial_counter = 0;

    vec3float old_acc = {0, 0, 0};
    vec3float old_gyr = {0, 0, 0};
    vec3float offset_gyr = {0, 0, 0};
    vec3float offset_accel = {0, 0, 0};
    vec3float last_accel = {0, 0, 0};
    vec3float offset_reference_accel = {0, 0, 0};
    vec3float old_acc_ref = {0, 0, 0};

    vec3float max_deviation_accel = {0, 0, 0};

    long last_sample;

    float dt();

public:
    IMUSensor(){};
    IMUSensor(MPU6050Sensor *a, MPU6050Sensor *b, MagnetometerSensor *m);
    void setup_instance();
    void update();
    void update_reference();
    void reset_reference();
};

#endif