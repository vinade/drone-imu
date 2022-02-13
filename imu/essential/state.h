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
    vec3float raw_magneto; // 48
    vec3float initial_mag = {0, 0, 0}; // 48

    float height;
    float distance_ground; // 56 bytes
} StateData;

StateData device_state;

#endif
