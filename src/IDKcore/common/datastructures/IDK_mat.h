#pragma once

#include "../IDKdecl.h"


template <int size>
float *idk::value_ptr(idk::mat_t<size> &mat)
{
    return mat[0];
}


template <int size>
class idk::mat_t
{
private:
    float *_data;

public:
    mat_t(): _data(new float[size]) {  };

    mat_t(float f): mat_t()
    {
        for (int i=0; i<size*size; i++)
        {
            if (i%(size+1) == 0)
                _data[i] = f;
            else
                _data[i] = 0.0f;
        }
    };

    float *operator [] (int i)
    {
        return _data + size*i;
    };
};


