#include "idk_camera_cs.hpp"
#include "idk_transform_cs.hpp"

static idk::RenderEngine *renptr;


void
idk::Camera_CS::init( idk::EngineAPI &api )
{
    addDependencies("transform");
    renptr = &api.getRenderer();
}


void
idk::Camera_CS::stage_A( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    Transform_CS &tCS = engine.getCS<Transform_CS>();

    for (auto &[obj_id, camera_id]: m_camera_ids)
    {
        idk::Camera    &camera    = ren.getCamera(camera_id);

        // Move camera position towards object position
        glm::vec3 opos = tCS.getPosition(obj_id);
        glm::vec3 cpos = camera.position();

        float alpha = 0.5f;

        camera.translate(alpha * (opos - cpos));
    }

}



idk::Camera &
idk::Camera_CS::getCamera( idk::RenderEngine &ren, int obj_id )
{
    int id = m_camera_ids[obj_id];
    return ren.getCamera(id);
}



int
idk::Camera_CS::getCameraID( int obj_id )
{
    return m_camera_ids[obj_id];
}



void
idk::Camera_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_camera_ids[obj_id] = renptr->createCamera();
}


void
idk::Camera_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{

}


void
idk::Camera_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    std::cout << "[Camera_CS] deleting obj_id: " << obj_id << "\n";
    m_camera_ids[obj_id] = -1;
}


void
idk::Camera_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{

}


void
idk::Camera_CS::onObjectSelection( int obj_id )
{
    
}
