#pragma once

#include "../IDKdecl.h"


template <typename T>
class idk::heap
{
private:
    idk::vector<T>      _data;
    void *              _comparator;

    size_t              _parent(size_t idx)         { return (idx-1)/2; };
    size_t              _left_child(size_t idx)     { return (2*idx)+1; };
    size_t              _right_child(size_t idx)    { return (2*idx)+2; };
    void                _heapify(size_t idx);

public:
                        heap() {  };
                        heap(idk::vector<T> v): _data(v) { _heapify(0); };

    size_t              size()                      { return _data.size();  };
    bool                empty()                     { return _data.empty(); };
    void                insert(T data);
    T                   pop();

};



template <typename T>
void
idk::heap<T>::_heapify(size_t idx)
{
    if (idx >= size())
        return;

    size_t smallest_idx = idx;
    size_t left = _left_child(idx);
    size_t right = _right_child(idx);

    if (left < size() && _data[left] < _data[smallest_idx])
        smallest_idx = left;

    if (right < size() && _data[right] < _data[smallest_idx])
        smallest_idx = right;

    if (smallest_idx != idx)
    {
        idk::swap(_data[idx], _data[smallest_idx]);
        _heapify(smallest_idx);
    }
}


template <typename T>
void
idk::heap<T>::insert(T data)
{
    _data.push(data);
 
    size_t idx = size()-1;
    while (idx > 0 && _data[idx] < _data[_parent(idx)])
    {
        size_t parent = _parent(idx);
        idk::swap(_data[parent], _data[idx]);
        idx = parent;
    }
    _heapify(0);
}


template <typename T>
T
idk::heap<T>::pop()
{
    idk::swap(_data.front(), _data.back());
    T element = _data.pop();
    _heapify(0);
    return element;
}

