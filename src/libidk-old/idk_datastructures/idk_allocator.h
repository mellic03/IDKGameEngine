#pragma once

#include <iostream>
#include <functional>
#include <stack>
#include <vector>


namespace idk { template <typename T> class Allocator; };


template <typename T>
class idk::Allocator
{
private:
    std::stack<int>         m_available_ids;
    std::vector<int>        m_object_indices;
    std::vector<T>          m_objects;

public:

    int                     create  (          );
    int                     create  ( const T& );
    T &                     get     ( int id   );
    void                    destroy ( int id   );

    std::vector<T> &        vector   ()       { return m_objects;        };
    T *                     data     ()       { return m_objects.data(); };
    size_t                  size     () const { return m_objects.size(); };
    size_t                  bytesize () const { return size()*sizeof(T); };

    typename std::vector<T>::iterator begin() { return m_objects.begin(); };
    typename std::vector<T>::iterator end()   { return m_objects.end();   };

};


template <typename T>
int
idk::Allocator<T>::create()
{
    return create(T());
}


template <typename T>
int
idk::Allocator<T>::create( const T &data )
{
    // New objects are always placed at the end of the array
    // ------------------------------------------------------------------------
    int idx = m_objects.size();
    m_objects.push_back(data);
    // ------------------------------------------------------------------------


    int id  = -1;

    // Find id
    // ------------------------------------------------------------------------
    if (m_available_ids.empty())
    {
        id = m_object_indices.size();
        m_object_indices.push_back(idx);
    }

    else
    {
        id = m_available_ids.top();
             m_available_ids.pop();
    }
    // ------------------------------------------------------------------------

    return id;
}


template <typename T>
T &
idk::Allocator<T>::get( int id )
{
    return m_objects[m_object_indices[id]];
}


template <typename T>
void
idk::Allocator<T>::destroy( int id )
{
    int idx = m_object_indices[id];

    // Find id of object which points to m_objects.back()
    // ------------------------------------------------------------------------
    int last_id  = -1;

    for (size_t i=0; i<m_object_indices.size(); i++)
    {
        if (m_object_indices[i] == m_objects.size() - 1)
        {
            last_id = i;
            break;
        }
    }
    // ------------------------------------------------------------------------

    // Swap and pop
    std::swap(m_objects[idx], m_objects.back());
    m_objects.pop_back();

    // Update index of element that was moved from end of array
    m_object_indices[last_id] = idx;

    // Unset id-idx and push id to stack
    m_object_indices[id] = -1;
    m_available_ids.push(id);
}


