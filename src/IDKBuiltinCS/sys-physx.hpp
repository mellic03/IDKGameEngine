#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>

#include "sys-transform.hpp"


#define NDEBUG
#include "../../thirdparty/physx/include/PxPhysicsAPI.h"


namespace idk
{
    class PhysXSys;

    glm::vec3  toGLM( const physx::PxVec3 & );
    glm::vec3  toGLM( const physx::PxExtendedVec3 & );
    glm::quat  toGLM( const physx::PxQuat &pq );
    glm::mat4  toGLM( const physx::PxTransform &P );

    physx::PxVec3       toPx( const glm::vec3 & );
    physx::PxQuat       toPx( const glm::quat & );
    physx::PxTransform  toPx( const glm::vec3 &, const glm::quat & );
    physx::PxTransform  toPx( const glm::mat4 & );

};



class idk::PhysXSys: public idk::ecs::System, public idk::LuaAPI
{
private:

    struct PhysXCallback
    {
        int   subject;
        int   target;
        float alpha;
        float stop;
        std::function<void()> callback;
    };

    inline static std::unordered_map<int, PhysXCallback> m_move_callbacks;

    static void         _updateCallbacks();
    static void         updateRigidDynamic( idk::EngineAPI & );
    static void         updateRigidStatic( idk::EngineAPI & );
    static void         updateKinematicController( idk::EngineAPI & );


public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;

    virtual void        exposeToLua( lua_State *LS ) final;

    static bool         initialized();

    static physx::PxRigidDynamic *createSphereCollider( int obj_id );
    static physx::PxRigidStatic *createRectStatic( int obj_id );


    static physx::PxRigidDynamic *createDynamic( const physx::PxTransform &,
                                                 const physx::PxGeometry &,
                                                 const physx::PxVec3 &v = physx::PxVec3(0) );

    static physx::PxRigidStatic *createMeshCollider( int obj_id );

    static physx::PxController *createCapsuleKinematic( int obj_id, float height, float radius );


    static void enableCCT( int obj_id );
    static void disableCCT( int obj_id );


    static bool isGrounded( int obj_id );
    static void addForce( int obj_id, const glm::vec3 & );

    static void moveCCT( int obj_id, float x, float y, float z );

    static float moveTowardsCCT( int subject, int target, float alpha );

    static void moveTowardsCallbackCCT( int subject, int target, float alpha, float stop,
                                        std::function<void()> callback );


    static bool hasCCT( int obj_id );


};



namespace idk
{
    struct PhysXSphereRigidDynamicCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int,                     obj_id,     -1 ) \
            X( bool,                    enabled,    true ) \
            X( float,                   radius,     1.0f ) \
            X( physx::PxRigidDynamic *, body,       nullptr )

        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS
    
        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            body = PhysXSys::createSphereCollider(obj_id);
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };


    struct PhysXRectRigidStaticCmp
    {
        int                   obj_id  = -1;
        bool                  enabled = true;
        physx::PxRigidStatic *body    = nullptr;


        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, enabled);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, enabled);
            return n;
        };
    
        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            this->body = PhysXSys::createRectStatic(obj_id);
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };
    };


    struct PhysXMeshRigidStaticCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int,                    obj_id,     -1 ) \
            X( bool,                   enabled,    false ) \
            X( physx::PxRigidStatic *, body,       nullptr )

        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            this->body = PhysXSys::createMeshCollider(obj_id);
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };


    struct PhysXKinematicControllerCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int,                     obj_id,     -1 ) \
            X( bool,                    enabled,    true ) \
            X( float,                   height,     1.75f ) \
            X( float,                   radius,     0.25f ) \
            X( glm::vec3,               velocity,   glm::vec3(0.0f) ) \
            X( physx::PxController *,   controller, nullptr )

        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS
    
        void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            this->obj_id = obj_id;
            this->controller = PhysXSys::createCapsuleKinematic(obj_id, height, 0.75);
        };

        void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

    };

};


