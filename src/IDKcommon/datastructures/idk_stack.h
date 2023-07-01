#pragma once

#include <cstring>
#include "datastructures_decl.h"


template <typename T>
class idk::stack
{
private:
    T *_data;
    size_t _size, _cap;

    void _resize(size_t cap);

public:
                stack(): _size(0), _cap(1), _data(new T[1]) {  };
                ~stack() { if (_data) delete[] _data; };

    void        push(const T &data);
    T &         top()               { return _data[_size];   };
    T           pop()               { return _data[--_size]; };
    size_t      size() const        { return _size;          };
    bool        empty()             { return _size == 0;     };
};


template <typename T>
void
idk::stack<T>::_resize(size_t cap)
{
    T *temp = new T[_size];
    memcpy((void *)temp, (void *)_data, _size * sizeof(T));

    delete[] _data;
    _data = new T[cap];

    memcpy((void *)_data, (void *)temp, _size * sizeof(T));

    delete[] temp;
    _cap = cap;
}


template <typename T>
void
idk::stack<T>::push(const T &data)
{
    if (_size+1 > _cap)
        _resize(2*_cap);

    _data[_size] = data;
    _size += 1;
}

