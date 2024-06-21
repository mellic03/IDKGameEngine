#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


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





class idk::PhysicsSys: public idk::ECS2::System
{
private:
    static void kinematicCapsule_staticRect( float timestep, KinematicCapsuleCmp &s_cmp, StaticRectCmp &r_cmp );
    static void _integrate ( idk::EngineAPI &api, float dt );

    inline static std::vector<idk::TextureWrapper> m_heightmaps;
    inline static float m_accumulator = 0.0f;


public:
    virtual void init   ( idk::EngineAPI & ) final;
    virtual void update ( idk::EngineAPI & ) final;

    static void addForce( int obj_id, const glm::vec3& );
    static bool raycast ( const glm::vec3 &origin, const glm::vec3 &dir, glm::vec3 &hit );

    static void bakeHeightmap( idk::TextureWrapper &wrapper );
    static float queryHeightmap( const glm::vec3 &position, const glm::vec3 &scale );

};


struct idk::PhysicsCmp
{
    int obj_id = -1;

    glm::vec3 linear  = glm::vec3(0.0f);
    glm::vec3 angular = glm::vec3(0.0f);

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};



struct idk::StaticRectCmp
{
    int  obj_id    = -1;
    bool visualise = false;

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};


struct idk::KinematicRectCmp
{
    int  obj_id    = -1;
    bool visualise = false;

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};


struct idk::KinematicCapsuleCmp
{
    int  obj_id    = -1;
    bool enabled   = true;
    bool visualise = false;

    float radius   = 0.24f;
    float bottom   = 0.75f;
    float top      = 0.25f;
    
    bool  grounded = false;
    float airtime  = 0.0f;


    glm::vec3 prev_pos = glm::vec3(0.0f);
    glm::vec3 curr_pos = glm::vec3(0.0f);
    glm::vec3 force = glm::vec3(0.0f);

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};

