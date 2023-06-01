#pragma once

#include "IDKdecl.h"


template <typename T>
class IDK::ptr_Allocator
{
private:
    IDK::vector<bool> _occupied;
    IDK::vector<int>  _unnocupied_indices;
    IDK::vector<T *> _objects;

public:
    ptr_Allocator() {  };
    ~ptr_Allocator();

    template <typename U>
    int add();
    int add();

    template <typename U>
    U *get(int id);
    T *get(int id);

    void remove(int id);

    template <typename lambda_t>
    void forEach(lambda_t fn);

};


template <typename T>
IDK::ptr_Allocator<T>::~ptr_Allocator()
{
    forEach(
        [](IDK::GameObject::Base *obj)
        {
            delete obj;
        }
    );
}


template <typename T>
template <typename U>
int IDK::ptr_Allocator<T>::add()
{
    if (_unnocupied_indices.size() > 0)
    {
        int id = _unnocupied_indices.pop();
        _objects[id] = new U;
        _occupied[id] = true;
        return id;
    }

    else
    {
        _occupied.push(true);
        _objects.push(new U);
        return _objects.size()-1;
    }
};


template <typename T>
int IDK::ptr_Allocator<T>::add()
{
    if (_unnocupied_indices.size() > 0)
    {
        int id = _unnocupied_indices.pop();
        _objects[id] = new T;
        _occupied[id] = true;
        return id;
    }

    else
    {
        _occupied.push(true);
        _objects.push(new T);
        return _objects.size()-1;
    }
};


template <typename T>
void IDK::ptr_Allocator<T>::remove(int id)
{
    delete _objects[id];
    _occupied[id] = false;
    _unnocupied_indices.push(id);
};


template <typename T>
template <typename U>
U *IDK::ptr_Allocator<T>::get(int id)
{
    return (U *)_objects[id];
};


template <typename T>
T *IDK::ptr_Allocator<T>::get(int id)
{
    return _objects[id];
};


template <typename T>
template <typename lambda_t>
void IDK::ptr_Allocator<T>::forEach(lambda_t fn)
{
    for (size_t i=0; i<_objects.size(); i++)
        if (_occupied[i])
            fn(_objects[i]);
}

