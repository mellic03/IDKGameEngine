#pragma once

#include "../IDKdecl.h"


template <typename T, int size>
class idk::vec_t { };


template <typename T, int size>
T *idk::value_ptr(idk::vec_t<T, size> &v)
{
    return &v.x;
}


template <typename T>
class idk::vec_t<T, 2>
{
private:

public:
    union { T x, r; };
    union { T y, g; };
    vec_t(T x, T y)
    {
        this->x = x;  this->y = y;
    };
    vec_t(T f): vec_t(f, f) {  };
    vec_t() {  };
};


template <typename T>
class idk::vec_t<T, 3>
{
private:

public:
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    vec_t(T x, T y, T z)
    {
        this->x = x;  this->y = y;  this->z = z;
    };
    vec_t(T f): vec_t(f, f, f) {  };
    vec_t() {  };
};


template <typename T>
class idk::vec_t<T, 4>
{
private:

public:
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };
    vec_t(T x, T y, T z, T w)
    {
        this->x = x;  this->y = y;  this->z = z;  this->w = w;
    };
    vec_t(T f): vec_t(f, f, f, f) {  };
    vec_t() {  };
};
