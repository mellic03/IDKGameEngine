#include "EditorUI-tabs.hpp"



void
EditorTab::models( idk::EngineAPI &api )
{
    ImGui::Begin("Models");

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    for (idk::Model &model: ren.modelSystem().getModels())
    {
        ImGui::Text(model.name.c_str());
        ImGui::Separator();
        ImGui::Text("vertices: %d", model.m_vertices->size());
        ImGui::Text("indices:  %d", model.m_indices.size());
        ImGui::Dummy({0, 10});
    }

    ImGui::End();
}


