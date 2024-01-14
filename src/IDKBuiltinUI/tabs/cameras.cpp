#include "EditorUI-tabs.hpp"



static void callback( idk::Camera &cam )
{
    static glm::vec3 offset(0.5f, 1.0f, 1.0f);
    // idkgui::dragVec3("Offset", &offset[0], -2.0f, 2.0f, 0.1f, "%.1f", 0.0f);
    // cam.setOffset(offset);

    ImGui::Spacing();

    ImGui::SliderFloat("Strength", &cam.m_abr_str, 0.0f, 0.1f, "%.4f");
    ImGui::SliderFloat2("R",  &cam.m_r_abr.x,  -1.0f, 1.0f, "%.0f");
    ImGui::SameLine();
    if (ImGui::Button("RReset"))
        cam.m_r_abr = glm::vec2(0.0f);

    ImGui::SliderFloat2("G",  &cam.m_g_abr.x,  -1.0f, 1.0f, "%.1f");
    ImGui::SameLine();
    if (ImGui::Button("GReset"))
        cam.m_g_abr = glm::vec2(0.0f);

    ImGui::SliderFloat2("B",  &cam.m_b_abr.x,  -1.0f, 1.0f, "%.1f");
    ImGui::SameLine();
    if (ImGui::Button("BReset"))
        cam.m_b_abr = glm::vec2(0.0f);

    ImGui::Spacing();
}



void
EditorTab::cameras( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    auto &cameras = ren.getCameras();

    ImGui::Begin("Cameras");
    EditorUI::selection_splitWindow2(cameras, callback);
    ImGui::End();
}

