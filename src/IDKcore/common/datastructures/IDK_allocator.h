#pragma once

#include "../IDKdecl.h"

template <typename T>
class idk::Allocator
{
private:
    idk::vector<bool>               _occupied;
    idk::vector<int>                _unnocupied_indices;
    idk::vector<T>                  _objects;

public:
                                    Allocator() {  };

    int                             add();
    int                             add(T);
    T &                             get(int id);
    void                            remove(int id);

    template <typename lambda_t>
    void                            forEach(lambda_t fn);

};


template <typename T>
int
idk::Allocator<T>::add()
{
    if (_unnocupied_indices.empty())
    {
        _occupied.push(true);
        _objects.push(T());
        return _objects.size()-1;
    }

    else
    {
        int id = _unnocupied_indices.pop();
        _objects[id] = T();
        _occupied[id] = true;
        return id;
    }
};


template <typename T>
int
idk::Allocator<T>::add(T data)
{
    if (_unnocupied_indices.empty())
    {
        _occupied.push(true);
        _objects.push(data);
        return _objects.size()-1;
    }

    else
    {
        int id = _unnocupied_indices.pop();
        _objects[id] = data;
        _occupied[id] = true;
        return id;
    }
};


template <typename T>
T &
idk::Allocator<T>::get(int id)
{
    return _objects[id];
};


template <typename T>
void
idk::Allocator<T>::remove(int id)
{
    _occupied[id] = false;
    _unnocupied_indices.push(id);
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

