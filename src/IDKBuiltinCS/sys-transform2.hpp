#pragma once

#include <libidk/idk_transform.hpp> 
#include <IDKECS/IDKECS.hpp>
#include "idk_components.hpp"


namespace idk
{
    class Transform2Sys;
    struct Transform2Cmp;
};



class idk::Transform2Sys: public idk::ECS::System
{
private:
    void _compute_transforms( int obj_id, const glm::mat4 &parent );


public:
    virtual void init   ( idk::EngineAPI& ) final;
    virtual void update ( idk::EngineAPI& ) final;

    glm::mat4 &getModelMatrix( int obj_id );
    glm::mat4 &getParentMatrix( int obj_id );

    // Convenience functions
    // -----------------------------------------------------------------------------------------
    Transform2Cmp &getCmp( int obj_id );

    void moveUp    ( int obj_id, float f );
    void moveRight ( int obj_id, float f );
    void moveFront ( int obj_id, float f );
    // -----------------------------------------------------------------------------------------
};




struct idk::Transform2Cmp
{
    int obj_id = -1;

    glm::vec3 position;
    glm::quat rotation;

    glm::vec3 up, right, front;

    glm::mat4 model;
    glm::mat4 parent;

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
};
