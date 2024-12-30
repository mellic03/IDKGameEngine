#pragma once

#include <libidk/idk_transform.hpp> 
#include <IDKECS/ecs1/idk_ecs.hpp>
#include "idk_components.hpp"


namespace idk
{
    class Transform2Sys;
    struct Transform2Cmp;
};



class idk::Transform2Sys: public idk::ECS::System
{
private:

public:
    virtual void init   ( idk::EngineAPI& ) final;
    virtual void update ( idk::EngineAPI& ) final;
    
    // Convenience functions
    // -----------------------------------------------------------------------------------------
    static void moveUp    ( int obj_id, float f );
    static void moveRight ( int obj_id, float f );
    static void moveFront ( int obj_id, float f );
    // -----------------------------------------------------------------------------------------
};




struct idk::Transform2Cmp
{
    int obj_id = -1;

    glm::vec3 position;
    glm::quat rotation;

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};
