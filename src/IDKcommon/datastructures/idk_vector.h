#pragma once

#include <cstring>
#include "datastructures_decl.h"


template <typename T>
class idk::vector
{
private:
    T                   *_data = nullptr;
    size_t              _size;
    size_t              _cap;

public:
                        vector(): _size(0), _cap(1), _data(new T[1]) {  };
                        vector(size_t size): _size(size), _cap(2*size+1), _data(new T[2*size+1]) {  };
                        vector(size_t size, const T &data);
                        vector(const vector<T> &other);
                        vector(vector<T> &&other);
                        ~vector() { if (_data) delete[] _data; };

    void                reserve(size_t size);
    void                resize(size_t size);

    void                push(const T &data);
    T                   pop()                { return _data[--_size]; };
    size_t              size()  const        { return _size;          };
    bool                empty() const        { return _size == 0;     };
    void                clear()              { _size = 0;             };
    void                erase()              { reserve(1); _size = 0; };

    T &                 front()              { return _data[0];       };
    T &                 back()               { return _data[_size-1]; };


                        class iterator;
    iterator            begin() { return iterator(_data);         };
    iterator            end()   { return iterator(_data + _size); };
 
    vector &            operator = (const vector &other);
    vector &            operator = (vector &&other) noexcept;
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
idk::vector<T>::vector(size_t size, const T &data): _size(size), _cap(2*size+1), _data(new T[2*size+1])
{
    for (T &element: *this)
        element = data;
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
        for (size_t i=0; i<other.size(); i++)
            _data[i] = other._data[i];
}


template <typename T>
idk::vector<T>::vector(vector<T> &&other)
{
    _size = other._size;
    _cap = other._cap;
    _data = other._data;
    other._data = nullptr;
}


template <typename T>
void
idk::vector<T>::reserve(size_t cap)
{
    size_t smallest = idk::min(_cap, cap);

    T *temp = new T[cap];
    for (size_t i=0; i<smallest; i++)
        temp[i] = _data[i];
    delete[] _data;

    _data = temp;

    _cap = cap;
    if (_cap < _size)
        _size = _cap;
}


template <typename T>
void
idk::vector<T>::resize(size_t size)
{
    if (size > _cap)
        reserve(size);
    _size = size;
}


template <typename T>
void
idk::vector<T>::push(const T &data)
{
    if (_size+1 >= _cap)
        reserve(2*_cap);

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
            for (size_t i=0; i<other.size(); i++)
                _data[i] = other._data[i];
    }
    return *this;
}


template <typename T>
idk::vector<T> &
idk::vector<T>::operator = (vector &&other) noexcept
{
    if (this == &other)
        return *this;

    if (_data)
        delete[] _data;

    _size = other._size;
    _cap = other._cap;
    _data = other._data;
    other._data = nullptr;

    return *this;
}
