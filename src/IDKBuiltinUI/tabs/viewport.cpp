#include "../EditorUI.hpp"
#include <IDKEvents/IDKEvents.hpp>
#include "../../external/include/idk_imgui/imguizmo.hpp"



static void
transform_component_ecs( idk::EngineAPI &api, int obj_id, float tsnap = 1.0f, float rsnap = 10.0f )
{
    auto &engine = api.getEngine();
    auto &ecs    = api.getECS();
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

    glm::mat4 view = camera.view();
    glm::mat4 proj = camera.projection();

    ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;
    ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE;

    glm::vec3 tsnp = glm::vec3(tsnap);
    glm::vec3 rsnp = glm::vec3(rsnap);
    float *ts      = &tsnp[0];
    float *rs      = &rsnp[0];

    if (eventsys.keylog().keyDown(idk::Keycode::LSHIFT))
    {
        mode = ImGuizmo::MODE::WORLD;
    }

    if (eventsys.keylog().keyDown(idk::Keycode::S))
    {
        op = ImGuizmo::OPERATION::SCALEU;
    }

    if (eventsys.keylog().keyDown(idk::Keycode::R))
    {
        op = ImGuizmo::OPERATION::ROTATE;
    }

    if (eventsys.keylog().keyDown(idk::Keycode::B))
    {
        op = ImGuizmo::OPERATION::BOUNDS;
    }

    auto &component = ecs.getComponent<idk::TransformCmp>(obj_id);
    int parent_id = ecs.getParent(obj_id);

    glm::mat4 parent = idk::TransformSys::getWorldMatrix(parent_id);
    glm::mat4 world  = idk::TransformSys::getWorldMatrix(obj_id);
    glm::mat4 model  = idk::TransformSys::getModelMatrix(obj_id);

    static float bounds[] = { -0.5f, -0.5f, -0.5f, +0.5f, +0.5f, +0.5f };

    ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(proj),
        op,
        mode,
        glm::value_ptr(world),
        nullptr,
        (op & ImGuizmo::OPERATION::TRANSLATE) ? ts : rs,
        op & ImGuizmo::OPERATION::BOUNDS ? bounds : nullptr
    );

    model = glm::inverse(parent) * world;

    glm::vec3 &scale = component.scale;
    scale.x = glm::length(glm::vec3(model[0]));
    scale.y = glm::length(glm::vec3(model[1]));
    scale.z = glm::length(glm::vec3(model[2]));

    model[0] /= scale.x;
    model[1] /= scale.y;
    model[2] /= scale.z;

    glm::vec3 &pos = component.position;
    pos = glm::vec3(model[3]);
    // if (ts)
    // {
    //     pos = tsnap * glm::floor(pos / tsnap);
    // }

    component.rotation = glm::quat_cast(model);

}






void
EditorUI_MD::_tab_viewport( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &ecs    = api.getECS();

    int w = int(ImGui::GetContentRegionAvail().x);
    int h = int(ImGui::GetContentRegionAvail().y);

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


    // GLuint texture = ren.m_vxgi_buffer.attachments[0];

    // ImGui::Image(
    //     *(ImTextureID *)(void *)(&texture),
    //     ImVec2(512, 512),
    //     ImVec2(0.0f, 1.0f),
    //     ImVec2(1.0f, 0.0f)
    // );


    if (ImGui::IsItemClicked(1))
    {
        ImGui::OpenPopup("Pop-out");
    }

    if (ImGui::BeginPopup("Pop-out"))
    {
        if (ImGui::MenuItem("Pop out"))
        {
            std::cout << "Yeetus\n";
        }
        ImGui::EndPopup();
    }


    int obj_id = ecs.getSelectedGameObject();

    if (ecs.hasComponent<idk::TransformCmp>(obj_id))
    {
        transform_component_ecs(api, obj_id, m_tsnap, m_rsnap);
    }
}

