#ifndef VEC3FLOAT_H
#define VEC3FLOAT_H

#include <math.h>

typedef struct st_vec3float
{
    float x;
    float y;
    float z;

    inline st_vec3float& operator=(const float);
    inline st_vec3float& operator=(const st_vec3float&);
    inline st_vec3float& operator+=(const st_vec3float&);
    inline st_vec3float& operator-=(const st_vec3float&);
    float size();
    st_vec3float unit();

    void rotate_X(float);
    void rotate_Y(float);
    void rotate_Z(float);

} vec3float;

inline const vec3float operator+(const vec3float&, const float);
inline const vec3float operator-(const vec3float&, const float);
inline const vec3float operator+(const vec3float&, const vec3float&);
inline const vec3float operator-(const vec3float&, const vec3float&);
inline const vec3float operator*(const vec3float&, const float);
inline const vec3float operator/(const vec3float&, const float);
inline const vec3float operator*(const float, vec3float&);
inline const vec3float operator+(const float, vec3float&);
inline const vec3float operator-(const float, vec3float&);

#endif
