#include "../EditorUI.hpp"
#include <IDKEvents/IDKEvents.hpp>
#include "../../external/include/idk_imgui/imguizmo.hpp"



static void
transform_component_ecs( idk::EngineAPI &api, int obj_id, float tsnap = 1.0f, float rsnap = 10.0f )
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
    float *ts      = &tsnp[0];
    float *rs      = &rsnp[0];

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

    auto &component = idk::ECS2::getComponent<idk::TransformCmp>(obj_id);
    int parent_id = idk::ECS2::getParent(obj_id);

    glm::mat4 world = idk::TransformSys::getModelMatrix(obj_id);
    glm::mat4 delta;

    static float bounds[] = { -0.5f, -0.5f, -0.5f, +0.5f, +0.5f, +0.5f };

    ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(proj),
        op,
        mode,
        glm::value_ptr(world),
        glm::value_ptr(delta),
        (op & ImGuizmo::OPERATION::TRANSLATE) ? ts : rs,
        op & ImGuizmo::OPERATION::BOUNDS ? bounds : nullptr
    );


    if (op == ImGuizmo::OPERATION::ROTATE)
    {
        glm::mat4 Mw = idk::TransformSys::getWorldMatrix(obj_id);
        glm::mat4 Ml = idk::TransformSys::getLocalMatrix(obj_id, false);

        glm::quat &Q = idk::TransformSys::getData(obj_id).rotation;

        glm::mat4 R = Mw * glm::mat4_cast(Q);
                  R = delta * R;
                  R = glm::inverse(Mw) * R;

        Q = glm::normalize(glm::quat_cast(R));
    }

    else if (op == ImGuizmo::OPERATION::TRANSLATE)
    {
        glm::vec3 dpos = delta[3];
        idk::TransformSys::translateWorldspace(obj_id, dpos);
    }

    else if (op == ImGuizmo::OPERATION::BOUNDS)
    {
        glm::vec3 &scale = idk::TransformSys::getData(obj_id).scale3;

        scale = glm::vec3(
            glm::length(glm::vec3(world[0])),
            glm::length(glm::vec3(world[1])),
            glm::length(glm::vec3(world[2]))
        );
    }


    // float &scale = component.scale;
    // scale = glm::length(glm::vec3(model[0]));

    // model[0] /= scale;
    // model[1] /= scale;
    // model[2] /= scale;

    // glm::vec3 &pos = component.position;
    // pos = glm::vec3(model[3]);

    // component.rotation = glm::quat_cast(model);

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


    int obj_id = idk::ECS2::getSelectedGameObject();
    if (obj_id == -1)
    {
        return;
    }

    if (idk::ECS2::hasComponent<idk::TransformCmp>(obj_id))
    {
        transform_component_ecs(api, obj_id, m_tsnap, m_rsnap);
    }
}

