#pragma once

#include "idk_components.hpp"
#include "sys-transform.hpp"


namespace idk
{
    struct ParticleCmp;
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
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};




class idk::ParticleSys: public idk::ECS2::System
{
private:


public:

    virtual void init   ( idk::EngineAPI& ) {  };
    virtual void update ( idk::EngineAPI& ) final;


};

