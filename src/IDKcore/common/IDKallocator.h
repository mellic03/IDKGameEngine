#pragma once

#include "IDKdecl.h"


template <typename T>
class IDK::Allocator
{
private:
    IDK::vector<bool> _occupied;
    IDK::vector<int>  _unnocupied_indices;
    IDK::vector<T> _objects;

public:
    Allocator() {  };

    int add();
    T &get(int id);
    void remove(int id);

    template <typename lambda_t>
    void forEach(lambda_t fn);

};


template <typename T>
int IDK::Allocator<T>::add()
{
    if (_unnocupied_indices.size() > 0)
    {
        int id = _unnocupied_indices.pop();
        _objects[id] = T();
        _occupied[id] = true;
        return id;
    }

    else
    {
        _occupied.push(true);
        _objects.push(T());
        return _objects.size()-1;
    }
};


template <typename T>
T &IDK::Allocator<T>::get(int id)
{
    return _objects[id];
};


template <typename T>
void IDK::Allocator<T>::remove(int id)
{
    _occupied[id] = false;
    _unnocupied_indices.push(id);
};


template <typename T>
template <typename lambda_t>
void IDK::Allocator<T>::forEach(lambda_t fn)
{
    for (size_t i=0; i<_objects.size(); i++)
        if (_occupied[i])
            fn(_objects[i]);
}

