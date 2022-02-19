#ifndef VEC3FLOAT_CPP
#define VEC3FLOAT_CPP

#include "vec3float.h"


inline vec3float& vec3float::operator=(const float s){
    this->x = s;
    this->y = s;
    this->z = s;
    return *this;
}

inline vec3float& vec3float::operator=(const vec3float& v){
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    return *this;
}

inline vec3float& vec3float::operator+=(const vec3float& v){
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
}

inline vec3float& vec3float::operator-=(const vec3float& v){
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
}

float vec3float::size(){
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

void vec3float::rotate_X(float angle){
    float ny = this->y;
    float nz = this->z;
    float cosa = cos(angle);
    float sina = sin(angle);
    this->y = ny * cosa - nz * sina;
    this->z = ny * sina + nz * cosa;
}

void vec3float::rotate_Y(float angle){
    float nx = this->x;
    float nz = this->z;
    float cosa = cos(angle);
    float sina = sin(angle);
    this->x = nx * cosa + nz * sina;
    this->z = -nx * sina + nz * cosa;
}

void vec3float::rotate_Z(float angle){
    float nx = this->x;
    float ny = this->y;
    float cosa = cos(angle);
    float sina = sin(angle);
    this->x = nx * cosa - ny * sina;
    this->y = nx * sina + ny * cosa;
}

vec3float vec3float::unit(){
    float s = this->size();
    return *this / s;
}

inline const vec3float operator+(const vec3float& v1, const float s) {
    vec3float res {
        v1.x + s,
        v1.y + s,
        v1.z + s
    };
    return res;
}

inline const vec3float operator-(const vec3float& v1, const float s) {
    vec3float res {
        v1.x - s,
        v1.y - s,
        v1.z - s
    };
    return res;
}

inline const vec3float operator+(const vec3float& v1, const vec3float& v2) {
    vec3float res {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };
    return res;
}

inline const vec3float operator-(const vec3float& v1, const vec3float& v2) {
    vec3float res {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    };
    return res;
}

inline const vec3float operator*(const vec3float& v1, const float s) {
    vec3float res {
        v1.x * s,
        v1.y * s,
        v1.z * s
    };
    return res;
}

inline const vec3float operator/(const vec3float& v1, const float s) {
    vec3float res {
        v1.x / s,
        v1.y / s,
        v1.z / s
    };
    return res;
}

inline const vec3float operator*(const float v, vec3float& vector) {
    return vector * v;
}

inline const vec3float operator+(const float v, vec3float& vector) {
    return vector + v;
}

inline const vec3float operator-(const float v, vec3float& vector) {
    return vector - v;
}

#endif
