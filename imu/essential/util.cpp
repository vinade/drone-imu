#ifndef UTIL_CPP
#define UTIL_CPP

#include "util.h"

float low_pass_filter(float value, float *acc, float weight)
{
    float aux;

    aux = (1.0 - weight) * (*acc) + weight * value;
    *acc = aux;

    return aux;
}

vec3float low_pass_filter(vec3float value, vec3float& acc, float weight)
{
    vec3float aux;

    aux = (1.0 - weight) * acc + weight * value;
    acc = aux;

    return aux;
}

float min_threshold(float value, float limit)
{
    if (abs(value) < limit){
        return 0;
    }

    return value;
}

inline void min_threshold(vec3float& value, vec3float limit)
{
    value.x = (abs(value.x) < limit.x)? 0 : value.x;
    value.y = (abs(value.y) < limit.y)? 0 : value.y;
    value.z = (abs(value.z) < limit.z)? 0 : value.z;
}

#endif
