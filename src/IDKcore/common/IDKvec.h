#pragma once

#include "IDKdecl.h"


template <int size>
class IDK::vec_t { };


template <int size>
float *IDK::value_ptr(IDK::vec_t<size> &v)
{
    return &v.x;
}


template <>
class IDK::vec_t<2>
{
private:

public:
    union { float x, r; };
    union { float y, g; };
    vec_t(float x, float y)
    {
        this->x = x;  this->y = y;
    };
    vec_t(float f): vec_t(f, f) {  };
    vec_t() {  };
};


template <>
class IDK::vec_t<3>
{
private:

public:
    union { float x, r; };
    union { float y, g; };
    union { float z, b; };
    vec_t(float x, float y, float z)
    {
        this->x = x;  this->y = y;  this->z = z;
    };
    vec_t(float f): vec_t(f, f, f) {  };
    vec_t() {  };
};


template <>
class IDK::vec_t<4>
{
private:

public:
    union { float x, r; };
    union { float y, g; };
    union { float z, b; };
    union { float w, a; };
    vec_t(float x, float y, float z, float w)
    {
        this->x = x;  this->y = y;  this->z = z;  this->w = w;
    };
    vec_t(float f): vec_t(f, f, f, f) {  };
    vec_t() {  };
};
