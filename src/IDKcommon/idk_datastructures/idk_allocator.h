#pragma once

#include <iostream>
#include <functional>
#include <vector>

#include "datastructures_decl.h"


/**
 * idk::Allocator<T> is good.
 * 
*/

template <typename T>
class idk::Allocator
{
private:
    std::vector<bool>       _is_occupied;
    std::vector<int>        _unnocupied_indices;
    std::vector<T>          _objects;

public:
                            Allocator();

    int                     add();
    int                     add( const T &data );

    T &                     get( int id );
    void                    remove( int id );
    size_t                  size() const { return _objects.size() - _unnocupied_indices.size(); };

    void                    for_each(std::function<void(T&)>);
    void                    for_each(std::function<void(int, T&)>);
    void                    for_each_pair(std::function<void(T&, T&)>);
};


template <typename T>
idk::Allocator<T>::Allocator(): _is_occupied(0), _unnocupied_indices(0), _objects(0)
{

}


template <typename T>
int
idk::Allocator<T>::add()
{
    if (_unnocupied_indices.empty())
    {
        _is_occupied.push_back(true);
        _objects.push_back(T());
        return _objects.size()-1;
    }

    else
    {
        int id = _unnocupied_indices.back(); _unnocupied_indices.pop_back();
        _objects[id] = T();
        _is_occupied[id] = true;
        return id;
    }
};


template <typename T>
int
idk::Allocator<T>::add(const T &data)
{
    if (_unnocupied_indices.empty())
    {
        _is_occupied.push_back(true);
        _objects.push_back(data);
        return _objects.size()-1;
    }

    else
    {
        int id = _unnocupied_indices.back(); _unnocupied_indices.pop_back();
        _objects[id] = data;
        _is_occupied[id] = true;
        return id;
    }
};



template <typename T>
T &
idk::Allocator<T>::get(int id)
{
    #ifdef IDK_DEBUG
    if (id >= _objects.size())
    {
        std::cout
        << "Runtime error in idk::Allocator<T>::get(int id)\n" 
        << "\tid " << id << " is >= size (" << _objects.size() << ")"
        << std::endl;
        exit(1);
    }
    #endif

    return _objects[id];
};


template <typename T>
void
idk::Allocator<T>::remove(int id)
{
    #ifdef IDK_DEBUG
    if (id >= _objects.size())
    {
        std::cout
        << "Runtime error in idk::Allocator<T>::remove(int id)\n" 
        << "\tid " << id << " is >= size (" << _objects.size() << ")"
        << std::endl;
        exit(1);
    }
    #endif

    _is_occupied[id] = false;
    _unnocupied_indices.push_back(id);
};


template <typename T>
void
idk::Allocator<T>::for_each(std::function<void(T&)> lambda_fn)
{
    for (size_t i=0; i<_objects.size(); i++)
    {
        if (_is_occupied[i] == false)
            continue;

        lambda_fn(_objects[i]);
    }
}


template <typename T>
void
idk::Allocator<T>::for_each(std::function<void(int, T&)> lambda_fn)
{
    for (size_t i=0; i<_objects.size(); i++)
    {
        if (_is_occupied[i] == false)
            continue;

        lambda_fn(i, _objects[i]);
    }
}


template <typename T>
void
idk::Allocator<T>::for_each_pair(std::function<void(T&, T&)> lambda_fn)
{
    for (size_t i=0; i<_objects.size(); i++)
    {
        if (_is_occupied[i] == false)
            continue;

        for (size_t j=i+1; j<_objects.size(); j++)
        {
            if (_is_occupied[j] == false)
                continue;

            lambda_fn(_objects[i], _objects[j]);
        }
    }
}
