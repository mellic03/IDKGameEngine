#pragma once

#include <cstring>
#include "datastructures_decl.h"


template <typename T>
class idk::vector
{
private:
    T *                 m_data = nullptr;
    size_t              m_size;
    size_t              m_cap;

public:
                        vector(): m_size(0), m_cap(1), m_data(new T[1]) {  };
                        vector(size_t size): m_size(size), m_cap(2*size+1), m_data(new T[2*size+1]) {  };
                        vector(size_t size, const T &data);
                        vector(const vector<T> &other);
                        vector(vector<T> &&other);
                        ~vector() { if (m_data) delete[] m_data; };

    void                reserve(size_t size);
    void                resize(size_t size);

    void                push(const T &data);
    T                   pop()               { return m_data[--m_size]; };
    size_t              size()  const       { return m_size;          };
    bool                empty() const       { return m_size == 0;     };
    void                clear()             { m_size = 0;             };
    void                erase()             { reserve(1); m_size = 0; };

    T *                 data()              { return &(m_data[0]);     };
    T &                 front()             { return m_data[0];        };
    T &                 back()              { return m_data[m_size-1]; };

                        class iterator;
    iterator            begin() { return iterator(m_data);         };
    iterator            end()   { return iterator(m_data + m_size); };
 
    vector &            operator = (const vector &other);
    vector &            operator = (vector &&other) noexcept;
    T &                 operator [] (size_t i) { return m_data[i]; };
};


template <typename T>
class idk::vector<T>::iterator
{
private:
    T *_ptr;
public:
    iterator(T *ptr): _ptr(ptr) {  }
    iterator &operator ++ ()
    {
        _ptr += 1;
        return *this;
    };
    iterator operator ++ (int)
    {
        iterator temp = *this;
        _ptr += 1;
        return temp;
    };
    
    T &operator * () { return *_ptr; };

    bool operator != (const iterator &rhs) const
    { return _ptr != rhs._ptr; };
};


template <typename T>
idk::vector<T>::vector(size_t size, const T &data): m_size(size), m_cap(2*size+1), m_data(new T[2*size+1])
{
    for (T &element: *this)
        element = data;
};


template <typename T>
idk::vector<T>::vector(const vector<T> &other)
{
    if (m_data)
        delete[] m_data;

    m_size = other.m_size;
    m_cap = other.m_cap;
    m_data = new T[m_cap];

    if (other.m_data)
        for (size_t i=0; i<other.size(); i++)
            m_data[i] = other.m_data[i];
}


template <typename T>
idk::vector<T>::vector(vector<T> &&other)
{
    m_size = other.m_size;
    m_cap = other.m_cap;
    m_data = other.m_data;
    other.m_data = nullptr;
}


template <typename T>
void
idk::vector<T>::reserve(size_t cap)
{
    size_t smallest = idk::min(m_cap, cap);

    T *temp = new T[cap];
    for (size_t i=0; i<smallest; i++)
        temp[i] = m_data[i];
    delete[] m_data;

    m_data = temp;

    m_cap = cap;
    if (m_cap < m_size)
        m_size = m_cap;
}


template <typename T>
void
idk::vector<T>::resize(size_t size)
{
    if (size > m_cap)
        reserve(size);
    m_size = size;
}


template <typename T>
void
idk::vector<T>::push(const T &data)
{
    if (m_size+1 >= m_cap)
        reserve(2*m_cap);

    m_data[m_size] = data;
    m_size += 1;
}


template <typename T>
idk::vector<T> &
idk::vector<T>::operator = (const vector &other)
{
    if (this != &other)
    {
        if (m_data)
            delete[] m_data;

        m_size = other.m_size;
        m_cap = other.m_cap;
        m_data = new T[m_cap];

        if (other.m_data)
            for (size_t i=0; i<other.size(); i++)
                m_data[i] = other.m_data[i];
    }
    return *this;
}


template <typename T>
idk::vector<T> &
idk::vector<T>::operator = (vector &&other) noexcept
{
    if (this == &other)
        return *this;

    if (m_data)
        delete[] m_data;

    m_size = other.m_size;
    m_cap = other.m_cap;
    m_data = other.m_data;
    other.m_data = nullptr;

    return *this;
}
