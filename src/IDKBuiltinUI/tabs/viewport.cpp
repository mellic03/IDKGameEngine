#include "../EditorUI.hpp"
#include <IDKEvents/IDKEvents.hpp>
#include "../../external/include/idk_imgui/imguizmo.hpp"



static void
transform_component_ecs( idk::EngineAPI &api, int obj_id, float tsnap, float rsnap, float ssnap )
{
    auto &engine = api.getEngine();
    
    auto &ren    = api.getRenderer();
    auto &eventsys = api.getEventSys();

    auto &camera = ren.getCamera();


    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    auto &io = ImGui::GetIO();
    float x = ImGui::GetWindowPos().x;
    float y = ImGui::GetWindowPos().y;
    float w = ImGui::GetWindowWidth();
    float h = ImGui::GetWindowHeight();
    ImGuizmo::SetRect(x, y, w, h);

    glm::mat4 view = camera.V;
    glm::mat4 proj = camera.P;

    ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;

    glm::vec3 tsnp = glm::vec3(tsnap);
    glm::vec3 rsnp = glm::vec3(rsnap);
    glm::vec3 ssnp = glm::vec3(ssnap);

    static ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE;

    if (eventsys.keylog().keyDown(idk::Keycode::LSHIFT))
    {
        mode = ImGuizmo::MODE::WORLD;
    }

    if (eventsys.keylog().keyDown(idk::Keycode::Z))
    {
        op = ImGuizmo::OPERATION::TRANSLATE;
    }

    if (eventsys.keylog().keyDown(idk::Keycode::X))
    {
        op = ImGuizmo::OPERATION::SCALEU;
    }

    if (eventsys.keylog().keyDown(idk::Keycode::C))
    {
        op = ImGuizmo::OPERATION::ROTATE;
    }

    if (eventsys.keylog().keyDown(idk::Keycode::V))
    {
        op = ImGuizmo::OPERATION::BOUNDS;
    }

    glm::vec3 snap;

    if (op == ImGuizmo::OPERATION::TRANSLATE)
    {
        snap = glm::vec3(tsnap);
    }

    else if (op == ImGuizmo::OPERATION::ROTATE)
    {
        snap = glm::vec3(rsnap);
    }

    else
    {
        snap = glm::vec3(ssnap);
    }

    glm::mat4 world = idk::TransformSys::getWorldMatrix(obj_id);
    glm::mat4 model = idk::TransformSys::getModelMatrix(obj_id);

    if (view == glm::inverse(model))
    {
        return;
    }

    static float bounds[] = { -0.5f, -0.5f, -0.5f, +0.5f, +0.5f, +0.5f };

    ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(proj),
        op,
        mode,
        glm::value_ptr(model),
        nullptr,
        &(snap[0]),
        (op & ImGuizmo::OPERATION::BOUNDS) ? bounds : nullptr
    );


    glm::mat4 local = glm::inverse(world) * model;
    idk::TransformSys::getTransform(obj_id) = idk::Transform::fromGLM(local);

}






void
EditorUI_MD::_tab_viewport( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    ImVec2 cornerA = ImGui::GetWindowContentRegionMin();
    ImVec2 cornerB = ImGui::GetWindowPos();
    ImVec2 corner = cornerA + cornerB;
    api.getEventSys().setMouseOffset(glm::vec2(corner.x, corner.y));


    int w = int(ImGui::GetContentRegionAvail().x);
    int h = int(ImGui::GetContentRegionAvail().y);

    w = 8*(w/8);
    h = 8*(h/8);

    glm::ivec2 size = ren.resolution();

    if (size.x != w || size.y != h)
    {
        ren.resize(w, h);
    }


    GLuint texture = ren.getFinalImage();

    ImGui::Image(
        *(ImTextureID *)(void *)(&texture),
        ImGui::GetContentRegionAvail(),
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f)
    );


    int obj_id = idk::ECS2::getSelectedGameObject();

    if (obj_id == -1)
    {
        return;
    }

    if (idk::ECS2::hasComponent<idk::TransformCmp>(obj_id))
    {
        transform_component_ecs(api, obj_id, m_tsnap, m_rsnap, m_ssnap);
    }
}

