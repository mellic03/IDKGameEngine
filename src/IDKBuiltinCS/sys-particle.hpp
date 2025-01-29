#pragma once

#include "idk_components.hpp"
#include "sys-transform.hpp"

#include <IDKGraphics/particle/particle_system.hpp>
#include <IDKGraphics/render/particle.hpp>


namespace idk
{
    struct ParticleCmp;
    struct TempParticle;

    class  ParticleSys;
};


struct idk::ParticleCmp
{
    int obj_id     = -1;
    int src_id     = -1;
    int emitter_id = -1;

    idk::ParticleDesc desc;

    size_t serialize   ( std::ofstream &stream ) const;
    size_t deserialize ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy         ( idk::ECS &ecs, int src_obj, int dst_obj );
};




class idk::ParticleSys: public idk::ECS::System
{
private:

public:

    virtual void init   ( idk::EngineAPI& );
    virtual void update ( idk::EngineAPI& ) final;


};

