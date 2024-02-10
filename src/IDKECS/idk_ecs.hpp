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


namespace idecs { class ECS; };



class idecs::ECS: public idk::LuaAPI
{
public:
    static constexpr size_t MAX_ENTITIES   = 512;
    static constexpr size_t MAX_COMPONENTS = 64;

    using callback_type = std::function<void(idk::EngineAPI&, int)>;


private:

    idk::EngineAPI &m_api;

    std::map<int, int>                  m_parent;
    std::map<int, std::set<int>>        m_children;

    idk::Allocator<Entity>              m_entities;
    std::set<int>                       m_entityIDs;
    std::deque<int>                     m_selection;

    size_t                              m_budget;
    bool                                m_should_readFile = false;
    std::string                         m_readFile_path   = "";

    idk::vector<iComponentArray *>       m_components;
    std::unordered_map<size_t, int>      m_componentIDs;
    std::unordered_map<std::string, int> m_component_names;
    std::vector<int>                     m_mandatory_components;

    idk::vector<System *>               m_systems;
    std::unordered_map<size_t, int>     m_systemIDs;


    void writeEntities   ( std::ofstream &stream );
    void writeComponents ( std::ofstream &stream );

    void readEntities    ( std::ifstream &stream );
    void readComponents  ( std::ifstream &stream );


public:

    ECS( idk::EngineAPI &api ): m_api(api) {  };

    void writeFile ( const std::string &filepath );
    void _readFile  ( const std::string &filepath );
    void readFile( const std::string &filepath );


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


    template <typename T>
    void setComponentBehaviour( const Behaviour &behaviour )
    {
        this->getComponentArray<T>().setBehaviour(behaviour);
    };

    

    template <typename T>
    T &getComponent( int obj_id );


    void giveComponent   ( int obj_id, int component );
    void giveComponent   ( int obj_id, const std::string &name );
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
    char *                  getGameObjectName( int obj_id );
    void                    setGameObjectName( int obj_id, const std::string &name);

    int                     getSelectedGameObject();
    void                    setSelectedGameObject( int obj_id );
    void                    deleteSelectedGameObject();
    int                     copySelectedGameObject();
    char *                  getSelectedGameObjectName();

    void                    giveChild     ( int parent_id, int child_id );
    void                    removeChild   ( int parent_id, int child_id );
    void                    clearChildren ( int parent_id );
    void                    removeParent  ( int child_id  );
    bool                    hasParent     ( int child_id  ) { return m_parent[child_id] != -1; };
    int                     getParent     ( int child_id  ) { return m_parent[child_id];       };
    std::set<int> &         getChildren   ( int parent_id ) { return m_children[parent_id];    };

    void                    init   ( idk::EngineAPI & );
    void                    update ( idk::EngineAPI & );

    virtual void            exposeToLua( lua_State *L ) final;

};








template <typename T>
int
idecs::ECS::registerComponent( const idk::string &name )
{
    ComponentArray<T> *C = new ComponentArray<T>(name, *this);
    size_t key = typeid(ComponentArray<T>).hash_code();
    
    m_components.push_back(C);
    m_componentIDs[key]     = m_components.size() - 1;
    m_component_names[name] = m_components.size() - 1;

    return m_components.size() - 1;
};


template <typename T>
idecs::ComponentArray<T> &
idecs::ECS::getComponentArray()
{
    size_t key = typeid(ComponentArray<T>).hash_code();
    int    idx = m_componentIDs[key];

    return m_components[idx]->cast<ComponentArray<T>>();
};



template <typename T>
T &
idecs::ECS::getComponent( int obj_id )
{
    if (hasComponent<T>(obj_id) == false)
    {
        giveComponent<T>(obj_id);
    }
    IDK_ASSERT("Object does not have component", hasComponent<T>(obj_id));

    ComponentArray<T> &v = getComponentArray<T>();

    size_t key = typeid(ComponentArray<T>).hash_code();
    int    idx = m_componentIDs[key];

    Entity &entity = m_entities.get(obj_id);
    return v.data().get(entity.components[idx]);
};


template <typename T>
void
idecs::ECS::giveComponent( int obj_id )
{
    size_t key = typeid(ComponentArray<T>).hash_code();
    int    idx = m_componentIDs[key];

    giveComponent(obj_id, idx);
}


template <typename T>
bool
idecs::ECS::hasComponent( int obj_id )
{
    size_t key = typeid(ComponentArray<T>).hash_code();
    int    idx = m_componentIDs[key];

    return hasComponent(obj_id, idx);
}


template <typename T>
void
idecs::ECS::removeComponent( int obj_id )
{
    size_t key = typeid(ComponentArray<T>).hash_code();
    int    idx = m_componentIDs[key];

    removeComponent(obj_id, idx);
}





template <typename T>
void
idecs::ECS::registerSystem()
{
    size_t key = typeid(T).hash_code();
    m_systemIDs[key] = m_systems.size();

    m_systems.push_back(dynamic_cast<System *>(new T));
};



template <typename T>
T &
idecs::ECS::getSystem()
{
    size_t key = typeid(T).hash_code();
    int    idx = m_systemIDs[key];

    return *dynamic_cast<T *>(m_systems[idx]);
}

