#pragma once

#include "../IDKdecl.h"
#include <functional>

#define cmp_t std::function<bool(const T &a, const T &b)>

template <typename T>
class idk::heap
{
private:
    idk::vector<T>      _data;
    cmp_t               _comparator;

    size_t              _parent(size_t idx)         { return (idx-1)/2; };
    size_t              _left_child(size_t idx)     { return (2*idx)+1; };
    size_t              _right_child(size_t idx)    { return (2*idx)+2; };
    void                _heapify(size_t idx);
    void                _reheap(size_t idx);

public:
                        heap() {  };
                        heap(cmp_t cmp): _comparator(cmp) {  };
                        heap(idk::vector<T> v): _data(v)  { _heapify(0); };

    size_t              size()                      { return _data.size();  };
    bool                empty()                     { return _data.empty(); };
    void                insert(T data);
    T                   pop();
    void                reheap();

};

#undef cmp_t


template <typename T>
void
idk::heap<T>::_heapify(size_t idx)
{
    if (idx >= size())
        return;

    size_t smallest_idx = idx;
    size_t left = _left_child(idx);
    size_t right = _right_child(idx);

    if (left < size() && _comparator(_data[left], _data[smallest_idx]))
        smallest_idx = left;

    if (right < size() && _comparator(_data[right], _data[smallest_idx]))
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
    while (idx > 0 && _comparator(_data[idx], _data[_parent(idx)]))
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


template <typename T>
void
idk::heap<T>::_reheap(size_t idx)
{
    if (idx == 0)
        return;

    size_t parent = _parent(idx);

    if (_comparator(_data[parent], _data[idx]) == false)
    {
        idk::swap(_data[parent], _data[idx]);
        _reheap(parent);
    }
}

template <typename T>
void
idk::heap<T>::reheap()
{
    if (_data.size() > 0)
    for (size_t i=(_data.size()/2) - 1; i>0; i--)
        _reheap(i);
}