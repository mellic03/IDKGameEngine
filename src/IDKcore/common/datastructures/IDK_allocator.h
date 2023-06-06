#pragma once

#include "../IDKdecl.h"

template <typename T>
class idk::Allocator
{
private:
    idk::vector<bool>               _occupied;
    idk::vector<uint>               _unnocupied;
    idk::vector<T>                  _objects;

public:
                                    Allocator();

    uint                            add();
    uint                            add(T);
    T &                             get(uint id);
    void                            remove(uint id);

    template <typename lambda_t>
    void                            forEach(lambda_t fn);

};

template <typename T>
idk::Allocator<T>::Allocator(): _occupied(1), _unnocupied(1), _objects(1)
{

}


template <typename T>
uint
idk::Allocator<T>::add()
{
    if (_unnocupied.empty())
    {
        _occupied.push(true);
        _objects.push(T());
        return _objects.size()-1;
    }

    else
    {
        uint id = _unnocupied.pop();
        _objects[id] = T();
        _occupied[id] = true;
        return id;
    }
};


template <typename T>
uint
idk::Allocator<T>::add(T data)
{
    if (_unnocupied.empty())
    {
        _occupied.push(true);
        _objects.push(data);
        return _objects.size()-1;
    }

    else
    {
        uint id = _unnocupied.pop();
        _objects[id] = data;
        _occupied[id] = true;
        return id;
    }
};


template <typename T>
T &
idk::Allocator<T>::get(uint id)
{
    return _objects[id];
};


template <typename T>
void
idk::Allocator<T>::remove(uint id)
{
    _occupied[id] = false;
    _unnocupied.push(id);
};


template <typename T>
template <typename lambda_t>
void
idk::Allocator<T>::forEach(lambda_t fn)
{
    for (size_t i=0; i<_objects.size(); i++)
        if (_occupied[i])
            fn(_objects[i]);
}

