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

#endif
