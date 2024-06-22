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
    auto &engine = api.getEngine();
    auto &events = api.getEventSys();
    auto &ren    = api.getRenderer();

    float dtime = engine.deltaTime();

    for (auto &cmp: ECS2::getComponentArray<CameraCmp>())
    {
        int obj_id = cmp.obj_id;
        int cam_id = cmp.cam_id;

        cmp.camera.V = glm::inverse(TransformSys::getModelMatrix(cmp.obj_id));
        ren.getCamera(cam_id) = cmp.camera;

    }
}


bool
idk::CameraSys::in_frustum( int subject, int target )
{
    // auto &ren = api_ptr->getRenderer();
    // auto &cmp = ECS2::getComponent<CameraCmp>(target);

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
idk::CameraCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = ECS2::getComponent<CameraCmp>(obj_id);
    cmp.cam_id = api.getRenderer().activeCamera();
}


void
idk::CameraCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    
}


void
idk::CameraCmp::onObjectCopy( int src_obj, int dst_obj )
{

}


