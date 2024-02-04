#include "EditorUI-tabs.hpp"



void
EditorTab::models( idk::EngineAPI &api )
{
    ImGui::Begin("Models");

    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    for (idk::Model &model: ren.modelSystem().getModels())
    {
        std::string name = model.name;
    
        if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf))
        {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                ImGui::SetDragDropPayload("MODEL_DRAG_DROP",
                    (const void *)(&model.id),
                    sizeof(int)
                );

                if (ImGui::IsDragDropActive())
                {
                    ImGui::Text(name.c_str());
                }
                ImGui::EndDragDropSource();
            }
            ImGui::TreePop();
        }

        // ImGui::Text(model.name.c_str());
        // ImGui::Separator();
        // ImGui::Text("vertices: %d", model.m_vertices->size());
        // ImGui::Text("indices:  %d", model.m_indices.size());
        // ImGui::Dummy({0, 10});
    }

    ImGui::End();
}


