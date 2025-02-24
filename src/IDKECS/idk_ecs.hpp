#pragma once

#include <libidk/idk_allocator.hpp>

#include <vector>
#include <string>
#include <set>
#include <map>

#include <functional>

#include <cxxabi.h>



namespace idk
{
    class EngineAPI;
    class ECS;
}



class idk::ECS
{
public:
    class iComponentArray;

    template <typename T>
    class ComponentArray;

    class System
    {
    protected:
        idk::ECS *m_ecs;

    private:
        friend class idk::ECS;
        std::string m_name = "default";
        double m_avg_time = 0.0;

    public:
        idk::ECS &getECS() { return *m_ecs; };

        virtual void init     ( idk::EngineAPI& ) = 0;
        virtual void update   ( idk::EngineAPI& ) = 0;
        virtual void shutdown ( idk::EngineAPI& ) {  };
    };


private:
    idk::EngineAPI *m_api_ptr;

    bool m_readfile = false;
    std::string m_filepath;

    std::function<void()> m_post_callback = []() {  };
    std::function<void()> m_pre_callback  = []() {  };

    void _load();

    std::map<size_t, std::unique_ptr<iComponentArray>> m_component_arrays;
    std::map<std::string, size_t> m_component_keys;
    std::map<std::string, std::vector<size_t>> m_component_categories;

    std::vector<System*> m_systems;
    std::map<size_t, int> m_system_keys;

    using userfn_type = std::function<void(idk::EngineAPI&, idk::ECS&, int)>;
    inline static std::map<size_t, userfn_type> m_user_callbacks;
    inline static userfn_type m_default_userfn = [](idk::EngineAPI&, idk::ECS&, int) {
        // LOG_WARN("idk::ECS::m_default_userfn", "Default userCallback");
    };

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


    idk::Allocator<Entity> m_entities;

    std::vector<int> m_delete_list;


    template <typename T>
    static size_t getkey()
    {
        return typeid(T).hash_code();
    };

    int m_selected_object = -1;


public:
    std::string name = "";

    // ECS() {  };
    ECS( idk::EngineAPI &api, const std::string name )
    :   m_api_ptr(&api),
        name(name)
    {

    };

    // void init     ( idk::EngineAPI &api );
    void update   ( idk::EngineAPI &api );
    void shutdown ( idk::EngineAPI &api );


    const idk::Allocator<Entity> &getEntities() { return m_entities; };

    int                  createGameObject ( const std::string &name = "Empty", bool persistent = true );
    int                  copyGameObject   ( int obj_id, bool deep=false );
    void                 _deleteGameObject ( int obj_id, bool deep=true );
    void                 deleteGameObject ( int obj_id, bool deep=true );

    void                 createPrefab     ( int obj_id, const std::string &filepath );
    int                  loadPrefab       ( const std::string &filepath );

    void                 gameObjectPersistent( int obj_id, bool p );
    bool*                getGameObjectPersistency( int obj_id );
    bool                 gameObjectExists ( int obj_id );

    const std::string &  getGameObjectName( int obj_id );
    void                 setGameObjectName( int obj_id, const std::string &name);
    int                  getSelectedGameObject();
    void                 setSelectedGameObject( int obj_id );
    void                 deleteSelectedGameObject();
    int                  copySelectedGameObject();
    const std::string &  getSelectedGameObjectName();
    
    void                 giveChild( int parent_id, int child_id );
    void                 removeChild( int parent_id, int child_id );
    
    int                  getParent( int obj_id );
    const std::set<int> &getChildren( int obj_id );
    bool                 hasParent( int obj_id );
    bool                 hasChildren( int obj_id );

    void registerPrefab             ( const std::string &name, const std::function<int()>& );
    int  createGameObjectFromPrefab ( const std::string &name );
    const std::map<std::string, std::function<int()>> &getPrefabs();


    template <typename T>
    T &giveComponent( int obj_id );

    void giveComponent( int obj_id, size_t key );

    template <typename T>
    bool hasComponent( int obj_id );

    template <typename T>
    T &getComponent( int obj_id );

    template <typename T>
    void removeComponent( int obj_id )
    {
        size_t key = getkey<T>();
        removeComponent(obj_id, key);
    }

    bool hasComponent( int obj_id, size_t key );
    void removeComponent( int obj_id, size_t key );

    template <typename T>
    void copyComponent( int src_obj, int dst_obj );

    template <typename T>
    ComponentArray<T> &getComponentArray();

    iComponentArray *getComponentArray( size_t key );
    iComponentArray *getComponentArray( const std::string &name );

    std::map<size_t, std::unique_ptr<iComponentArray>> &getComponentArrays();
    std::map<std::string, iComponentArray*> getComponentArraysSorted();
    std::map<std::string, iComponentArray*> getComponentArraysByCategory( const std::string& );

    const auto &getComponentCategories()
    {
        return m_component_categories;
    };

    size_t getComponentKey( const std::string &name )
    {
        return m_component_keys[name];
    }

    template <typename T>
    void registerComponent( const std::string &name, const std::string &category = "Builtin" );
    bool isRegisteredComponent( const std::string &name ); 


    template <typename T>
    void registerSystem()
    {
        auto *sys = dynamic_cast<System *>(new T);
        sys->m_ecs = this;
        sys->init(*m_api_ptr);

        m_systems.push_back(sys);
        m_system_keys[getkey<T>()] = m_systems.size() - 1;
    }

    template <typename system_type>
    system_type &getSystem();


    template <typename component_type>
    static void setUserCallback( const userfn_type &callback )
    {
        size_t key = getkey<ComponentArray<component_type>>();
        m_user_callbacks[key] = callback;
        LOG_INFO("Assigned callback for component_type {}", key);
    }

    template <typename component_type>
    static void clearUserCallback()
    {
        size_t key = getkey<ComponentArray<component_type>>();
        m_user_callbacks.erase(key);
        LOG_INFO("Removed callback for component_type {}", key);
    }

    static userfn_type getUserCallback( size_t key );

    template <typename component_type>
    static userfn_type getUserCallback()
    {
        size_t key = getkey<ComponentArray<component_type>>();
        return getUserCallback(key);
    }




    void save( const std::string &filepath );
    void load( const std::string &filepath );


    void preSceneLoad( const std::function<void()> &callback )
    {
        m_pre_callback = callback;
    }

    void postSceneLoad( const std::function<void()> &callback )
    {
        m_post_callback = callback;
    }

    const std::string &getCurrentScene() { return m_filepath; };

};



#include "idk_ecs_cmp.hpp"
#include "idk_ecs.inl"

