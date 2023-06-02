#pragma once

#include <cstring>
#include "IDKdecl.h"


template <typename T>
class IDK::vector
{
private:
    T *_data;
    size_t _size, _cap;

public:
                        vector(): _size(0), _cap(1), _data(new T[1]) {  };
                        vector(size_t size): _size(size), _cap(size), _data(new T[size]) {  };
                        ~vector() { delete[] _data; };

    void                resize(size_t size);
   
    void                push(T data);
    T                   pop()                   { return _data[--_size]; };
    size_t              size()  const           { return _size;          };
    bool                empty() const           { return _size == 0;     };
    void                clear()                 { _size = 0;             };
    void                erase()                 { resize(1); _size = 0; };

    T &                 front()                 { return _data[0];       };
    T &                 back()                  { return _data[_size-1]; };


                        class iterator;
    iterator            begin() { return iterator(_data); };
    iterator            end()   { return iterator(_data + _size); };
    

    T &                 operator [] (size_t i) { return _data[i]; };
};



template <typename T>
class IDK::vector<T>::iterator
{
private:
    T *_ptr;
public:
    iterator(T *ptr): _ptr(ptr) {  }
    iterator &operator ++ ()
    {
        _ptr += 1;
        return *this;
    };
    iterator &operator ++ (int)
    {
        iterator temp = *this;
        _ptr += 1;
        return temp;
    };
    
    T &operator * () { return *_ptr; };

    bool operator != (const iterator &rhs) const
    { return _ptr != rhs._ptr; };
};


template <typename T>
void
IDK::vector<T>::resize(size_t cap)
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
IDK::vector<T>::push(T data)
{
    if (_size+1 > _cap)
        resize(2*_cap);

    _data[_size] = data;
    _size += 1;
}
