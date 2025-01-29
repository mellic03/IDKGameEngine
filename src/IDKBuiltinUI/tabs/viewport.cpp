#include "../EditorUI.hpp"
#include <IDKEvents/IDKEvents.hpp>
#include <IDKIO/IDKIO.hpp>
#include "../../external/include/idk_imgui/imguizmo.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


static void
transform_component_ecs( idk::EngineAPI &api, idk::ECS &ecs, int obj_id,
                         float tsnap, float rsnap, float ssnap )
{
    auto &engine = api.getEngine();
    auto &IO     = api.getIO();
    auto &ren    = api.getRenderer();
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

    if (IO.keyDown(idk::Keycode::LSHIFT))
    {
        mode = ImGuizmo::MODE::WORLD;
    }

    if (IO.keyDown(idk::Keycode::Z))
    {
        op = ImGuizmo::OPERATION::TRANSLATE;
    }

    if (IO.keyDown(idk::Keycode::X))
    {
        op = ImGuizmo::OPERATION::SCALEU;
    }

    if (IO.keyDown(idk::Keycode::C))
    {
        op = ImGuizmo::OPERATION::ROTATE;
    }

    if (IO.keyDown(idk::Keycode::V))
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


    // glm::mat4 world = idk::TransformSys::getWorldMatrix(obj_id);
    // glm::mat4 model = idk::TransformSys::getModelMatrix(obj_id);

    auto &tsys = ecs.getSystem<idk::TransformSys>();
    glm::mat4 world = tsys.getWorldMatrix(obj_id);
    glm::mat4 model = tsys.getModelMatrix(obj_id);

    if (view == glm::inverse(model))
    {
        return;
    }

    if (ecs.hasComponent<idk::ModelCmp>(obj_id))
    {
        if (ecs.getComponent<idk::ModelCmp>(obj_id).viewspace)
        {
            view = glm::mat4(1.0f);
        }
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
    auto &cmp = tsys.getTransformCmp(obj_id);
    auto &T   = tsys.getTransform(obj_id);
    T = idk::Transform::fromGLM(local, T.scale.w);

    glm::vec3 scale1;
    glm::vec3 scale2;
    glm::quat rotation1;
    glm::quat rotation2;
    glm::vec3 position;
    glm::vec3 skew;
    glm::vec4 persp;
    glm::decompose(local, scale1, rotation1, position, skew, persp);
    glm::decompose(world, scale2, rotation2, position, skew, persp);

    idk::Transform T1; T1.position = glm::vec3(0.0f);
    glm::mat4 invR = idk::Transform::toGLM(T1, cmp.pitch, cmp.roll, cmp.yaw);
              invR = glm::mat4(glm::inverse(glm::mat3(invR)));

    T.position = glm::vec3(local[3]);
    T.rotation = glm::normalize(glm::quat_cast(glm::mat4_cast(rotation1) * invR));
    T.scale    = glm::vec4(scale1, T.scale.w);

    // cmp.position = glm::vec3(local[3]);
    // cmp.rotation = glm::normalize(glm::quat_cast(glm::mat4_cast(rotation1)));
}





static void
display_texture( float w, float h, uint32_t texture )
{
    uint64_t img0 = texture;
    void *img1 = (void *)(&img0);

    ImGui::Image(
        *((ImTextureID *)img1),
        ImVec2(w, h),
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f)
    );
}


void
EditorUI_MD::_tab_viewport( idk::EngineAPI &api, idk::ECS &ecs )
{
    auto &engine = api.getEngine();
    auto &IO     = api.getIO();
    auto &ren    = api.getRenderer();

    ImVec2 cornerA = ImGui::GetWindowContentRegionMin();
    ImVec2 cornerB = ImGui::GetWindowPos();
    ImVec2 corner = cornerA + cornerB;

    IO.setViewportOffset(corner.x, corner.y);


    float ratio = float(ren.width()) / ren.height();
    float w = ImGui::GetContentRegionAvail().x;
    float h = w / ratio;

    if (h > ImGui::GetContentRegionAvail().y);
    {
        h = ImGui::GetContentRegionAvail().y;
        w = h * ratio;
    }

    display_texture(w, h, ren.getFinalImage());

    // int h = int(ImGui::GetContentRegionAvail().y);

    // w = 8*(w/8);
    // h = 8*(h/8);

    // glm::ivec2 size = ren.resolution();

    // if (size.x != w || size.y != h)
    // {
    //     ren.resize(w, h);
    // }


    // GLuint texture = ren.getFinalImage();

    // ImGui::Image(
    //     *(ImTextureID *)(void *)(&texture),
    //     ImGui::GetContentRegionAvail(),
    //     ImVec2(0.0f, 1.0f),
    //     ImVec2(1.0f, 0.0f)
    // );


    int obj_id = ecs.getSelectedGameObject();

    if (obj_id == -1)
    {
        return;
    }

    if (IO.mouseCaptured())
    {
        return;
    }

    if (ecs.hasComponent<idk::TransformCmp>(obj_id))
    {
        transform_component_ecs(api, ecs, obj_id, m_tsnap, m_rsnap, m_ssnap);
    }
}

