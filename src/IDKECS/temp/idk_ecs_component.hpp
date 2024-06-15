#pragma once

#include "common.hpp"

#include <libidk/idk_pod.hpp>
#include <libidk/idk_serialize.hpp>

#include "idk_ecs_file.hpp"

#include <filesystem>


namespace idk::ecs
{
    template <typename A>
    concept is_assignable = requires( A a, idk::EngineAPI &api, int obj_id, int dst_obj )
    {
        { a.onObjectAssignment(api, obj_id)    };
        { a.onObjectDeassignment(api, obj_id)  };
        { a.onObjectCopy(api, obj_id, dst_obj) };
    };

    template <typename A>
    concept is_component = idk::ecs::is_assignable<A> && idk::is_explicitly_serializable<A>;




    class ECS;

    class iComponentArray;

    template <typename T> requires idk::ecs::is_component<T>
    class ComponentArray;


    struct Behaviour
    {
        using function_type_A = std::function<void(idk::EngineAPI&, int)>;
        using function_type_B = std::function<void(idk::EngineAPI&, int, int)>;

        function_type_A _userBehaviour  = [](idk::EngineAPI&, int) {  };

        function_type_A _onAssignment   = [](idk::EngineAPI&, int) {  };
        function_type_A _onDeassignment = [](idk::EngineAPI&, int) {  };
        function_type_B _onCopy         = [](idk::EngineAPI&, int, int) {  };


        using function_type_C = std::function<void(ECS &, std::ofstream &)>;
        using function_type_D = std::function<void(ECS &, std::ifstream &)>;

        function_type_C _serialize      = [](ECS &, std::ofstream&) {  };
        function_type_D _deserialize    = [](ECS &, std::ifstream&) {  };
    };

};


class idk::ecs::iComponentArray
{
protected:
    idk::string     m_name;
    int             m_id;
    idk::EngineAPI &m_apiref;

    std::function<void(idk::EngineAPI&, int)> m_callback;

public:

    iComponentArray( idk::EngineAPI &api, const idk::string &name, int id )
    :   m_apiref(api),
        m_name(name),
        m_id(id)
    {
        
    };

    int  getID(        ) { return m_id; };
    void setID( int id ) { m_id = id;   };

    void setuserCallback( std::function<void(idk::EngineAPI&, int)> callback )
    {
        m_callback = callback;
    };

    void userCallback( idk::EngineAPI &api, int obj_id )
    {
        m_callback(api, obj_id);
    };

    virtual int         create  ( int obj_id ) = 0;
    virtual void        destroy ( int obj_id, int data_id ) = 0;
    virtual void        copy    ( int, int, int, int ) = 0;

    virtual size_t      serialize   ( std::ofstream & ) = 0;
    virtual size_t      deserialize ( std::ifstream & ) = 0;
    virtual void        allObjectAssignment() = 0;

    virtual void *      bytes() = 0;
    virtual uint32_t    nbytes() = 0;
    virtual uint32_t    numObjects() = 0;


    template <typename T>
    T &cast() { return *dynamic_cast<T *>(this); };

    const idk::string   &name() { return m_name; };
};





template <typename T> requires idk::ecs::is_component<T>
class idk::ecs::ComponentArray: public idk::ecs::iComponentArray
{
private:
    idk::Allocator<T>   m_data;

public:

    ComponentArray( idk::EngineAPI &api, const idk::string &name, int id )
    :   iComponentArray(api, name, id)
    {

    };


    IDK_ALLOCATOR_ACCESS(Component, T, m_data);

    idk::vector<T>::iterator begin() { return m_data.begin(); };
    idk::vector<T>::iterator end()   { return m_data.end(); };


    virtual int create( int obj_id ) final
    {
        int id = m_data.create();
        m_data.get(id).onObjectAssignment(m_apiref, obj_id);
        return id;
    };


    virtual void copy( int obj_id, int dst_obj, int dst_id, int src_id ) final
    {
        m_data.get(dst_id).onObjectCopy(m_apiref, obj_id, dst_obj);
    };


    virtual void destroy( int obj_id, int data_id ) final
    {
        m_data.get(data_id).onObjectDeassignment(m_apiref, obj_id);
        m_data.destroy(data_id);
    };


    virtual size_t serialize( std::ofstream &stream ) final
    {
        size_t n = 0;
        n += idk::streamwrite(stream, m_id);
        n += idk::streamwrite(stream, m_name);
        n += idk::streamwrite(stream, m_data);
        return n;
    };

    virtual size_t deserialize( std::ifstream &stream ) final
    {
        size_t n = 0;
        n += idk::streamread(stream, m_id);
        n += idk::streamread(stream, m_name);
        n += idk::streamread(stream, m_data);

        return n;
    };

    virtual void allObjectAssignment() final
    {
        for (auto &cmp: m_data)
        {
            cmp.onObjectAssignment(m_apiref, cmp.obj_id);
        }
    };


    virtual void *bytes() final
    {
        return m_data.data();
    };


    virtual uint32_t nbytes() final
    {
        std::ofstream stream("IDKGE/temp/dummy.bin", std::ios::binary);
        size_t n = this->serialize(stream);
        stream.close();
        std::filesystem::remove("IDKGE/temp/dummy.bin");
        return n;
    };


    virtual uint32_t numObjects() final
    {
        return m_data.size();
    };

};
