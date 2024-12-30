#include "sys-transform2.hpp"

#include <libidk/idk_log.hpp>
#include <libidk/idk_geometry.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


static idk::EngineAPI *api_ptr;


void
idk::Transform2Sys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}



void
idk::Transform2Sys::update( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();
    float dtime = api.getEngine().deltaTime();

    for (auto &cmp: ecs.getComponentArray<idk::Transform2Cmp>())
    {

    }
}




size_t
idk::Transform2Cmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
};


size_t
idk::Transform2Cmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
};


void
idk::Transform2Cmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = ECS2::getComponent<Transform2Cmp>(obj_id);

};


void
idk::Transform2Cmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::Transform2Cmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<Transform2Cmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<Transform2Cmp>(dst_obj);
    dst.obj_id = dst_obj;
};
