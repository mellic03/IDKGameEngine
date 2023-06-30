#pragma once

#include "../IDKdecl.h"
#include <iostream>

template <typename T>
class idk::queue
{
private:
    idk::vector<T>      _data;
    size_t              _size;
    size_t              _capacity;
    size_t              _front;
    size_t              _back;

public:
                        queue(): _data(1), _size(0), _capacity(1), _front(0), _back(0) {  };
    void                enque(T data);
    void                deque();
    T &                 front();
    T &                 back();
    bool                empty();
    size_t              size()  { return _size; };
};


template <typename T>
void
idk::queue<T>::enque(T data)
{
    if (_size >= _capacity)
    {
        _capacity *= 2;
        _data.resize(_capacity);
    }

    _back = (_back + 1) % _capacity;
    _data[_back] = data;
    _size += 1;
}


template <typename T>
void
idk::queue<T>::deque()
{
    if (_size == 0)
        return;

    _front = (_front + 1) % _capacity;
    _size -= 1;
}


template <typename T>
T &
idk::queue<T>::front()
{
    return _data[_front];
}

template <typename T>
T &
idk::queue<T>::back()
{
    return _data[_back];
}


template <typename T>
bool
idk::queue<T>::empty()
{
    return _size == 0;
}


