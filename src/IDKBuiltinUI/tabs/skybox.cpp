#include "EditorUI-tabs.hpp"
#include <idk_icons/idk_Icons.hpp>

#include <filesystem>


void
EditorTab::skybox( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();

    ImGui::Begin("Skybox");
    {
        // if (ImGui::BeginTable("Skybox Table", 1, EditorTab::resizeable_table_flags))
        // {
        //     ImGui::TableNextRow();
        //     ImGui::TableNextColumn();

        //     static int selected = 0;
        //     ren.current_skybox = selected;

        //     ImGuiStyle& style = ImGui::GetStyle();
        //     float left_w = 0.25 * ImGui::GetContentRegionAvail().x;
        //     float right_w = ImGui::GetContentRegionAvail().x - left_w;
        //     ImGuiID child_id = ImGui::GetID((void*)(intptr_t)52);

        //     for (int i=0; i<ren.skyboxes.size(); i++)
        //     {
        //         if (ImGui::Selectable(std::to_string(i).c_str(), selected == i))
        //         {
        //             selected = i;
        //         }
        //     }


        //     static std::string selection = "";
        //     static bool open = false;

        //     if (ImGui::Button(IDK_ICON_FOLDER_OPENED "Load"))
        //     {
        //         ImGui::OpenPopup("File Selection");
        //         open = true;
        //     }

        //     if (idkImGui::fileSelectPopup("File Selection", open, "./", selection))
        //     {
        //         std::string filepath = std::filesystem::relative(selection);
        //         ren.loadSkybox(filepath + "/");
        //     }

        //     ImGui::EndTable();
        // }
    }
    ImGui::End();
}


