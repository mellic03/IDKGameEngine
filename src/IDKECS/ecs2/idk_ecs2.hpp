#pragma once

#include <libidk/idk_allocator.hpp>

#include <vector>
#include <string>
#include <set>
#include <map>


namespace idk
{
    class ECS2;
}



class idk::ECS2
{
private:

    class iComponentArray;

    template <typename T>
    class ComponentArray;

    inline static
    std::map<size_t, iComponentArray*> m_component_arrays;

    inline static
    std::map<std::string, size_t>      m_component_keys;


    class System
    {
    public:
        virtual void init     ( idk::EngineAPI& ) = 0;
        virtual void update   ( idk::EngineAPI& ) = 0;
        virtual void shutdown ( idk::EngineAPI& ) {  };
    };

    inline static
    std::map<std::string, System*> m_systems;


    struct Entity
    {
        std::string name = "";

        int parent = -1;
        std::set<int> children;
    
        std::map<size_t, int> components;
        std::set<std::string> component_names;

        size_t serialize( std::ofstream& );
        size_t deserialize( std::ifstream& );
    };


    inline static
    idk::Allocator<Entity> m_entities;


    template <typename T>
    static size_t getkey()
    {
        return typeid(T).hash_code();
    };


public:

    static const idk::Allocator<Entity> &getEntities() { return m_entities; };

    static int  createGameObject( const std::string &name );
    static void deleteGameObject( int obj_id, bool deep=false );

    static int  getParent( int obj_id );
    static void giveChild( int parent_id, int child_id );
    static void removeChild( int parent_id, int child_id );

    template <typename T>
    static void giveComponent( int obj_id );

    template <typename T>
    static bool hasComponent( int obj_id );

    template <typename T>
    static T &getComponent( int obj_id );

    template <typename T>
    static ComponentArray<T> &getComponentArray();

    static iComponentArray *getComponentArray( size_t key )
    {
        return m_component_arrays[key];
    }

    static iComponentArray *getComponentArray( const std::string &name )
    {
        size_t key = m_component_keys[name];
        return getComponentArray(key);
    }

    static size_t getComponentKey( const std::string &name )
    {
        return m_component_keys[name];
    }

    template <typename T>
    static void registerComponent( const std::string &name );


    static void save( const std::string &filepath );
    static void load( const std::string &filepath );

};



#include "idk_ecs2_cmp.hpp"
#include "idk_ecs2.inl"

