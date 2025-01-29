#include "sys-camera.hpp"
#include "sys-transform.hpp"

#include <IDKEvents/IDKEvents.hpp>

#include <filesystem>
namespace fs = std::filesystem;


static idk::EngineAPI *api_ptr;



void
idk::CameraSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}



void
idk::CameraSys::update( idk::EngineAPI &api )
{
    auto &ecs  = getECS();
    auto &tsys = ecs.getSystem<TransformSys>();
    auto &ren  = api.getRenderer();

    for (auto &cmp: ecs.getComponentArray<CameraCmp>())
    {
        int obj_id = cmp.obj_id;
        int cam_id = cmp.cam_id;
        
        auto &cam = ren.getCamera(cam_id);

        cam.fov      = cmp.camera.fov;
        cam.bloom    = cmp.camera.bloom;
        cam.exposure = cmp.camera.exposure;
    
        cam.setTransform(tsys.getModelMatrix(obj_id));
    }
}


float&
idk::CameraSys::getFov( int obj_id )
{
    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<CameraCmp>(obj_id);
    return api_ptr->getRenderer().getCamera(cmp.cam_id).fov;
}


float&
idk::CameraSys::getFovOffset( int obj_id )
{
    auto &ecs = getECS();
    auto &cmp = ecs.getComponent<CameraCmp>(obj_id);
    return api_ptr->getRenderer().getCamera(cmp.cam_id).fov_offset;
}


bool
idk::CameraSys::in_frustum( int subject, int target )
{
    // auto &ren = api_ptr->getRenderer();
    // auto &cmp = ecs.getComponent<CameraCmp>(target);

    // IDK_Camera &cam = ren.getCamera(cmp.cam_id);

    return false;
}




size_t
idk::CameraCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, camera);
    return n;
}

size_t
idk::CameraCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, camera);
    cam_id = api_ptr->getRenderer().activeCamera();
    return n;
}


void
idk::CameraCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<CameraCmp>(obj_id);
    cmp.cam_id = api.getRenderer().activeCamera();
}


void
idk::CameraCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    
}


void
idk::CameraCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{

}


