#include "sys-planet.hpp"


void
idk::PlanetSys::init( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    float dt  = api.dtime();

}


void
idk::PlanetSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    float dt  = api.dtime();


}



size_t
idk::PlanetCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, desc);
    return n;
};


size_t
idk::PlanetCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, desc);
    return n;
};


void
idk::PlanetCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<PlanetCmp>(obj_id);

};


void
idk::PlanetCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<PlanetCmp>(obj_id);
};


void
idk::PlanetCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    auto &src = ecs.getComponent<PlanetCmp>(src_obj);
    auto &dst = ecs.getComponent<PlanetCmp>(dst_obj);
};


