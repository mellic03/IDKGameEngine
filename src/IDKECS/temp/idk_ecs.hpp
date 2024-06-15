#pragma once

#include "common.hpp"

#include "idk_ecs_entity.hpp"
#include "idk_ecs_component.hpp"
#include "idk_ecs_system.hpp"

#include <libidk/idk_scripting.hpp>



namespace idk
{
    class EngineAPI;
};


namespace idk::ecs { class ECS; };



class idk::ecs::ECS: public idk::LuaAPI
{
public:
    static constexpr size_t MAX_ENTITIES   = 512;
    static constexpr size_t MAX_COMPONENTS = 64;

    using callback_type = std::function<void(idk::EngineAPI&, int)>;


private:

    idk::EngineAPI &m_api;

    std::map<int, int>                   m_parent;
    std::map<int, std::set<int>>         m_children;

    idk::Allocator<Entity>               m_entities;
    std::set<int>                        m_entityIDs;
    std::deque<int>                      m_selection;

    size_t                               m_budget;
    bool                                 m_should_readFile = false;
    std::string                          m_readFile_path   = "";


    std::map<int, iComponentArray *>        m_components;
    std::unordered_map<size_t, int>         m_component_IDs;
    std::unordered_map<std::string, size_t> m_component_keys;
    std::vector<int>                        m_mandatory_components;

    idk::vector<System *>               m_systems;
    std::unordered_map<size_t, int>     m_systemIDs;




    void writeEntities   ( std::ofstream &stream );
    void writeComponents ( std::ofstream &stream );

    void readEntities    ( std::ifstream &stream );
    void readComponents  ( std::ifstream &stream );


public:

    template <typename T>
    int  getComponentID();

    template <typename T>
    size_t getComponentKey();

    int    getComponentID( size_t key );
    size_t getComponentKey( const std::string & );
    bool   componentExists( const std::string & );


    ECS( idk::EngineAPI &api ): m_api(api) {  };

    void writeFile ( const std::string &filepath );
    void _readFile  ( const std::string &filepath );
    void readFile( const std::string &filepath );


    IDK_ALLOCATOR_ACCESS(Entity, idk::ecs::Entity, m_entities);


    size_t numComponents() { return m_components.size(); };

    template <typename T>
    int registerComponent( const idk::string &name );


    template <typename T>
    int registerComponentMandatory( const idk::string &name )
    {
        int id = registerComponent<T>(name);
        m_mandatory_components.push_back(id);
        return id;
    };


    template <typename T>
    ComponentArray<T> &getComponentArray();
    iComponentArray   *getComponentArray( int id ) { return m_components[id]; };
    iComponentArray   *getComponentArray( const std::string & );


    template <typename T>
    T &getComponent( int obj_id );


    void giveComponent   ( int obj_id, int component );
    void copyComponent   ( int obj_id, int src_id, int component );
    bool hasComponent    ( int obj_id, int component );
    void removeComponent ( int obj_id, int component );

    template <typename T> void giveComponent   ( int obj_id );
    template <typename T> bool hasComponent    ( int obj_id );
    template <typename T> void removeComponent ( int obj_id );



    template <typename T>
    void registerSystem();


    template <typename T>
    T &getSystem();



    int                     createGameObject( const std::string &name = "Empty" );
    int                     copyGameObject( int obj_id );
    void                    deleteGameObject( int obj_id );
    std::set<int> &         gameObjects();
    bool                    gameObjectExists( int obj_id );
    const std::string &     getGameObjectName( int obj_id );
    void                    setGameObjectName( int obj_id, const std::string &name);
    int                     getGameObjectByName( const std::string &name );


    int                     getSelectedGameObject();
    void                    setSelectedGameObject( int obj_id );
    void                    deleteSelectedGameObject();
    int                     copySelectedGameObject();
    const std::string &     getSelectedGameObjectName();

    void                    giveChild     ( int parent_id, int child_id );
    void                    removeChild   ( int parent_id, int child_id );
    void                    clearChildren ( int parent_id );
    void                    removeParent  ( int child_id  );
    bool                    hasParent     ( int child_id  ) { return m_parent[child_id] != -1; };
    int                     getParent     ( int child_id  ) { return m_parent[child_id];       };
    std::set<int> &         getChildren   ( int parent_id ) { return m_children[parent_id];    };

    void                    init     ( idk::EngineAPI& );
    void                    update   ( idk::EngineAPI& );
    void                    shutdown ( idk::EngineAPI& );

    virtual void            exposeToLua( lua_State *L ) final;

};



template <typename T>
size_t
idk::ecs::ECS::getComponentKey()
{
    return typeid(ComponentArray<T>).hash_code();
}




template <typename T>
int
idk::ecs::ECS::getComponentID()
{
    return getComponentID(getComponentKey<T>());
}



template <typename T>
int
idk::ecs::ECS::registerComponent( const idk::string &name )
{
    int component_id = m_component_IDs.size();

    ComponentArray<T> *C = new ComponentArray<T>(m_api, name, component_id);
    size_t key = typeid(ComponentArray<T>).hash_code();

    m_components[component_id] = C;
    m_component_IDs[key]       = component_id;
    m_component_keys[name]     = key;

    return component_id;
};


template <typename T>
idk::ecs::ComponentArray<T> &
idk::ecs::ECS::getComponentArray()
{
    size_t key = getComponentKey<T>();
    int    id  = getComponentID(key);
    return *dynamic_cast<ComponentArray<T> *>(getComponentArray(id));
};



template <typename T>
T &
idk::ecs::ECS::getComponent( int obj_id )
{
    if (hasComponent<T>(obj_id) == false)
    {
        giveComponent<T>(obj_id);
    }
    IDK_ASSERT("Object does not have component", hasComponent<T>(obj_id));

    ComponentArray<T> &comp_array = getComponentArray<T>();

    int idx = getComponentID<T>();

    Entity &entity = getEntity(obj_id);
    return comp_array.getComponent(entity.components[idx]);
};




template <typename T>
void
idk::ecs::ECS::giveComponent( int obj_id )
{
    int component = getComponentID<T>();
    giveComponent(obj_id, component);
}


template <typename T>
bool
idk::ecs::ECS::hasComponent( int obj_id )
{
    int component = getComponentID<T>();
    return hasComponent(obj_id, component);
}


template <typename T>
void
idk::ecs::ECS::removeComponent( int obj_id )
{
    int component = getComponentID<T>();
    removeComponent(obj_id, component);
}






template <typename T>
void
idk::ecs::ECS::registerSystem()
{
    size_t key = typeid(T).hash_code();
    m_systemIDs[key] = m_systems.size();

    m_systems.push_back(dynamic_cast<System *>(new T));
};



template <typename T>
T &
idk::ecs::ECS::getSystem()
{
    size_t key = typeid(T).hash_code();
    int    idx = m_systemIDs[key];

    return *dynamic_cast<T *>(m_systems[idx]);
}

