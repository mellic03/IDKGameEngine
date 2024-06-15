#include "idk_systems.hpp"
#include <IDKEvents/IDKEvents.hpp>

#include "sys-transform.hpp"


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

    for (auto &cmp: idk::ECS2::getComponentArray<idk::CameraCmp>())
    {
        int obj_id = cmp.obj_id;
        int cam_id = cmp.cam_id;

        if (cam_id == -1)
        {
            continue;
        }

        IDK_ASSERT("Object does not exist", obj_id >= 0);
        IDK_ASSERT("Camera does not exist", cam_id >= 0);

        auto &camera = ren.getCamera(cam_id);
        camera.bloom = cmp.bloom;
        camera.fov   = cmp.fov;

        camera.chromatic_r = cmp.chromatic_r;
        camera.chromatic_g = cmp.chromatic_g;
        camera.chromatic_b = cmp.chromatic_b;
        camera.chromatic_strength = cmp.chromatic_strength;



        camera.setModelMatrix(TransformSys::getModelMatrix(obj_id));

    }
}


bool
idk::CameraSys::in_frustum( int subject, int target )
{
    auto &ren = api_ptr->getRenderer();
    auto &cmp = idk::ECS2::getComponent<idk::CameraCmp>(target);

    idk::Camera &cam = ren.getCamera(cmp.cam_id);

    return false;
}



void
idk::LightSys::init( idk::EngineAPI &api )
{
    for (auto &cmp: idk::ECS2::getComponentArray<idk::DirlightCmp>())
    {
        DirlightCmp::onObjectAssignment(api, cmp.obj_id);
    }

    for (auto &cmp: idk::ECS2::getComponentArray<idk::PointlightCmp>())
    {
        PointlightCmp::onObjectAssignment(api, cmp.obj_id);
    }

    for (auto &cmp: idk::ECS2::getComponentArray<idk::SpotlightCmp>())
    {
        SpotlightCmp::onObjectAssignment(api, cmp.obj_id);
    }
}


void
idk::LightSys::update( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    for (auto &[obj_id, light_id, diffuse, ambient]: idk::ECS2::getComponentArray<idk::DirlightCmp>())
    {
        if (light_id == -1)
        {
            light_id = ren.createDirlight();
        }

        auto &light = ren.getDirlight(light_id);

        glm::mat4 M = TransformSys::getModelMatrix(obj_id);

        glm::vec3 pos = TransformSys::getPositionWorldspace(obj_id);
        glm::vec3 dir = glm::vec3(M * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

        light.transform = M;
        light.direction = glm::vec4(dir, 0.0f);
        light.ambient   = ambient;
        light.diffuse   = diffuse;
    }

    for (auto &[obj_id, light_id, diffuse, radius]: idk::ECS2::getComponentArray<idk::PointlightCmp>())
    {
        auto &light = ren.getPointlight(light_id);
    
        glm::mat4 M    = TransformSys::getModelMatrix(obj_id);
        light.position = M[3];
        light.diffuse  = diffuse;
        light.radius   = radius;
    }

    for (auto &[obj_id, light_id, diffuse, angle, radius]: idk::ECS2::getComponentArray<idk::SpotlightCmp>())
    {
        auto &light = ren.getSpotlight(light_id);

        glm::mat4 M = TransformSys::getModelMatrix(obj_id);
        light.position = M[3];
        light.orientation = glm::normalize(glm::quat_cast(M));

        light.diffuse = diffuse;
        light.angle   = angle;
        light.radius  = radius;
    }

}

