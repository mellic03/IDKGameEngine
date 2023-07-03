#pragma once

#include <iostream>
#include <functional>
#include <vector>

#include "datastructures_decl.h"


template <typename T>
class idk::Allocator
{
private:
    std::vector<bool>               _occupied;
    std::vector<int>                _unnocupied;
    std::vector<T>                  _objects;

public:
                                    Allocator();

    int                             add();
    int                             add( const T &data );
    T &                             get( int id );
    void                            remove( int id );
    size_t                          size() const { return _objects.size() - _unnocupied.size(); };

    void                            for_each(std::function<void(T&)>);
    void                            for_each_pair(std::function<void(T&, T&)>);
};

template <typename T>
idk::Allocator<T>::Allocator(): _occupied(0), _unnocupied(0), _objects(0)
{

}


template <typename T>
int
idk::Allocator<T>::add()
{
    if (_unnocupied.empty())
    {
        _occupied.push_back(true);
        _objects.push_back(T());
        return _objects.size()-1;
    }

    else
    {
        int id = _unnocupied.back(); _unnocupied.pop_back();
        _objects[id] = T();
        _occupied[id] = true;
        return id;
    }
};


template <typename T>
int
idk::Allocator<T>::add(const T &data)
{
    if (_unnocupied.empty())
    {
        _occupied.push_back(true);
        _objects.push_back(data);
        return _objects.size()-1;
    }

    else
    {
        int id = _unnocupied.back(); _unnocupied.pop_back();
        _objects[id] = data;
        _occupied[id] = true;
        return id;
    }
};



template <typename T>
T &
idk::Allocator<T>::get(int id)
{
    if (id >= _objects.size())
    {
        std::cout
        << "Runtime error in idk::Allocator<T>::get(int id)\n" 
        << "\tid " << id << " is >= size (" << _objects.size() << ")"
        << std::endl;
        exit(1);
    }

    return _objects[id];
};


template <typename T>
void
idk::Allocator<T>::remove(int id)
{
    if (id >= _objects.size())
    {
        std::cout
        << "Runtime error in idk::Allocator<T>::remove(int id)\n" 
        << "\tid " << id << " is >= size (" << _objects.size() << ")"
        << std::endl;
        exit(1);
    }

    _occupied[id] = false;
    _unnocupied.push_back(id);
};


template <typename T>
void
idk::Allocator<T>::for_each(std::function<void(T&)> lambda_fn)
{
    for (size_t i=0; i<_objects.size(); i++)
    {
        if (_occupied[i] == false)
            continue;

        lambda_fn(_objects[i]);
    }
}


template <typename T>
void
idk::Allocator<T>::for_each_pair(std::function<void(T&, T&)> lambda_fn)
{
    for (size_t i=0; i<_objects.size(); i++)
    {
        if (_occupied[i] == false)
            continue;

        for (size_t j=i+1; j<_objects.size(); j++)
        {
            if (_occupied[j] == false)
                continue;

            lambda_fn(_objects[i], _objects[j]);
        }
    }
}
