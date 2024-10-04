#pragma once

#include <IDKECS/IDKECS.hpp>
#include <libidk/idk_serialize.hpp>



namespace idk
{
    struct AtmosphereCmp;
    class  PlanetSys;

};



class idk::PlanetSys: public idk::ECS2::System
{
public:
    virtual void init   ( idk::EngineAPI & ) final;
    virtual void update ( idk::EngineAPI & ) final;

};




struct idk::AtmosphereCmp
{
    int obj_id   = -1;
    int atmosphere_id = -1;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};


