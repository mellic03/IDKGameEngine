#pragma once

#include "idk_ecs.hpp"
#include <libidk/idk_serialize.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <functional>


/*
    TODO:
        - Don't write directly to stream, first write to a buffer instead.
*/


class idk::ECS::iComponentArray
{
protected:
    using callback_type = std::function<void(idk::EngineAPI&, idk::ECS&, int)>;
    idk::ECS   *m_ecs;
    std::string m_name;
    size_t      m_key;

public:
    iComponentArray( idk::ECS *ecs, const std::string &name, size_t key, callback_type callback )
    :   m_ecs  (ecs),
        m_name (name),
        m_key  (key)
    {
        userCallback = callback;
    }

    virtual const std::string &getName() const { return m_name; };
    virtual const size_t       getKey () const { return m_key; };

    virtual int  createComponent( int ) = 0;
    virtual void destroyComponent( int ) = 0;

    virtual void onObjectAssignment( idk::EngineAPI&, int ) = 0;
    virtual void onObjectDeassignment( idk::EngineAPI&, int ) = 0;
    virtual void onObjectCopy( int, int ) = 0;

    callback_type userCallback = []( idk::EngineAPI&, idk::ECS&, int )
    {
        // LOG_WARN() << "Default userCallback REE";
    };

    virtual size_t nbytes_serialized () = 0;
    virtual size_t serialize   ( std::ofstream& ) = 0;
    virtual size_t deserialize ( std::ifstream& ) = 0;
};



template <typename T>
class idk::ECS::ComponentArray: public idk::ECS::iComponentArray
{
private:
    idk::EngineAPI *m_api;
    idk::Allocator<T> m_data;

public:

    ComponentArray( idk::EngineAPI *api, idk::ECS *ecs, const std::string &name, size_t key )
    :   iComponentArray(ecs, name, key, [](idk::EngineAPI&, idk::ECS&, int){}),
        m_api(api)
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
        for (auto &cmp: m_data)
        {
            onObjectDeassignment(*m_api, cmp.obj_id);
        }

        m_data.clear();

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
        T::onObjectAssignment(api, *m_ecs, obj_id);
    };

    virtual void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
    {
        T::onObjectDeassignment(api, *m_ecs, obj_id);
    };

    virtual void onObjectCopy( int src_obj, int dst_obj )
    {
        T::onObjectCopy(*m_ecs, src_obj, dst_obj);
    };

    size_t size() const { return m_data.size(); };

    virtual size_t nbytes_serialized()
    {
        std::ofstream stream("tmp.bin", std::ios::binary);
        size_t nbytes = serialize(stream);
        stream.close();

        return nbytes;
    };


    idk::Allocator<T> &getData() { return m_data; };

    std::vector<T>::iterator begin() { return m_data.begin(); };
    std::vector<T>::iterator end()   { return m_data.end();   };

};
