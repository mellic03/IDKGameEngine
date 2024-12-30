#pragma once

#include "sys-transform.hpp"


namespace idk
{
    struct PlanetDesc;
    struct PlanetCmp;
    class  PlanetSys;
};


class idk::PlanetSys: public idk::ECS::System
{
private:

public:

    virtual void init   ( idk::EngineAPI& );
    virtual void update ( idk::EngineAPI& ) final;

};



struct idk::PlanetDesc
{
    float radius  = 32.0f;
    float gravity = 9.8f;
};


struct idk::PlanetCmp
{
    int obj_id = -1;
    PlanetDesc desc;

    size_t serialize   ( std::ofstream &stream ) const;
    size_t deserialize ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};
