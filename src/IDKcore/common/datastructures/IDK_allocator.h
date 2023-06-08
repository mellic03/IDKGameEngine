#pragma once

#include "../IDKdecl.h"


template <typename T>
class idk::Allocator
{
private:
    idk::vector<bool>               _occupied;
    idk::vector<uint>               _unnocupied;
    idk::vector<T>                  _objects;
    size_t                          _size;

public:
                                    Allocator();

    uint                            add();
    uint                            add(const T &data);
    T &                             get(uint id);
    void                            remove(uint id);
    size_t                          size() const { return _size; };

    template <typename lambda_t>
    void                            for_each(lambda_t fn);

};

template <typename T>
idk::Allocator<T>::Allocator(): _occupied(0), _unnocupied(0), _objects(0), _size(0)
{

}


template <typename T>
uint
idk::Allocator<T>::add()
{
    _size += 1;

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
idk::Allocator<T>::add(const T &data)
{
    _size += 1;

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
    _size -= 1;

    _occupied[id] = false;
    _unnocupied.push(id);
};


template <typename T>
template <typename lambda_t>
void
idk::Allocator<T>::for_each(lambda_t fn)
{
    for (size_t i=0; i<_objects.size(); i++)
        if (_occupied[i])
            fn(_objects[i]);
}

