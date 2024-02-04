#pragma once

#include "common.hpp"
#include <libidk/idk_pod.hpp>


namespace idecs
{
    class iComponentArray;

    template <typename T>
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


class idecs::iComponentArray
{
protected:
    idk::string     m_name;

public:

    iComponentArray( const idk::string &name )
    :   m_name(name)
    {
        
    };


    virtual void       setBehaviour( const Behaviour & ) = 0;
    virtual Behaviour &getBehaviour() = 0;

    virtual int         create  ( int obj_id ) = 0;
    virtual void        destroy ( int obj_id ) = 0;
    virtual void        copy    ( int, int, int ) = 0;

    virtual void        serialize   ( std::ofstream & ) = 0;
    virtual void        deserialize ( std::ifstream & ) = 0;


    template <typename T>
    T &cast() { return *dynamic_cast<T *>(this); };

    const idk::string   &name() { return m_name; };
};





template <typename T>
class idecs::ComponentArray: public idecs::iComponentArray
{
private:
    Behaviour           m_behaviour;
    idk::Allocator<T>   m_data;

public:

    virtual void setBehaviour( const Behaviour &behaviour ) final
    {
        m_behaviour = behaviour;
    };

    virtual Behaviour &getBehaviour() final
    {
        return m_behaviour;
    };

    ComponentArray( const idk::string &name ): iComponentArray(name)
    {
        static_assert(std::is_standard_layout_v<T> == true);
    };


    idk::Allocator<T> &data() { return m_data; };


    virtual int create( int obj_id ) final
    {
        int id = m_data.create( T(obj_id) );
        return id;
    };


    virtual void copy( int obj_id, int dst_id, int src_id ) final
    {
        m_data.get(dst_id) = T(obj_id, &m_data.get(src_id));
    };


    virtual void destroy( int id ) final
    {
        m_data.destroy(id);
    };


    virtual void serialize( std::ofstream &stream ) final
    {
        stream << m_name;
        stream << m_data;
    };

    virtual void deserialize( std::ifstream &stream ) final
    {
        m_data.clear();

        stream >> m_name;
        stream >> m_data;
    };

};
