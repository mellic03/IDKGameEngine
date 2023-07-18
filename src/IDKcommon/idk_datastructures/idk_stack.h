#pragma once

#include <cstring>
#include "datastructures_decl.h"


template <typename T>
class idk::stack
{
private:
    T *m_data;
    size_t m_size, m_cap;

    void _resize(size_t cap);

public:
                stack(): m_size(0), m_cap(1), m_data(new T[1]) {  };
                ~stack() { if (m_data) delete[] m_data; };

    void        push(const T &data);
    T &         top()               { return m_data[m_size];   };
    T           pop()               { return m_data[--m_size]; };
    size_t      size() const        { return m_size;          };
    bool        empty()             { return m_size == 0;     };
};


template <typename T>
void
idk::stack<T>::_resize(size_t cap)
{
    T *temp = new T[m_size];
    memcpy((void *)temp, (void *)m_data, m_size * sizeof(T));

    delete[] m_data;
    m_data = new T[cap];

    memcpy((void *)m_data, (void *)temp, m_size * sizeof(T));

    delete[] temp;
    m_cap = cap;
}


template <typename T>
void
idk::stack<T>::push(const T &data)
{
    if (m_size+1 > m_cap)
        _resize(2*m_cap);

    m_data[m_size] = data;
    m_size += 1;
}

