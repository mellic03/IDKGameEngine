#pragma once

#include <libidk/idk_allocator.hpp>
#include <libidk/idk_log.hpp>

#include <vector>
#include <string>
#include <set>
#include <map>

#include <functional>

#include <cxxabi.h>



namespace idk
{
    class EngineAPI;
    class ECS2;
}



class idk::ECS2
{
public:
    class iComponentArray;

    template <typename T>
    class ComponentArray;

    class System
    {
    private:
        friend class idk::ECS2;
        std::string m_name = "default";
        double m_avg_time = 0.0;

    public:
        virtual void init     ( idk::EngineAPI& ) = 0;
        virtual void update   ( idk::EngineAPI& ) = 0;
        virtual void shutdown ( idk::EngineAPI& ) {  };
    };


private:

    inline static idk::EngineAPI *m_api_ptr;

    inline static bool m_readfile = false;
    inline static std::string m_filepath;

    inline static std::vector<std::function<void()>> m_callbacks;

    static void _load();

    inline static
    std::map<size_t, std::unique_ptr<iComponentArray>> m_component_arrays;

    inline static
    std::map<std::string, size_t> m_component_keys;

    inline static
    std::map<std::string, std::vector<size_t>> m_component_categories;

    inline static
    std::vector<System*> m_systems;


    struct Entity
    {
        int id = -1;
        bool persistent = true;

        std::string name = "";

        int parent = -1;
        std::set<int> children;

        std::map<size_t, int> components;
        std::set<std::string> component_names;

        size_t serialize( std::ofstream& ) const;
        size_t deserialize( std::ifstream& );
    };


    inline static
    idk::Allocator<Entity> m_entities;

    inline static
    std::vector<int> m_delete_list;


    template <typename T>
    static size_t getkey()
    {
        return typeid(T).hash_code();
    };

    inline static int m_selected_object = -1;


public:
    static void init     ( idk::EngineAPI &api );
    static void update   ( idk::EngineAPI &api );
    static void shutdown ( idk::EngineAPI &api );


    static const idk::Allocator<Entity> &getEntities() { return m_entities; };

    static int                  createGameObject ( const std::string &name = "Empty", bool persistent = true );
    static int                  copyGameObject   ( int obj_id, bool deep=false );
    static void                 _deleteGameObject ( int obj_id, bool deep=true );
    static void                 deleteGameObject ( int obj_id, bool deep=true );

    static void                 createPrefab     ( int obj_id, const std::string &filepath );
    static int                  loadPrefab       ( const std::string &filepath );

    static void                 gameObjectPersistent( int obj_id, bool p );
    static bool*                getGameObjectPersistency( int obj_id );
    static bool                 gameObjectExists ( int obj_id );

    static const std::string &  getGameObjectName( int obj_id );
    static void                 setGameObjectName( int obj_id, const std::string &name);
    static int                  getSelectedGameObject();
    static void                 setSelectedGameObject( int obj_id );
    static void                 deleteSelectedGameObject();
    static int                  copySelectedGameObject();
    static const std::string &  getSelectedGameObjectName();
    
    static void                 giveChild( int parent_id, int child_id );
    static void                 removeChild( int parent_id, int child_id );
    
    static int                  getParent( int obj_id );
    static const std::set<int> &getChildren( int obj_id );
    static bool                 hasParent( int obj_id );
    static bool                 hasChildren( int obj_id );

    static void registerPrefab             ( const std::string &name, std::function<int()> );
    static int  createGameObjectFromPrefab ( const std::string &name );
    static const std::map<std::string, std::function<int()>> &getPrefabs();




    template <typename T>
    static void giveComponent( int obj_id );
    static void giveComponent( int obj_id, size_t key );

    template <typename T>
    static bool hasComponent( int obj_id );

    template <typename T>
    static T &getComponent( int obj_id );

    template <typename T>
    static void removeComponent( int obj_id )
    {
        size_t key = getkey<T>();
        removeComponent(obj_id, key);
    }

    static bool hasComponent( int obj_id, size_t key );
    static void removeComponent( int obj_id, size_t key );

    template <typename T>
    static void copyComponent( int src_obj, int dst_obj );

    template <typename T>
    static ComponentArray<T> &getComponentArray();

    static iComponentArray *getComponentArray( size_t key );
    static iComponentArray *getComponentArray( const std::string &name );

    static std::map<size_t, std::unique_ptr<iComponentArray>> &getComponentArrays();
    static std::map<std::string, iComponentArray*> getComponentArraysSorted();
    static std::map<std::string, iComponentArray*> getComponentArraysByCategory( const std::string& );

    static const auto &getComponentCategories()
    {
        return m_component_categories;
    };


    template <typename T>
    static void userCallback( idk::EngineAPI&, int )
    {
        const auto &name = idk::ECS2::getComponentArray<T>().getName();
        LOG_WARN() << "Default userCallback<" << name << ">()";
    }

    static size_t getComponentKey( const std::string &name )
    {
        return m_component_keys[name];
    }

    template <typename T>
    static void registerComponent( const std::string &name, const std::string &category = "Builtin" );

    static bool isRegisteredComponent( const std::string &name ); 


    template <typename T>
    static void registerSystem()
    {
        m_systems.push_back(dynamic_cast<System *>(new T));

        int status;
        char *demangled = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status);

        if (status == 0)
        {
            m_systems.back()->m_name = std::string(demangled);
        }

        if (demangled)
        {
            std::free(demangled);
        }
    }


    static void save( const std::string &filepath );
    static void load( const std::string &filepath );

    static void onSceneLoad( std::function<void()> callback )
    {
        m_callbacks.push_back(callback);
    }

    static const std::string &getCurrentScene() { return m_filepath; };

};



#include "idk_ecs2_cmp.hpp"
#include "idk_ecs2.inl"

