#pragma once

#include <cstring>
#include "../IDKdecl.h"


template <typename T>
class idk::vector
{
private:
    T                   *_data = nullptr;
    size_t              _size;
    size_t              _cap;

public:
                        vector(): _size(0), _cap(1), _data(new T[1]) {  };
                        vector(size_t size): _size(size), _cap(size), _data(new T[size]) {  };
                        vector(const vector<T> &other);
                        ~vector() { if (_data) delete[] _data; };

    void                resize(size_t size);

    void                push(const T &data);
    T                   pop()                { return _data[--_size]; };
    size_t              size()  const        { return _size;          };
    bool                empty() const        { return _size == 0;     };
    void                clear()              { _size = 0;             };
    void                erase()              { resize(1); _size = 0; };

    T &                 front()              { return _data[0];       };
    T &                 back()               { return _data[_size-1]; };


                        class iterator;
    iterator            begin() { return iterator(_data);         };
    iterator            end()   { return iterator(_data + _size); };
 
                        vector &operator = (const vector &other);
    T &                 operator [] (size_t i) { return _data[i]; };
};


template <typename T>
class idk::vector<T>::iterator
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
idk::vector<T>::vector(const vector<T> &other)
{
    if (_data)
        delete[] _data;

    _size = other._size;
    _cap = other._cap;
    _data = new T[_cap];

    if (other._data)
        memcpy((void *)_data, (void *)other._data, _cap*sizeof(T));
}


template <typename T>
void
idk::vector<T>::resize(size_t cap)
{
    size_t newsize = (_cap < cap) ? _cap : cap;

    T *temp = new T[newsize];
    memcpy((void *)temp, (void *)_data, newsize * sizeof(T));
    delete[] _data;
    
    _data = new T[cap];
    memcpy((void *)_data, (void *)temp, newsize * sizeof(T));
    delete[] temp;

    _cap = cap;

    if (_cap < _size)
        _size = _cap;
}


template <typename T>
void
idk::vector<T>::push(const T &data)
{
    if (_size+1 > _cap)
        resize(2*_cap);

    _data[_size] = data;
    _size += 1;
}


template <typename T>
idk::vector<T> &
idk::vector<T>::operator = (const vector &other)
{
    if (this != &other)
    {
        if (_data)
            delete[] _data;

        _size = other._size;
        _cap = other._cap;
        _data = new T[_cap];

        if (other._data)
            memcpy((void *)_data, (void *)other._data, _cap*sizeof(T));
    }
    return *this;
}