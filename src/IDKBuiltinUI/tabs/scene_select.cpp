#include "../EditorUI.hpp"



idk::ECS*
EditorUI_MD::_tab_scene_select( idk::EngineAPI &api )
{
    ImGui::Begin("ECS Scenes");

    constexpr auto flags = ImGuiTableFlags_SizingStretchSame
                         | ImGuiTableFlags_Resizable
                         | ImGuiTableFlags_BordersOuter
                         | ImGuiTableFlags_BordersV;

    static idk::ECS *selection = nullptr;

    if (ImGui::BeginTable("table1", 2, flags))
    {
        for (auto &[name, ecs]: api.getScenes())
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            if (ImGui::Selectable(name.c_str(), selection == ecs))
            {
                selection = ecs;
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();

    return selection;
}


