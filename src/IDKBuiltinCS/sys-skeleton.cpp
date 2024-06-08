#include "sys-skeleton.hpp"
#include "sys-transform.hpp"

#include <IDKEvents/IDKEvents.hpp>


void
idk::SkeletonSys::init( idk::EngineAPI &api )
{

}


void
idk::SkeletonSys::update( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ecs    = api.getECS();
	float dtime  = engine.deltaTime();

    for (auto &cmp: ecs.getComponentArray<idk::SkeletonCmp>())
    {
        
    }

}




size_t
idk::SkeletonCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
}


size_t
idk::SkeletonCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
}


void
idk::SkeletonCmp::onObjectAssignment( idk::EngineAPI &api, int obj )
{
    obj_id = obj;
}


void
idk::SkeletonCmp::onObjectDeassignment( idk::EngineAPI &api, int obj )
{

}


void
idk::SkeletonCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{
    IDK_ASSERT("Cannot copy idk::SkeletonCmp!", false);
}



