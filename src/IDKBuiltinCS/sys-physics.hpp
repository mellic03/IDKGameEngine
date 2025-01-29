#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>

#include <libidk/idk_geometry.hpp>


namespace idk
{
    struct PhysicsCmp;
    struct StaticRectCmp;
    struct KinematicRectCmp;
    struct KinematicCapsuleCmp;

    namespace PhysicsConstants
    {
        static constexpr float G = 9.8f;
    };

    class  PhysicsSys;
}





class idk::PhysicsSys: public idk::ECS::System
{
private:

    static void kinematicCapsule_staticRect( float dt, KinematicCapsuleCmp&, StaticRectCmp& ) {  };

    static void kinematicCapsule_triangle( float dt, KinematicCapsuleCmp&, idk::geometry::Triangle& ) {  };
    static void kinematicCapsule_triangleGrid( float dt, KinematicCapsuleCmp& ) {  };
    static void _insert_triangle( const idk::geometry::Triangle& ) {  };

    static void _integrate ( idk::EngineAPI &api, float dt ) {  };

    inline static std::vector<idk::TextureWrapper> m_heightmaps;
    inline static float m_accumulator = 0.0f;


public:
    virtual void init   ( idk::EngineAPI & ) final {  };
    virtual void update ( idk::EngineAPI & ) final {  };

    static void applyForce( int obj_id, const glm::vec3& ) {  };
    static void addImpulse( int obj_id, const glm::vec3& ) {  };
    static void jump( int obj_id, float force ) {  };
    static bool raycast ( const glm::vec3 &origin, const glm::vec3 &dir, glm::vec3 &hit ) { return false; };

    static void bakeMeshCollider( int obj_id ) {  };

    static void bakeHeightmap( idk::TextureWrapper &wrapper ) {  };
    static float queryHeightmap( TextureWrapper &wrapper, const glm::vec3 &position, const glm::vec3 &scale ) { return -1.0f; };

};


struct idk::PhysicsCmp
{
    int obj_id = -1;

    glm::vec3 linear  = glm::vec3(0.0f);
    glm::vec3 angular = glm::vec3(0.0f);

    size_t      serialize            ( std::ofstream &stream ) const;
    size_t      deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy         ( idk::ECS &ecs, int src_obj, int dst_obj );
};



struct idk::StaticRectCmp
{
    int  obj_id    = -1;
    bool visualise = false;

    size_t      serialize            ( std::ofstream &stream ) const;
    size_t      deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy         ( idk::ECS &ecs, int src_obj, int dst_obj );
};


struct idk::KinematicRectCmp
{
    int  obj_id    = -1;
    bool visualise = false;

    size_t      serialize            ( std::ofstream &stream ) const;
    size_t      deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy         ( idk::ECS &ecs, int src_obj, int dst_obj );
};


struct idk::KinematicCapsuleCmp
{
    int  obj_id    = -1;
    bool enabled   = true;
    bool visualise = false;

    float radius   = 0.24f;
    float bottom   = 0.75f;
    float top      = 0.25f;

    bool  crouch = false;
    bool  grounded = false;
    float airtime  = 0.0f;

    glm::vec3 prev_pos = glm::vec3(0.0f);
    glm::vec3 curr_pos = glm::vec3(0.0f);

    glm::vec3 force    = glm::vec3(0.0f);
    glm::vec3 impulse  = glm::vec3(0.0f);


    float     gravity  = -PhysicsConstants::G;
    glm::vec3 acc      = glm::vec3(0.0f);
    glm::vec3 vel      = glm::vec3(0.0f);


    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy         ( idk::ECS &ecs, int src_obj, int dst_obj );
};

