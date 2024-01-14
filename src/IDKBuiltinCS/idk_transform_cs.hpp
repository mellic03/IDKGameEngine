#pragma once

#include <IDKGameEngine/IDKGameEngine.hpp>


namespace idk
{
    class Transform_CS;
};


class idk::Transform_CS: public idk::ComponentSystem
{
private:

    struct ComponentData
    {
        int obj_id;
        glm::mat4 transform;
        glm::mat4 parented_transform;

        ComponentData( int id ): obj_id(id), transform(1.0f), parented_transform(1.0f) {  };
    };

    idk::Allocator<ComponentData>   m_data;
    std::map<int, int>              m_keys;
    std::map<int, int>              m_parent;
    std::map<int, std::set<int>>    m_children;


    glm::mat4 _compute_parented( int obj_id );

    int &getKey( int obj_id )
    {
        IDK_ASSERT("Object does not have component!", m_keys[obj_id] != -1);
        return m_keys[obj_id];
    };


public:
    void            init    ( idk::EngineAPI & ) {  };
    void            stage_A ( idk::EngineAPI & );
    void            stage_B ( idk::EngineAPI & ) {  };
    void            stage_C ( idk::EngineAPI & ) {  };

    void            onObjectAssignment( int obj_id, idk::Engine & );
    void            onObjectCreation( int obj_id, idk::Engine & );
    void            onObjectDeletion( int obj_id, idk::Engine & );
    void            onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );
    void            onObjectSelection( int obj_id );

    void            translate ( int obj_id, glm::vec3 v );
    void            rotateX   ( int obj_id, float r     );
    void            rotateY   ( int obj_id, float r     );
    void            rotateZ   ( int obj_id, float r     );
    void            scale     ( int obj_id, glm::vec3 s );

    glm::vec3 &     getPosition( int obj_id );

    void            giveParent    ( int parent_id, int child_id );
    void            giveChild     ( int parent_id, int child_id );
    void            removeChild   ( int parent_id, int child_id );
    void            clearChildren ( int parent_id );
    void            removeParent  ( int child_id );


    const auto &    getChildren   ( int parent_id ) { return m_children[parent_id]; };


    glm::mat4 &     getModelMatrix( int obj_id );
    glm::mat4       getModelMatrixParented( int obj_id );
    glm::mat4       getParentModelMatrix( int obj_id );

};

