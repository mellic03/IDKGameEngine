#include "EditorUI-tabs.hpp"


void
EditorTab::dirlights( idk::EngineAPI &api )
{
    ImGui::Begin("Directional Lights");

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    idk::Dirlight &dirlight = ren.lightSystem().getDirlight(0);

    ImGui::ColorEdit4("Ambient", &dirlight.ambient[0]);
    ImGui::ColorEdit4("Diffuse", &dirlight.diffuse[0]);
    ImGui::DragFloat3("Direction", &dirlight.direction[0], 0.01f, -1.0f, +1.0f);

    ImGui::End();
}


