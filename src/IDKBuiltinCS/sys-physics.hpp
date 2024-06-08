#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


namespace idk
{
    struct PhysicsCmp;
    // struct RigidBodyRectCmp;
    // struct RigidBodySphereCmp;

    class  PhysicsSys;
}





class idk::PhysicsSys: public idk::ecs::System
{
private:

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};


struct idk::PhysicsCmp
{
    int obj_id = -1;

    glm::vec3 linear  = glm::vec3(0.0f);
    glm::vec3 angular = glm::vec3(0.0f);

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    void    onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    void    onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    void    onObjectCopy         ( idk::EngineAPI &api, int src_obj, int dst_obj );
};


// struct idk::RigidBodyRectCmp
// {
//     int obj_id = -1;

//     glm::vec3 extents;

//     size_t  serialize            ( std::ofstream &stream ) const;
//     size_t  deserialize          ( std::ifstream &stream );
//     void    onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
//     void    onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
//     void    onObjectCopy         ( idk::EngineAPI &api, int src_obj, int dst_obj );
// };


// struct idk::RigidBodySphereCmp
// {
//     int obj_id = -1;

//     size_t  serialize            ( std::ofstream &stream ) const;
//     size_t  deserialize          ( std::ifstream &stream );
//     void    onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
//     void    onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
//     void    onObjectCopy         ( idk::EngineAPI &api, int src_obj, int dst_obj );
// };


