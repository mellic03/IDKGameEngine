#include "sys-physics.hpp"


void
idk::PhysicsSys::init( idk::EngineAPI &api )
{
    
}



void
idk::PhysicsSys::update( idk::EngineAPI &api )
{

}



size_t
idk::PhysicsCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;

    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, linear);
    n += idk::streamwrite(stream, angular);

    return n;
};


size_t
idk::PhysicsCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
        
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, linear);
    n += idk::streamread(stream, angular);

    return n;
};


void
idk::PhysicsCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = obj_id;
};


void
idk::PhysicsCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = -1;
};


void
idk::PhysicsCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{
    // auto &src = api.getECS().getComponent<TransformCmp>(src_obj);

    // *this = src;
    // this->obj_id = dst_obj;
};


