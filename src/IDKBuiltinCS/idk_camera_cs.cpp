#include "idk_camera_cs.hpp"
#include "idk_transform_cs.hpp"
#include "../../external/include/idk_imgui/imgui.hpp"
#include "../../external/include/idk_imgui/imguizmo.hpp"
#include "../IDKBuiltinUI/idkui_components.hpp"


static idk::EngineAPI *api_ptr;


void
idk::Camera_CS::init( idk::EngineAPI &api )
{
    addDependencies("transform");
    api_ptr = &api;
}


void
idk::Camera_CS::stage_A( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &tCS    = engine.getCS<Transform_CS>();

    for (auto &[obj_id, cam_id, follow]: m_data)
    {
        idk::Camera &camera = ren.getCamera(cam_id);

        if (follow)
        {
            glm::vec3 opos = tCS.getPosition(obj_id);
            glm::vec3 cpos = camera.position();

            float alpha = 0.5f;
            camera.translate(alpha * (opos - cpos));
        }
    }

}



idk::Camera &
idk::Camera_CS::getCamera( int obj_id )
{
    int id = m_data.get(m_keys[obj_id]).cam_id;
    return api_ptr->getRenderer().getCamera(id);
}



int
idk::Camera_CS::getCameraID( int obj_id )
{
    return m_data.get(m_keys[obj_id]).cam_id;
}



void
idk::Camera_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    ComponentData data = {
        .obj_id = obj_id,
        .cam_id = api_ptr->getRenderer().createCamera(),
        .follow = true
    };

    m_keys[obj_id] = m_data.create(data);

    // m_camera_ids[obj_id] = api_ptr->getRenderer().createCamera();
}


void
idk::Camera_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{

}


void
idk::Camera_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    m_data.destroy(m_keys[obj_id]);
    m_keys[obj_id] = -1;
}


void
idk::Camera_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{

}


void
idk::Camera_CS::onObjectSelection( int obj_id )
{
    std::string label = "Camera_ID: " + std::to_string(getCameraID(obj_id));
    ImGui::Text(label.c_str());
    ImGui::Checkbox("Follow", &m_data.get(m_keys[obj_id]).follow);


    auto &engine = api_ptr->getEngine();
    auto &ren    = api_ptr->getRenderer();
    auto &tCS    = engine.getCS<idk::Transform_CS>();

    idk::Camera &cam = this->getCamera(obj_id);

    glm::mat4 &cam_model = cam.model();
    glm::mat4 &obj_model = tCS.getModelMatrix(obj_id);
    glm::mat4 transform  = tCS.getModelMatrixParented(obj_id);
    glm::mat4 parent     = tCS.getParentModelMatrix(obj_id);

    idk::ui::transform_component2(
        *api_ptr, ren.getCamera(), engine.getCS<idk::Transform_CS>(),
        cam_model, transform, parent
    );
}
