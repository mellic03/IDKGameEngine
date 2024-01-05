#pragma once

#include <IDKGameEngine/IDKGameEngine.hpp>


namespace idk { class Transform_CS; };


class idk::Transform_CS: public idk::ComponentSystem
{
private:
    idk::Allocator<glm::mat4>    m_matrices;
    std::map<int, int>           m_keys;
    std::map<int, int>           m_parent;
    std::map<int, std::set<int>> m_children;


public:
    void        init    ( idk::EngineAPI & ) {  };
    void        stage_A ( idk::EngineAPI & );
    void        stage_B ( idk::EngineAPI & ) {  };
    void        stage_C ( idk::EngineAPI & ) {  };

    void        onObjectAssignment( int obj_id, idk::Engine & );
    void        onObjectCreation( int obj_id, idk::Engine & );
    void        onObjectDeletion( int obj_id, idk::Engine & );
    void        onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void                translate ( int obj_id, glm::vec3 v );
    void                rotateX   ( int obj_id, float r     );
    void                rotateY   ( int obj_id, float r     );
    void                rotateZ   ( int obj_id, float r     );
    void                scale     ( int obj_id, glm::vec3 s );

    glm::vec3 &         getPosition( int obj_id );


    // int                 getParentID ( int child_id );

    // void                setParent   ( int parent_id, int child_id );
    // void                unsetParent ( int child_id                );

    void                giveParent    ( int parent_id, int child_id );
    void                giveChild     ( int parent_id, int child_id );
    void                removeChild   ( int parent_id, int child_id );
    void                clearChildren ( int parent_id );
    void                removeParent  ( int child_id );


    const auto &        getChildren   ( int parent_id ) { return m_children[parent_id]; };



    glm::mat4 &         getModelMatrix( int obj_id );
    glm::mat4           getParentModelMatrix( int obj_id );
    glm::mat4           getModelMatrixParented( int obj_id );

};

