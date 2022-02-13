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

#endif
