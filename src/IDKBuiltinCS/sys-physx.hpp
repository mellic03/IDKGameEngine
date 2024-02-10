#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


#define NDEBUG
#include "../../thirdparty/physx/include/PxPhysicsAPI.h"


namespace idk
{
    class PhysXSys;

    glm::vec3       toGLM( const physx::PxVec3 & );
    glm::quat       toGLM( const physx::PxQuat &pq );
    glm::mat4       toGLM( const physx::PxTransform &P );

    physx::PxVec3   toPX( const glm::vec3 & );


};



class idk::PhysXSys: public idecs::System, public idk::LuaAPI
{
public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;

    virtual void        exposeToLua( lua_State *LS ) final;

    static bool         initialized();

    static physx::PxRigidDynamic *createSphereCollider( const glm::vec3 &p, float r,
                                                        const glm::vec3 &v = glm::vec3(0.0f) );

    static physx::PxRigidDynamic *createDynamic( const physx::PxTransform &,
                                                 const physx::PxGeometry &,
                                                 const physx::PxVec3 &v = physx::PxVec3(0) );

    static physx::PxRigidStatic *createMeshCollider( int obj_id );


    static bool isGrounded( int obj_id );


};



namespace idk
{
    struct PhysXSphereRigidDynamicCmp
    {
        int     obj_id = -1;
        float   radius;
        physx::PxRigidDynamic *body = nullptr;

        PhysXSphereRigidDynamicCmp( int obj = -1, PhysXSphereRigidDynamicCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            if (other)
            {
                radius = other->radius;
                body   = other->body;
                return;
            }

            body = PhysXSys::createSphereCollider(glm::vec3(0.0f), 1.0f);

        };

        ~PhysXSphereRigidDynamicCmp()
        {

        };
    };


    struct PhysXMeshRigidStaticCmp
    {
        int obj_id = -1;
        physx::PxRigidStatic *body;

        PhysXMeshRigidStaticCmp( int obj = -1, PhysXMeshRigidStaticCmp *other = nullptr, idecs::ECS *ecs = nullptr )
        : obj_id(obj)
        {
            if (obj != -1)
            {
                body = PhysXSys::createMeshCollider(obj);
            }
        };

        ~PhysXMeshRigidStaticCmp()
        {
            // physx::PxCreateDynamic()
        };
    };

};


