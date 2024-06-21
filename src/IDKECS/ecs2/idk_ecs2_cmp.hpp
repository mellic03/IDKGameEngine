#pragma once

#include "idk_ecs2.hpp"
#include <libidk/idk_serialize.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <functional>


class idk::ECS2::iComponentArray
{
protected:
    using callback_type = std::function<void(idk::EngineAPI&, int)>;
    std::string m_name;

public:
    iComponentArray( const std::string &name, callback_type callback )
    :   m_name(name)
    {
        userCallback = callback;
    }

    virtual const std::string &getName() const { return m_name; };

    virtual int  createComponent( int ) = 0;
    virtual void destroyComponent( int ) = 0;

    virtual void onObjectAssignment( idk::EngineAPI&, int ) = 0;
    virtual void onObjectDeassignment( idk::EngineAPI&, int ) = 0;
    virtual void onObjectCopy( int, int ) = 0;

    callback_type userCallback = []( idk::EngineAPI &api, int obj_id )
    {
        LOG_WARN() << "Default userCallback";
    };

    virtual size_t serialize   ( std::ofstream& ) = 0;
    virtual size_t deserialize ( std::ifstream& ) = 0;
};



template <typename T>
class idk::ECS2::ComponentArray: public idk::ECS2::iComponentArray
{
private:
    idk::Allocator<T> m_data;

public:

    ComponentArray( const std::string &name )
    :   iComponentArray(name,
        ECS2::userCallback<T>)
    {

    }

    virtual ~ComponentArray() = default;

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


    virtual int createComponent( int obj_id )
    {
        int cmp = m_data.create();
        m_data.get(cmp).obj_id = obj_id;
        return cmp;
    };


    T &getComponent( int component )
    {
        return m_data.get(component);
    };


    virtual void destroyComponent( int component )
    {
        m_data.destroy(component);
    };


    virtual void onObjectAssignment( idk::EngineAPI &api, int obj_id )
    {
        T::onObjectAssignment(api, obj_id);
    };

    virtual void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
    {
        T::onObjectDeassignment(api, obj_id);
    };

    virtual void onObjectCopy( int src_obj, int dst_obj )
    {
        T::onObjectCopy(src_obj, dst_obj);
    };

    size_t size() const { return m_data.size(); };

    idk::Allocator<T> &getData() { return m_data; };

    std::vector<T>::iterator begin() { return m_data.begin(); };
    std::vector<T>::iterator end()   { return m_data.end();   };

};
