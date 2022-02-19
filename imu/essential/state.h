#ifndef STATE_H
#define STATE_H

#include "util.h"

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
} StateData;

StateData device_state;

#endif
