#pragma once

#include "../IDKdecl.h"


template <int size>
float *idk::raw_ptr(idk::mat_t<size> &mat)
{
    return mat[0];
}



template <int size>
class idk::mat_t
{
private:
    float *         _data;

public:
                    mat_t(): _data(new float[size*size]) {  };
                    mat_t(float f);
                    mat_t(const mat_t &other);
                    mat_t(mat_t &&other);
                    ~mat_t();

    mat_t &         operator = (const mat_t &other);
    mat_t &         operator = (mat_t &&other) noexcept;

    float *operator [] (int i)
    {
        return _data + size*i;
    };
};



template <int size>
idk::mat_t<size>::mat_t(float f): mat_t()
{
    for (int i=0; i<size*size; i++)
    {
        if (i%(size+1) == 0)
            _data[i] = f;
        else
            _data[i] = 0.0f;
    }
};



template <int size>
idk::mat_t<size>::mat_t(const mat_t<size> &other)
{
    _data = new float[size*size];

    for (size_t i=0; i<size; i++)
        _data[i] = other._data[i];
};


template <int size>
idk::mat_t<size>::mat_t(mat_t<size> &&other)
{
    _data = other._data;
    other._data = nullptr;
};


template <int size>
idk::mat_t<size>::~mat_t()
{
    delete[] _data;
}


template <int size>
idk::mat_t<size> &
idk::mat_t<size>::operator = (const idk::mat_t<size> &other)
{
    if (this == &other)
        return *this;

    if (_data == nullptr)
        _data = new float[size*size];

    for (size_t i=0; i<size; i++)
        _data[i] = other._data[i];

    return *this;
}


template <int size>
idk::mat_t<size> &
idk::mat_t<size>::operator = (idk::mat_t<size> &&other) noexcept
{
    if (this == &other)
        return *this;

    if (_data)
        delete[] _data;
    _data = other._data;
    other._data = nullptr;

    return *this;
}

