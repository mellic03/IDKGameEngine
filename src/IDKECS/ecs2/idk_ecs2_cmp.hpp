#pragma once

#include "idk_ecs2.hpp"
#include <libidk/idk_serialize.hpp>


class idk::ECS2::iComponentArray
{
protected:
    std::string m_name;

public:
    iComponentArray( const std::string &name )
    :   m_name(name)
    {

    }

    virtual const std::string &getName() const { return m_name; };

    virtual size_t serialize   ( std::ofstream& ) = 0;
    virtual size_t deserialize ( std::ifstream& ) = 0;
};



template <typename T>
class idk::ECS2::ComponentArray: public idk::ECS2::iComponentArray
{
private:
    idk::Allocator<T> m_data;


public:
    using iComponentArray::iComponentArray;


    virtual size_t serialize( std::ofstream &stream )
    {
        size_t n = 0;
        n += idk::streamwrite(stream, m_name);
        n += idk::streamwrite(stream, m_data);
        return n;
    }

    virtual size_t deserialize( std::ifstream &stream )
    {
        size_t n = 0;
        n += idk::streamread(stream, m_name);
        n += idk::streamread(stream, m_data);
        return n;
    }


    int createComponent()
    {
        return m_data.create();
    };

    T &getComponent( int component )
    {
        return m_data.get(component);
    };

    void destroyComponent( int component )
    {
        m_data.destroy(component);
    };


    idk::Allocator<T> &getData() { return m_data; };

    std::vector<T>::iterator begin() { return m_data.begin(); };
    std::vector<T>::iterator end()   { return m_data.end();   };

};
