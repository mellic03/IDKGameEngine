#pragma once

// #include <map>
// #include <set>

// #include <libidk/idk_componentsystem.hpp>
// #include <libidk/idk_allocator.hpp>
// #include <libidk/idk_dynamiclib.hpp>
// #include <memory>

// #include <cstdint>
// #include <cstddef>



namespace idk
{
    class GameScene;
};



class IDK_VISIBLE idk::GameScene
{
private:
//     idk::Allocator<int>                         m_gameobjects;
//     idk::Allocator<std::string>                 m_gameobject_names;
//     std::map<int, std::map<int, bool>>          m_component_matrix;

//     std::unordered_map<size_t, int>             m_componentsystem_ids;
//     std::unordered_map<int, std::string>        m_componentsystem_names;


//     void                _CS_stage_A( idk::EngineAPI & );
//     void                _CS_stage_B( idk::EngineAPI & );
//     void                _CS_stage_C( idk::EngineAPI & );
//     void                _CS_onObjectAssignment( int component_id, int obj_id );
//     void                _CS_onObjectDeassignment( int component_id, int obj_id );
//     void                _CS_onObjectCreation( int obj_id );
//     void                _CS_onObjectDeletion( int obj_id );
//     void                _CS_onObjectCopy( int src_obj_id, int dest_obj_id );


public:

//     int                 createGameObject  ( const std::string &name );
//     int                 copyGameObject    ( int obj_id );
//     void                deleteGameObject  ( int obj_id );
//     std::string &       getGameObjectName ( int obj_id );
//     void                setGameObjectName ( int obj_id, const std::string & );
//     Allocator<int> &    getGameObjects()  { return m_gameobjects; };


//     template <typename CS_type> void            giveComponent( int obj_id );
//     template <typename CS_type> void            removeComponent( int obj_id);
//     template <typename CS_type> bool            hasComponent( int obj_id );
//     template <typename CS_type> CS_type &       getComponentSystem();
//     const Allocator<ComponentSystem *> &        getComponentSystems() { return m_componentsystems; };

};


// template <typename CS_type>
// bool 
// idk::GameScene::hasComponent( int obj_id )
// {
//     auto type_idx = std::type_index(typeid(CS_type)).hash_code();
//     int  cs_id    = m_componentsystem_ids[type_idx];

//     return m_component_matrix[obj_id][cs_id];
// }


// template <typename CS_type>
// void
// idk::GameScene::giveComponent( int obj_id )
// {
//     int cs_id = getCS<CS_type>().ID();
//     _CS_onObjectAssignment(cs_id, obj_id);
//     m_component_matrix[obj_id][cs_id] = true;
// }


// template <typename CS_type>
// void
// idk::GameScene::removeComponent( int obj_id )
// {
//     int cs_id = getCS<CS_type>().ID();
//     _CS_onObjectDeassignment(cs_id, obj_id);
//     m_component_matrix[obj_id][cs_id] = false;
// }


// template <typename CS_type>
// CS_type &
// idk::GameScene::getCS()
// {
//     auto type_idx = std::type_index(typeid(CS_type)).hash_code();
//     int  cs_id    = m_componentsystem_ids[type_idx];
//     return *dynamic_cast<CS_type *>(getCS(cs_id));
// }


