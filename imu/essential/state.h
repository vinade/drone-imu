#ifndef STATE_H
#define STATE_H

#include "util.h"

#define STATUS_INITIAL 0
#define STATUS_SYSTEM_SETUP 1
#define STATUS_CONNECTING 2
#define STATUS_CALIBRATING_GYROS_ACCEL 3
#define STATUS_CALIBRATING_ACCEL_ESTIMATIVE 4
#define STATUS_READY 5


typedef struct struct_state
{
    // vec3float gyro;  // 12
    vec3float angle; // 12
    vec3float accel;
    vec3float coords;
    vec3float magneto; // 48
    vec3float accel_estimative_position;
    vec3float accel_estimative_velocity;

    float height;
    float distance_ground; // 56 bytes

    int status = STATUS_INITIAL;
} StateData;

StateData device_state;

#endif
