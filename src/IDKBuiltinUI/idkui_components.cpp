// #include "idkui_components.hpp"

// #include <IDKGameEngine/IDKengine.hpp>
// #include <IDKEvents/IDKEvents.hpp>
// #include <IDKBuiltinCS/IDKBuiltinCS.hpp>

// #include <idk_imgui/imgui.hpp>
// #include <idk_imgui/imguizmo.hpp>


// void
// idk::ui::transform_component( idk::EngineAPI &api, idk::Camera &camera, idk::Hierarchy_CS &CS,
//                               glm::mat4 &model, glm::mat4 parent, float tsnap, float rsnap )
// {
//     auto &engine   = api.getEngine();
//     auto &eventsys = api.getEventSys();

//     ImGuizmo::SetOrthographic(false);

//     auto &io = ImGui::GetIO();
//     float x = ImGui::GetWindowPos().x;
//     float y = ImGui::GetWindowPos().y;
//     float w = io.DisplaySize.x; // ImGui::GetWindowWidth();
//     float h = io.DisplaySize.y; // ImGui::GetWindowHeight();
//     ImGuizmo::SetRect(0, 0, w, h);

//     glm::mat4 view = camera.view() * parent;
//     glm::mat4 proj = camera.projection();

//     ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;
//     ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE;

//     glm::vec3 tsnp = glm::vec3(tsnap);
//     glm::vec3 rsnp = glm::vec3(rsnap);
//     float *ts      = nullptr;
//     float *rs      = nullptr;

//     if (eventsys.keylog().keyDown(idk::Keycode::LSHIFT))
//     {
//         mode = ImGuizmo::MODE::WORLD;
//     }

//     if (eventsys.keylog().keyDown(idk::Keycode::S))
//     {
//         op = ImGuizmo::OPERATION::SCALEU;
//     }

//     if (eventsys.keylog().keyDown(idk::Keycode::R))
//     {
//         op = ImGuizmo::OPERATION::ROTATE;
//     }

//     if (eventsys.keylog().keyDown(idk::Keycode::LALT))
//     {
//         ts = &tsnp[0];
//         rs = &rsnp[0];
//     }

//     ImGuizmo::Manipulate(
//         glm::value_ptr(view),
//         glm::value_ptr(proj),
//         op,
//         mode,
//         glm::value_ptr(model),
//         nullptr,
//         ts
//     );

// }



// void
// idk::ui::transform_component2( idk::EngineAPI &api, idk::Camera &camera, idk::Hierarchy_CS &CS,
//                                glm::mat4 &model, glm::mat4 transform, glm::mat4 parent )
// {
//     auto &engine   = api.getEngine();
//     auto &eventsys = api.getEventSys();

//     ImGuizmo::SetOrthographic(false);

//     auto &io = ImGui::GetIO();
//     float x = ImGui::GetWindowPos().x;
//     float y = ImGui::GetWindowPos().y;
//     float w = io.DisplaySize.x; // ImGui::GetWindowWidth();
//     float h = io.DisplaySize.y; // ImGui::GetWindowHeight();
//     ImGuizmo::SetRect(0, 0, w, h);

//     glm::mat4 view     = camera.view();
//     glm::mat4 proj     = camera.projection();
//     glm::mat4 inv_view = glm::inverse(view);
//     glm::mat4 inv_proj = glm::inverse(proj);


//     ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;

//     if (eventsys.keylog().keyDown(idk::Keycode::LSHIFT))
//     {
//         mode = ImGuizmo::MODE::WORLD;
//     }


//     glm::mat4 delta(1.0f);

//     ImGuizmo::Manipulate(
//         glm::value_ptr(view),
//         glm::value_ptr(proj),
//         ImGuizmo::OPERATION::TRANSLATE,
//         mode,
//         glm::value_ptr(model),
//         glm::value_ptr(delta)
//     );

//     ImGuizmo::Manipulate(
//         glm::value_ptr(view),
//         glm::value_ptr(proj),
//         ImGuizmo::OPERATION::ROTATE,
//         mode,
//         glm::value_ptr(model),
//         glm::value_ptr(delta)
//     );

//     // model = glm::inverse(parent) * transform;

// }