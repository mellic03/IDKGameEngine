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
    std::vector<bool>       m_is_occupied;
    std::vector<int>        m_unnocupied_indices;
    std::vector<T>          m_objects;

public:
                            Allocator();

    int                     add();
    int                     add( const T &data );

    T &                     get( int id );
    void                    remove( int id );
    int                     size() const { return m_objects.size() - m_unnocupied_indices.size(); };

    void                    for_each(std::function<void(T&)>);
    void                    for_each(std::function<void(int, T&)>);
    void                    for_each_pair(std::function<void(T&, T&)>);
};


template <typename T>
idk::Allocator<T>::Allocator(): m_is_occupied(0), m_unnocupied_indices(0), m_objects(0)
{

}


template <typename T>
int
idk::Allocator<T>::add()
{
    if (m_unnocupied_indices.empty())
    {
        m_is_occupied.push_back(true);
        m_objects.push_back(T());
        return m_objects.size()-1;
    }

    else
    {
        int id = m_unnocupied_indices.back(); m_unnocupied_indices.pop_back();
        m_objects[id] = T();
        m_is_occupied[id] = true;
        return id;
    }
};


template <typename T>
int
idk::Allocator<T>::add(const T &data)
{
    if (m_unnocupied_indices.empty())
    {
        m_is_occupied.push_back(true);
        m_objects.push_back(data);
        return m_objects.size()-1;
    }

    else
    {
        int id = m_unnocupied_indices.back(); m_unnocupied_indices.pop_back();
        m_objects[id] = data;
        m_is_occupied[id] = true;
        return id;
    }
};



template <typename T>
T &
idk::Allocator<T>::get(int id)
{
    #ifdef IDK_DEBUG
    if (id >= (int)m_objects.size())
    {
        std::cout
        << "Runtime error in idk::Allocator<T>::get(int id)\n" 
        << "\tid " << id << " is >= size (" << m_objects.size() << ")"
        << std::endl;
        exit(1);
    }
    #endif

    return m_objects[id];
};


template <typename T>
void
idk::Allocator<T>::remove(int id)
{
    #ifdef IDK_DEBUG
    if (id >= (int)m_objects.size())
    {
        std::cout
        << "Runtime error in idk::Allocator<T>::remove(int id)\n" 
        << "\tid " << id << " is >= size (" << m_objects.size() << ")"
        << std::endl;
        exit(1);
    }
    #endif

    m_is_occupied[id] = false;
    m_unnocupied_indices.push_back(id);
};


template <typename T>
void
idk::Allocator<T>::for_each(std::function<void(T&)> lambda_fn)
{
    for (size_t i=0; i<m_objects.size(); i++)
    {
        if (m_is_occupied[i] == false)
            continue;

        lambda_fn(m_objects[i]);
    }
}


template <typename T>
void
idk::Allocator<T>::for_each(std::function<void(int, T&)> lambda_fn)
{
    for (size_t i=0; i<m_objects.size(); i++)
    {
        if (m_is_occupied[i] == false)
            continue;

        lambda_fn(i, m_objects[i]);
    }
}


template <typename T>
void
idk::Allocator<T>::for_each_pair(std::function<void(T&, T&)> lambda_fn)
{
    for (size_t i=0; i<m_objects.size(); i++)
    {
        if (m_is_occupied[i] == false)
            continue;

        for (size_t j=i+1; j<m_objects.size(); j++)
        {
            if (m_is_occupied[j] == false)
                continue;

            lambda_fn(m_objects[i], m_objects[j]);
        }
    }
}
